//
// Created by constantin on 10/11/22.
//

#pragma once
#include "ray.h"

/**
 * Interface for Ray intersection
 */
class Intersectable {
public:
    virtual ~Intersectable()=default;
    virtual bool Intersect(const Ray &, double &, double &, double &) const = 0;
};

