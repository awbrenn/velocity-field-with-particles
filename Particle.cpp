//
// Created by awbrenn on 9/15/16.
//

#include "Particle.h"

Particle::Particle(Vector3d Pos, Vector3d Vel, Vector3d Color, double Mass, double Lifetime) {
  pos = Pos;
  vel = Vel;
  color = Color;
  mass = Mass;
  lifetime = Lifetime;
  life_left = lifetime;
  active = true;
}