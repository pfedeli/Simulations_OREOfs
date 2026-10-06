#include "SteppingAction.hh"
#include "EventAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"


SteppingAction::SteppingAction(EventAction *eventAction) : fEventAction(eventAction)
{
}


void SteppingAction::UserSteppingAction(const G4Step *step)
{
    // get pre and post step points
    G4StepPoint *preStepPoint = step->GetPreStepPoint();
    G4StepPoint *postStepPoint = step->GetPostStepPoint();

    // get the current and next particle postion
    G4ThreeVector preStepPos = preStepPoint->GetPosition();
    G4ThreeVector postStepPos = postStepPoint->GetPosition();
    G4ThreeVector pos = preStepPos + G4UniformRand() * (postStepPos - preStepPos);

    // get the volume of the current step
    G4LogicalVolume *volume =
        preStepPoint->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    G4String volumeName = volume->GetName();

    // get track and particle name
    G4Track *track = step->GetTrack();
    G4String partName = track->GetDefinition()->GetParticleName();
    G4int trackID = track->GetTrackID();

    // get the energy deposited during in this step
    G4double edep = step->GetTotalEnergyDeposit();

    // get eventID
    G4int eventID =
        G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    // instantiating The Analysis Manager
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();


    // score the Edep in the Oreo crystals
    if (volumeName == "Crystal")
        {
         
            //G4cout << "volume is " << volumeName << G4endl;
            G4int crystalID = preStepPoint->GetTouchableHandle()->GetCopyNumber();
            //G4cout << "copy number = " << crystalID << G4endl;  
            fEventAction->AddEdep(edep, crystalID);
            // //G4cout << "edep: " << edep << G4endl;
            // if (edep > 0)
            //     G4cout << "[C] " << partName << " trk=" << trackID << " " << edep/MeV << G4endl;
        }
}
