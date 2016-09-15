//
// Created by Austin Brennan on 9/15/16.
//

#include "Solver.h"

Solver::Solver(size_t Max_num_of_particles, std::vector<Emitter> Emitters, double h, size_t substeps) {
  init(Max_num_of_particles, Emitters, h, substeps);
}

Solver::Solver(size_t Max_num_of_particles, Emitter emitter, double h, size_t substeps) {
  std::vector<Emitter> emitter_vector;
  emitter_vector.push_back(emitter);
  init(Max_num_of_particles, emitter_vector,h,substeps);
}

void Solver::init(size_t Max_num_of_particles, std::vector<Emitter> Emitters, double h, size_t Substeps) {
  srand (time(NULL));
  max_num_of_particles = Max_num_of_particles;
  particles.reserve(max_num_of_particles);
  inactive_indices.reserve(max_num_of_particles);
  emitters = Emitters;
  substeps = Substeps;
  missed_particle = 0.0;
}

void Solver::update() {
  size_t particles_to_emit;

  for(int i = 0; i < substeps; ++i) {
    if (inactive_indices.size() < max_num_of_particles) {
      for (auto emitter = emitters.begin(); emitter != emitters.end(); ++emitter) {
        particles_to_emit = getNumerOfParticlesToEmit(emitter);
      }
    }
    else { break; }
  }
}

size_t Solver::getNumerOfParticlesToEmit(vector<Emitter>::iterator emitter) {
  // the exact amount of particles we should emit at this timestep
  double particles_to_emit_exact = emitter->rate * h;

  // since we can only emit whole particles we floor the exact particle count
  size_t particles_to_emit_actual = (size_t) particles_to_emit_exact;

  // accumulate the fractional amount of particles missed
  missed_particle += particles_to_emit_exact - floor(particles_to_emit_exact);

  // when we have one whole missed particle add it to the particles to emit
  if (missed_particle > 1.0f) {
    ++particles_to_emit_actual;
    missed_particle -= 1.0f;
  }

  return particles_to_emit_actual;
}
