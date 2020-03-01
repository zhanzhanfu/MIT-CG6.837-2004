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

