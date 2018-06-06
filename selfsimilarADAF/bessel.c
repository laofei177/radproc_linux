#include <math.h>
#define float double

float bessj0(float x)
// Returns the Bessel function J_0(x) for any real x.
{
  float ax,z;
  double xx,y,ans,ans1,ans2;     // Accumulate polynomials in double precision.

  if ((ax=fabs(x)) < 8.0) {      // Direct rational function fit.
    y=x*x;
    ans1=57568490574.0+y*(-13362590354.0+y*(651619640.7
					    +y*(-11214424.18+y*(77392.33017+y*(-184.9052456)))));
    ans2=57568490411.0+y*(1029532985.0+y*(9494680.718
					  +y*(59272.64853+y*(267.8532712+y*1.0))));
    ans=ans1/ans2;
  } else {              // Fitting function.
    z=8.0/ax;
    y=z*z;
    xx=ax-0.785398164;
    ans1=1.0+y*(-0.1098628627e-2+y*(0.2734510407e-4
				    +y*(-0.2073370639e-5+y*0.2093887211e-6)));
    ans2 = -0.1562499995e-1+y*(0.1430488765e-3
			       +y*(-0.6911147651e-5+y*(0.7621095161e-6
						       -y*0.934935152e-7)));
    ans=sqrt(0.636619772/ax)*(cos(xx)*ans1-z*sin(xx)*ans2);
  }
  return ans;
}

float bessy0(float x)
// Returns the Bessel function Y_0(x) for positive x.
{
  float bessj0(float x);
  float z;
  double xx,y,ans,ans1,ans2;               // Accumulate polynomials in double precision.

  if (x < 8.0) {                           // Rational function approximation.
    y=x*x;
    ans1 = -2957821389.0+y*(7062834065.0+y*(-512359803.6
					    +y*(10879881.29+y*(-86327.92757+y*228.4622733))));
    ans2=40076544269.0+y*(745249964.8+y*(7189466.438
					 +y*(47447.26470+y*(226.1030244+y*1.0))));
    ans=(ans1/ans2)+0.636619772*bessj0(x)*log(x);
  } else {                   // Fitting function.
    z=8.0/x;
    y=z*z;
    xx=x-0.785398164;
    ans1=1.0+y*(-0.1098628627e-2+y*(0.2734510407e-4
				    +y*(-0.2073370639e-5+y*0.2093887211e-6)));
    ans2 = -0.1562499995e-1+y*(0.1430488765e-3
			       +y*(-0.6911147651e-5+y*(0.7621095161e-6
						       +y*(-0.934945152e-7))));
    ans=sqrt(0.636619772/x)*(sin(xx)*ans1+z*cos(xx)*ans2);
  }
  return ans;
}

float bessj1(float x)
// Returns the Bessel function J_1(x) for any real x.
{
  float ax,z;
  double xx,y,ans,ans1,ans2;                   // Accumulate polynomials in double precision.

  if ((ax=fabs(x)) < 8.0) {                    // Direct rational approximation.
    y=x*x;
    ans1=x*(72362614232.0+y*(-7895059235.0+y*(242396853.1
					      +y*(-2972611.439+y*(15704.48260+y*(-30.16036606))))));
    ans2=144725228442.0+y*(2300535178.0+y*(18583304.74
					   +y*(99447.43394+y*(376.9991397+y*1.0))));
    ans=ans1/ans2;
  } else {                 // Fitting function.
    z=8.0/ax;
    y=z*z;
    xx=ax-2.356194491;
    ans1=1.0+y*(0.183105e-2+y*(-0.3516396496e-4
			       +y*(0.2457520174e-5+y*(-0.240337019e-6))));
    ans2=0.04687499995+y*(-0.2002690873e-3
			  +y*(0.8449199096e-5+y*(-0.88228987e-6
						 +y*0.105787412e-6)));
    ans=sqrt(0.636619772/ax)*(cos(xx)*ans1-z*sin(xx)*ans2);
    if (x < 0.0) ans = -ans;
  }
  return ans;
}

