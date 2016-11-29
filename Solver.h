//
// Created by Austin Brennan on 9/15/16.
//

#ifndef PARTICLESYSTEM_SOLVER_H
#define PARTICLESYSTEM_SOLVER_H

#include "Emitter.h"
#include "VelocityGrid.h"
#include "Collider.h"
#include <random>
#include <vector>
#include <algorithm>
#include <cfloat>

class Solver {
  private:
    // Member variables
    size_t max_num_of_particles; // maximum number of particles
    std::vector<size_t> inactive_indices;
    std::vector<Emitter> emitters;
    double h; // timestep
    size_t substeps;
    double missed_particle; // fraction of a particle to emit in a later timestep
    std::default_random_engine generator;

    Vector3d calculate_acceleration(Vector3d pos, Vector3d vel);
    double uniform_distribution(double min, double max);
    double gaussian_distribution(double x, double std_deviation);
    void emitParticles(size_t number_of_particles, vector<Emitter>::iterator emitter);
    void updateParticles();
    size_t getNumerOfParticlesToEmit(vector<Emitter>::iterator emitter);
    int particleInCollider(Vector3d particle_position);
    int NO_COLLISION = -1;

    // Constructor helper
    void init(size_t max_num_of_particles, std::vector<Emitter> emitters, std::vector<Collider> colliders,
              VelocityGrid velocity_grid, double Velocity_grid_scale, double Gravity, Vector3d Wind_force,
              double Air_resistance, double h, size_t substeps);

  public:
    std::vector<Particle> particles;
    std::vector<Collider> colliders;
    VelocityGrid velocity_grid;
    double velocity_grid_scale;
    double gravity;
    Vector3d wind_force;
    double air_resistance;

    // Constructors
    Solver(size_t max_num_of_particles, std::vector<Emitter> emitters, std::vector<Collider> colliders,
           VelocityGrid velocity_grid, double velocity_grid_scale, double gravity, Vector3d wind_force,
           double air_resistance, double h, size_t substeps);

    // Member functions
    void update();
};


#endif //PARTICLESYSTEM_SOLVER_H
