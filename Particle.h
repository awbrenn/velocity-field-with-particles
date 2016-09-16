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

    Particle(Vector3d pos, Vector3d vel, double mass);
};


#endif //PARTICLESYSTEM_PARTICLE_H
