//
// Created by constantin on 08/11/22.
//

#pragma once
#include "primitive.h"

class Cylinder : public Primitive{
protected:
    Vector c;
    double h;
    double r;
public:
    Cylinder(const Vector &c, double h, double r);

    virtual ~Cylinder()=default;

    bool Intersect(const Ray &ray, double &d, double &d1, double &d2) const override;

    bool IsInside(const Vector &p) const override;

    Vector Center() const override;

    double Volume() const override;

    void Translate(const Vector &v) override;

    void Scale(double x) override;

    const Vector &getC() const;

    double getR() const;

    double getH() const;
};



