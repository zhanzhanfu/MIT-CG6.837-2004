#pragma once

#include "object3d.h"

//Sphere
class Sphere : public Object3D {
public:
    Sphere(const Vec3f &c, float r, Material *m) : center(c), radius(r) {
        material = m;
    }

    virtual bool intersect(const Ray &r, Hit &h, float tmin) const;

    virtual void paint() const;

    Vec3f center;
    float radius;
};


