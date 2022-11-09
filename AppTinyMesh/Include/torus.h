//
// Created by constantin on 09/11/22.
//

#pragma once

#include "primitive.h"

class Torus : public Primitive {
private:
    Vector c;
    double a;
    double b;
public:
    Torus(const Vector &c, double a, double b);

    const Vector &getC() const;

    double getA() const;

    double getB() const;

    bool IsInside(const Vector &p) const override;

    Vector Center() const override;

    double Volume() const override;

    void Translate(const Vector &v) override;

    void Scale(double x) override;
};
