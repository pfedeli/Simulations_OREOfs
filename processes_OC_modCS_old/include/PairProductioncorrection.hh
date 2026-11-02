#ifndef PAIRPRODUCTIONCORRECTION_H
#define PAIRPRODUCTIONCORRECTION_H
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

///
/// \brief The PairProductionCorrection class It is used for accumulate correction
/// functions of Pair Production
class PairProductionCorrection
{
public:
    PairProductionCorrection();
    /// PWO are old coefficients, PWO_001 are for axis 001 PWO
    enum axis_material {PWO=0, W111=1, PWO_001=2, W111_3mrad=3};
                        
    G4double  ComputeCorrection(G4double eKineticEnergy);
    void set_axis(axis_material a);
    void set_axis(G4int axisID=0); //gpaterno
    
protected:
    axis_material axis;
    G4double ComputeCorrectionPWO(G4double gammaEnergy);
    G4double ComputeCorrectionPWO_001(G4double gammaEnergy);
    G4double ComputeCorrectionW_111(G4double gammaEnergy);
    G4double ComputeCorrectionW_111_3mrad(G4double gammaEnergy);
};

#endif // PAIRPRODUCTIONCORRECTION_H
