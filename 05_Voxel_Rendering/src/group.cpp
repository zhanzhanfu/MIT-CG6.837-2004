#include "group.h"

//Group
bool Group::intersect(const Ray &r, Hit &h, float tmin) {
    bool flag = false;
    for (int i = 0; i < num_objects; ++i) {
        if (objects[i]->intersect(r, h, tmin)) {
            flag = true;
        }
    }
    return flag;
}

void Group::addObject(int index, Object3D *obj) {
    objects[index] = obj;
    boundingBox->Extend(obj->getBoundingBox());
}

bool Group::intersectShadowRay(const Ray &r, Hit &h, float tmin) {
    for (int i = 0; i < num_objects; ++i) {
        if (objects[i]->intersect(r, h, tmin))
            return true;
    }
    return false;
}

void Group::insertIntoGrid(Grid *g, Matrix *m) {
    for (int i = 0; i < num_objects; ++i) {
        objects[i]->insertIntoGrid(g, m);
    }
}
