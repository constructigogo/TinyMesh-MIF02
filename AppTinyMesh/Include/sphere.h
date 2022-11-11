//
// Created by constantin on 27/09/22.
//

#ifndef APPTINYMESH_SPHERE_H
#define APPTINYMESH_SPHERE_H

#include "primitive.h"

class Sphere : public Primitive {
    Vector c;
    double r;
public:
    const Vector &getC() const;

    double getR() const;

    double getR2() const;

    virtual ~Sphere()=default;

public:
    Sphere(const Vector &center, const double &r);

    bool IsInside(const Vector &p) const override;

    double DistanceToCenter(const Vector &pos) const;

    double DistanceToSphere(const Vector &pos) const;

    double SquaredDistanceToCenter(const Vector &pos) const;

    double SquaredDistanceToSphere(const Vector &pos) const;

    double OneMinusPercentToCenter(const Vector &pos) const;

    Vector Center() const override;

    double Volume() const override;

    void Translate(const Vector &v) override;

    void Scale(double x) override;

    bool Intersect(const Ray &ray, double &d, double &d1, double &d2) const override;


private:
    double r2;
};

#endif //APPTINYMESH_SPHERE_H
