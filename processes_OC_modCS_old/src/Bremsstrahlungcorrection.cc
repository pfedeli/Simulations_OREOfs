#include "Bremsstrahlungcorrection.hh"

BremsstrahlungCorrection::BremsstrahlungCorrection()
{
	axis=PWO;
}

void BremsstrahlungCorrection::set_axis(axis_material a)
{
    axis=a;
}

void BremsstrahlungCorrection::set_axis(G4int axisID)
{
	switch (axisID) {
		case 0:
			axis=PWO;
			G4cout << "Set PWO axis" << G4endl;
			break;
		case 1:
			axis=W111;
			G4cout << "Set W111 axis, but no correction applied!" << G4endl;
			break;
		case 2:
			axis=PWO_001;
			G4cout << "Set PWO_001 axis" << G4endl;
			break;
		case 3:
			axis=W111_electron;
			G4cout << "Set W111_electron axis" << G4endl;
			break;
		case 4:
			axis=W111_positron;
			G4cout << "Set W111_positron axis" << G4endl;
			break;
		case 5:
			axis=W111_3mrad_electron;
			G4cout << "Set W111_3mrad_electron axis" << G4endl;
			break;
		case 6:
			axis=W111_3mrad_positron;
			G4cout << "Set W111_3mrad_positron axis" << G4endl;
			break;
	}
}

G4double BremsstrahlungCorrection::ComputeCorrection(G4double eKineticEnergy)
{
	switch (static_cast<int>(axis)) {
		case PWO:
			return ComputeCorrectionPWO(eKineticEnergy);
			break;
		case W111:
		    G4cout << "Set W111 axis, but no correction applied!" << G4endl;
			return 1;
			break;
		case PWO_001:
			return ComputeCorrectionPWO_001(eKineticEnergy);
			break;
		case W111_electron:
		    return ComputeCorrectionW_111_electron(eKineticEnergy);
			break;
		case W111_positron:
		    return ComputeCorrectionW_111_positron(eKineticEnergy);
			break;
		case W111_3mrad_electron:
		    return ComputeCorrectionW_111_3mrad_electron(eKineticEnergy);
			break;
		case W111_3mrad_positron:
		    return ComputeCorrectionW_111_3mrad_positron(eKineticEnergy);
			break;
		default:
		    printf("\nWrong type of axis and material %d.\n", axis);
		    return 1;
		    break;
    }
}

G4double BremsstrahlungCorrection::ComputeCorrectionPWO(G4double eKineticEnergy)
{

    //calculate correction for cross section
    //  approximation from 0 to 1 TeV  by  polynomial
    G4double value=1;
    G4double energyG;

    energyG=eKineticEnergy/GeV;
    if(energyG<5)
    {
       value=1.0 + 0.075 *energyG + 0.025*energyG*energyG;
    }
    else
    {
        if(energyG<20)
        {
            value=1.016 + 0.2042*energyG - 0.00148*energyG*energyG ;
        }
        else
        {
            if(energyG<60)
            {
                value=1.145 + 0.248617*energyG - 0.00462*energyG*energyG + 0.0000298333*energyG*energyG*energyG ;
            }
            else
            {
                if(energyG<150)
                {
                    value=1.42824 + 0.128068*energyG - 0.00108813*energyG*energyG + 3.14325e-6*energyG*energyG*energyG;
                }
                else
                {
                    if(energyG<400)
                    {
                        value=6.408 + 0.00170583*energyG + 6.675e-6*energyG*energyG - 1.48333e-8*energyG*energyG*energyG;
                    }
                    else
                    {
                        value=5.67375 + 0.00751086*energyG - 0.0000114588*energyG*energyG + 5.69229e-9*energyG*energyG*energyG;
                    }
                }
            }
        }
    }
    return value;
}

