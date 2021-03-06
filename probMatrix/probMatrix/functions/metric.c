#include "metric.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////////////////////
// METRIC COMPONENTS (in Boyer-Lindquist coordinates)

double massBH = 1.0;
double spinBH = 0.0;
double lambda = 0.8;

double g_tt(double r, double theta)
{  
  double sigma = r*r + spinBH*spinBH*sin(theta)*sin(theta);
  return - (1.0 - 2.0*massBH*r / sigma);
}
double g_rr(double r, double theta)
{  
  double delta = r*r - 2.0 * massBH * r + spinBH*spinBH;
  double sigma = r*r + spinBH*spinBH*sin(theta)*sin(theta);
  return sigma / delta;
}
double g_thetatheta(double r, double theta)
{  
  return r*r + spinBH*spinBH*sin(theta)*sin(theta);
}
double g_tphi(double r, double theta)
{
  double sigma = r*r + spinBH*spinBH*sin(theta)*sin(theta);
  return - 2.0*massBH*r*spinBH / sigma * cos(theta)*cos(theta);
}
double g_phiphi(double r, double theta)
{  
  double sigma = r*r + spinBH*spinBH*sin(theta)*sin(theta);
  return (r*r + spinBH*spinBH + 2.0*massBH*r*spinBH*spinBH*cos(theta)*cos(theta) / sigma )
    * cos(theta)*cos(theta);
}
