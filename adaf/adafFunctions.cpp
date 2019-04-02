#include "adafFunctions.h"
#include "globalVariables.h"
#include <fmath/physics.h>

// Electrons

double electronTemp(double r)
{
	double logr_actual = log(r/schwRadius);
	double aux = logr.front();
	size_t pos_r=0;
	while (aux < logr_actual) {
		pos_r++;
		aux = logr[pos_r];
	}
	double m = (logTe[pos_r]-logTe[pos_r-1])/(logr[pos_r]-logr[pos_r-1]);
	double logT = m*(logr_actual-logr[pos_r-1])+logTe[pos_r-1];
	return eMeanMolecularWeight*exp(logT);
}

// Ions
double ionTemp(double r) {
	double logr_actual = log(r/schwRadius);
	double aux = logr.front();
	size_t pos_r=0;
	while (aux < logr_actual) {
		pos_r++;
		aux = logr[pos_r];
	}
	double m = (logTi[pos_r]-logTi[pos_r-1])/(logr[pos_r]-logr[pos_r-1]);
	double logT = m*(logr_actual-logr[pos_r-1])+logTi[pos_r-1];
	
	
	return iMeanMolecularWeight*exp(logT);
}

double radialVel(double r) {
	double logr_actual = log(r/schwRadius);
	double aux = logr.front();
	size_t pos_r=0;
	while (aux < logr_actual) {
		pos_r++;
		aux = logr[pos_r];
	}
	double m = (logv[pos_r]-logv[pos_r-1])/(logr[pos_r]-logr[pos_r-1]);
	double logrv = m*(logr_actual-logr[pos_r-1])+logv[pos_r-1];
	return -exp(logrv);
}

double keplAngVel(double r)
{
	return sqrt(gravitationalConstant*blackHoleMass/r) / (r-schwRadius);
}

double sqrdSoundVel(double r)
{
    return boltzmann / (magFieldPar*atomicMassUnit) * ( ionTemp(r) / iMeanMolecularWeight 
							+ electronTemp(r) / eMeanMolecularWeight );
}

double height_fun(double r)
{
    return sqrt(sqrdSoundVel(r))/keplAngVel(r);
}

double angularVel(double r)
{
	return alpha*sqrdSoundVel(r)/(-radialVel(r)*r) + (j*schwRadius*cLight)/(r*r);
}

double costhetaH(double r)
{
	double cs = sqrt(sqrdSoundVel(r));
	double omR = angularVel(r)*r;
	return sqrt(pi/2.0) * cs/omR * erf(omR / (sqrt(2.0)*cs));
}

double accRateADAF(double r)
{
	double rOut = exp(logr.back())*schwRadius;
	double result = accRateOut*pow(r/rOut,s);
	if (r > rTr) result *= (rTr/r);
	return result;
}

double massDensityADAF(double r)
{
	return accRateADAF(r) / 
		(4.0*pi*r*r*(-radialVel(r))*costhetaH(r));
}

double accRateColdDisk(double r)
{
	return accRateOut*(1.0-rTr/r);
}

double tempCD(double r)
{
	double lj = r/sqrt(paso_r);
	double lj1 = r*sqrt(paso_r);
	double area = 2.0*pi*(lj1*lj1-lj*lj);
	
	double aux = 3.0*gravitationalConstant*blackHoleMass*accRateColdDisk(r)/2.0 *
				(1.0/lj * (1.0-2.0/3.0*sqrt(rTr/lj))-1.0/lj1*(1.0-2.0/3.0*sqrt(rTr/lj1)));
	return pow(aux/area/ stefanBoltzmann,0.25);
}

double electronDensity(double r)
{
	return massDensityADAF(r)/(atomicMassUnit*eMeanMolecularWeight);
}

double ionDensity(double r)
{
	return massDensityADAF(r)/(atomicMassUnit*iMeanMolecularWeight);
}