float bessy1(float x)
// Returns the Bessel function Y_1(x) for positive x.
{
  float bessj1(float x);
  float z;
  double xx,y,ans,ans1,ans2;                   // Accumulate polynomials in double precision.

  if (x < 8.0) {                               // Rational function approximation of (6.5.8).
    y=x*x;
    ans1=x*(-0.4900604943e13+y*(0.1275274390e13
				+y*(-0.5153438139e11+y*(0.7349264551e9
							+y*(-0.4237922726e7+y*0.8511937935e4)))));
    ans2=0.2499580570e14+y*(0.4244419664e12
			    +y*(0.3733650367e10+y*(0.2245904002e8
						   +y*(0.1020426050e6+y*(0.3549632885e3+y)))));
    ans=(ans1/ans2)+0.636619772*(bessj1(x)*log(x)-1.0/x);
  } else {                                      // Fitting function (6.5.10).
    z=8.0/x;
    y=z*z;
    xx=x-2.356194491;
    ans1=1.0+y*(0.183105e-2+y*(-0.3516396496e-4
			       +y*(0.2457520174e-5+y*(-0.240337019e-6))));
    ans2=0.04687499995+y*(-0.2002690873e-3
			  +y*(0.8449199096e-5+y*(-0.88228987e-6
						 +y*0.105787412e-6)));
    ans=sqrt(0.636619772/x)*(sin(xx)*ans1+z*cos(xx)*ans2);
  }
  return ans;
}

float bessy(int n, float x)
// Returns the Bessel function Y_n(x) for positive x and n≥2.
{
  float bessy0(float x);
  float bessy1(float x);
  void nrerror(char error_text[]);
  int j;
  float by,bym,byp,tox;

  if (n < 2) nrerror("Index n less than 2 in bessy");
  tox=2.0/x;
  by=bessy1(x);                // Starting values for the recurrence.
  bym=bessy0(x);
  for (j=1;j<n;j++) {          // Recurrence.
    byp=j*tox*by-bym;
    bym=by;
    by=byp;
  }
  return by;
}

#define ACC 40.0
#define BIGNO 1.0e10
#define BIGNI 1.0e-10               // Make larger to increase accuracy.
float bessj(int n, float x)
// Returns the Bessel function J_n(x) for any real x and n≥2.
{
  float bessj0(float x);
  float bessj1(float x);
  void nrerror(char error_text[]);
  int j,jsum,m;
  float ax,bj,bjm,bjp,sum,tox,ans;

  if (n < 2) nrerror("Index n less than 2 in bessj");
  ax=fabs(x);
  if (ax == 0.0)
    return 0.0;
  else if (ax > (float) n) {      // Upwards recurrence from J_0 and J_1 .
    tox=2.0/ax;
    bjm=bessj0(ax);
    bj=bessj1(ax);
    for (j=1;j<n;j++) {
      bjp=j*tox*bj-bjm;
      bjm=bj;
      bj=bjp;
    }
    ans=bj;
  } else {                        // Downwards recurrence from an even m here computed.
    tox=2.0/ax;
    m=2*((n+(int) sqrt(ACC*n))/2);
    jsum=0;                       // jsum will alternate between 0 and 1; when it is
    bjp=ans=sum=0.0;              // 1, we accumulate in sum the even terms.
    bj=1.0;
    for (j=m;j>0;j--) {           // The downward recurrence.
      bjm=j*tox*bj-bjp;
      bjp=bj;
      bj=bjm;
      if (fabs(bj) > BIGNO) {     // Renormalize to prevent overflows.
	bj *= BIGNI;
	bjp *= BIGNI;
	ans *= BIGNI;
	sum *= BIGNI;
      }
      if (jsum) sum += bj;        // Accumulate the sum.
      jsum=!jsum;                 // Change 0 to 1 or vice versa.
      if (j == n) ans=bjp;        // Save the unnormalized answer.
    }
    sum=2.0*sum-bj;
    ans /= sum;
  }
  return x < 0.0 && (n & 1) ? -ans : ans;
}

float bessi0(float x)
// Returns the modified Bessel function I_0(x) for any real x.
{
  float ax,ans;
  double y;                     // Accumulate polynomials in double precision.

  if ((ax=fabs(x)) < 3.75) {    // Polynomial fit.
    y=x/3.75;
    y*=y;
    ans=1.0+y*(3.5156229+y*(3.0899424+y*(1.2067492
					 +y*(0.2659732+y*(0.360768e-1+y*0.45813e-2)))));
  } else {
    y=3.75/ax;
    ans=(exp(ax)/sqrt(ax))*(0.39894228+y*(0.1328592e-1
					  +y*(0.225319e-2+y*(-0.157565e-2+y*(0.916281e-2
									     +y*(-0.2057706e-1+y*(0.2635537e-1+y*(-0.1647633e-1
														  +y*0.392377e-2))))))));
  }
  return ans;
}

