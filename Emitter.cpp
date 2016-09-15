//
// Created by awbrenn on 9/15/16.
//

#include "Emitter.h"

Emitter::Emitter(double Rate, double Radius, bool On_surface, Vector3d Pos) {
  rate = Rate;
  radius = Radius;
  on_surface = On_surface;
  pos = Pos;
}