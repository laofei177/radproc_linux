#include "torusSampling.h"
//#include "nr.h"
#include "nrutil.h"
#include "torusParameters.h"
#include "ran1.h"
#include <stdio.h>

#define PI 3.14159265359
#define thomson 6.652458734e-25
#define RG 1.5e11

extern double rMin, rCenter, l_0;
extern double spinBH, lambda;
extern long idum;

void torusSampling()
{
  criticalRadii(&rMin, &rCenter);
  l_0 = specificAngularMom();
  double rMax = edge();
  int nR=10,nTheta=3,scat=0,nPhot=2,esc=0;
  long idum=0;

  FILE *fp1;
  fp1=fopen("torusSample.txt", "w");
  
  double dr=(rMax-rMin)/nR,r0;
  printf("%f\n",dr);
  double *rCells,*r;
  rCells=dvector(0,nR);
  r=dvector(1,nR);
  rCells[0]=rMin;
  for(int i=1;i<=nR;i++) {
    rCells[i]=rCells[i-1]+dr;
    r[i]=rCells[i-1]+dr/2;
    r0=r[i];
    double dyaux=(exp(-2.0)-1.0)/nTheta;
    int count=0;
    for(int k=1;k<=nTheta+1;k++) {
      double yaux=1.0+(k-1)*dyaux;
      double theta0=(PI/12.0)*(-log(yaux));
      double y0=r0*cos(theta0);
      double z0=r0*sin(theta0);
      double *rInt;
      rInt=dvector(1,nPhot);
      if (w(r0,theta0) > 0) {
	fprintf(fp1,"%f   %f\n",y0,z0);
	fprintf(fp1,"%f   %f\n",y0,-z0);
	fprintf(fp1,"%f   %f\n",-y0,z0);
	fprintf(fp1,"%f   %f\n",-y0,-z0);
      }
      double drprim=dr/10;
      for(int j=1;j<=nPhot;j++) {
	double phiprim=2.0*PI*ran1(&idum);
	double xrand=PI*ran1(&idum);
	double thetaprim=acos((PI/2.0-xrand)/(PI/2.0));
	double rprim=drprim/2.0;
	double ne;
	do {
	  double xprim=rprim*sin(thetaprim)*cos(phiprim);
	  double yprim=rprim*sin(thetaprim)*sin(phiprim);
	  double zprim=rprim*cos(thetaprim);
	  double x1=xprim;
      double y1=y0+yprim;
	  double z1=z0+zprim;
	  double r1=sqrt(x1*x1+y1*y1+z1*z1);
	  double theta1=atan(z1/sqrt(x1*x1+y1*y1));
	  ne=electronDensity(r1,abs(theta1));
	  double prob=ne*thomson*(drprim*RG);
	  double event=ran1(&idum);
	  if(event < prob) {
	    scat=1;
	    count++;
	    rInt[count]=r1;
	  }
	  else {
	    rprim+=drprim;
	  }
	  //printf("%f   %f\n",r1, ne);
	} while(ne > 0.0 && scat == 0);
	if(scat == 0) esc++;
	scat=0;
      }
      printf("llegó1\n");
    }
    printf("llegó2\n");
  }
  fclose(fp1);
  printf("llegó3\n");
}
