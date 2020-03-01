#pragma once

#include "object3d.h"


//Triangle
class Triangle : public Object3D {
public:
    Triangle(const Vec3f &a, const Vec3f &b, const Vec3f &c, Material *m) : a(a), b(b), c(c) {
        material = m;
        Vec3f::Cross3(normal, b - a, c - a);
        normal.Normalize();
    }

    virtual bool intersect(const Ray &r, Hit &h, float tmin) const;

    virtual void paint() const;

    Vec3f a;
    Vec3f b;
    Vec3f c;
    Vec3f normal;
};