#include "PairProductioncorrection.hh"

PairProductionCorrection::PairProductionCorrection()
{
	axis=PWO;
}

void PairProductionCorrection::set_axis(axis_material a)
{
    axis=a;
}

void PairProductionCorrection::set_axis(G4int axisID)
{
	switch (axisID) {
		case 0:
			axis=PWO;
			G4cout << "Set PWO axis" << G4endl;
			break;
		case 1:
			axis=W111;
			G4cout << "Set W111 axis" << G4endl;
			break;
		case 2:
			axis=PWO_001;
			G4cout << "Set PWO_001 axis" << G4endl;
			break;
		case 3:
			axis=W111;
			G4cout << "Set W111 axis" << G4endl;
			break;
		case 4:
			axis=W111;
			G4cout << "Set W111 axis" << G4endl;
			break;
		case 5:
			axis=W111_3mrad;
			G4cout << "Set W111_3mrad" << G4endl;
			break;
		case 6:
			axis=W111_3mrad;
			G4cout << "Set W111_3mrad" << G4endl;
			break;
	}
}

G4double  PairProductionCorrection::ComputeCorrection(G4double eKineticEnergy)
{
    switch (static_cast<int>(axis)) {
    	case PWO:
    		return ComputeCorrectionPWO(eKineticEnergy);
    		break;
    	case W111:
     		return  ComputeCorrectionW_111(eKineticEnergy);
     		break;
    	case PWO_001:
    		return ComputeCorrectionPWO_001(eKineticEnergy);
    		break;
    	case W111_3mrad:
    		return ComputeCorrectionW_111_3mrad(eKineticEnergy);
    		break;
    	default:
        	printf("\nWrong type of axis and material %d.\n", axis);
        	return 1;
        	break;
    }
}

G4double PairProductionCorrection::ComputeCorrectionPWO(G4double gammaEnergy)
{
    //calculate correction for cross section
    //approximation from 10 to 1 TeV  by  polynomial
    G4double value=1;
    G4double energyG;

    energyG=gammaEnergy/GeV;
    if(energyG>10.0)
    {
        if(energyG<20.0)
        {
            value=1.0 - 0.01*energyG + 0.001 *energyG*energyG;
        }
        else
        {
            if(energyG<40)
            {
                value=0.72 + 0.02*energyG + 0.0002*energyG*energyG ;
            }
            else
            {
                if(energyG<100)
                {
                    value=0.135 + 0.0476583*energyG - 0.000125*energyG*energyG - 2.08333e-8*energyG*energyG*energyG;
                }
                else
                {
                    if(energyG<300)
                    {
                        value=-2.081 + 0.0864267*energyG - 0.0003403*energyG*energyG + 4.71333e-7*energyG*energyG*energyG;
                    }
                    else
                    {
                        if(energyG<600)
                        {
                            value=4.095 + 0.007835*energyG - 5.55e-6*energyG*energyG;
                        }
                        else
                        {
                            value=4.317 + 0.00592*energyG - 2.975e-6*energyG*energyG;
                        };
                    };
                };

            };
        };
    }
    else
    {
        value=1;
    };
    return value;
}

G4double PairProductionCorrection::ComputeCorrectionPWO_001(G4double gammaEnergy)
{
    //calculate correction for cross section
    //  approximation from 10 to 1 TeV  by  polynomial
    G4double value=1;
    G4double energyG;

    energyG=gammaEnergy/GeV;
    if(energyG>10.0)
    {
        if(energyG<67.4)
        {
            value=0.763848+0.0151049*energyG + 0.000477248 *energyG*energyG -4.70277e-06 *energyG*energyG*energyG;
        }
        else
        {
            if(energyG<167.8584)
            {
                value=-0.115807  + 0.0588293*energyG -0.000348754*energyG*energyG+7.72463e-07*energyG*energyG*energyG;
            }
            else
            {
                if(energyG<290)
                {
                    value=1.9767 + 0.0143781*energyG -3.42286e-05*energyG*energyG +3.46971e-08*energyG*energyG*energyG;
                }
                else
                {
                    if(energyG<557)
                    {
                        value=3.34553 + 0.00388837*energyG -5.36416e-06*energyG*energyG + 3.36887e-09*energyG*energyG*energyG;
                    }
                    else
                    {
                        if(energyG<758)
                        {
                            value=4.57564 -0.000686347*energyG +7.74603e-07*energyG*energyG;
                        }
                        else
                        {
                            value=4.26434 + 0.000535472*energyG -2.95232e-07*energyG*energyG;
                        }
                    }
                }

            }
        };
    }
    else
    {
        value=1;
    };
    return value;
}
G4double PairProductionCorrection::ComputeCorrectionW_111(G4double gammaEnergy)
{
    //calculate correction for cross section
    //  approximation from 10 to 1 TeV  by  polynomial
    G4double value=1;
    G4double energyG;

    energyG=gammaEnergy/GeV;
    if(energyG>10.0)
    {
        if(energyG<62)
        {
            value=0.299031+0.0841384*energyG -0.000450991 *energyG*energyG;
        }
        else
        {
            if(energyG<160)
            {
                value=2.03933  + 0.0327963*energyG -7.51477e-05*energyG*energyG;
            }
            else
            {
                    value=4.47771 + 0.00579909*energyG -2.46054e-06*energyG*energyG;
            }
        };
    }
    else
    {
        value=1;
    };
    return value;
}

G4double PairProductionCorrection::ComputeCorrectionW_111_3mrad(G4double gammaEnergy)
{
    //calculate correction for cross section
    //  approximation from 10 to 1 TeV  by  polynomial
    G4double value=1;
    G4double energyG;

    energyG=gammaEnergy/GeV;
    if(energyG>10.0)
    {
        if(energyG<62)
        {
            value=0.732456+0.0672808*energyG -0.000514399 *energyG*energyG;
        }
        else
        {
            if(energyG<160)
            {
                value=2.32235  + 0.0125342*energyG -3.28697e-05*energyG*energyG;
            }
            else
            {
                    value=3.04292 + 0.00363105*energyG -5.30128e-6*energyG*energyG;
            }
        };
    }
    else
    {
        value=1;
    };
    return value;
}
