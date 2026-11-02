//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// Custom EmPhysicsList used to activate SF effects in crystals by properly
// modifing the cross-sections for pair-prodcution and Bremsstrahlung.
// Originally created by Viktar and then modified by Pietro Monti Guarnieri to
// activate SF effects in a region and then by gpaterno to activate SF effects
// in more than one region and pass a parameter to select the proper correction
// to apply depending on the material/axis considered.
// 
// Last modify: 07/08/2023 (gpaterno)
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// class header
#include "EmStandardPhysics.hh"

// Include some needed classes
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"
#include "G4LossTableManager.hh"

// Include the models for the G4 particle processes
#include "G4ComptonScattering.hh"             // Photons
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4LivermorePhotoElectricModel.hh"
#include "G4RayleighScattering.hh"

#include "G4eMultipleScattering.hh"           // Charged particles
#include "G4MuMultipleScattering.hh"
#include "G4hMultipleScattering.hh"
#include "G4CoulombScattering.hh"
#include "G4eCoulombScatteringModel.hh"
#include "G4WentzelVIModel.hh"
#include "G4UrbanMscModel.hh"

#include "G4MuBremsstrahlungModel.hh"
#include "G4MuPairProductionModel.hh"
#include "G4hBremsstrahlungModel.hh"
#include "G4hPairProductionModel.hh"
 
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
#include "G4UAtomicDeexcitation.hh"

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"
 
#include "G4hIonisation.hh"
#include "G4ionIonisation.hh"

// Include the particles 
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4KaonPlus.hh"
#include "G4KaonMinus.hh"
#include "G4Proton.hh"
#include "G4AntiProton.hh"
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4He3.hh"
#include "G4Alpha.hh"
#include "G4GenericIon.hh"

// Include what is needed to modify the Physics Lists 
#include "G4PhysicsListHelper.hh"
#include "G4BuilderType.hh"
#include "G4EmModelActivator.hh"
#include "G4EmConfigurator.hh"                    
#include "G4RegionStore.hh"

// Include the modified Bremsstrahlung and Pair Production processes
#include "eBremsstrahlung_electron.hh"
#include "eBremsstrahlung_positron.hh"
#include "GammaConversion.hh"

// [Inserted in March 2023] Include some other standard and non standard process
#include "eBremsstrahlungRelModel_electron.hh"     // Custom e+/e- bremsstrahlung
#include "eBremsstrahlungRelModel_positron.hh"
#include "SeltzerBergerModel_electron.hh"
#include "SeltzerBergerModel_positron.hh"
#include "BetheHeitlerModel.hh"                    // Custom photon pair production
#include "PairProductionRelModel.hh"

#include "G4SeltzerBergerModel.hh"                 // Standard e+/e-/gamma processes
#include "G4BetheHeitlerModel.hh"
#include "G4DummyModel.hh"
#include "G4PairProductionRelModel.hh"
#include "G4eBremsstrahlungRelModel.hh"
#include "G4EmParameters.hh"
#include "G4eeToTwoGammaModel.hh"
#include "G4KleinNishinaCompton.hh"
#include "G4LivermoreRayleighModel.hh"
#include "G4MollerBhabhaModel.hh"
#include "G4UniversalFluctuation.hh"

#include "G4BraggModel.hh"                         // Standard processes for other particles
#include "G4IonFluctuations.hh"
#include "G4BetheBlochModel.hh"
#include "G4MuBetheBlochModel.hh"
#include "G4eCoulombScatteringModel.hh"
#include "G4BraggIonModel.hh"
#include "G4ICRU73QOModel.hh"

