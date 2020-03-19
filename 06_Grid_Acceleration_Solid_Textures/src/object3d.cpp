#include "object3d.h"
#include <GL/freeglut.h>
#include <vector>
#include "grid.h"
#include "raytracing_stats.h"

//copy from Matrix.cpp
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

void Object3D::insertIntoGrid(Grid *g, Matrix *m) {
    assert(boundingBox != nullptr);
    BoundingBox *bb_new = boundingBox;
    if (m) {
        Vec3f obj_min = boundingBox->getMin();
        Vec3f obj_max = boundingBox->getMax();
        float x1 = obj_min.x(), y1 = obj_min.y(), z1 = obj_min.z();
        float x2 = obj_max.x(), y2 = obj_max.y(), z2 = obj_max.z();
        Vec3f v1 = Vec3f(x1, y1, z1);
        Vec3f v2 = Vec3f(x1, y1, z2);
        Vec3f v3 = Vec3f(x1, y2, z1);
        Vec3f v4 = Vec3f(x1, y2, z2);
        Vec3f v5 = Vec3f(x2, y1, z1);
        Vec3f v6 = Vec3f(x2, y1, z2);
        Vec3f v7 = Vec3f(x2, y2, z1);
        Vec3f v8 = Vec3f(x2, y2, z2);
        m->Transform(v1);
        m->Transform(v2);
        m->Transform(v3);
        m->Transform(v4);
        m->Transform(v5);
        m->Transform(v6);
        m->Transform(v7);
        m->Transform(v8);
        bb_new = new BoundingBox(Vec3f(INFINITY, INFINITY, INFINITY), Vec3f(-INFINITY, -INFINITY, -INFINITY));
        bb_new->Extend(v1);
        bb_new->Extend(v2);
        bb_new->Extend(v3);
        bb_new->Extend(v4);
        bb_new->Extend(v5);
        bb_new->Extend(v6);
        bb_new->Extend(v7);
        bb_new->Extend(v8);
    }

    BoundingBox *grid_bb = g->getBoundingBox();
    Vec3f grid_min = grid_bb->getMin();
    Vec3f grid_max = grid_bb->getMax();
    int nx = g->nx;
    int ny = g->ny;
    int nz = g->nz;
    float cellx = (grid_max - grid_min).x() / float(nx);
    float celly = (grid_max - grid_min).y() / float(ny);
    float cellz = (grid_max - grid_min).z() / float(nz);
    Vec3f obj_min = bb_new->getMin();
    Vec3f obj_max = bb_new->getMax();
    int start_i = int((obj_min - grid_min).x() / cellx);
    int start_j = int((obj_min - grid_min).y() / celly);
    int start_k = int((obj_min - grid_min).z() / cellz);
    int end_i = int((obj_max - grid_min).x() / cellx);
    int end_j = int((obj_max - grid_min).y() / celly);
    int end_k = int((obj_max - grid_min).z() / cellz);
    if (start_i > nx || start_j > ny || start_k > nz || end_i < 0 || end_j < 0 || end_k < 0)
        return;
    start_i = max(start_i, 0);
    start_j = max(start_j, 0);
    start_k = max(start_k, 0);
    end_i = min(end_i, nx - 1);
    end_j = min(end_j, ny - 1);
    end_k = min(end_k, nz - 1);
    //note this
    if (start_i == nx) start_i--;
    if (start_j == ny) start_j--;
    if (start_k == nz) start_k--;

    for (int k = start_k; k <= end_k; ++k) {
        for (int j = start_j; j <= end_j; ++j) {
            for (int i = start_i; i <= end_i; ++i) {
                int index = nx * ny * k + nx * j + i;
                g->opaque[index].push_back(new Transform(*m, this));
            }
        }
    }
}


