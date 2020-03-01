#include "sphere.h"

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

void Sphere::insertIntoGrid(Grid *g, Matrix *m) {
    BoundingBox *bb = g->getBoundingBox();
    Vec3f minP = bb->getMin();
    Vec3f maxP = bb->getMax();

    int nx = g->nx;
    int ny = g->ny;
    int nz = g->nz;
    float gridx = (maxP - minP).x() / float(nx);
    float gridy = (maxP - minP).y() / float(ny);
    float gridz = (maxP - minP).z() / float(nz);
    for (int k = 0; k < nz; ++k) {
        for (int j = 0; j < ny; ++j) {
            for (int i = 0; i < nx; ++i) {
                float x = (i + 0.5) * gridx;
                float y = (j + 0.5) * gridy;
                float z = (k + 0.5) * gridz;
                if ((Vec3f(x, y, z) - (center - minP)).Length() < radius + gridx / 2) {
                    int t = nx * ny * k + nx * j + i;
                    g->opaque[t] = true;
                }
            }
        }
    }

}