// PhysicsConstructorFactory
#include "G4PhysicsConstructorFactory.hh"
G4_DECLARE_PHYSCONSTR_FACTORY(EmStandardPhysics);

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EmStandardPhysics::EmStandardPhysics(G4int ver, const G4String&, const G4int matID)
: G4VPhysicsConstructor("EmStandard"), verbose(ver), fMatID(matID)
{
  G4EmParameters* param = G4EmParameters::Instance();
  param->SetDefaults();
  param->SetVerbose(verbose);
  SetPhysicsType(bElectromagnetic);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EmStandardPhysics::~EmStandardPhysics()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EmStandardPhysics::ConstructParticle()
{
  // Construct all the particles for the EM interactions.
  
  // Gamma
  G4Gamma::Gamma();
 
  // Leptons
  G4Electron::Electron();
  G4Positron::Positron();
  G4MuonPlus::MuonPlus();
  G4MuonMinus::MuonMinus();
 
  // Charged mesons
  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();
  G4KaonPlus::KaonPlusDefinition();
  G4KaonMinus::KaonMinusDefinition();

  // Charged barions
  G4Proton::Proton();
  G4AntiProton::AntiProton();
 
  // Charged ions
  G4Deuteron::Deuteron();
  G4Triton::Triton();
  G4He3::He3();
  G4Alpha::Alpha();
  G4GenericIon::GenericIonDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void EmStandardPhysics::ConstructProcess()
{
  // ============================================================================
  // 		Construct all the standard processes for the particles
  // ============================================================================
  
  // Define a PhysicsListHelper to register processes
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

  // muon & hadron bremsstrahlung and pair production
  G4MuBremsstrahlung* mub = new G4MuBremsstrahlung();
  G4MuPairProduction* mup = new G4MuPairProduction();
  G4hBremsstrahlung* pib = new G4hBremsstrahlung();
  G4hPairProduction* pip = new G4hPairProduction();
  G4hBremsstrahlung* kb = new G4hBremsstrahlung();
  G4hPairProduction* kp = new G4hPairProduction();
  G4hBremsstrahlung* pb = new G4hBremsstrahlung();
  G4hPairProduction* pp = new G4hPairProduction();
 
  // muon & hadron multiple scattering
  G4MuMultipleScattering* mumsc = new G4MuMultipleScattering();
  mumsc->SetEmModel(new G4WentzelVIModel());
  G4CoulombScattering* muss = new G4CoulombScattering();

  G4hMultipleScattering* pimsc = new G4hMultipleScattering();
  pimsc->SetEmModel(new G4WentzelVIModel());
  G4CoulombScattering* piss = new G4CoulombScattering();

  G4hMultipleScattering* kmsc = new G4hMultipleScattering();
  kmsc->SetEmModel(new G4WentzelVIModel());
  G4CoulombScattering* kss = new G4CoulombScattering();

  G4hMultipleScattering* hmsc = new G4hMultipleScattering("ionmsc");

  // high energy limit for e+- scattering models
  G4double highEnergyLimit = G4EmParameters::Instance()->MscEnergyLimit();
 
  // Add standard EM Processes
  G4ParticleTable* table = G4ParticleTable::GetParticleTable();

  for(const auto& particleName : partList.PartNames()) {
    G4ParticleDefinition* particle = table->FindParticle(particleName);
    if (!particle) { continue; }
    
    if (particleName == "gamma") {
      // Standard photon processes: photoelectric, Compton, Rayleigh
      G4PhotoElectricEffect* pee = new G4PhotoElectricEffect();
      pee->SetEmModel(new G4LivermorePhotoElectricModel());
      ph->RegisterProcess(pee, particle);
      ph->RegisterProcess(new G4ComptonScattering(), particle);
      ph->RegisterProcess(new G4RayleighScattering(), particle);

      // Use custom class to call the standard pair production process
      GammaConversion* GconvCrys = new GammaConversion("gammaconversion");
      ph->RegisterProcess(GconvCrys, particle);

    } else if (particleName == "e-") {
      // Standard electron processes: scattering, ionization, etc.
      G4eMultipleScattering* msc = new G4eMultipleScattering();
      G4UrbanMscModel* msc1 = new G4UrbanMscModel();
      G4WentzelVIModel* msc2 = new G4WentzelVIModel();
      msc1->SetHighEnergyLimit(highEnergyLimit);
      msc2->SetLowEnergyLimit(highEnergyLimit);
      msc->SetEmModel(msc1);
      msc->SetEmModel(msc2);

	  G4CoulombScattering* ss = new G4CoulombScattering();
      G4eCoulombScatteringModel* ssm = new G4eCoulombScatteringModel(); 
      ss->SetEmModel(ssm); 
      ss->SetMinKinEnergy(highEnergyLimit);
      ssm->SetLowEnergyLimit(highEnergyLimit);
      ssm->SetActivationLowEnergyLimit(highEnergyLimit);

      ph->RegisterProcess(msc, particle);
      ph->RegisterProcess(new G4eIonisation(), particle);
      ph->RegisterProcess(ss, particle);

      // Use custom class to call the standard bremsstrahlung process
      eBremsstrahlung_electron* eBs_e = new eBremsstrahlung_electron("electron_bs");
      ph->RegisterProcess(eBs_e, particle);

    } else if (particleName == "e+") {
      // Standard positron processes: scattering, ionization, etc.
      G4eMultipleScattering* msc = new G4eMultipleScattering();
      G4UrbanMscModel* msc1 = new G4UrbanMscModel();
      G4WentzelVIModel* msc2 = new G4WentzelVIModel();
      msc1->SetHighEnergyLimit(highEnergyLimit);
      msc2->SetLowEnergyLimit(highEnergyLimit);
      msc->SetEmModel(msc1);
      msc->SetEmModel(msc2);

      G4CoulombScattering* ss = new G4CoulombScattering();
      G4eCoulombScatteringModel* ssm = new G4eCoulombScatteringModel();       
      ss->SetEmModel(ssm); 
      ss->SetMinKinEnergy(highEnergyLimit);
      ssm->SetLowEnergyLimit(highEnergyLimit);
      ssm->SetActivationLowEnergyLimit(highEnergyLimit);

      ph->RegisterProcess(msc, particle);
      ph->RegisterProcess(new G4eIonisation(), particle);
      ph->RegisterProcess(ss, particle);

      // Add also the positron annihilation process
      ph->RegisterProcess(new G4eplusAnnihilation(), particle);
      
      // Use custom class to call the standard bremsstrahlung process
      eBremsstrahlung_positron* eBs_p = new eBremsstrahlung_positron("positron_bs");
      ph->RegisterProcess(eBs_p, particle);

    } else if (particleName == "mu+" ||
               particleName == "mu-"    ) {
      // Muon processes: scattering, ionization, etc.
      ph->RegisterProcess(mumsc, particle);
      ph->RegisterProcess(new G4MuIonisation(), particle);
      ph->RegisterProcess(mub, particle);
      ph->RegisterProcess(mup, particle);
      ph->RegisterProcess(muss, particle);

    } else if (particleName == "alpha" ||
               particleName == "He3") {
      // Ion processes: scattering and ionization
      ph->RegisterProcess(new G4hMultipleScattering(), particle);
      ph->RegisterProcess(new G4ionIonisation(), particle);

    } else if (particleName == "GenericIon") {
      // Ion processes: scattering and ionization
      ph->RegisterProcess(hmsc, particle);
      ph->RegisterProcess(new G4ionIonisation(), particle);

    } else if (particleName == "pi+" ||
               particleName == "pi-" ) {
      // Pion processes: scattering, ionization, etc
      ph->RegisterProcess(pimsc, particle);
      ph->RegisterProcess(new G4hIonisation(), particle);
      ph->RegisterProcess(pib, particle);
      ph->RegisterProcess(pip, particle);
      ph->RegisterProcess(piss, particle);

    } else if (particleName == "kaon+" ||
               particleName == "kaon-" ) {
      // Kaon processes: scattering, ionization, etc
      ph->RegisterProcess(kmsc, particle);
      ph->RegisterProcess(new G4hIonisation(), particle);
      ph->RegisterProcess(kb, particle);
      ph->RegisterProcess(kp, particle);
      ph->RegisterProcess(kss, particle);

    } else if (particleName == "proton" ||
         particleName == "anti_proton") {
      // Proton processes: scattering, ionization, etc
      G4hMultipleScattering* pmsc = new G4hMultipleScattering();
      pmsc->SetEmModel(new G4WentzelVIModel());

      ph->RegisterProcess(pmsc, particle);
      ph->RegisterProcess(new G4hIonisation(), particle);
      ph->RegisterProcess(pb, particle);
      ph->RegisterProcess(pp, particle);
      ph->RegisterProcess(new G4CoulombScattering(), particle);

    } else if (particleName == "B+" ||
               particleName == "B-" ||
               particleName == "D+" ||
               particleName == "D-" ||
               particleName == "Ds+" ||
               particleName == "Ds-" ||
               particleName == "anti_He3" ||
               particleName == "anti_alpha" ||
               particleName == "anti_deuteron" ||
               particleName == "anti_lambda_c+" ||
               particleName == "anti_omega-" ||
               particleName == "anti_sigma_c+" ||
               particleName == "anti_sigma_c++" ||
               particleName == "anti_sigma+" ||
               particleName == "anti_sigma-" ||
               particleName == "anti_triton" ||
               particleName == "anti_xi_c+" ||
               particleName == "anti_xi-" ||
               particleName == "deuteron" ||
               particleName == "lambda_c+" ||
               particleName == "omega-" ||
               particleName == "sigma_c+" ||
               particleName == "sigma_c++" ||
               particleName == "sigma+" ||
               particleName == "sigma-" ||
               particleName == "tau+" ||
               particleName == "tau-" ||
               particleName == "triton" ||
               particleName == "xi_c+" ||
               particleName == "xi-" ) {
      // Other particles: scattering, ionization
      ph->RegisterProcess(hmsc, particle);
      ph->RegisterProcess(new G4hIonisation(), particle);
    }
  }

  // Deexcitation
  G4VAtomDeexcitation* de = new G4UAtomicDeexcitation();
  G4LossTableManager::Instance()->SetAtomDeexcitation(de);

  G4EmModelActivator mact(GetPhysicsName());


  // ============================================================================
  // ============================================================================
  //                      STRONG FIELD PROCESSES IN TARGET REGIONS
  // ============================================================================
  // To activate the physics of oriented crystal only in target regions, 
  // it is necessary to do a few steps. First of all, invoke the EmConfigurator 
  // tool and pre-instantiate the EM models which will be used later on. 
  // The strategy will always be the same: we define a model to be applied to an 
  // already-declared process and then we use the SetExtraEmModel method of the 
  // EmConfigurator to declare that THIS model should be used in target regions.
  // Note: we must re-declare the models for the standard processes, otherwise 
  // they will not be activated in target regions.
  // For more details see the G4 example: extended/medical/dna/microdosimetry.
  
  G4EmConfigurator* em_config = G4LossTableManager::Instance()->EmConfigurator();
  G4VEmModel* mod;

  // Recall the G4EmParameters object, to know the kinetic energy limits 
  // to be applied to almost all the models.
  G4EmParameters* param = G4EmParameters::Instance();
  G4double emin = param->MinKinEnergy();
  G4double emax = param->MaxKinEnergy();
  
  G4RegionStore* regionStore = G4RegionStore::GetInstance(); //gpaterno
  G4String regionName;
  G4String SFregionTag = "Crystal";
  
  G4cout << G4endl;
  G4cout << "### Activate processes in SF_Regions ###" << G4endl; //gpaterno
  G4cout << "fMatID: " << fMatID << G4endl;
  G4cout << "Custom (modified) models with SF effects in Oriented Crystals: " << G4endl;

  // ============================================================================
  // Models for the SF interactions of electrons and positrons in target regions
  
  // Custom model for Bremsstrahlung
  mod = new SeltzerBergerModel_electron(fMatID);  
  G4double energyLimit = std::min(mod->HighEnergyLimit(), GeV);
  mod->SetLowEnergyLimit(emin);
  mod->SetHighEnergyLimit(energyLimit);
  mod->SetActivationLowEnergyLimit(emin);
  mod->SetActivationHighEnergyLimit(energyLimit);
  mod->SetSecondaryThreshold(param->BremsstrahlungTh());
  //mod->SetLPMFlag(false);
  G4EmParameters::Instance()->SetLPM(false);
  for (auto itr = regionStore->begin(); itr != regionStore->end(); itr++) {
    regionName = (*itr)->GetName();
		if (regionName.find(SFregionTag) != std::string::npos) {
			em_config->SetExtraEmModel("e-","electron_bs",mod,regionName);
			G4cout << "Activated custom SF SeltzerBerger model (electron_bs) for e- in " 
			       << regionName << G4endl;
		}
  }

  mod = new SeltzerBergerModel_positron(fMatID);
  mod->SetLowEnergyLimit(emin);
  mod->SetHighEnergyLimit(energyLimit);
  mod->SetActivationLowEnergyLimit(emin);
  mod->SetActivationHighEnergyLimit(energyLimit);
  mod->SetSecondaryThreshold(param->BremsstrahlungTh());
  //mod->SetLPMFlag(false);
  G4EmParameters::Instance()->SetLPM(false);
  for (auto itr = regionStore->begin(); itr != regionStore->end(); itr++) {
    regionName = (*itr)->GetName();
		if (regionName.find(SFregionTag) != std::string::npos) {
			em_config->SetExtraEmModel("e+","positron_bs",mod,regionName);
			G4cout << "Activated custom SF SeltzerBerger model (positron_bs) for e+ in " 
			       << regionName << G4endl;
		}
  }

  // Custom correction for high energy particles (from approx. 80 GeV onwards)
  if (emax > energyLimit) {    
    mod = new eBremsstrahlungRelModel_electron(fMatID);
    //mod->SetMaterial4correction(1);  //gpaterno, this does not work
    mod->SetLowEnergyLimit(energyLimit);
    mod->SetHighEnergyLimit(emax);
    mod->SetActivationLowEnergyLimit(energyLimit);
    mod->SetActivationHighEnergyLimit(emax);
    mod->SetSecondaryThreshold(param->BremsstrahlungTh());
    for (auto itr = regionStore->begin(); itr != regionStore->end(); itr++) {
      regionName = (*itr)->GetName();
		if (regionName.find(SFregionTag) != std::string::npos) {
			em_config->SetExtraEmModel("e-","electron_bs",mod,regionName);
			G4cout << "Activated custom SF Bremsstrahlung model (electron_bs) for e- in " 
			       << regionName << G4endl;
		}
    }

    mod = new eBremsstrahlungRelModel_positron(fMatID);
    mod->SetLowEnergyLimit(energyLimit);
    mod->SetHighEnergyLimit(emax);
    mod->SetActivationLowEnergyLimit(energyLimit);
    mod->SetActivationHighEnergyLimit(emax);
    mod->SetSecondaryThreshold(param->BremsstrahlungTh());
    for (auto itr = regionStore->begin(); itr != regionStore->end(); itr++) {
      regionName = (*itr)->GetName();
		if (regionName.find(SFregionTag) != std::string::npos) {
			em_config->SetExtraEmModel("e+","positron_bs",mod,regionName);
			G4cout << "Activated custom SF Bremsstrahlung model for (positron_bs) e+ in " 
			       << regionName << G4endl;
		}
    }
  }

  // ============================================================================
  // Models for the SF interactions of gamma photons in target regions
  emin = std::max(param->MinKinEnergy(), 2*CLHEP::electron_mass_c2);
  emax = param->MaxKinEnergy();
  
  // Custom model for Pair Production
  mod = new BetheHeitlerModel(fMatID);       
  energyLimit = std::min((mod->HighEnergyLimit()), 80*GeV);
  mod->SetLowEnergyLimit(emin);
  mod->SetHighEnergyLimit(energyLimit);
  mod->SetActivationLowEnergyLimit(emin);
  mod->SetActivationHighEnergyLimit(energyLimit);
  for (auto itr = regionStore->begin(); itr != regionStore->end(); itr++) {
    regionName = (*itr)->GetName();
		if (regionName.find(SFregionTag) != std::string::npos) {
			em_config->SetExtraEmModel("gamma","gammaconversion",mod,regionName);
			G4cout << "Activated custom SF BetheHeitler model (gammaconversion) in " 
			       << regionName << G4endl;
		}
  }
  
  // Custom correction for high energy particles (from approx. 80 GeV onwards)
  if (emax > energyLimit) {    
    mod = new PairProductionRelModel(fMatID);
    mod->SetLowEnergyLimit(energyLimit);
    mod->SetHighEnergyLimit(emax);
    mod->SetActivationLowEnergyLimit(energyLimit);
    mod->SetActivationHighEnergyLimit(emax);
    for (auto itr = regionStore->begin(); itr != regionStore->end(); itr++) {
      regionName = (*itr)->GetName();
		if (regionName.find(SFregionTag) != std::string::npos) {
			em_config->SetExtraEmModel("gamma","gammaconversion",mod,regionName);
			G4cout << "Activated custom SF PairProduction model (gammaconversion) in " 
			       << regionName << G4endl;
		}
	}
  }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
