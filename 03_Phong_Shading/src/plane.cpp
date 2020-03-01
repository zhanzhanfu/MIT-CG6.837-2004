#include "plane.h"
#include <GL/freeglut.h>

//Plane
bool Plane::intersect(const Ray &r, Hit &h, float tmin) const {
    Vec3f ro = r.getOrigin();
    Vec3f rd = r.getDirection();
    float denom = normal.Dot3(rd);
    if (fabs(denom) < 0.0001)
        return false;
    float t = (distance - normal.Dot3(ro)) / denom;
    if (t > tmin && t < h.getT()) {
        h.set(t, material, normal, r);
        return true;
    }
    return false;
}

void Plane::paint() const {
    Vec3f u(1, 0, 0);
    Vec3f n = normal;
    n.Normalize();
    if (fabs(n.Dot3(u) - 1) < 0.01)
        u = Vec3f(0, 1, 0);
    Vec3f v;
    Vec3f::Cross3(v, n, u);
    Vec3f::Cross3(u, v, n);
    Vec3f center = distance * n;
    const int INF = 10000;
    Vec3f pos[4] = {center + INF * u, center + INF * v, center - INF * u, center - INF * v};
    material->glSetMaterial();
    glBegin(GL_QUADS);
    glNormal3f(n.x(), n.y(), n.z());
    glVertex3f(pos[0].x(), pos[0].y(), pos[0].z());
    glVertex3f(pos[1].x(), pos[1].y(), pos[1].z());
    glVertex3f(pos[2].x(), pos[2].y(), pos[2].z());
    glVertex3f(pos[3].x(), pos[3].y(), pos[3].z());
    glEnd();
}
