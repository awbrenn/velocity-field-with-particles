//
// Created by awbrenn on 9/12/16.
//

#include "VelocityGrid.h"

void VelocityGrid::generate_voxel_locations() {
  spacing.x = (max_coord.x - min_coord.x) / x_res;
  spacing.y = (max_coord.y - min_coord.y) / y_res;
  spacing.z = (max_coord.z - min_coord.z) / z_res;

  voxel_locations.reserve((size_t)  x_res * y_res * z_res);

  for (int zi = 0; zi < z_res; ++zi) {
    for (int yi = 0; yi < y_res; ++yi) {
      for (int xi = 0; xi < x_res; ++xi) {
        Vector3d voxel_location(min_coord.x + (((double) xi) * spacing.x + spacing.x/2.0),
                                min_coord.y + (((double) yi) * spacing.y + spacing.y/2.0),
                                min_coord.z + (((double) zi) * spacing.z + spacing.z/2.0));
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

Vector3d VelocityGrid::get_velocity(Vector3d location) {
  int xi, yi, zi;

}