//
// Created by constantin on 08/11/22.
//

#pragma once

#include "mathematics.h"
#include "intersectable.h"

class Primitive : public Intersectable {
public:

    ~Primitive() = default;

    virtual bool IsInside(const Vector &p) const = 0;

    virtual Vector Center() const = 0;

    virtual double Volume() const = 0;

    virtual void Translate(const Vector &v) = 0;

    virtual void Scale(double x) = 0;

    virtual bool Intersect(const Ray &ray, double &d, double &d1, double &d2) const = 0;
};
