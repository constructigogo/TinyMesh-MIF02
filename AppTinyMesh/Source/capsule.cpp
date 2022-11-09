//
// Created by constantin on 09/11/22.
//

#include "capsule.h"

bool Capsule::IsInside(const Vector &p) const {
    return Cylinder::IsInside(p);
}

double Capsule::Volume() const {
    return Cylinder::Volume();
}

Capsule::Capsule(const Vector &c, double h, double r) : Cylinder(c, h, r) {}
