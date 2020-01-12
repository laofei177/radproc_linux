#include "adafFunctions.h"
#include "globalVariables.h"
#include "read.h"
#include "write.h"
#include <iostream>
#include <fstream>
#include <fmath/physics.h>
#include <fparameters/parameters.h>
#include <gsl/gsl_sf_bessel.h>
#include <cstddef>
#include <fparameters/SpaceIterator.h>
#include <fparameters/Dimension.h>
#include <fmath/RungeKutta.h>

using namespace std;
// Electrons

double electronTemp(double r)
{
	if (calculateNewTemp) {
		ifstream adafNewTemps;
		adafNewTemps.open("output.txt");
		
		Vector logr_aux(200,0.0);
		Vector logTe_aux(200,0.0);
	
		size_t i=0;
		while( ! adafNewTemps.eof() ) {
			adafNewTemps >> logr_aux[i] >> logTe_aux[i];
			i++;
		}
		adafNewTemps.close();
		
		double logr_actual = log10(r/schwRadius);
		size_t pos_r = 0;
		double aux = logr_aux.front();
		while (aux < logr_actual && pos_r < logr.size()-1) {
			pos_r++;
			aux = logr_aux[pos_r];
		}
		double m = (logTe_aux[pos_r]-logTe_aux[pos_r-1])/(logr_aux[pos_r]-logr_aux[pos_r-1]);
		double logT = m*(logr_actual-logr_aux[pos_r-1])+logTe_aux[pos_r-1];
		double temp = pow(10,logT);
		double r_rg = r/(schwRadius/2.0);
		//return (r_rg <= 30) ? temp * 1.4/pow(r_rg,0.097) : temp;
		return temp;
		
	} else {
		double logr_actual = log(r/schwRadius);
		double aux = logr.front();
		size_t pos_r = 0;
		while (aux < logr_actual && pos_r < logr.size()-1) {
			pos_r++;
			aux = logr[pos_r];
		}
		double m = (logTe[pos_r]-logTe[pos_r-1])/(logr[pos_r]-logr[pos_r-1]);
		double logT = m*(logr_actual-logr[pos_r-1])+logTe[pos_r-1];
		double temp = eMeanMolecularWeight*exp(logT);
		double r_rg = r/(schwRadius/2.0);
		return (r_rg <= 30) ? temp * 1.4/pow(r_rg,0.097) : temp;
		//return temp;
	}
	//return temp;
}

double electronTempOriginal(double r)
{
	double logr_actual = log(r/schwRadius);
	double aux = logr.front();
	size_t pos_r = 0;
	while (aux < logr_actual && pos_r < logr.size()-1) {
		pos_r++;
		aux = logr[pos_r];
	}
	double m = (logTe[pos_r]-logTe[pos_r-1])/(logr[pos_r]-logr[pos_r-1]);
	double logT = m*(logr_actual-logr[pos_r-1])+logTe[pos_r-1];
	double temp = eMeanMolecularWeight*exp(logT);
	double r_rg = r/(schwRadius/2.0);
	//return (r_rg <= 30) ? temp * 1.4/pow(r_rg,0.097) : temp;
	return temp;
}

// Ions
double ionTemp(double r) {
	double logr_actual = log(r/schwRadius);
	double aux = logr.front();
	size_t pos_r=0;
	while (aux < logr_actual && pos_r < logr.size()-1) {
		pos_r++;
		aux = logr[pos_r];
	}
	double m = (logTi[pos_r]-logTi[pos_r-1])/(logr[pos_r]-logr[pos_r-1]);
	double logT = m*(logr_actual-logr[pos_r-1])+logTi[pos_r-1];
	
	double temp = iMeanMolecularWeight*exp(logT);
	double r_rg = r/(schwRadius/2.0);
	return temp;
	//return (r_rg <= 30) ? temp * 1.4/pow(r_rg,0.097) : temp;
}

double radialVel(double r) {
	double logr_actual = log(r/schwRadius);
	double aux = logr.front();
	size_t pos_r=0;
	while (aux < logr_actual && pos_r < logr.size()-1) {
		pos_r++;
		aux = logr[pos_r];
	}
	double m = (logv[pos_r]-logv[pos_r-1])/(logr[pos_r]-logr[pos_r-1]);
	double logrv = m*(logr_actual-logr[pos_r-1])+logv[pos_r-1];
	double r_rg = r / (schwRadius/2.0);
	double vel = -exp(logrv);
	return (r_rg <= 30 ? vel / (0.93*exp(2.13/r_rg)) : vel);
}

