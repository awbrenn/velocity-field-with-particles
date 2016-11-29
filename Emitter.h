//
// Created by awbrenn on 9/15/16.
//

#ifndef PARTICLESYSTEM_EMITTER_H
#define PARTICLESYSTEM_EMITTER_H

#include "Particle.h"

class Emitter {
  public:
    double rate; // rate of particle emission [particle/sec]
    Vector3d size; // radius of the emitter
    double particle_mass_avg; // average particle mass
    double particle_mass_sdv; // standard deviation from that mass
    double particle_life_avg; // average lifetime of a particle
    double particle_life_sdv; // standard deviation from that lifetime
    double particle_color_sdv; // standard deviation from that color
    bool on_surface; // flag for only emitting on sphere surface
    bool even_dist;
    Vector3d pos; // position of the emitter
    Vector3d particle_color_avg; // average color of a particle

    // Constructor
    Emitter(double rate, Vector3d size, bool on_surface, bool even_dist, Vector3d pos, double particle_mass_avg,
            double particle_mass_sdv, double particle_life_avg, double particle_life_sdv, Vector3d particle_color_avg,
            double particle_color_sdv);
};


#endif //PARTICLESYSTEM_EMITTER_H
