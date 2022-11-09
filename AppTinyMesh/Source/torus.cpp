//
// Created by constantin on 09/11/22.
//

#include "torus.h"

bool Torus::IsInside(const Vector &p) const {
    return false;//TODO
}

Vector Torus::Center() const {
    return Vector();
}

double Torus::Volume() const {
    return 0;//TODO
}

void Torus::Translate(const Vector &v) {

}

void Torus::Scale(double x) {

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