double accretionTime(double r) {
	return integSimpson(log(r/schwRadius),logr.front(),[&](double logrr)
			{
				double rr = exp(logrr)*schwRadius;
				return rr/radialVel(rr);
			},20);
	//return RungeKuttaSimple(r,exp(logr.front())*schwRadius,[&](double x) {
	//						return 1.0/radialVel(x);});
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
	if (height_method == 1)
		return sqrt(sqrdSoundVel(r))/keplAngVel(r);
	else
		return r*costhetaH(r);
}

double volume(double r)
{
	double rB2 = r*sqrt(paso_r);
	double rB1 = rB2/paso_r;
	if (height_method == 1)
		return 2.0*height_fun(r)*pi*(rB2*rB2-rB1*rB1)*sqrt(pi)/2.0;
	else
		return (4.0/3.0)*pi*costhetaH(r)*(rB2*rB2*rB2-rB1*rB1*rB1);
}

double angularVel(double r)
{
	return alpha*sqrdSoundVel(r)/(-radialVel(r)*r) + (j*schwRadius*cLight)/(r*r);
}

double costhetaH(double r)
{
	double cs = sqrt(sqrdSoundVel(r));
	double omR = angularVel(r)*r;
	double result = sqrt(pi/2.0) * cs/omR * erf(omR / (sqrt(2.0)*cs));
	return result;
}

double accRateADAF(double r)
{
	double accRateCorona = GlobalConfig.get<double>("accretionRateCorona")*accRateOut;
	double rOut = exp(logr.back())*schwRadius;
	double result = accRateOut * (r < rOut ? pow(r/rOut,s) : 0.0);
	int processesFlags[numProcesses];
	readThermalProcesses(processesFlags);
	if (processesFlags[3]) {
		if (r > rTr) result = accRateCorona*pow(r/rOut,s)*(rTr/r);
	}
	return result;
}

double massDensityADAF(double r)
{
	if (height_method == 0)
		return accRateADAF(r) / (4.0*pi*r*height_fun(r)*(-radialVel(r)));
	else 
		return accRateADAF(r) / (4.0*pi*r*height_fun(r)*sqrt(0.5*pi)*(-radialVel(r)));
}

double magneticField(double r)
{
	return sqrt(8.0*pi*(1.0-magFieldPar)*massDensityADAF(r)*sqrdSoundVel(r));
}

double accRateColdDisk(double r)
{
	double rOutADAF = exp(logr.back())*schwRadius;
	int processesFlags[numProcesses];
	return (r < rOutADAF ? accRateOut*pow(r/rOutADAF,s)*(1.0-rTr/r) : accRateOut);
}

double auxCD(double r)
{
	double lj = r/sqrt(paso_r);
	double lj1 = r*sqrt(paso_r);
	return 3.0*gravitationalConstant*blackHoleMass*accRateColdDisk(r)/2.0 *
			(1.0/lj * (1.0-2.0/3.0*sqrt(rTr/lj))-1.0/lj1*(1.0-2.0/3.0*sqrt(rTr/lj1)));
}

double electronDensity(double r)
{
	double rOut = exp(logr.back())*schwRadius;
	return (r > schwRadius && r < rOut) ? massDensityADAF(r)/(atomicMassUnit*eMeanMolecularWeight) : 0.0;
}

double electronDensityTheta(double r, double theta)
{
	double thetaMinLocal;
	double rOut = exp(logr.back())*schwRadius;
	if (r < rOut) {
		if (height_method == 0) {
			thetaMinLocal = acos(height_fun(r)/r);
			return (theta > thetaMinLocal && theta < pi-thetaMinLocal) ?
											electronDensity(r) : 0.0;
		} else {
			double z = r*cos(theta);
			double rho = r*sin(theta);
			double h = height_fun(rho);
			return electronDensity(rho)*exp(-z*z/(2.0*h*h));
		}
	} else
		return 0.0;
}

double ionDensity(double r)
{
	return massDensityADAF(r)/(atomicMassUnit*iMeanMolecularWeight);
}

double qie(double r, double Ti, double Te)
{
	double theta_e = boltzmann*Te / electronRestEnergy;
	double theta_i = boltzmann*Ti / (protonMass*cLight2);
	double lnLambda = 20.0;
	double diftemps = boltzmann*(Ti-Te);
	double aux = 1.5*electronMass/protonMass * electronDensity(r) * ionDensity(r) *
					thomson * cLight * lnLambda * diftemps;
	double aux2 = (sqrt(2.0/pi)+sqrt(theta_e+theta_i))/pow(theta_e+theta_i,1.5);
	return aux*aux2;
}

