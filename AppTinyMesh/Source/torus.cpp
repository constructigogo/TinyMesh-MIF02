//
// Created by constantin on 09/11/22.
//

#include "torus.h"

bool Torus::IsInside(const Vector &p) const {
    return false;//TODO
}

Vector Torus::Center() const {
    return c;
}

double Torus::Volume() const {
    return (Math::PI()*a*a)*(2*Math::PI()*b);
}

void Torus::Translate(const Vector &v) {
    Torus::c+=v;
}

void Torus::Scale(double x) {
    Torus::a= x*a;
    Torus::b= x*b;
}

Torus::Torus(const Vector &c, double a, double b) : c(c), a(a), b(b) {}

const Vector &Torus::getC() const {
    return c;
}

double Torus::getA() const {
    return a;
}

double Torus::getB() const {
    return b;
}

bool Torus::Intersect(const Ray &ray, double &d, double &d1, double &d2) const {
    return false;
}
