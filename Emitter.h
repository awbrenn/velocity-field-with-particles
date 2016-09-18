//
// Created by awbrenn on 9/15/16.
//

#ifndef PARTICLESYSTEM_EMITTER_H
#define PARTICLESYSTEM_EMITTER_H

#include "Particle.h"

class Emitter {
  public:
    double rate; // rate of particle emission [particle/sec]
    double radius; // radius of the emitter
    double particle_mass_avg; // average particle mass
    double particle_mass_sdv; // standard deviation from that mass
    double particle_life_avg; // average lifetime of a particle
    double particle_life_sdv; // standard deviation from that lifetime
    bool on_surface; // flag for only emitting on sphere surface
    Vector3d pos; // position of the emitter

    // Constructor
    Emitter(double rate, double radius, bool on_surface, Vector3d pos,
            double particle_mass_avg, double particle_mass_sdv,
            double particle_life_avg, double particle_life_sdv);
};


#endif //PARTICLESYSTEM_EMITTER_H
