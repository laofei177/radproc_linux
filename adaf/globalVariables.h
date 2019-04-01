#pragma once
#include <cstddef>
#include <fmath/physics.h>

// PARAMETERS (from parameter.json)
extern const double blackHoleMass;              // Black hole mass
extern const double iMeanMolecularWeight;		// Ion mean molecular weight
extern const double eMeanMolecularWeight;		// Electron mean molecular weight
extern const double magFieldPar;				// Magnetic field parameter (beta)
extern const double accRateOut;					// Accretion rate at rOut
extern const double s;							// Wind index
extern const double alpha;
extern const double j;
extern const double paso_r;

extern const double logMinEnergy;				// Logarithm of the minimum energy for photons
extern const double logMaxEnergy;				// Logarithm of the maximum energy for photons
extern const size_t nR,nE;						// Number of points in each dimension

extern const size_t nRaux;
extern const Vector logr;
extern const Vector logTi;
extern const Vector logTe;
extern const Vector logv;

// PARAMETERS
extern const double schwRadius;					// Gravitationl Radius [cm]
extern const int numProcesses;                  // Number of thermal processes

void adafParameters();