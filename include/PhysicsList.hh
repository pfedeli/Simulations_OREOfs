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
/// \file PhysicsList.hh
/// \brief Definition of the PhysicsList class
//
// gpaterno, August 2026
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "G4VModularPhysicsList.hh"

class G4VPhysicsConstructor;
class G4GenericMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/// PhysicsList action class with a defualt EM physics and methods/commands to
/// activate Strong Field effects in Oriented Crystals.

class PhysicsList: public G4VUserPhysicsList
{
public:
    PhysicsList(G4int verb = 0);
    ~PhysicsList() override;

    void ConstructParticle() override;
    void ConstructProcess() override;
    void SetCuts() override;

    //custom methods
    void ActivateOCprocessesWithFSmodel();
    void ActivateCoherentPairProduction();

private:
    PhysicsList & operator = (const PhysicsList &right);
    PhysicsList(const PhysicsList&);
 
    G4VModularPhysicsList* fDefaultPhysicsList{nullptr};

    G4GenericMessenger* fMessenger{nullptr};
    void DefineCommands();
    
    G4bool fCoherentPairProduction = false;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

