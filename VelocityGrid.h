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
    Vector3d spacing;
    vector<Vector3d> voxel_locations;

    // get a velocity by a flat index
    Vector3d get_voxel_location(int index);

    // get a velocity from an x, y, and z index
    Vector3d get_voxel_location(int xi, int yi, int zi);

    // get a velocity by a flat index
    Vector3d get_velocity(int index);

    // get a velocity from an x, y, and z index
    Vector3d get_velocity(int xi, int yi, int zi);

    // get a velocity from an x, y, and z index
    Vector3d get_velocity(Vector3d location);

    // generate a locations for the center of each voxel
    void generate_voxel_locations();
};

#endif //PARTICLESYSTEM_VELOCITYGRID_H
