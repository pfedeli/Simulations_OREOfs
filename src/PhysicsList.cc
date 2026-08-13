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
/// \file PhysicsList.cc
/// \brief Implementation of the PhysicsList class
//
// gpaterno, August 2026
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

//header of the class
#include "PhysicsList.hh"

//other classes
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

#include "G4ProcessManager.hh"
#include "G4Threading.hh"

//Particles
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleTableIterator.hh"

//EM Physics Lists
#include "G4EmStandardPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
//#include "G4EmPenelopePhysicsMI.hh"
#include "G4EmLowEPPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"

//EM options
#include "G4EmSaturation.hh"
#include "G4LossTableManager.hh"
#include "G4UAtomicDeexcitation.hh"

//Hadronic and Extra Physics Lists
#include "G4EmExtraPhysics.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4IonPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4HadronPhysicsFTFP_BERT.hh"
#include "FTFP_BERT.hh"

//Optical processes
#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpMieHG.hh"
#include "G4OpBoundaryProcess.hh"

//Decays
#include "G4Decay.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4PhysicsListHelper.hh"
#include "G4RadioactiveDecay.hh"
#include "G4NuclideTable.hh"

//Messenger
#include "G4GenericMessenger.hh"

//for OC physics
#include "G4RegionStore.hh"
#include "G4FastSimulationPhysics.hh"
#include "G4CoherentPairProductionPhysics.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList(G4int verb)
{
    G4cout << G4endl << "### PhysicsList instantiated ###" << G4endl; 
    G4cout << "Physics verbosity: " << verboseLevel << G4endl; 

    //Set verbosity
    SetVerboseLevel(verb);  

    //Define commands for this class
    DefineCommands();

    //Set defualt physics
    fDefaultPhysicsList = new FTFP_BERT(verboseLevel);
    fDefaultPhysicsList->ReplacePhysics(new G4EmLivermorePhysics());
    G4cout << "FTFP_BERT activated in all volumes" << G4endl; 
    G4cout << "replace EM physics with G4EmLivermorePhysics" << G4endl;
    
    //set OC Physics with FS model
    ActivateOCprocessesWithFSmodel();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList()
{ 
    delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructParticle()
{
    G4cout << "PhysicsList::ConstructParticle()" << G4endl;

    //Construct the particle of the default physics list.
    //There is no need to do the same for fStrongFieldPhysics.
    fDefaultPhysicsList->ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructProcess()
{     
    G4cout << "PhysicsList::ConstructProcess()" << G4endl;

    //Set Default physics from a ModularPhysicsList (it includes transportation)
    fDefaultPhysicsList->ConstructProcess();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts() //called automatically
{
    defaultCutValue = 0.1*CLHEP::mm;

    if (verboseLevel > 0) {
        G4cout << "called PhysicsList::SetCuts()" << G4endl;  
        G4cout << "Cuts set in the whole world: " 
             << defaultCutValue/mm << " mm" << G4endl << G4endl;
    }

    G4VUserPhysicsList::SetCuts();

    DumpCutValuesTable();
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ActivateCoherentPairProduction()
{
    fCoherentPairProduction = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ActivateOCprocessesWithFSmodel()
{
    //Create a helper tool used to activate the FastSimulation
    G4FastSimulationPhysics* fastSimulationPhysics = new G4FastSimulationPhysics();
    fastSimulationPhysics->BeVerbose();
    //Activate FastSimulation for particles having FastSimulation models
    fastSimulationPhysics->ActivateFastSimulation("e-");
    fastSimulationPhysics->ActivateFastSimulation("e+");
    fastSimulationPhysics->ActivateFastSimulation("pi-");
    fastSimulationPhysics->ActivateFastSimulation("pi+");
    fastSimulationPhysics->ActivateFastSimulation("mu-");
    fastSimulationPhysics->ActivateFastSimulation("mu+");
    fastSimulationPhysics->ActivateFastSimulation("proton");
    fastSimulationPhysics->ActivateFastSimulation("anti_proton");
    fastSimulationPhysics->ActivateFastSimulation("GenericIon");

    //Attach the FastSimulation constructor to the physics list
    fDefaultPhysicsList->RegisterPhysics(fastSimulationPhysics);

    G4cout << "PhysicsList::ActivateOCprocessesWithFSmodel()" << G4endl;

    //Coherent pair production model (new: September 2024, to test!)
    if (fCoherentPairProduction) {
        G4CoherentPairProductionPhysics* coherentPairProductionPhysics =
            new G4CoherentPairProductionPhysics();
        G4String crystal_region = "Crystal";
        coherentPairProductionPhysics->SetNameG4Region(crystal_region);
        fDefaultPhysicsList->RegisterPhysics(coherentPairProductionPhysics);

        G4cout << "G4CoherentPairProductionPhysics activated in " 
               << crystal_region << "!" <<G4endl;
    }
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::DefineCommands()
{
    //Directory
    fMessenger = new G4GenericMessenger(this,
                                        "/phys/",
                                        "PhysicsList control");

    //Commands

    fMessenger->DeclareMethod("setOCprocessesWithFSmodel",
                              &PhysicsList::ActivateOCprocessesWithFSmodel,
           "Activate physics of Oriented Crystals (OC) through FasSimModel.");

    fMessenger->DeclareMethod("setCoherentPairProduction",
                              &PhysicsList::ActivateCoherentPairProduction,
                "Activate Coherent pair production. It require FasSimModel.");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

