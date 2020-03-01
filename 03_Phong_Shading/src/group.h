#pragma once

#include "object3d.h"

//Group
class Group : public Object3D {
public:
    Group(int n) : num_objects(n) {
        objects = new Object3D *[num_objects];
    }

    ~Group() {
        for (int i = 0; i < num_objects; ++i) {
            delete objects[i];
        }
        delete[] objects;
    }

    virtual bool intersect(const Ray &r, Hit &h, float tmin) const;

    virtual void paint() const;

    void addObject(int index, Object3D *obj);

    int num_objects;
    Object3D **objects;
};
