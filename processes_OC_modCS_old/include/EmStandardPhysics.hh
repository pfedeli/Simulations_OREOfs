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

#ifndef EmStandardPhysics_h
#define EmStandardPhysics_h 1
 
#include "G4VPhysicsConstructor.hh"
#include "globals.hh"
#include "G4EmParticleList.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EmStandardPhysics : public G4VPhysicsConstructor
{
public:

  explicit EmStandardPhysics(G4int ver=0, 
                             const G4String& name="", 
                             const G4int matID=0);
 
  virtual ~EmStandardPhysics();

  virtual void ConstructParticle();

  virtual void ConstructProcess();
 
 private:
   G4int  verbose;
   G4EmParticleList partList;
   
   G4int fMatID; //gpaterno
 }; 
 
 //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
 #endif
 
