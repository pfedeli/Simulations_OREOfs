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
    
    
    fCrystalMaterialCmd = new G4UIcmdWithAString("/crystal/setCrystalMaterial",this);  
    fCrystalMaterialCmd->SetGuidance("Set Crystal Material");
    fCrystalMaterialCmd->SetParameterName("matname",true); 
    
    fCrystalSizeCmd = new G4UIcmdWith3VectorAndUnit("/crystal/setCrystalSize",this);
    fCrystalSizeCmd->SetGuidance("Set Crystal size");
    fCrystalSizeCmd->SetParameterName("cryX","cryY","cryZ",false);
    fCrystalSizeCmd->SetUnitCategory("Length");
    fCrystalSizeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fCrystalLatticeCmd = new G4UIcmdWithAString("/crystal/setCrystalLattice",this);  
    fCrystalLatticeCmd->SetGuidance("Set Crystal Lattice");
    fCrystalLatticeCmd->SetParameterName("lattice",false);
      
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
    
    fDetectorVersion = new G4UIcmdWithAnInteger("/crystal/setDetectorVersion", this);
    fDetectorVersion->SetGuidance("set Detector version: 0: single crystal, 1: OREO.");
    fDetectorVersion->SetParameterName("DetectorVersion",true);
    //fDetectorVersion->SetDefaultValue(0);
    fDetectorVersion->SetRange("DetectorVersion>=0 && DetectorVersion<=1");
    
    fGapCmd = new G4UIcmdWithADoubleAndUnit("/crystal/setGap",this);
    fGapCmd->SetGuidance("Set Gap");
    fGapCmd->SetParameterName("gap",false);
    fGapCmd->SetUnitCategory("Length");
    fGapCmd->SetRange("gap>=0.");
    fGapCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fnCrystalsX = new G4UIcmdWithAnInteger("/crystal/nCrystalsX", this);
    fnCrystalsX->SetGuidance("set nCrystalsX");
    fnCrystalsX->SetParameterName("ncx",false);   
    fnCrystalsX->SetRange("ncx>=1 && ncx<=7");
    
    fnCrystalsY = new G4UIcmdWithAnInteger("/crystal/nCrystalsY", this);
    fnCrystalsY->SetGuidance("set nCrystalsY");
    fnCrystalsY->SetParameterName("ncy",false);   
    fnCrystalsY->SetRange("ncy>=1 && ncy<=7");
 
    
    fVirtualDetectorSizeCmd = 
        new G4UIcmdWith3VectorAndUnit("/crystal/setVirtualDetectorSize",this);
    fVirtualDetectorSizeCmd->SetGuidance("Set VirtualDetector size");
    fVirtualDetectorSizeCmd->SetParameterName("vdX","vdY","vdZ",false);
    fVirtualDetectorSizeCmd->SetUnitCategory("Length");
    fVirtualDetectorSizeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

DetectorConstructionMessenger::~DetectorConstructionMessenger()
{
    delete fCmdDir;
        
    delete fCrystalMaterialCmd;
    delete fCrystalSizeCmd;
    delete fCrystalLatticeCmd;
    delete fCrystalAngleXCmd;
    delete fCrystalAngleYCmd;
    delete fRadModelCmd;    
    delete fOCeffectsCmd;
    delete fPotentialPathCmd;
    delete fDetectorVersion;
    delete fGapCmd;
    delete fnCrystalsX;
    delete fnCrystalsY;
    
    delete fVirtualDetectorSizeCmd; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void DetectorConstructionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if (command == fCrystalMaterialCmd) 
        {fDetector->SetCrystalMaterial(newValue);}
    if (command == fCrystalSizeCmd) 
        {fDetector->SetCrystalSize(fCrystalSizeCmd->GetNew3VectorValue(newValue));}    
    if (command == fCrystalLatticeCmd) 
        {fDetector->SetCrystalLattice(newValue);}  
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
    if (command == fGapCmd) 
        {fDetector->SetCrystalGap(fGapCmd->GetNewDoubleValue(newValue));}
        
    if (command == fnCrystalsX)
        {fDetector->SetnCrystalsX(fnCrystalsX->GetNewIntValue(newValue));}
    if (command == fnCrystalsY)
        {fDetector->SetnCrystalsY(fnCrystalsY->GetNewIntValue(newValue));}
                
    if (command == fVirtualDetectorSizeCmd) 
        {fDetector->
            SetVirtualDetectorSize(fVirtualDetectorSizeCmd->GetNew3VectorValue(newValue));}           
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

