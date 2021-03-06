#include "torusParameters.h"
#include <math.h>

double rCenter, rCusp, l_0;
double n = 1.5;                  // Polytropic index
double temp_ec = 1.0e11;         // Electron temperature at the torus center
double xi = 1.0e-3;              // Electron over Ion temperature
double mu_i = 1.23;              // Mean molecular weight for ions
double mu_e = 1.14;              // Mean molecular weight for electrons
double energyC = 1.e-15;         // Energy density at the torus center
double beta = 0.5;               // Magnetic over total pressure

double boltzmann = 1.38065e-16;   // Boltzmann constant
double atomicMassUnit = 1.6605e-24;
double pK;                        // Polytropic constant

// ANGULAR VELOCITIY OF THE TORUS
double angularVel(double r, double theta)
{  
  return - ( g_tphi(r, theta) + l_0 * g_tt(r,theta) ) / ( g_phiphi(r, theta) + l_0 * g_tphi(r, theta) );
}
// POTENTIAL FUNCTION
double potential(double r, double theta) {
  double aux = g_tt(r,theta) + 2.0*angularVel(r, theta)*g_tphi(r, theta) + 
    g_phiphi(r, theta) * angularVel(r, theta) * angularVel(r, theta);
  return (aux < 0.0) ? 0.5 * log(-aux / (g_tt(r, theta)+angularVel(r, theta)*g_tphi(r, theta)) / 
				 (g_tt(r, theta)+angularVel(r, theta)*g_tphi(r, theta))) : 0.0;
}
// NORMALIZED POTENTIAL
double w(double r, double theta)
{ 
  double potentialS = potential(rCusp, 0.0);         // potential at the torus surface
  double potentialC = potential(rCenter, 0.0);      // potential at the torus center

  return (potential(r, theta) - potentialS) / (potentialC - potentialS);
}

// ENERGY DENSITY
double energyDensity(double r, double theta)  // = mass density (c=1)
{
  double M_1 = mu_i * xi / (mu_e + mu_i * xi);
  pK = boltzmann * temp_ec / ( (1.0 - beta) * atomicMassUnit * pow(energyC, 2.0/3.0) * mu_e * M_1 );
  return ( w(r, theta) > 0.0 ) ? pow( (pow(pK*pow(energyC, 1.0/n) + 1.0, w(r, theta)) - 1.0) / pK, n) : 0.0;
}

double electronDensity(double r, double theta)
{
  return energyDensity(double r, double theta) / (atomicMassUnit*mu_e);
}

/*
// PRESSURE
double pressureTot (double r, double theta)
{
    return pK * pow(energyDensity(r, theta), 1.0 + 1.0/n);
}
// TEMPERATURES //////////////////////////////////////////////////////////////////////////

// Electrons
double temp_e(double r, double theta)
{
    return (w(r, theta) > 0.0) ? (1.0 - w(r,theta)) * M_0 + w(r,theta) * M_1 *
                mu_e * ( (1.0-beta)*atomicMassUnit*pressureTot(r,theta) ) / ( boltzmann * energyDensity(r, theta) ) + 2.7 : 2.7;
}
// Ions
double temp_i(double r, double theta) { 
    return (w(r, theta) > 0.0) ? ( (1 - w(r, theta))*M_0 + w(r, theta)*M_1 ) *
                mu_i * ( (1.0-beta)*atomicMassUnit*pressureTot(r, theta) ) / ( boltzmann * energyDensity(r, theta) ) + 2.7: 2.7;
}
///////////////////////////////////////////////////////////////////////////////////////////
*/
