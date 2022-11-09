//
// Created by constantin on 08/11/22.
//

#pragma once

#include "mathematics.h"

class Primitive {
public:

    ~Primitive(){}

    virtual bool IsInside(const Vector & p) const=0;

    virtual Vector Center() const=0;

    virtual double Volume() const=0;

    virtual void Translate(const Vector & v)=0;

    virtual void Scale(double x)=0;
};
