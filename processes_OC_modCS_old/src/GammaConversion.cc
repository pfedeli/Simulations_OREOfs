// -------------------------------------------------------------------
//
// GEANT4 Class file
//
// This file implements the class for the modified pair production process.
// Class based on G4PairProduction.
//
// -------------------------------------------------------------------
// Some include...

#include "GammaConversion.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4BetheHeitlerModel.hh"
#include "G4PairProductionRelModel.hh"
#include "G4Electron.hh"
#include "G4EmParameters.hh"

//#include "BetheHeitlerModel.hh"
//#include "PairProductionRelModel.hh"
#include "G4BetheHeitlerModel.hh"
#include "G4PairProductionRelModel.hh"

// This is a later addition and is important to allow the localized modification
// of the G4 processes (in specified regions)

#include "G4RegionStore.hh"
#include "G4EmConfigurator.hh"
#include "G4LossTableManager.hh"

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GammaConversion::GammaConversion(const G4String& processName,
  G4ProcessType type):G4VEmProcess (processName, type),
    isInitialised(false)
{
  SetMinKinEnergy(2.0*electron_mass_c2);
  SetProcessSubType(fGammaGeneralProcess);
  SetStartFromNullFlag(true);
  SetBuildTableFlag(true);
  SetSecondaryParticle(G4Electron::Electron());
  SetLambdaBinning(220);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
GammaConversion::~GammaConversion()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool GammaConversion::IsApplicable(const G4ParticleDefinition& p)
{
  return (&p == G4Gamma::Gamma());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GammaConversion::InitialiseProcess(const G4ParticleDefinition*)
{
  if (!isInitialised) {
    // Instantiate the parameters for the pair production model
    G4EmParameters* param = G4EmParameters::Instance();
    G4double emin = std::max(param->MinKinEnergy(), 2*electron_mass_c2);
    G4double emax = param->MaxKinEnergy();
    SetMinKinEnergy(emin);

    // Activate the model for the world: G4 standard process (low-energy)
    if (!EmModel(0)) { SetEmModel(new G4BetheHeitlerModel()); }
    EmModel(0)->SetLowEnergyLimit(emin);
    G4double energyLimit = std::min(EmModel(0)->HighEnergyLimit(), 80*GeV);
    EmModel(0)->SetHighEnergyLimit(energyLimit);
    AddEmModel(1,EmModel(0));

    // Activate the model for the world: G4 standard process (high-energy)
    if (emax > energyLimit) {
      if (!EmModel(1)) { SetEmModel(new G4PairProductionRelModel()); }
      EmModel(1)->SetLowEnergyLimit(energyLimit);
      EmModel(1)->SetHighEnergyLimit(emax);
      AddEmModel(1,EmModel(1));
    }
 
    // Flag for initialization of the process
    isInitialised = true;
  } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double GammaConversion::MinPrimaryEnergy(const G4ParticleDefinition*,
					                              const G4Material*)
{
  return 2*electron_mass_c2;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//void GammaConversion::PrintInfo()
//{}         

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GammaConversion::ProcessDescription(std::ostream& out) const
{
  out << "  Gamma conversion";
  G4VEmProcess::ProcessDescription(out);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

