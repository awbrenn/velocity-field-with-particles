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
    bool on_surface; // flag for only emitting on sphere surface
    Vector3d pos; // position of the emitter

    // Constructor
    Emitter(double rate, double radius, bool on_surface, Vector3d pos);
};


#endif //PARTICLESYSTEM_EMITTER_H
