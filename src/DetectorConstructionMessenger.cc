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
// gpaterno, October 2025
//
/// \file DetectorConstructionMessenger.cc
/// \brief Implementation of the DetectorConstruction messenger class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstructionMessenger.hh"
#include "DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"

#include "G4RunManager.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstructionMessenger::DetectorConstructionMessenger(DetectorConstruction* det):
fDetector(det)
{
    fCmdDir = new G4UIdirectory("/crystal/");
    fCmdDir->SetGuidance("crystal Control");    
      
    fCrystalAngleXCmd = new G4UIcmdWithADouble("/crystal/setCrystalAngleX",this);
    fCrystalAngleXCmd->SetGuidance("Set crystal orientation with respet to the beam");
    fCrystalAngleXCmd->SetParameterName("angX",false);
    fCrystalAngleXCmd->AvailableForStates(G4State_PreInit,G4State_Idle); 
    
    fCrystalAngleYCmd = new G4UIcmdWithADouble("/crystal/setCrystalAngleY",this);
    fCrystalAngleYCmd->SetGuidance("Set crystal orientation with respet to the beam");
    fCrystalAngleYCmd->SetParameterName("angY",false);
    fCrystalAngleYCmd->AvailableForStates(G4State_PreInit,G4State_Idle); 
    
    fRadModelCmd = new G4UIcmdWithABool("/crystal/setRadiationModel", this);
    fRadModelCmd->SetGuidance("set Radiation Model");      
    fRadModelCmd->SetParameterName("ActivateRadiationModel",true);
    fRadModelCmd->SetDefaultValue(false);  
    
    fOCeffectsCmd = new G4UIcmdWithABool("/crystal/setOCeffects", this);
    fOCeffectsCmd->SetGuidance("set Oriented Crystal effects");      
    fOCeffectsCmd->SetParameterName("OCeffects",true);
    fOCeffectsCmd->SetDefaultValue(false);
    
    fPotentialPathCmd = new G4UIcmdWithAString("/crystal/setChannelingDataPath",this);
    fPotentialPathCmd->
            SetGuidance("Set the path where to find the available data "
                        "for the G4ChannelingFastSimModel "
                        "if different from G4CHANNELINGDATA");
    fPotentialPathCmd->SetParameterName("channelingDataPath",false);
    fPotentialPathCmd->SetDefaultValue("");       
    
    fVirtualDetectorSizeCmd = 
        new G4UIcmdWith3VectorAndUnit("/crystal/setVirtualDetectorSize",this);
    fVirtualDetectorSizeCmd->SetGuidance("Set VirtualDetector size");
    fVirtualDetectorSizeCmd->SetParameterName("vdX","vdY","vdZ",false);
    fVirtualDetectorSizeCmd->SetUnitCategory("Length");
    fVirtualDetectorSizeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    
    fScoringCrystalExitCmd = new G4UIcmdWithABool("/det/setScoringCrystalExit",this);
    fScoringCrystalExitCmd->SetGuidance("set IWantScoringCrystalExit");      
    fScoringCrystalExitCmd->SetParameterName("IWantScoringCrystalExit",true);
    fScoringCrystalExitCmd->SetDefaultValue(false);
    fScoringCrystalExitCmd->AvailableForStates(G4State_PreInit,G4State_Idle);      

    fDetectorVersion = new G4UIcmdWithAnInteger("/crystal/setDetectorVersion", this);
    fDetectorVersion->SetGuidance("set Detector version");      
    fDetectorVersion->SetParameterName("DetectorVersion",true);
    fDetectorVersion->SetDefaultValue(0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

DetectorConstructionMessenger::~DetectorConstructionMessenger()
{
    delete fCmdDir;
        
    delete fCrystalAngleXCmd;
    delete fCrystalAngleYCmd;
    delete fRadModelCmd;    
    delete fOCeffectsCmd;
    delete fPotentialPathCmd;
    
    delete fFieldValueCmd;
    delete fFieldRegionLengthCmd;
    
    
    delete fVirtualDetectorSizeCmd;
    
    delete fScoringCrystalExitCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void DetectorConstructionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{

    if (command == fCrystalAngleXCmd) 
        {fDetector->SetCrystalAngleX(fCrystalAngleXCmd->GetNewDoubleValue(newValue));}    
    if (command == fCrystalAngleYCmd) 
        {fDetector->SetCrystalAngleY(fCrystalAngleYCmd->GetNewDoubleValue(newValue));}         
    if (command == fRadModelCmd) 
        {fDetector->SetRadiationModel(fRadModelCmd->GetNewBoolValue(newValue));}    
    if (command == fOCeffectsCmd) 
        {fDetector->SetOCeffects(fOCeffectsCmd->GetNewBoolValue(newValue));}
    if (command == fPotentialPathCmd) 
        {fDetector->SetPotentialPath(newValue);}  
    if (command == fDetectorVersion)
        {fDetector->SetDetectorVersion(fDetectorVersion->GetNewIntValue(newValue));}

    if (command == fVirtualDetectorSizeCmd) 
        {fDetector->
            SetVirtualDetectorSize(fVirtualDetectorSizeCmd->GetNew3VectorValue(newValue));}
    if (command == fScoringCrystalExitCmd) 
        {fDetector->
            SetScoringCrystalExit(fScoringCrystalExitCmd->GetNewBoolValue(newValue));}             
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