G4double BremsstrahlungCorrection::ComputeCorrectionPWO_001(G4double eKineticEnergy)
{

    //calculate correction for cross section
    //  approximation from 0 to 1 TeV  by  polynomial
    G4double value=1;
    G4double energyG;

    energyG=eKineticEnergy/GeV;
    if(energyG<1)
    {
       value=1.0;
    }
    else
    {
        if(energyG<18.41975)
        {
            value=0.654271 + 0.465334*energyG -0.0361848*energyG*energyG+0.0010645*energyG*energyG*energyG;
        }
        else
        {
            if(energyG<200.84059)
            {
                value=2.84864 + 0.0242124*energyG -0.000169524*energyG*energyG + 3.81253e-07*energyG*energyG*energyG;
            }
            else
            {
                    value=4.13753 -0.000838656*energyG +6.4116e-07*energyG*energyG -1.97439e-10*energyG*energyG*energyG;
            }
        }
    }
    return value;
}

G4double BremsstrahlungCorrection::ComputeCorrectionW_111_electron(G4double eKineticEnergy)
{
    //calculate correction for cross section
    //  approximation from 0 to 1 TeV  by  polynomial
    G4double value=1;
    G4double energyG;

    energyG=eKineticEnergy/GeV;
    if(energyG<1)
    {
        value=1.0;
    }
    else
    {
        if(energyG<9.5)
        {
            value=2.47222 + 0.628284*energyG -0.0366603*energyG*energyG;
        }
        else
        {
            if(energyG<45)
            {
                value=4.66268 + 0.0633186*energyG -0.000729992*energyG*energyG;
            }
            else
            {
                value=6.1775 +3.65186e-05*energyG -4.19386e-06*energyG*energyG;
            }
        }
    }
    return value;
}
G4double BremsstrahlungCorrection::ComputeCorrectionW_111_positron(G4double eKineticEnergy)
{
    //calculate correction for cross section
    //  approximation from 0 to 1 TeV  by  polynomial
    G4double value=1;
    G4double energyG;

    energyG=eKineticEnergy/GeV;
    if(energyG<1)
    {
        value=1.0;
    }
    else
    {
        if(energyG<8.5)
        {
            value=0.449704 + 1.03147*energyG -0.0681459*energyG*energyG;
        }
        else
        {
            if(energyG<16)
            {
                value=1.25058 + 0.488534*energyG -0.0144321*energyG*energyG;
            }
            else
            {
                if(energyG<32)
                {
                    value=7.07627 -0.176846*energyG +0.00455684*energyG*energyG;
                }
                else
                {
                    value=5.99231 +0.00389363*energyG -1.67781e-05*energyG*energyG;
                }
            }
        }
    }
    return value;
}

G4double BremsstrahlungCorrection::ComputeCorrectionW_111_3mrad_electron(G4double eKineticEnergy)
{
    //calculate correction for cross section
    //  approximation from 0 to 1 TeV  by  polynomial
    G4double value=1;
    G4double energyG;

    energyG=eKineticEnergy/GeV;
    if(energyG<1)
    {
        value=1.0;
    }
    else
    {
        if(energyG<9.5)
        {
            value=1.82337 + 0.487443*energyG -0.0319659*energyG*energyG;
        }
        else
        {
            if(energyG<45)
            {
                value=3.72712 + 0.0177542*energyG -0.00016182*energyG*energyG;
            }
            else
            {
                value=4.33585 -0.00319053*energyG +5.56825e-07*energyG*energyG;
            }
        }
    }
    return value;
}
G4double BremsstrahlungCorrection::ComputeCorrectionW_111_3mrad_positron(G4double eKineticEnergy)
{
    //calculate correction for cross section
    //  approximation from 0 to 1 TeV  by  polynomial
    G4double value=1;
    G4double energyG;

    energyG=eKineticEnergy/GeV;
    if(energyG<1)
    {
        value=1.0;
    }
    else
    {
        if(energyG<8.5)
        {
            value=1.45762 + 0.614643*energyG -0.0411202*energyG*energyG;
        }
        else
        {
            if(energyG<16)
            {
                value=2.96736 + 0.114422*energyG -0.00297188*energyG*energyG;
            }
            else
            {
                if(energyG<32)
                {
                    value=2.83383 +0.108914*energyG -0.0022029*energyG*energyG;
                }
                else
                {
                    value=4.22193 -0.00195466*energyG -1.80209e-06*energyG*energyG;
                }
            }
        }
    }
    return value;
}
