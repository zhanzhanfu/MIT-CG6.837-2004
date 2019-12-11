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

    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;

    Material *material;

};

//Sphere
class Sphere : public Object3D {
public:
    Sphere(Vec3f c, float r, Material *m) : center(c), radius(r) {
        material = m;
    }

    ~Sphere() override {}

    bool intersect(const Ray &r, Hit &h, float tmin) override;

    Vec3f center;
    float radius;
};

//Plane
class Plane : public Object3D {
public:
    Plane(Vec3f &n, float d, Material *m) : normal(n), distance(d) {
        material = m;
        normal.Normalize();
    }

    ~Plane() override {}

    bool intersect(const Ray &r, Hit &h, float tmin) override;

    Vec3f normal;
    float distance;

};

//Triangle
class Triangle : public Object3D {
public:
    Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m) : a(a), b(b), c(c) {
        material = m;
        Vec3f::Cross3(normal, b - a, c - a);
        normal.Normalize();
    }

    ~Triangle() override {}

    bool intersect(const Ray &r, Hit &h, float tmin) override;

    Vec3f a;
    Vec3f b;
    Vec3f c;
    Vec3f normal;
};

//Transform
class Transform : public Object3D {
public:
    Transform(Matrix &m, Object3D *o):matrix(m),object(o) {}

    ~Transform() override {}

    bool intersect(const Ray &r, Hit &h, float tmin) override;

    Matrix matrix;
    Object3D *object;
};

//Group
class Group : public Object3D {
public:
    Group(int n) : num_objects(n) {
        objects = new Object3D *[num_objects];
    }

    ~Group() override {
        for (int i = 0; i < num_objects; ++i) {
            delete objects[i];
        }
        delete[] objects;
    }

    bool intersect(const Ray &r, Hit &h, float tmin) override;

    void addObject(int index, Object3D *obj);

    int num_objects;
    Object3D **objects;
};
