//
// Created by awbrenn on 11/20/16.
//

#ifndef PARTICLESYSTEM_OBJWRITER_H
#define PARTICLESYSTEM_OBJWRITER_H


#include <vector>
#include <fstream>
#include "Particle.h"

class ObjWriter {
  public:
    bool writeParticles(vector<Particle> particles, string output_path,
                        string file_base_name, unsigned int frame_number);
};


#endif //PARTICLESYSTEM_OBJWRITER_H
