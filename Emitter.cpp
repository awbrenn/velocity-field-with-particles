//
// Created by awbrenn on 9/15/16.
//

#include "Emitter.h"

Emitter::Emitter(double Rate, double Radius, bool On_surface, Vector3d Pos,
                 double Particle_mass_avg, double Particle_mass_sdv,
                 double Particle_life_avg, double Particle_live_sdv,
                 Vector3d Particle_color_avg, double Particle_color_sdv) {
  rate = Rate;
  radius = Radius;
  on_surface = On_surface;
  pos = Pos;
  particle_mass_avg = Particle_mass_avg;
  particle_mass_sdv = Particle_mass_sdv;
  particle_life_avg = Particle_life_avg;
  particle_life_sdv = Particle_live_sdv;
  particle_color_avg = Particle_color_avg;
  particle_color_sdv = Particle_color_sdv;
}