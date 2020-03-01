#pragma once

#include "object3d.h"

//Transform
class Transform : public Object3D {
public:
    Transform(const Matrix &m, Object3D *o) : matrix(m), object(o) {}

    virtual bool intersect(const Ray &r, Hit &h, float tmin) const;

    virtual void paint() const;

    Matrix matrix;
    Object3D *object;
};