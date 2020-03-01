#pragma once

#include "ray.h"
#include "hit.h"
#include "material.h"
#include "matrix.h"

//Object3D
class Object3D {
public:
    Object3D() : material(nullptr) {}

    virtual ~Object3D() {}

    virtual bool intersect(const Ray &r, Hit &h, float tmin) const = 0;

    virtual void paint() const = 0;

    Material *material;

};










