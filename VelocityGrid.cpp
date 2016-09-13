//
// Created by awbrenn on 9/12/16.
//

#include "VelocityGrid.h"

void VelocityGrid::generate_voxel_locations() {
  double x_spacing;
  double y_spacing;
  double z_spacing;

  x_spacing = (max_coord.x - min_coord.x) / x_res;
  y_spacing = (max_coord.y - min_coord.y) / y_res;
  z_spacing = (max_coord.z - min_coord.z) / z_res;

  voxel_locations.reserve((size_t)  x_res * y_res * z_res);

  for (int zi = 0; zi < z_res; ++zi) {
    for (int yi = 0; yi < y_res; ++yi) {
      for (int xi = 0; xi < x_res; ++xi) {
        Vector3d voxel_location(min_coord.x + (((double) xi) * x_spacing + x_spacing/2.0),
                                min_coord.y + (((double) yi) * y_spacing + y_spacing/2.0),
                                min_coord.z + (((double) zi) * z_spacing + z_spacing/2.0));
        voxel_locations.push_back(voxel_location);
      }
    }
  }
}

Vector3d VelocityGrid::get_voxel_location(int index) {
  return voxel_locations[index];
}

Vector3d VelocityGrid::get_voxel_location(int xi, int yi, int zi) {
  return voxel_locations[xi + yi * x_res + zi * x_res * y_res];
}

Vector3d VelocityGrid::get_velocity(int index) {
  return velocities[index];
}

Vector3d VelocityGrid::get_velocity(int xi, int yi, int zi) {
  return velocities[xi + yi * x_res + zi * x_res * y_res];
}