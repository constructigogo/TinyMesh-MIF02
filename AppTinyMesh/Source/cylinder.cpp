//
// Created by constantin on 08/11/22.
//

#include "cylinder.h"

bool Cylinder::IsInside(const Vector &p) const {
    return false;
}

Vector Cylinder::Center() const {
    return Vector();
}

double Cylinder::Volume() const {
    return 0;
}

void Cylinder::Translate(const Vector &v) {

}

void Cylinder::Scale(double x) {

}

const Vector &Cylinder::getC() const {
    return c;
}

double Cylinder::getR() const {
    return r;
}

double Cylinder::getH() const {
    return h;
}

Cylinder::Cylinder(const Vector &c, double h, double r) : c(c), h(h), r(r) {}