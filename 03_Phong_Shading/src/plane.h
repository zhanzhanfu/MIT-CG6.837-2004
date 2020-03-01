#pragma once

#include "object3d.h"

//Plane
class Plane : public Object3D {
public:
    Plane(const Vec3f &n, float d, Material *m) : normal(n), distance(d) {
        material = m;
        normal.Normalize();
    }

    virtual bool intersect(const Ray &r, Hit &h, float tmin) const;

    virtual void paint() const;

    Vec3f normal;
    float distance;

};