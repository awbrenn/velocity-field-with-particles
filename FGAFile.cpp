//
// Created by awbrenn on 9/12/16.
//

#include "FGAFile.h"

void FGAFile::read(const char *filename, VelocityGrid *velocity_grid) {
  std::ifstream fga_stream;
  fga_stream.open(filename, std::ios_base::in);
  char seperator;

  if (fga_stream) {
    // get resolution of the velocity grid;
    fga_stream >> velocity_grid->x_res >> seperator >>
                  velocity_grid->y_res >> seperator >>
                  velocity_grid->z_res >> seperator;

    fga_stream >> velocity_grid->min_coord.x >> seperator >>
                  velocity_grid->min_coord.y >> seperator >>
                  velocity_grid->min_coord.z >> seperator;

    fga_stream >> velocity_grid->max_coord.x >> seperator >>
                  velocity_grid->max_coord.y >> seperator >>
                  velocity_grid->max_coord.z >> seperator;

    velocity_grid->velocities.reserve((size_t) velocity_grid->x_res *
                                               velocity_grid->y_res *
                                               velocity_grid->z_res);

    Vector3d velocity;
    while (fga_stream >> velocity.x >> seperator >>
                         velocity.y >> seperator >>
                         velocity.z >> seperator) {
      velocity_grid->velocities.push_back(velocity);
    }
  }
  else {
    std::cerr << "Unable to open file " << filename << std::endl;
    exit(EXIT_FAILURE);
  }

  if (!fga_stream.eof()) {
    std::cerr << "Error: Unable to read file." << std::endl;
    exit(EXIT_FAILURE);
  }
}
