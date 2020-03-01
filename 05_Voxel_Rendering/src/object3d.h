#pragma once

#include "ray.h"
#include "hit.h"
#include "material.h"
#include "matrix.h"
#include "boundingbox.h"

class Grid;

//Object3D
class Object3D {
public:
    Object3D() : material(nullptr), boundingBox(nullptr) {}

    virtual ~Object3D() {}

    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;

    virtual BoundingBox *getBoundingBox() { return nullptr; }

    virtual void insertIntoGrid(Grid *g, Matrix *m) {}

    Material *material;

    BoundingBox *boundingBox;

};


//Plane
class Plane : public Object3D {
public:
    Plane(const Vec3f &n, float d, Material *m) : normal(n), distance(d) {
        material = m;
        normal.Normalize();
    }

    ~Plane() {}

    bool intersect(const Ray &r, Hit &h, float tmin);

    Vec3f normal;
    float distance;

};

//Triangle
class Triangle : public Object3D {
public:
    Triangle(const Vec3f &a, const Vec3f &b, const Vec3f &c, Material *m) : a(a), b(b), c(c) {
        material = m;
        Vec3f::Cross3(normal, b - a, c - a);
        normal.Normalize();
    }

    ~Triangle() {}

    bool intersect(const Ray &r, Hit &h, float tmin);

    Vec3f a;
    Vec3f b;
    Vec3f c;
    Vec3f normal;
};

//Transform
class Transform : public Object3D {
public:
    Transform(const Matrix &m, Object3D *o) : matrix(m), object(o) {}

    ~Transform() {}

    bool intersect(const Ray &r, Hit &h, float tmin);

    Matrix matrix;
    Object3D *object;
};




