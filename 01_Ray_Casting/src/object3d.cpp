#include "object3d.h"

//Sphere
//Algebraic
//bool Sphere::intersect(const Ray &r, Hit &h, float tmin) {
//    bool flag = false;
//    Vec3f ro = r.getOrigin() - center;
//    Vec3f rd = r.getDirection();
//
//    float a = rd.Dot3(rd);
//    float b = 2 * ro.Dot3(rd);
//    float c = ro.Dot3(ro) - radius * radius;
//    float delta = b * b - 4 * a * c;
//    float t = INFINITY;
//    if (delta >= 0) {
//        float d = sqrt(delta);
//        float t1 = (-b - d) / (2 * a);
//        float t2 = (-b + d) / (2 * a);
//        if (t1 >= tmin) {
//            t = t1;
//        } else if (t2 >= tmin) {
//            t = t2;
//        }
//        if (t < h.getT()) {
//            h.set(t, material, r);
//            flag = true;
//        }
//    }
//    return flag;
//}

//Geometric
bool Sphere::intersect(const Ray &r, Hit &h, float tmin) {
    bool flag = false;
    Vec3f ro = r.getOrigin() - center;
    Vec3f rd = r.getDirection();

    float tp = -ro.Dot3(rd);
    //if (tp < 0)
    //    return false;
    float d2 = ro.Dot3(ro) - tp * tp;
    if (d2 > radius * radius)
        return false;
    float tt = sqrt(radius * radius - d2);
    float t1 = tp - tt;  //out
    float t2 = tp + tt;  //in
    float t = t1;
    //if(ro.Length() - radius>0)
    //    t = t1;
    //else
    //    t = t2;
    if (t > tmin && t < h.getT()) {
        h.set(t, material, r);
        flag = true;
    }
    return flag;
}


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
}