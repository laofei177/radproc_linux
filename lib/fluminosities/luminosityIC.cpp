#include "luminosityIC.h"



#include <flosses/crossSectionInel.h>
#include <fparameters/parameters.h>
#include <fmath/RungeKutta.h>
//#include <fmath/interpolation.h>
#include <fmath/physics.h>

#include <algorithm>


double cICemi(double u, double E, double mass, double phEmin)  //limite inferior
{
	double Erep = mass*cLight2;

	double condition = u*(u-E);

	double inf = E*P2(Erep) / (4.0*condition);

	return std::max(phEmin,inf);  //puse la condicion Ega < s*Ee/1+s
}

double dICemi(double u, double E, double phEmax)         //limite superior   
{          
	return std::min(E,phEmax);  //esta es la condicion epsilon < Ega                               
}



double fICemi(double u, double t, double E, const Particle& creator, const SpaceCoord& distCoord,
				const ParamSpaceValues& tpf)   //funcion a integrar  u=Ee
{    
	double distCreator;
	if (u < creator.emin() || u> creator.emax()){
		distCreator = 0.0;
	}
	else{
		distCreator = creator.distribution.interpolate({ { 0, u } }, &distCoord); 
	}
	//double distCreator = creator.dist(u);// interpol(u, Ecreator, Ncreator, Ncreator.size() - 1);

	double Erep = creator.mass*cLight2;
	  

	double s = 4*u*t/P2(Erep);       //equivalente al gamma

	double r = E/(s*u*(1-E/u));   //equivalente al q

	double Nph = tpf.interpolate({ { 0, t } }, &distCoord);  

	double function = distCreator*(Nph/t)
            			*(2*r*log(r)+(1+2*r)*(1-r)+(1-r)*P2(r*s)/(2*(1+r*s)))/P2(u);
	
	double condition = s*u/(1+s);

	return function;
}

double luminosityIC(double E, const Particle& creator, const SpaceCoord& distCoord, const ParamSpaceValues& tpf,
					double phEmin, double phEmax)
{
	
	double mass = creator.mass;
	double cte  = 3.0*crossSectionThomson(creator.mass)*P2(creator.mass)*pow(cLight,5)/4;

	double integral = RungeKutta(creator.emin(), creator.emax(),
		[E,mass,phEmin](double u){
			return cICemi(u,E,mass,phEmin);
		}, 
		[E,phEmax](double u){
			return dICemi(u,E,phEmax);
		}, 
		[E,&creator,&distCoord, tpf](double u, double t){
			return fICemi(u, t,E,creator, distCoord, tpf); 
		});    //le asigno a la variable integral el resultado de la integracion   

	double luminosity = integral*cte*P2(E);

	return luminosity;

	}

