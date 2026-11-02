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
// This is a class of conversion gamma to electron-positron pair. 
// The electro-magnetic models were changed into new models.
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GammaConversion_h
#define GammaConversion_h 1

#include "globals.hh"
#include "G4VEmProcess.hh"
#include "G4Gamma.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4ParticleDefinition;
class G4VEmModel;
class G4MaterialCutsCouple;
class G4DynamicParticle;

class GammaConversion : public G4VEmProcess

{
public:  // with description

  explicit GammaConversion(const G4String& processName ="conv",
			               G4ProcessType type = fElectromagnetic);

  virtual ~GammaConversion();

  // true for Gamma only.
  virtual G4bool IsApplicable(const G4ParticleDefinition&) final;

  virtual G4double MinPrimaryEnergy(const G4ParticleDefinition*,
				                    const G4Material*) override;

  // Print few lines of informations about the process: validity range,
  //virtual void PrintInfo() override;

  // print documentation in html format
  virtual void ProcessDescription(std::ostream&) const override;

protected:

  /// The changing was there
  virtual void InitialiseProcess(const G4ParticleDefinition*) override;

private:
     
  GammaConversion & operator=(const  GammaConversion &right) = delete;
  GammaConversion(const  GammaConversion&) = delete;

  G4bool  isInitialised;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
#endif
 
