//
// Created by constantin on 27/09/22.
//

#include "sphere.h"

Sphere::Sphere(const Vector &center, const double &r) : c(center), r(r), r2(r * r) {}

bool Sphere::IsInside(const Vector &p) const {
    return SquaredDistanceToCenter(p) < r2;
}

double Sphere::DistanceToCenter(const Vector &pos) const {
    return Norm(pos - c);
}

double Sphere::SquaredDistanceToCenter(const Vector &pos) const {
    return SquaredNorm(pos - c);
}

double Sphere::DistanceToSphere(const Vector &pos) const {
    return DistanceToCenter(pos) - r;
}

double Sphere::SquaredDistanceToSphere(const Vector &pos) const {
    return SquaredDistanceToCenter(pos) - r2;
}

double Sphere::OneMinusPercentToCenter(const Vector &pos) const {
    return 1 - Math::Min((SquaredDistanceToCenter(pos))/r2,1);
}

const Vector &Sphere::getC() const {
    return c;
}

double Sphere::getR() const {
    return r;
}

double Sphere::getR2() const {
    return r2;
}

double Sphere::Volume() const {
    return (4.0/3.0)* Math::PI() * Sphere::r2*Sphere::r;
}

void Sphere::Translate(const Vector &v) {
    Sphere::c+=v;
}

void Sphere::Scale(double x) {
    Sphere::r*=x;
    Sphere::r2 = Sphere::r * Sphere::r;
}

Vector Sphere::Center() const {
    return c;
}

