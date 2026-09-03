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
// gpaterno, August 2026
//
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Transform3D.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4VSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
#include "G4RegionStore.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

#include "G4SubtractionSolid.hh"

#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4PSDoseDeposit.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4SDParticleFilter.hh"

#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include <G4ChordFinder.hh>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
{
    // instantiate the messenger
    fMessenger = new DetectorConstructionMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume *DetectorConstruction::Construct()
{
    G4cout << G4endl << "### DetectorConstruction::Construct() ###" << G4endl
           << G4endl;

    // check overlap option
    G4bool checkOverlaps = true;

    // Materials
    G4NistManager *nist = G4NistManager::Instance();
    G4Material *Silicon = nist->FindOrBuildMaterial("G4_Si");
    G4Material *PWO = nist->FindOrBuildMaterial("G4_PbWO4");
    G4Material *Diamond = nist->FindOrBuildMaterial("G4_C");
    G4Material *Tungsten = nist->FindOrBuildMaterial("G4_W");
    G4Material *Iridium = nist->FindOrBuildMaterial("G4_Ir");
    G4Material *Copper = nist->FindOrBuildMaterial("G4_Cu");
    G4Material *Germanium = nist->FindOrBuildMaterial("G4_Ge");
    G4Element *elBi = nist->FindOrBuildElement("Bi");
    G4Element *elGe = nist->FindOrBuildElement("Ge");
    G4Element *elO = nist->FindOrBuildElement("O");
    G4Material *BGO = new G4Material("G4_BGO", 7.13 * g / cm3, 3);
    BGO->AddElement(elBi, 0.671054);
    BGO->AddElement(elGe, 0.17482);
    BGO->AddElement(elO, 0.154126);

    // Vacuum
    G4double z = 7.;
    G4double a = 14.007 * CLHEP::g / CLHEP::mole;
    G4double density = CLHEP::universe_mean_density;
    G4double pressure = 1.E-6 * 1.E-3 * CLHEP::bar; // 10-6 mbar
    G4double temperature = 300. * CLHEP::kelvin;    // 300 K
    G4Material *Vacuum = new G4Material("Vacuum",
                                        z,
                                        a,
                                        density,
                                        kStateGas,
                                        temperature,
                                        pressure);

    
    // ------------------- World -------------------------
    G4Box *solidWorld = new G4Box("World", 3. * m, 3. * m, 20. * m);

    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld,
                                                      Vacuum,
                                                      "World");

    logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());

    G4VPhysicalVolume *physWorld = new G4PVPlacement(0,               // no rotation
                                                     G4ThreeVector(), // centre position
                                                     logicWorld,      // its logical volume
                                                     "World",         // its name
                                                     0,               // its mother volume
                                                     false,           // no boolean operation
                                                     0,               // copy number
                                                     checkOverlaps);  // overlaps checking

    // --------------- OREO  -------------------------
    // set visualization attributes
    G4VisAttributes *CrystalVisAttribute =
        new G4VisAttributes(G4Colour(0., 0., 1., 0.8));
    CrystalVisAttribute->SetForceSolid(true);
        
    // Crystal region (necessary for the FastSim model)
    fCrystalRegion = new G4Region("Crystal");
    
    //Select crystal material
    if (fCrystalMaterialStr == "PWO") {
        fCrystalMaterial = PWO;
    } else if (fCrystalMaterialStr == "BGO") {
        fCrystalMaterial = BGO;
    } else if (fCrystalMaterialStr == "C") {
        fCrystalMaterial = Diamond;
    } else if (fCrystalMaterialStr == "W") {
        fCrystalMaterial = Tungsten;
    } else if (fCrystalMaterialStr == "Ir") {
        fCrystalMaterial = Iridium;
    } else if (fCrystalMaterialStr == "Cu") {
        fCrystalMaterial = Copper;
    } else if (fCrystalMaterialStr == "Ge") {
        fCrystalMaterial = Germanium;
    } else {
        fCrystalMaterial = Silicon;
    } 
    
    // Detector version
    G4cout << "detector version is: " << fDetectorVersion << G4endl;
    G4double tollfCrystalZ = 0. * mm;

    if (fDetectorVersion == 0)
    {
        G4cout << "Detector version 0 (default)" << G4endl;
        //fCrystalMaterial = Tungsten;
        //fCrystalSize = G4ThreeVector(25.*mm, 25.*mm, 25.*mm);
        if (fAngleX != 0 || fAngleY != 0)
        {
            tollfCrystalZ = 0.15 * mm;
        }
        fCrystalZ = -fCrystalSize.z() * 0.5 - tollfCrystalZ;

        // Crystal solid and logic volumes
        G4Box *crystalSolid = new G4Box("Crystal",
                                        fCrystalSize.x() * 0.5,
                                        fCrystalSize.y() * 0.5,
                                        fCrystalSize.z() * 0.5);

        fCrystalLogic[0] = new G4LogicalVolume(crystalSolid,
                                              fCrystalMaterial,
                                              "Crystal");
        
        fCrystalLogic[0]->SetVisAttributes(CrystalVisAttribute);
        fCrystalRegion->AddRootLogicalVolume(fCrystalLogic[0]);
        fScoringVolume.push_back(fCrystalLogic[0]);

        G4ThreeVector posCrystal = G4ThreeVector(0. * mm, 0. * mm, fCrystalZ);

        G4RotationMatrix *crystalRotationMatrix = new G4RotationMatrix;
        crystalRotationMatrix->rotateY(-fAngleX);
        crystalRotationMatrix->rotateX(-fAngleY);

        // Crystal placement
        new G4PVPlacement(crystalRotationMatrix,
                          posCrystal,
                          fCrystalLogic[0],
                          "Crystal",
                          logicWorld,
                          false,
                          0,
                          checkOverlaps);
    }

    else if (fDetectorVersion == 1)
    {
        G4cout << "Detector version 1 (OREO)" << G4endl;
        //fCrystalMaterial = Tungsten;
        //fCrystalMaterial = PWO;
        //fLattice = "<111>";
        //fCrystalSize = G4ThreeVector(25.*mm, 25.*mm, 45.*mm);
        if (fAngleX != 0 || fAngleY != 0)
        {
            tollfCrystalZ = 0.15 * mm;
        }
        fCrystalZ = -fCrystalSize.z() * 0.5 - tollfCrystalZ;

        //const G4int nCrystalsX = 3; //they are class members now
        //const G4int nCrystalsY = 3;
        G4double crystalPitchX = fCrystalSize.x() + fCrystalGap;
        G4double crystalPitchY = fCrystalSize.y() + fCrystalGap;

        G4Box *crystalSolid = new G4Box("Crystal",
                                        fCrystalSize.x() * 0.5,
                                        fCrystalSize.y() * 0.5,
                                        fCrystalSize.z() * 0.5);

        //fCrystalLogic = new G4LogicalVolume(crystalSolid, fCrystalMaterial, "Crystal");
        //fCrystalLogic->SetVisAttributes(CrystalVisAttribute);

        std::stringstream crystalID;
        G4String crystalName = "Crystal";
        G4int copyNo = 0;
        for (G4int ix = 0; ix < nCrystalsX; ix++)
        {
            for (G4int iy = 0; iy < nCrystalsY; iy++)
            {
                crystalID << copyNo;
                //crystalName = "Crystal_" + crystalID.str();
            
                fCrystalLogic[copyNo] = new G4LogicalVolume(crystalSolid, 
                                                            fCrystalMaterial, 
                                                            crystalName);
                
                fCrystalLogic[copyNo]->SetVisAttributes(CrystalVisAttribute);
                fCrystalRegion->AddRootLogicalVolume(fCrystalLogic[copyNo]);
                fScoringVolume.push_back(fCrystalLogic[copyNo]);                

                //G4double x = (ix - 1) * crystalPitchX; // -1,0,1 -> centrato
                //G4double y = (iy - 1) * crystalPitchY;
                G4double dx = 0;
                G4double dy = 0;
                if (nCrystalsX % 2 == 0) {dx = 0.5;}
                if (nCrystalsY % 2 == 0) {dy = 0.5;}                  
                G4double x = (ix-floor(nCrystalsX*0.5)+dx) * crystalPitchX;
                G4double y = (iy-floor(nCrystalsY*0.5)+dy) * crystalPitchY;
                
                G4ThreeVector posCrystal(x, y, fCrystalZ);

                // rotazione: se tutti orientati allo stesso modo rispetto al fascio,
                // basta clonare la stessa matrice per ognuno (è relativa al frame locale)
                G4RotationMatrix *rot = new G4RotationMatrix;
                rot->rotateY(-fAngleX);
                rot->rotateX(-fAngleY);

                new G4PVPlacement(rot,
                                  posCrystal,
                                  fCrystalLogic[copyNo],
                                  crystalName,
                                  logicWorld,
                                  false,
                                  copyNo,
                                  checkOverlaps);
                copyNo++;
                crystalID.str("");
            }
        }
    }
    else
    {
        G4ExceptionDescription msg;
        msg << "fDetectorVersion = " << fDetectorVersion << " invalid (expected 0 or 1)";
        G4Exception("DetectorConstruction::Construct", "InvalidDetectorVersion",
                    FatalException, msg);
    }

    // Print Crystal info
    G4cout << "Crystal material: " << fCrystalMaterial->GetName() << G4endl;
    G4cout << "Crystal size: " << fCrystalSize.x() / mm
           << "x" << fCrystalSize.y() / mm
           << "x" << fCrystalSize.z() / mm << " mm3" << G4endl;
    G4cout << "CrystalZ: " << fCrystalZ / mm << " mm" << G4endl;
    G4cout << G4endl;


    // --------------- virtual Detectors -----------------
    // position
    G4double VirtualDetector0Z = fVirtualDetectorSize.z() * 0.5;
    G4ThreeVector posVirtualDetector0 = G4ThreeVector(0, 0, VirtualDetector0Z);
    G4ThreeVector frontVirtualDetector0 = G4ThreeVector(0, 0, 
                                                VirtualDetector0Z - fVirtualDetectorSize.z() * 0.5);
    G4cout << "VirtualDetector0Z: " << VirtualDetector0Z / mm << " mm" << G4endl << G4endl;
    fVirtualDetectorPositionVector.push_back(frontVirtualDetector0);

    // virtual Detector volume
    G4Box *VirtualDetectorSolid = new G4Box("VirtualDetector",
                                            fVirtualDetectorSize.x() * 0.5,
                                            fVirtualDetectorSize.y() * 0.5,
                                            fVirtualDetectorSize.z() * 0.5);

    fVirtualDetectorLogic0 = new G4LogicalVolume(VirtualDetectorSolid,
                                                 Vacuum,
                                                 "VirtualDetector0");

    G4VisAttributes *VirtualDetectorVisAttribute =
        new G4VisAttributes(G4Colour(1., 1., 1.));
    VirtualDetectorVisAttribute->SetForceSolid(false);
    fVirtualDetectorLogic0->SetVisAttributes(VirtualDetectorVisAttribute);

    new G4PVPlacement(0,
                      posVirtualDetector0,
                      fVirtualDetectorLogic0,
                      "VirtualDetector0",
                      logicWorld,
                      false,
                      0,
                      checkOverlaps);

 
    // always return the physical World
    return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
    // activate OC effects through FastSim model
    if (fActivateOCeffects)
    {
        G4RegionStore *regionStore = G4RegionStore::GetInstance();
        G4Region *RegionCh = regionStore->GetRegion("Crystal");

        G4ChannelingFastSimModel *ChannelingModel =
            new G4ChannelingFastSimModel("ChannelingModel", RegionCh);

        if (fCrystalMaterial->GetName() == "G4_W")
        {
            ChannelingModel->Input(fCrystalLogic[0]->GetMaterial(), fLattice);
        }
        else
        {
            ChannelingModel->Input(fCrystalLogic[0]->GetMaterial(), fLattice, fPotentialPath);
            G4cout << "fPotentialPath: " << fPotentialPath << G4endl;
        }
        G4double fParticleLEth = 200. * MeV; // deafult 200.*MeV (5.*GeV -> much faster)
        G4double fLindhardAngles = 100;      // default 100
        ChannelingModel->SetLowKineticEnergyLimit(fParticleLEth, "e-");
        ChannelingModel->SetLowKineticEnergyLimit(fParticleLEth, "e+");
        ChannelingModel->SetLindhardAngleNumberHighLimit(fLindhardAngles, "e-");
        ChannelingModel->SetLindhardAngleNumberHighLimit(fLindhardAngles, "e+");
        
        G4double fHighAngleLimit = 0.; //rad
        ChannelingModel->SetDefaultHighAngleLimit(fHighAngleLimit);
        //NOTE: The actual angular cut, for each particle, is the Max of fHighAngleLimit and 
        //the number of LindhardAngles times the corresponding Lindhard angle itself.

        G4cout << G4endl;
        G4cout << "Oriented Crystal effects set through FastSim model" << G4endl;
        //G4cout << "Crystal Lattice: " << fLattice << G4endl;
        G4cout << "Crystal AngleX: " << fAngleX << " rad" << G4endl;
        G4cout << "Crystal AngleY: " << fAngleY << " rad" << G4endl;
        G4cout << "fParticleLEth: " << fParticleLEth / MeV << " MeV" << G4endl;
        G4cout << "fLindhardAngles: " << fLindhardAngles << G4endl;
        G4cout << "fHighAngleLimit: " << fHighAngleLimit*1e3 << " mrad" << G4endl;
        G4cout << "ActivateRadiationModel: " << fActivateRadiationModel << G4endl;

        if (fActivateRadiationModel)
        {
            ChannelingModel->RadiationModelActivate();
            G4int fSamplingPhotonsNumber = 150;        // default 150
            G4int fNSmallTrajectorySteps = 10000;      // default 10000
            G4double fRadiactionAngleFactor = 4.;      // deafult 4
            G4double fSinglePhotonRadProbLimit = 0.25; // default 0.25
            G4double fLEthreshold = 1. * MeV;
            ChannelingModel->GetRadiationModel()->SetSamplingPhotonsNumber(fSamplingPhotonsNumber);
            ChannelingModel->GetRadiationModel()->SetNSmallTrajectorySteps(fNSmallTrajectorySteps);
            ChannelingModel->GetRadiationModel()->SetRadiationAngleFactor(fRadiactionAngleFactor);
            ChannelingModel->GetRadiationModel()
                ->SetSinglePhotonRadiationProbabilityLimit(fSinglePhotonRadProbLimit);
            ChannelingModel->GetRadiationModel()
                ->SetSpectrumEnergyRange(fLEthreshold, 20. * GeV, 100);

            G4cout << "SamplingPhotonsNumber: "
                   << fSamplingPhotonsNumber << G4endl;
            G4cout << "NSmallTrajectorySteps: "
                   << fNSmallTrajectorySteps << G4endl;
            G4cout << "fRadiactionAngleFactor: "
                   << fRadiactionAngleFactor << G4endl;
            G4cout << "fSinglePhotonRadProbLimit: "
                   << fSinglePhotonRadProbLimit << G4endl;
            G4cout << "Low Eenergy threshold to emit photons and record their energy: "
                   << fLEthreshold / MeV << " MeV" << G4endl << G4endl;
        }
        else
        {
            G4cout << G4endl;
        }
    }


    // Sensitive Volumes (Virtual Detectors)
    G4VSensitiveDetector *vDetector = new SensitiveDetector("det");
    G4SDManager::GetSDMpointer()->AddNewDetector(vDetector);
    fVirtualDetectorLogic0->SetSensitiveDetector(vDetector);


    G4cout << "### End of DetectorConstruction ###" << G4endl << G4endl << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
