//
// Created by constantin on 09/11/22.
//

#pragma once

#include "cylinder.h"

class Capsule : public Cylinder {
public:
    Capsule(const Vector &c, double h, double r);

    virtual ~Capsule() = default;

    bool Intersect(const Ray &ray, double &d, double &d1, double &d2) const override;

    bool IsInside(const Vector &p) const override;

    double Volume() const override;
};