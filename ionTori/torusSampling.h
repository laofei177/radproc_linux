#pragma once

#include <time.h>
#include <stdlib.h>

#include "torusParameters.h"
#include <fparticle/Particle.h>


//void torusSampling(Particle& p, double **prob);
void torusSampling(Particle& p, Matrix& prob);