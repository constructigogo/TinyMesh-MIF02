//
// Created by constantin on 09/11/22.
//

#include "capsule.h"

bool Capsule::IsInside(const Vector &p) const {
    return Cylinder::IsInside(p);
}

double Capsule::Volume() const {
    return (Math::PI()*r*r)*(((4.0/3.0)*r)+(h*2.0));
}

Capsule::Capsule(const Vector &c, double h, double r) : Cylinder(c, h, r) {}

bool Capsule::Intersect(const Ray &ray, double &d, double &d1, double &d2) const {
    return false;
}
