#include "object3d.h"
#include <GL/freeglut.h>
#include <vector>

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
bool Sphere::intersect(const Ray &r, Hit &h, float tmin) {
    Vec3f ro = r.getOrigin() - center;
    Vec3f rd = r.getDirection();

    float a = rd.Dot3(rd);
    float b = 2 * ro.Dot3(rd);
    float c = ro.Dot3(ro) - radius * radius;
    float delta = b * b - 4 * a * c;
    float t = INFINITY;
    if (delta >= 0) {
        float d = sqrt(delta);
        float t1 = (-b - d) / (2 * a);
        float t2 = (-b + d) / (2 * a);
        if (t1 >= tmin) {
            t = t1;
        } else if (t2 >= tmin) {
            t = t2;
        }
        if (t < h.getT()) {
            Vec3f normal = ro + t * rd;
            normal.Normalize();
            h.set(t, material, normal, r);
            return true;
        }
    }
    return false;
}

//Geometric
//bool Sphere::intersect(const Ray &r, Hit &h, float tmin) {
//    Vec3f ro = r.getOrigin() - center;
//    Vec3f rd = r.getDirection();
//    float rd_len = rd.Length();
//    float tp = -ro.Dot3(rd) / (rd_len);
//    if (tp < 0)
//        return false;
//    float d2 = ro.Dot3(ro) - tp * tp;
//    if (d2 > radius * radius)
//        return false;
//    float tt = sqrt(radius * radius - d2);
//    float t1 = tp - tt;  //out
//    float t2 = tp + tt;  //in
//    float t;
//    if (ro.Length() - radius > 0)
//        t = t1;
//    else
//        t = t2;
//    t = t / rd_len;
//    if (t > tmin && t < h.getT()) {
//        Vec3f normal = ro + t * rd;
//        normal.Normalize();
//        h.set(t, material, normal, r);
//        return true;
//    }
//    return false;
//}

extern int theta_steps;
extern int phi_steps;
extern bool gouraud;
const float PI = 3.14159265358979323846;

void Sphere::paint() const {
    material->glSetMaterial();
    float dt = 2 * PI / theta_steps;
    float dp = PI / phi_steps;
    std::vector<Vec3f> position;
    std::vector<Vec3f> normal;
    // initialize all the position for points
    for (int p = 0; p <= phi_steps; p++) {
        for (int t = 0; t < theta_steps; t++) {
            float theta = t * dt;
            float phi = p * dp;
            Vec3f pos = Vec3f(sin(phi) * cos(theta), cos(phi), sin(phi) * sin(theta));
            position.push_back(center + pos * radius);
            normal.push_back(pos);
        }
    }
    glBegin(GL_QUADS);
    for (int phi = 0; phi < phi_steps; phi++) {
        for (int theta = 0; theta < theta_steps; theta++) {
            int index[4] = {phi * theta_steps + theta,
                            (phi + 1) * theta_steps + theta,
                            (phi + 1) * theta_steps + (theta + 1) % theta_steps,
                            phi * theta_steps + (theta + 1) % theta_steps,};

            Vec3f n;
            if (!gouraud) {
                Vec3f::Cross3(n, position[index[0]] - position[index[1]], position[index[2]] - position[index[1]]);
                glNormal3f(n.x(), n.y(), n.z());
            }
            for (int i = 0; i < 4; i++) {
                if (gouraud) {
                    glNormal3f(normal[index[i]].x(), normal[index[i]].y(), normal[index[i]].z());
                }
                glVertex3f(position[index[i]].x(), position[index[i]].y(), position[index[i]].z());
            }
        }
    }
    glEnd();
}


//Plane
bool Plane::intersect(const Ray &r, Hit &h, float tmin) {
    Vec3f ro = r.getOrigin();
    Vec3f rd = r.getDirection();
    float denom = normal.Dot3(rd);
    if (fabs(denom) < 0.00001)
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

void Triangle::paint() const {
    material->glSetMaterial();
    glBegin(GL_TRIANGLES);
    glNormal3f(normal.x(), normal.y(), normal.z());
    glVertex3f(a.x(), a.y(), a.z());
    glVertex3f(b.x(), b.y(), b.z());
    glVertex3f(c.x(), c.y(), c.z());
    glEnd();
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
        Ray ray_local(ro, rd);
        if (object->intersect(ray_local, h, tmin)) {
            Vec3f normal = h.getNormal();
            Matrix normalMatrix = invM;
            normalMatrix.Transpose();
            normalMatrix.TransformDirection(normal); //trans normal to world space
            normal.Normalize();
            h.set(h.getT(), h.getMaterial(), normal, r);
            return true;
        }
    }
    return false;
}

void Transform::paint() const {
    glPushMatrix();
    GLfloat *glMatrix = matrix.glGet();
    glMultMatrixf(glMatrix);
    delete[] glMatrix;
    object->paint();
    glPopMatrix();
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


void Group::paint() const {
    for (int i = 0; i < num_objects; ++i) {
        objects[i]->paint();
    }
}

bool Group::intersectShadowRay(const Ray &r, Hit &h, float tmin) {
    for (int i = 0; i < num_objects; ++i) {
        if (objects[i]->intersect(r, h, tmin))
            return true;
    }
    return false;
}