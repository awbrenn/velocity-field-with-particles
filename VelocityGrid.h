//
// Created by awbrenn on 9/12/16.
//

#ifndef PARTICLESYSTEM_VELOCITYGRID_H
#define PARTICLESYSTEM_VELOCITYGRID_H

#include <vector>
#include "Vector.h"

class VelocityGrid {
  public:
    int x_res, y_res, z_res; // resolution
    Vector3d min_coord; // the bottom back left corner of the volume
    Vector3d max_coord; // the top front right corner of the volume
    vector<Vector3d> velocities;
};


#endif //PARTICLESYSTEM_VELOCITYGRID_H
