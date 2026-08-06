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
/// \file DetectorConstruction.hh
/// \brief Description of the DetectorConstruction class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4ios.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include <vector>

#include "G4Region.hh"
#include "G4PVPlacement.hh"

#include "DetectorConstructionMessenger.hh"
#include "G4ChannelingFastSimModel.hh"

#define NSpheresMax 10000

class G4VPhysicalVolume;
class G4LogicalVolume;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    ~DetectorConstruction() override = default;

    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;
    
    //method to get the scoring volumes
    std::vector<G4LogicalVolume*> GetScoringVolume() const {
        return fScoringVolume;}        
       
    //methods to set the Crystal (Radiator) features
    void SetCrystalSize(G4ThreeVector val) {fCrystalSize = val;}
    void SetCrystalLattice(G4String val) {fLattice = val;}
    void SetCrystalAngleX(G4double val) {fAngleX = val;}
    void SetCrystalAngleY(G4double val) {fAngleY = val;}
    G4double GetCrystalZ() const {return fCrystalZ;}
    void SetRadiationModel(G4bool val) {fActivateRadiationModel = val;}
    void SetOCeffects(G4bool val) {fActivateOCeffects = val;}
    G4bool GetOCeffects() const {return fActivateOCeffects;}
    G4LogicalVolume* GetCrystalVolume() const {return fCrystalLogic;}
    void SetPotentialPath(const G4String path){fPotentialPath = path;}
    void SetDetectorVersion(const G4int version){fDetectorVersion = version;}
       
    //methods to set/Get the Virtual Detector features
    void SetVirtualDetectorSize(G4ThreeVector val) {fVirtualDetectorSize = val;}
    std::vector<G4ThreeVector> GetVirtualDetectorPositionVector() const {
        return fVirtualDetectorPositionVector;}
       
    //methods to set and get ScoreCrystalExit (27/09/2024)
    void SetScoringCrystalExit(G4bool bval) {fScoringCrystalExit = bval;} 
    G4bool GetScoringCrystalExit() const {return fScoringCrystalExit;}
        
protected:
  std::vector<G4LogicalVolume*> fScoringVolume; //for spheres only

private:
    DetectorConstructionMessenger* fMessenger;  
            
    G4Region* fCrystalRegion{nullptr};
    G4LogicalVolume* fCrystalLogic{nullptr};
    G4Material* fCrystalMaterial{nullptr};
    G4ThreeVector fCrystalSize = G4ThreeVector(25.*mm, 25.*mm, 45.*mm);
    G4String fLattice = "<111>";  
    G4double fAngleX = 0.e-6; //rad
    G4double fAngleY = 0.e-6; //rad
    G4double fCrystalZ = 0.;
    G4bool fActivateRadiationModel = true;
    G4bool fActivateOCeffects = true;
    //G4String fPotentialPath = "/Users/pierluigifedeli/Simulations/Simulations_code/OREOfs/potentialPath/";
    G4String fPotentialPath = "/Users/pierluigifedeli/Simulations/Simulations_code/OREOfs/potentialPath/";
    G4int fDetectorVersion = 0;
    G4double crystalGap = 0.1 *mm;

    G4ThreeVector fVirtualDetectorSize = G4ThreeVector(40.*cm, 40.*cm, 0.01*mm);
    std::vector<G4ThreeVector> fVirtualDetectorPositionVector;
    G4LogicalVolume* fVirtualDetectorLogic0{nullptr};
    G4LogicalVolume* fVirtualDetectorLogic1{nullptr};
    G4LogicalVolume* fVirtualDetectorLogic2{nullptr};
            
    G4bool fScoringCrystalExit = false;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
