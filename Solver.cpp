//
// Created by Austin Brennan on 9/15/16.
//

#include "Solver.h"

Solver::Solver(size_t Max_num_of_particles, std::vector<Emitter> Emitters,
               std::vector<Collider> Colliders, VelocityGrid Velocity_grid,
               double H, size_t Substeps) {
  init(Max_num_of_particles, Emitters, Colliders, Velocity_grid, H, Substeps);
}

void Solver::init(size_t Max_num_of_particles, std::vector<Emitter> Emitters,
                  std::vector<Collider> Colliders, VelocityGrid Velocity_grid,
                  double H, size_t Substeps) {
  srand ((unsigned int) time(NULL));
  max_num_of_particles = Max_num_of_particles;
  particles.reserve(max_num_of_particles);
  inactive_indices.reserve(max_num_of_particles);
  emitters = Emitters;
  colliders = Colliders;
  velocity_grid = Velocity_grid;
  h = H;
  substeps = Substeps;
  missed_particle = 0.0;
}


void Solver::updateParticles() {
  Vector3d vel_from_grid; // velocity from the velocity grid
  Vector3d pos_new, vel_new, pos_collision, vel_collision, vel_normal, vel_tangent;
  Vector3d collider_normal;
  double distance_to_collider, total_distance;
  int collider_index;
  Collider collider; //
  double particle_mass, s;

  // update the particle attributes
  for(auto p = particles.begin(); p != particles.end(); ++p) {
    vel_from_grid = velocity_grid.get_velocity(p->pos);

    // adjust the mass in the case that we have a negative or zero mass
    particle_mass = p->mass ? p->mass > 0.0 : DBL_MIN;

    vel_new = p->vel + vel_from_grid * 1.0/particle_mass * h;
    pos_new = p->pos + ((vel_new + p->vel) / 2.0) * h;

    collider_index = particleInCollider(pos_new);
    if (particleInCollider(pos_new) == NO_COLLISION) { // no collision occurred
      p->pos = pos_new;
      p->vel = vel_new;
    }
    else { // a collision occurred
      collider = colliders.at((size_t) collider_index);
      distance_to_collider = (collider.pos - p->pos).norm() - collider.radius;
      total_distance = (pos_new - p->pos).norm(); // total distance the particle traveled
      s = distance_to_collider / total_distance; // fraction of timestep before collision
      vel_collision = p->vel + vel_from_grid * 1.0/particle_mass * h * s;
      pos_collision = p->pos + p->vel * h * s;
      collider_normal = (pos_collision - collider.pos).normalize();
      vel_normal = -1.0 * (vel_collision * collider_normal);
      vel_tangent = vel_collision - vel_normal;
      vel_collision = vel_normal + vel_tangent;
      vel_from_grid = velocity_grid.get_velocity(pos_collision);
      p->vel = vel_collision + vel_from_grid * 1.0/particle_mass * h * (1.0 - s);
      p->pos = pos_collision + vel_collision * h * (1.0 - s);
    }

    p->life_left = p->life_left - h; // increment the lifetime down by a timestep
  }

  // TODO use an active and inactive list instead of creating and removing particles
  // remove particles which have no life left
  particles.erase(std::remove_if(particles.begin(), particles.end(),
                                 [](const Particle & p) {return p.life_left <= 0.0; }),
                  particles.end());
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


int Solver::particleInCollider(Vector3d particle_position) {
  int collider = NO_COLLISION; // the collider the particle is inside

  // check each collider to see if the particle is inside it
  for (auto c = colliders.begin(); c != colliders.end(); ++c) {
    if ((c->pos - particle_position).norm() < c->radius) {
      collider = c - colliders.begin();
    }
  }

  return collider;
}


void Solver::emitParticles(size_t number_of_particles, vector<Emitter>::iterator emitter) {
  Vector3d pos; // initial position of particle
  Vector3d vel; // initial velocity of particle
  Vector3d color;
  double mass; // initial mass of particle
  double lifetime; // initial lifetime of particle

  for(int i = 0; i < number_of_particles; ++i) {
    // TODO: update this to actually emit in a sphere (Emitting in cube now)
    // create initial particle attributes
    pos.x = emitter->pos.x + gaussian_distribution(emitter->pos.x, emitter->radius);
    pos.y = emitter->pos.y + gaussian_distribution(emitter->pos.y, emitter->radius);
    pos.z = emitter->pos.x + gaussian_distribution(emitter->pos.x, emitter->radius);
    vel = (0.0, 0.0, 0.0);
    mass = gaussian_distribution(emitter->particle_mass_avg, emitter->particle_mass_sdv);
    lifetime = gaussian_distribution(emitter->particle_life_avg, emitter->particle_life_sdv);
    color.x = gaussian_distribution(emitter->particle_color_avg.x, emitter->particle_color_sdv);
    color.y = gaussian_distribution(emitter->particle_color_avg.y, emitter->particle_color_sdv);
    color.z = gaussian_distribution(emitter->particle_color_avg.z, emitter->particle_color_sdv);

    // Don't create the particle if it is inside a collider
    if (particleInCollider(pos) == NO_COLLISION) {
      // create a new particle
      particles.push_back(Particle(pos, vel, color, mass, lifetime));
    }
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
