#include "object3d.h"

//copy from Matrix.C
float det2(float a, float b,
           float c, float d) {
    return a * d - b * c;
}

float det3(float a1, float a2, float a3,
           float b1, float b2, float b3,
           float c1, float c2, float c3) {
    return
            a1 * det2(b2, b3, c2, c3)
            - b1 * det2(a2, a3, c2, c3)
            + c1 * det2(a2, a3, b2, b3);
}

//Sphere
//Algebraic
//bool Sphere::intersect(const Ray &r, Hit &h, float tmin) {
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
//            Vec3f normal = ro + t * rd;
//            normal.Normalize();
//            h.set(t,material,normal,r);
//            return true;
//        }
//    }
//    return false;
//}

//Geometric
bool Sphere::intersect(const Ray &r, Hit &h, float tmin) {
    Vec3f ro = r.getOrigin() - center;
    Vec3f rd = r.getDirection();
    float rd_len = rd.Length();
    float tp = -ro.Dot3(rd) / (rd_len);
    if (tp < 0)
        return false;
    float d2 = ro.Dot3(ro) - tp * tp;
    if (d2 > radius * radius)
        return false;
    float tt = sqrt(radius * radius - d2);
    float t1 = tp - tt;  //out
    float t2 = tp + tt;  //in
    float t;
    if (ro.Length() - radius > 0)
        t = t1;
    else
        t = t2;
    t = t / rd_len;
    if (t > tmin && t < h.getT()) {
        Vec3f normal = ro + t * rd;
        normal.Normalize();
        h.set(t, material, normal, r);
        return true;
    }
    return false;
}


//Plane
bool Plane::intersect(const Ray &r, Hit &h, float tmin) {
    Vec3f ro = r.getOrigin();
    Vec3f rd = r.getDirection();
    float denom = normal.Dot3(rd);
    if (denom == 0)
        return false;
    float t = (distance - normal.Dot3(ro)) / denom;
    if (t > tmin && t < h.getT()) {
        h.set(t, material, normal, r);
        return true;
    }
    return false;
}

//Triangle
bool Triangle::intersect(const Ray &r, Hit &h, float tmin) {
    Vec3f ro = r.getOrigin();
    Vec3f rd = r.getDirection();
    float abx = a.x() - b.x();
    float aby = a.y() - b.y();
    float abz = a.z() - b.z();
    float acx = a.x() - c.x();
    float acy = a.y() - c.y();
    float acz = a.z() - c.z();
    float aox = a.x() - ro.x();
    float aoy = a.y() - ro.y();
    float aoz = a.z() - ro.z();
    float rdx = rd.x();
    float rdy = rd.y();
    float rdz = rd.z();
    float A = det3(abx, acx, rdx, aby, acy, rdy, abz, acz, rdz);
    float beta = det3(aox, acx, rdx, aoy, acy, rdy, aoz, acz, rdz) / A;
    float gamma = det3(abx, aox, rdx, aby, aoy, rdy, abz, aoz, rdz) / A;
    if (beta > 0 && gamma > 0 && (beta + gamma) < 1) {
        float t = det3(abx, acx, aox, aby, acy, aoy, abz, acz, aoz) / A;
        if (t > tmin && t < h.getT()) {
            h.set(t, material, normal, r);
            return true;
        }
    }
    return false;
}

//Transform
bool Transform::intersect(const Ray &r, Hit &h, float tmin) {
    Vec3f ro = r.getOrigin();
    Vec3f rd = r.getDirection();
    Matrix invM = matrix;
    if (invM.Inverse()) {
        invM.Transform(ro);
        invM.TransformDirection(rd);
        //rd.Normalize();
        Ray invR(ro, rd);
        if (object->intersect(invR, h, tmin)) {
            Vec3f normal = h.getNormal();
            Matrix invMT = invM;
            invMT.Transpose();
            invMT.TransformDirection(normal); //trans normal to world space
            normal.Normalize();
            h.set(h.getT(), h.getMaterial(), normal, r);
            return true;
        }
    }
    return false;
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

bool Group::intersectShadowRay(const Ray &r, Hit &h, float tmin) {
    for (int i = 0; i < num_objects; ++i) {
        if (objects[i]->intersect(r, h, tmin))
            return true;
    }
}


