//
// Created by awbrenn on 9/18/16.
//

#include "Collider.h"

Collider::Collider(Vector3d Pos, double Radius) {
  pos = Pos;
  radius = Radius;
}

Collider::Collider() {
  pos = Vector3d();
  radius = 1.0;
}
