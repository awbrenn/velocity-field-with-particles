//
// Created by awbrenn on 9/18/16.
//

#ifndef PARTICLESYSTEM_COLLIDER_H
#define PARTICLESYSTEM_COLLIDER_H


#include "Vector.h"

// TODO subclass collider to SphereCollider
// TODO make other types of colliders
class Collider {
  public:
    Vector3d pos; // position of the collider
    double radius; // radius of the collider

    Collider();
    Collider(Vector3d pos, double radius);
};


#endif //PARTICLESYSTEM_COLLIDER_H
