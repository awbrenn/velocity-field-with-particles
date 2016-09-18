//
// Created by awbrenn on 9/15/16.
//

#ifndef PARTICLESYSTEM_PARTICLE_H
#define PARTICLESYSTEM_PARTICLE_H

#include "Matrix.h"

class Particle {
  public:
    Vector3d pos;
    Vector3d vel;
    double mass;
    double lifetime;
    double life_left; // amount of life left in the particle
    bool active;
    Particle(Vector3d pos, Vector3d vel, double mass, double lifetime);
};


#endif //PARTICLESYSTEM_PARTICLE_H
