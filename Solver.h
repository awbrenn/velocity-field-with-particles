//
// Created by Austin Brennan on 9/15/16.
//

#ifndef PARTICLESYSTEM_SOLVER_H
#define PARTICLESYSTEM_SOLVER_H

#include "Emitter.h"
#include <vector>

class Solver {
  private:
    // Member variables
    size_t max_num_of_particles; // maximum number of particles
    std::vector<Particle> particles;
    std::vector<size_t> inactive_indices;
    std::vector<Emitter> emitters;
    double h; // timestep
    size_t substeps;
    double missed_particle; // fraction of a particle to emit in a later timestep

    size_t getNumerOfParticlesToEmit(vector<Emitter>::iterator emitter);

    // Constructor helper
    void init(size_t max_num_of_particles, std::vector<Emitter> emitters, double h, size_t substeps);

  public:
    // Constructors
    Solver(size_t max_num_of_particles, std::vector<Emitter> emitters, double h, size_t substeps);
    Solver(size_t max_num_of_particles, Emitter emitter, double h, size_t substeps);

    // Member functions
    void update();
};


#endif //PARTICLESYSTEM_SOLVER_H
