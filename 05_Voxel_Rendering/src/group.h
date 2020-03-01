#pragma once

#include "object3d.h"
#include "grid.h"

//Group
class Group : public Object3D {
public:
    Group(int n) : num_objects(n) {
        objects = new Object3D *[num_objects];
        boundingBox = new BoundingBox(Vec3f(INT_MAX, INT_MAX, INT_MAX), Vec3f(INT_MIN, INT_MIN, INT_MIN));
    }

    ~Group() {
        for (int i = 0; i < num_objects; ++i) {
            delete objects[i];
        }
        delete[] objects;
        delete boundingBox;
    }

    bool intersect(const Ray &r, Hit &h, float tmin);

    void addObject(int index, Object3D *obj);

    bool intersectShadowRay(const Ray &r, Hit &h, float tmin);

    virtual BoundingBox *getBoundingBox() { return boundingBox; }

    virtual void insertIntoGrid(Grid *g, Matrix *m);

    int num_objects;
    Object3D **objects;
};