//Sphere
//Algebraic
bool Sphere::intersect(const Ray &r, Hit &h, float tmin) {
    RayTracingStats::IncrementNumIntersections();//-------------------------------------
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

void Sphere::insertIntoGrid(Grid *g, Matrix *m) {
    if (m) {
        Object3D::insertIntoGrid(g, m);
        return;
    }
    BoundingBox *grid_bb = g->getBoundingBox();
    Vec3f grid_min = grid_bb->getMin();
    Vec3f grid_max = grid_bb->getMax();
    int nx = g->nx;
    int ny = g->ny;
    int nz = g->nz;
    float cellx = (grid_max - grid_min).x() / float(nx);
    float celly = (grid_max - grid_min).y() / float(ny);
    float cellz = (grid_max - grid_min).z() / float(nz);
    float diagonal = sqrt(cellx * cellx + celly * celly + cellz * cellz);
    for (int k = 0; k < nz; ++k) {
        for (int j = 0; j < ny; ++j) {
            for (int i = 0; i < nx; ++i) {
                float x = (i + 0.5) * cellx;
                float y = (j + 0.5) * celly;
                float z = (k + 0.5) * cellz;
                if ((Vec3f(x, y, z) - (center - grid_min)).Length() < radius + diagonal / 2) {
                    int index = nx * ny * k + nx * j + i;
                    g->opaque[index].push_back(this);
                }
            }
        }
    }
}

//Plane
bool Plane::intersect(const Ray &r, Hit &h, float tmin) {
    RayTracingStats::IncrementNumIntersections();//-------------------------------------
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

void Plane::insertIntoGrid(Grid *g, Matrix *m) {
    g->infinitePrimitives.push_back(this);
}

//Triangle
bool Triangle::intersect(const Ray &r, Hit &h, float tmin) {
    RayTracingStats::IncrementNumIntersections();//-------------------------------------
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


BoundingBox *Triangle::getTriangleBoundingBox(const Matrix *m) const {
    if (!m) return boundingBox;
    Vec3f aa = a, bb = b, cc = c;
    m->Transform(aa);
    m->Transform(bb);
    m->Transform(cc);
    BoundingBox *t = new BoundingBox(Vec3f(min(aa.x(), bb.x()), min(aa.y(), bb.y()), min(aa.z(), bb.z())),
                                     Vec3f(max(aa.x(), bb.x()), max(aa.y(), bb.y()), max(aa.z(), bb.z())));
    t->Extend(cc);
    return t;
}

void Triangle::insertIntoGrid(Grid *g, Matrix *m) {
    BoundingBox *bb_new = getTriangleBoundingBox(m);
    BoundingBox *grid_bb = g->getBoundingBox();
    Vec3f grid_min = grid_bb->getMin();
    Vec3f grid_max = grid_bb->getMax();
    int nx = g->nx;
    int ny = g->ny;
    int nz = g->nz;
    float cellx = (grid_max - grid_min).x() / float(nx);
    float celly = (grid_max - grid_min).y() / float(ny);
    float cellz = (grid_max - grid_min).z() / float(nz);
    Vec3f obj_min = bb_new->getMin();
    Vec3f obj_max = bb_new->getMax();
    int start_i = int((obj_min - grid_min).x() / cellx);
    int start_j = int((obj_min - grid_min).y() / celly);
    int start_k = int((obj_min - grid_min).z() / cellz);
    int end_i = int((obj_max - grid_min).x() / cellx);
    int end_j = int((obj_max - grid_min).y() / celly);
    int end_k = int((obj_max - grid_min).z() / cellz);
    if (start_i > nx || start_j > ny || start_k > nz || end_i < 0 || end_j < 0 || end_k < 0)
        return;
    start_i = max(start_i, 0);
    start_j = max(start_j, 0);
    start_k = max(start_k, 0);
    end_i = min(end_i, nx - 1);
    end_j = min(end_j, ny - 1);
    end_k = min(end_k, nz - 1);
    //note this
    if (start_i == nx) start_i--;
    if (start_j == ny) start_j--;
    if (start_k == nz) start_k--;

    for (int k = start_k; k <= end_k; ++k) {
        for (int j = start_j; j <= end_j; ++j) {
            for (int i = start_i; i <= end_i; ++i) {
                int index = nx * ny * k + nx * j + i;
                if(m)
                    g->opaque[index].push_back(new Transform(*m, this));
                else
                    g->opaque[index].push_back(this);
            }
        }
    }
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

void Transform::insertIntoGrid(Grid *g, Matrix *m) {
    if (m) {
        Matrix t = (*m) * matrix;
        object->insertIntoGrid(g, &t);
    } else {
        object->insertIntoGrid(g, &matrix);
    }

}

BoundingBox *Transform::getBoundingBox() const {
    assert(boundingBox != nullptr);
    if (object->is_triangle()) {
        return object->getTriangleBoundingBox(&matrix);
    }
    Vec3f obj_min = boundingBox->getMin();
    Vec3f obj_max = boundingBox->getMax();
    float x1 = obj_min.x(), y1 = obj_min.y(), z1 = obj_min.z();
    float x2 = obj_max.x(), y2 = obj_max.y(), z2 = obj_max.z();
    Vec3f v1 = Vec3f(x1, y1, z1);
    Vec3f v2 = Vec3f(x1, y1, z2);
    Vec3f v3 = Vec3f(x1, y2, z1);
    Vec3f v4 = Vec3f(x1, y2, z2);
    Vec3f v5 = Vec3f(x2, y1, z1);
    Vec3f v6 = Vec3f(x2, y1, z2);
    Vec3f v7 = Vec3f(x2, y2, z1);
    Vec3f v8 = Vec3f(x2, y2, z2);
    matrix.Transform(v1);
    matrix.Transform(v2);
    matrix.Transform(v3);
    matrix.Transform(v4);
    matrix.Transform(v5);
    matrix.Transform(v6);
    matrix.Transform(v7);
    matrix.Transform(v8);

    BoundingBox *bb_new = new BoundingBox(Vec3f(INFINITY, INFINITY, INFINITY), Vec3f(-INFINITY, -INFINITY, -INFINITY));
    bb_new->Extend(v1);
    bb_new->Extend(v2);
    bb_new->Extend(v3);
    bb_new->Extend(v4);
    bb_new->Extend(v5);
    bb_new->Extend(v6);
    bb_new->Extend(v7);
    bb_new->Extend(v8);
    return bb_new;
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
    if (obj->getBoundingBox())
        boundingBox->Extend(obj->getBoundingBox());
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

void Group::insertIntoGrid(Grid *g, Matrix *m) {
    for (int i = 0; i < num_objects; ++i) {
        objects[i]->insertIntoGrid(g, m);
    }
}