double qie_beta(double r, double Ti, double Te)
{
	double lnLambda = 20.0;
	double theta_e = boltzmann*Te / electronRestEnergy;
	double theta_i = boltzmann*Ti/(protonMass*cLight2);
	double xe = 1.0/theta_e;
	double xi = 1.0/theta_i;
	double xei = xe+xi;

	double k2i = gsl_sf_bessel_Kn(2,xi);
	double k2e = gsl_sf_bessel_Kn(2,xe);
	double k1ei = gsl_sf_bessel_K1(xei);
	double k0ei = gsl_sf_bessel_K0(xei);

	double sumtheta = theta_i + theta_e;
	double aux1 = 1.875*thomson*(electronMass/protonMass)*cLight * lnLambda * 
			electronDensity(r)*ionDensity(r) * boltzmann*(Ti-Te);
	double aux2 = (2.0*sumtheta*sumtheta+1.0)/sumtheta;
	
	return (xei > 300.0 ? (xi > 150.0 ? (xe > 150.0 ? aux1*sqrt(2.0*xei/(pi*xe*xi))*
				(aux2+2.0) : aux1*sqrt(xi/xei)*(aux2+2.0)*exp(-xe)/k2e) : 
				aux1*sqrt(xe/xei)*(aux2+2.0)*exp(-xi)/k2i) : 
				aux1*(aux2 * k1ei/k2i + 2.0*k0ei/k2i)/k2e);

}

double dlogrho_dlogr(double r)
{
	double logr = log(r);
	double dlogr = log(paso_r);
	double logr2 = logr + 0.5*dlogr;
	double logr1 = logr - 0.5*dlogr;
	double r2 = exp(logr2);
	double r1 = exp(logr1);
	double logrho2 = log(massDensityADAF(r2));
	double logrho1 = log(massDensityADAF(r1));
	return (logrho2 - logrho1) / dlogr;
}

double dlogom_dlogr(double r)
{
	double logr = log(r);
	double dlogr = log(paso_r);
	double logr2 = logr + dlogr;
	double logr1 = logr - dlogr;
	double r2 = exp(logr2);
	double r1 = exp(logr1);
	double logOm2 = log(angularVel(r2));
	double logOm1 = log(angularVel(r1));
	return 0.5 * (logOm2 - logOm1) / dlogr;
}

double Qplus(double r, double Ti, double Te)
{
	double pressure = massDensityADAF(r) * (boltzmann/atomicMassUnit/magFieldPar) *
						(Ti/iMeanMolecularWeight + Te/eMeanMolecularWeight);
	return -alpha*pressure*angularVel(r)*dlogom_dlogr(r);
}

double Qmin_func(double r, Matrix lumOut, Matrix lumInICm, Vector energies, State& st)
{
	double logr_actual = log10(r);
	double aux = log10(st.denf_e.ps[DIM_R][0]);
	size_t pos_r = 0;
	while (aux < logr_actual) {
		pos_r++;
		aux = log10(st.denf_e.ps[DIM_R][pos_r]);
	}
	
	double r1 = st.denf_e.ps[DIM_R][pos_r-1];
	double vol1 = volume(r1);
	
	double r2 = st.denf_e.ps[DIM_R][pos_r];
	double vol2 = volume(r2);
	
	double eVar = pow(energies[nE-1]/energies[0],1.0/(nE-1));
	double Qmin = 0.0;
	for (size_t jE=0;jE<nE;jE++)  {
		double frequency = energies[jE]/planck;
		double dfrequency = frequency * (sqrt(eVar)-1.0/sqrt(eVar));
		double qO2 = lumOut[jE][pos_r]/vol2;
		double qO1 = lumOut[jE][pos_r-1]/vol1;
		double qI2 = lumInICm[jE][pos_r]/vol2;
		double qI1 = lumInICm[jE][pos_r-1]/vol1;
		double mO = (qO1 > 0.0 && qO2 > 0.0) ? safeLog10(qO2/qO1) / safeLog10(r2/r1) : 0.0;
		double mI = (qI1 > 0.0 && qI2 > 0.0) ? safeLog10(qI2/qI1) / safeLog10(r2/r1) : 0.0;
		double logqO = (qO1 > 0.0) ? mO*(logr_actual-log10(r1))+safeLog10(qO1) : -100;
		double logqI = (qI1 > 0.0) ? mI*(logr_actual-log10(r1))+safeLog10(qI1) : -100;;
		Qmin += (pow(10.0,logqO)-pow(10.0,logqI))*dfrequency;
	}
	return Qmin;
}