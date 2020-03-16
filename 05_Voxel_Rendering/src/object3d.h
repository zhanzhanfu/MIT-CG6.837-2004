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

    virtual void paint() const = 0;

    virtual Material *getMaterial() const { return material; }

    virtual BoundingBox *getBoundingBox() const { return boundingBox; }

    virtual void insertIntoGrid(Grid *g, Matrix *m);

    virtual bool is_triangle() const { return false; } //use a special case for transformed triangles

    virtual BoundingBox *getTriangleBoundingBox(const Matrix *m) const { return boundingBox; } //use a special case for transformed triangles

protected:
    Material *material;
    BoundingBox *boundingBox;
};

//Sphere
class Sphere : public Object3D {
public:
    Sphere(const Vec3f &c, float r, Material *m) : center(c), radius(r) {
        material = m;
        boundingBox = new BoundingBox(center - Vec3f(radius, radius, radius), center + Vec3f(radius, radius, radius));
    }

    ~Sphere() override {}

    bool intersect(const Ray &r, Hit &h, float tmin) override;

    virtual void paint() const;

    virtual void insertIntoGrid(Grid *g, Matrix *m);

private:
    Vec3f center;
    float radius;
};

//Plane
class Plane : public Object3D {
public:
    Plane(const Vec3f &n, float d, Material *m) : normal(n), distance(d) {
        material = m;
        normal.Normalize();
    }

    ~Plane() override {}

    bool intersect(const Ray &r, Hit &h, float tmin) override;

    virtual void paint() const;

    virtual BoundingBox *getBoundingBox() const { return nullptr; }

    virtual void insertIntoGrid(Grid *g, Matrix *m) { return; }

private:
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
        boundingBox = new BoundingBox(Vec3f(min(a.x(), b.x()), min(a.y(), b.y()), min(a.z(), b.z())),
                                      Vec3f(max(a.x(), b.x()), max(a.y(), b.y()), max(a.z(), b.z())));
        boundingBox->Extend(c);
    }

    ~Triangle() override {}

    bool intersect(const Ray &r, Hit &h, float tmin) override;

    virtual void paint() const;

    virtual void insertIntoGrid(Grid *g, Matrix *m);

    virtual bool is_triangle() const { return true; }

    virtual BoundingBox *getTriangleBoundingBox(const Matrix *m) const;

private:
    Vec3f a, b, c;
    Vec3f normal;
};

//Transform
class Transform : public Object3D {
public:
    Transform(const Matrix &m, Object3D *o) : matrix(m), object(o) {
        material = object->getMaterial();
        boundingBox = object->getBoundingBox();
    }

    ~Transform() override {}

    bool intersect(const Ray &r, Hit &h, float tmin) override;

    virtual void paint() const;

    virtual BoundingBox *getBoundingBox() const;

    virtual void insertIntoGrid(Grid *g, Matrix *m);

private:
    Matrix matrix;
    Object3D *object;
};

//Group
class Group : public Object3D {
public:
    Group(int n) : num_objects(n) {
        objects = new Object3D *[num_objects];
        boundingBox = new BoundingBox(Vec3f(INFINITY, INFINITY, INFINITY), Vec3f(-INFINITY, -INFINITY, -INFINITY));
    }

    ~Group() override {
        for (int i = 0; i < num_objects; ++i) {
            delete objects[i];
        }
        delete[] objects;
    }

    bool intersect(const Ray &r, Hit &h, float tmin) override;

    void addObject(int index, Object3D *obj);


    virtual void paint() const;

    bool intersectShadowRay(const Ray &r, Hit &h, float tmin);

    virtual void insertIntoGrid(Grid *g, Matrix *m);

private:
    int num_objects;
    Object3D **objects;
};
