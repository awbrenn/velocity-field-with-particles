//
// Created by awbrenn on 9/12/16.
//

#ifndef PARTICLESYSTEM_FGAFILE_H
#define PARTICLESYSTEM_FGAFILE_H

#include "Vector.h"
#include "VelocityGrid.h"
#include <iostream>
#include <fstream>
#include <vector>

class FGAFile {
  public:
//    int fp; // file pointer
//    int reader
    void read(const char* filename, VelocityGrid *velocity_grid);
};


#endif //PARTICLESYSTEM_FGAFILE_H
