
#include "transform.h"
#include <GL/freeglut.h>
//Transform
bool Transform::intersect(const Ray &r, Hit &h, float tmin) const{
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

void Transform::paint() const{
    glPushMatrix();
    GLfloat *glMatrix = matrix.glGet();
    glMultMatrixf(glMatrix);
    delete[] glMatrix;
    object->paint();
    glPopMatrix();
}
