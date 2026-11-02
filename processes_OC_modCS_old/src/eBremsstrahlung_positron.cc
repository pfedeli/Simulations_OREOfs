// -------------------------------------------------------------------
//
// GEANT4 Class file
//
// This file implements the class for the modified bremsstrahlung process.
// Class based on G4eBremsstrahlung.
//
// -------------------------------------------------------------------
// Some include...

#include "eBremsstrahlung_positron.hh"
#include "G4SystemOfUnits.hh"
#include "G4Gamma.hh"
//#include "SeltzerBergerModel_positron.hh"
//#include "eBremsstrahlungRelModel_positron.hh"
#include "G4SeltzerBergerModel.hh"
#include "G4eBremsstrahlungRelModel.hh"
#include "G4UnitsTable.hh"
#include "G4LossTableManager.hh"

#include "G4ProductionCutsTable.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4EmParameters.hh"
//#include "LogicalCrystalVolume.hh"

// This is a later addition and is important to allow the localized modification
// of the G4 processes (in specified regions)

#include "G4RegionStore.hh"
#include "G4EmConfigurator.hh"

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
 
eBremsstrahlung_positron::eBremsstrahlung_positron(const G4String& name):
  G4VEnergyLossProcess(name), 
  isInitialised(false)
{
  SetProcessSubType(fBremsstrahlung);
  SetSecondaryParticle(G4Gamma::Gamma());
  SetIonisation(false);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

eBremsstrahlung_positron::~eBremsstrahlung_positron()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool eBremsstrahlung_positron::IsApplicable(const G4ParticleDefinition& p)
{
  return (&p == G4Positron::Positron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void 
eBremsstrahlung_positron::InitialiseEnergyLossProcess(const G4ParticleDefinition*,
					       const G4ParticleDefinition*)
{
  if (!isInitialised) {
    // Instantiate the parameters for the bremsstrahlung model
    G4EmParameters* param = G4EmParameters::Instance();
    G4double emin = param->MinKinEnergy();
    G4double emax = param->MaxKinEnergy();
    G4VEmFluctuationModel* fm = nullptr;

    // Activate the model for the world: G4 standard process (low-energy)
    if (!EmModel(0)) { SetEmModel(new G4SeltzerBergerModel()); }
    EmModel(0)->SetLowEnergyLimit(emin);
    G4double energyLimit = std::min(EmModel(0)->HighEnergyLimit(), GeV);
    EmModel(0)->SetHighEnergyLimit(energyLimit);
    EmModel(0)->SetSecondaryThreshold(param->BremsstrahlungTh());
    //EmModel(0)->SetLPMFlag(false);
    G4EmParameters::Instance()->SetLPM(false);
    AddEmModel(1,EmModel(0),fm);

    // Activate the model for the world: G4 standard process (high-energy)
    if (emax > energyLimit) {
      if (!EmModel(1)) { SetEmModel(new G4eBremsstrahlungRelModel()); }
      EmModel(1)->SetLowEnergyLimit(energyLimit);
      EmModel(1)->SetHighEnergyLimit(emax); 
      EmModel(1)->SetSecondaryThreshold(param->BremsstrahlungTh());
      //EmModel(1)->SetLPMFlag(param->LPM());
      G4EmParameters::Instance()->SetLPM(G4EmParameters::Instance()->LPM());
      AddEmModel(1,EmModel(1),fm);
    }

    // Flag for initialization of the process
    isInitialised = true;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void eBremsstrahlung_positron::StreamProcessInfo(std::ostream& out) const
{
  if (EmModel(0)) {
    G4EmParameters* param = G4EmParameters::Instance();
    G4double eth = param->BremsstrahlungTh(); 
    out << "      LPM flag: " << param->LPM() << " for E > " 
	<< EmModel(0)->HighEnergyLimit()/GeV << " GeV";
    if(eth < DBL_MAX) { 
      out << ",  VertexHighEnergyTh(GeV)= " << eth/GeV; 
    }
    out << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void eBremsstrahlung_positron::ProcessDescription(std::ostream& out) const
{
  out << " For positron Bremsstrahlung in crystal ";
  G4VEnergyLossProcess::ProcessDescription(out);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