float bessk0(float x)
// Returns the modified Bessel function K_0(x) for positive real x.
{
  float bessi0(float x);
  double y,ans;               // Accumulate polynomials in double precision.

  if (x <= 2.0) {             // Polynomial fit.
    y=x*x/4.0;
    ans=(-log(x/2.0)*bessi0(x))+(-0.57721566+y*(0.42278420
						+y*(0.23069756+y*(0.3488590e-1+y*(0.262698e-2
										  +y*(0.10750e-3+y*0.74e-5))))));
  } else {
    y=2.0/x;
    ans=(exp(-x)/sqrt(x))*(1.25331414+y*(-0.7832358e-1
					 +y*(0.2189568e-1+y*(-0.1062446e-1+y*(0.587872e-2
									      +y*(-0.251540e-2+y*0.53208e-3))))));
  }
  return ans;
}

float bessi1(float x)
// Returns the modified Bessel function I_1(x) for any real x.
{
  float ax,ans;
  double y;                      // Accumulate polynomials in double precision.

  if ((ax=fabs(x)) < 3.75) {     // Polynomial fit.
    y=x/3.75;
    y*=y;
    ans=ax*(0.5+y*(0.87890594+y*(0.51498869+y*(0.15084934
					       +y*(0.2658733e-1+y*(0.301532e-2+y*0.32411e-3))))));
  } else {
    y=3.75/ax;
    ans=0.2282967e-1+y*(-0.2895312e-1+y*(0.1787654e-1
					 -y*0.420059e-2));
    ans=0.39894228+y*(-0.3988024e-1+y*(-0.362018e-2
				       +y*(0.163801e-2+y*(-0.1031555e-1+y*ans))));
    ans *= (exp(ax)/sqrt(ax));
  }
  return x < 0.0 ? -ans : ans;
}

float bessk1(float x)
// Returns the modified Bessel function K_1(x) for positive real x.
{
  float bessi1(float x);
  double y,ans;                       // Accumulate polynomials in double precision.

  if (x <= 2.0) {                     // Polynomial fit.
    y=x*x/4.0;
    ans=(log(x/2.0)*bessi1(x))+(1.0/x)*(1.0+y*(0.15443144
					       +y*(-0.67278579+y*(-0.18156897+y*(-0.1919402e-1
										 +y*(-0.110404e-2+y*(-0.4686e-4)))))));
  } else {
    y=2.0/x;
    ans=(exp(-x)/sqrt(x))*(1.25331414+y*(0.23498619
					 +y*(-0.3655620e-1+y*(0.1504268e-1+y*(-0.780353e-2
									      +y*(0.325614e-2+y*(-0.68245e-3)))))));
  }
  return ans;
}

float bessk(int n, float x)
// Returns the modified Bessel function K_n(x) for positive x and n≥2.
{
  float bessk0(float x);
  float bessk1(float x);
  void nrerror(char error_text[]);
  int j;
  float bk,bkm,bkp,tox;

  if (n < 2) nrerror("Index n less than 2 in bessk");
  tox=2.0/x;
  bkm=bessk0(x);                       // Upward recurrence for all x...
  bk=bessk1(x);
  for (j=1;j<n;j++) {                  // ...and here it is.
    bkp=bkm+j*tox*bk;
    bkm=bk;
    bk=bkp;
  }
  return bk;
}

float bessi(int n, float x)
// Returns the modified Bessel function I_n(x) for any real x and n≥2.
{
  float bessi0(float x);
  void nrerror(char error_text[]);
  int j;
  float bi,bim,bip,tox,ans;

  if (n < 2) nrerror("Index n less than 2 in bessi");
  if (x == 0.0)
    return 0.0;
  else {
    tox=2.0/fabs(x);
    bip=ans=0.0;
    bi=1.0;
    for (j=2*(n+(int) sqrt(ACC*n));j>0;j--) {        // Downward recurrence from even m.
      bim=bip+j*tox*bi;
      bip=bi;
      bi=bim;
      if (fabs(bi) > BIGNO) {                        // Renormalize to prevent overflows.
	ans *= BIGNI;
	bi *= BIGNI;
	bip *= BIGNI;
      }
      if (j == n) ans=bip;
    }
    ans *= bessi0(x)/bi;                              // Normalize with bessi0.
    return x < 0.0 && (n & 1) ? -ans : ans;
  }
}
