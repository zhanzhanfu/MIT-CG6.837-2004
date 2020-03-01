#include "group.h"


//Group
bool Group::intersect(const Ray &r, Hit &h, float tmin) const{
    bool flag = false;
    for (int i = 0; i < num_objects; ++i) {
        if (objects[i]->intersect(r, h, tmin)) {
            flag = true;
        }
    }
    return flag;
}


void Group::addObject(int index, Object3D *obj){
    objects[index] = obj;
}

void Group::paint() const {
    for (int i = 0; i < num_objects; ++i) {
        objects[i]->paint();
    }
}
