#pragma once

#include "object3d.h"
#include "grid.h"

//Sphere
class Sphere : public Object3D {
public:
    Sphere(const Vec3f &c, float r, Material *m) : center(c), radius(r) {
        material = m;
        boundingBox = new BoundingBox(center - Vec3f(r, r, r), center + Vec3f(r, r, r));
    }

    ~Sphere() { delete boundingBox; }

    virtual bool intersect(const Ray &r, Hit &h, float tmin);

    virtual BoundingBox *getBoundingBox() { return boundingBox; }

    virtual void insertIntoGrid(Grid *g, Matrix *m);

    Vec3f center;
    float radius;
};