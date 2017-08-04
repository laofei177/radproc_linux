#pragma once


#include <fparticle/Particle.h>

/*vel is the lateral jet expansion*/
double adiabaticLosses(double E, double z, double vel_lat, double gamma);  //en [erg/s]

/*Diffusion rate in the Bohm regime*/
double diffusionRate(double E, double radius, double magneticField);   //en [s]^-1

double accelerationRate(double E, double magneticField, double accEfficiency); //en [s]^-1

double escapeRate(double size, double vel);  //en [s]^-1