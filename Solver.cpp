//
// Created by Austin Brennan on 9/15/16.
//

#include "Solver.h"

Solver::Solver(size_t Max_num_of_particles, std::vector<Emitter> Emitters,
               VelocityGrid velocity_grid, double h, size_t substeps) {
  init(Max_num_of_particles, Emitters, velocity_grid, h, substeps);
}

Solver::Solver(size_t Max_num_of_particles, Emitter emitter,
               VelocityGrid velocity_grid, double h, size_t substeps) {
  std::vector<Emitter> emitter_vector;
  emitter_vector.push_back(emitter);
  init(Max_num_of_particles, emitter_vector, velocity_grid, h,substeps);
}

void Solver::init(size_t Max_num_of_particles, std::vector<Emitter> Emitters,
                  VelocityGrid Velocity_grid, double H, size_t Substeps) {
  srand (time(NULL));
  max_num_of_particles = Max_num_of_particles;
  particles.reserve(max_num_of_particles);
  inactive_indices.reserve(max_num_of_particles);
  emitters = Emitters;
  velocity_grid = Velocity_grid;
  h = H;
  substeps = Substeps;
  missed_particle = 0.0;
}


void Solver::updateParticles() {
  Vector3d vel_from_grid; // velocity from the velocity grid
  Vector3d vel_new;

  for(auto p = particles.begin(); p != particles.end(); ++p) {
    vel_from_grid = velocity_grid.get_velocity(p->pos);

    vel_new = p->vel + vel_from_grid * h;
    p->pos = p->pos + ((vel_new + p->vel) / 2.0) * h;
    p->vel = vel_new;
  }
}


void Solver::update() {
  size_t particles_to_emit;

  for(int i = 0; i < substeps; ++i) {
    // if we can still emit particles, emit them
    if (particles.size() < max_num_of_particles) {
      for (auto emitter = emitters.begin(); emitter != emitters.end(); ++emitter) {
        // get the number of particles to emit this timestep
        particles_to_emit = getNumerOfParticlesToEmit(emitter);

        // emit the particles
        emitParticles(particles_to_emit, emitter);
      }
    }
    // move the particles
    updateParticles();
  }
}


double Solver::uniform_distribution(double min, double max) {
  return min + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(max-min)));
}

double Solver::gaussian_distribution(double x, double std_deviation) {
  std::normal_distribution<double> distribution(x,std_deviation);
  return distribution(generator);
}


void Solver::emitParticles(size_t number_of_particles, vector<Emitter>::iterator emitter) {
  Vector3d pos; // initial position of particle
  Vector3d vel; // initial velocity of particle
  double mass; // initial mass of particle

  for(int i = 0; i < number_of_particles; ++i) {
    // TODO: update this to actually emit in a sphere (Emitting in cube now)
    pos.x = emitter->pos.x + gaussian_distribution(emitter->pos.x, emitter->radius);
    pos.y = emitter->pos.y + gaussian_distribution(emitter->pos.y, emitter->radius);
    pos.z = emitter->pos.x + gaussian_distribution(emitter->pos.x, emitter->radius);
    vel = (0.0, 0.0, 0.0);
    mass = 1.0;
    particles.push_back(Particle(pos, vel, mass));
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
