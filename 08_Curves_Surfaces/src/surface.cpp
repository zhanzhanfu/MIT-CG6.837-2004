#include "surface.h"
#include <GL/freeglut.h>


void SurfaceOfRevolution::Paint(ArgParser *args) {
    curve->Paint(args);
}

TriangleMesh *SurfaceOfRevolution::OutputTriangles(ArgParser *args) {
    return curve->OutputTriangles(args);
}

void SurfaceOfRevolution::OutputBSpline(FILE *file) {
    curve->OutputBSpline(file);
}

void SurfaceOfRevolution::moveControlPoint(int selectedPoint, float x, float y) {
    curve->moveControlPoint(selectedPoint, x, y);
}

void SurfaceOfRevolution::addControlPoint(int selectedPoint, float x, float y) {
    curve->addControlPoint(selectedPoint, x, y);
}

void SurfaceOfRevolution::deleteControlPoint(int selectedPoint) {
    curve->deleteControlPoint(selectedPoint);
}


void BezierPatch::Paint(ArgParser *args) {
    glLineWidth(5);
    glColor3f(0, 0, 1);
    glBegin(GL_LINE_STRIP);
    for (Vec3f &v: vertices)
        glVertex3f(v.x(), v.y(),v.z());
        glEnd();

    glLineWidth(3);
    glColor3f(0, 1, 0);
    glBegin(GL_POINTS);
    int patch_tessellation = args->patch_tessellation;
    float d = 1.0f / patch_tessellation;
    vector<Matrix> G(4);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            G[i].Set(j, 0, vertices[i * 4 + j].x());
            G[i].Set(j, 1, vertices[i * 4 + j].y());
            G[i].Set(j, 2, vertices[i * 4 + j].z());
        }
    }
    for (int i = 0; i <= patch_tessellation; ++i) {
        float t = i * d;
        float t2 = t * t;
        float t3 = t2 * t;
        Matrix G_new;
        for (int j = 0; j < 4; ++j) {
            Vec4f T(t3, t2, t, 1);
            Bezier_Matrix.Transform(T);
            G[j].Transform(T);
            G_new.Set(j, 0, T.x());
            G_new.Set(j, 1, T.y());
            G_new.Set(j, 2, T.z());
        }
        for (int j = 0; j <= patch_tessellation; ++j) {
            float s = j * d;
            float s2 = s * s;
            float s3 = s2 * s;
            Vec4f S(s3, s2, s, 1);
            Bezier_Matrix.Transform(S);
            G_new.Transform(S);
            glVertex3f(S.x(), S.y(), S.z());
        }
    }
    glEnd();

    glPointSize(5);
    glColor3f(1, 0, 0);
    for (int i = 0; i < 16; i += 4) {
        glBegin(GL_POINTS);
        glVertex3f(vertices[i].x(), vertices[i].y(), vertices[i].z());
        glVertex3f(vertices[i + 1].x(), vertices[i + 1].y(), vertices[i + 1].z());
        glVertex3f(vertices[i + 2].x(), vertices[i + 2].y(), vertices[i + 2].z());
        glVertex3f(vertices[i + 3].x(), vertices[i + 3].y(), vertices[i + 3].z());
        glEnd();
    }
}

TriangleMesh *BezierPatch::OutputTriangles(ArgParser *args) {
    int patch_tessellation = args->patch_tessellation;
    float d = 1.0f / patch_tessellation;
    TriangleNet *triangleNet = new TriangleNet(patch_tessellation, patch_tessellation);
    vector<Matrix> G(4);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            G[i].Set(j, 0, vertices[i * 4 + j].x());
            G[i].Set(j, 1, vertices[i * 4 + j].y());
            G[i].Set(j, 2, vertices[i * 4 + j].z());
        }
    }
    for (int i = 0; i <= patch_tessellation; ++i) {
        float t = i * d;
        float t2 = t * t;
        float t3 = t2 * t;
        Matrix G_new;
        for (int j = 0; j < 4; ++j) {
            Vec4f T(t3, t2, t, 1);
            Bezier_Matrix.Transform(T);
            G[j].Transform(T);
            G_new.Set(j, 0, T.x());
            G_new.Set(j, 1, T.y());
            G_new.Set(j, 2, T.z());
        }
        for (int j = 0; j <= patch_tessellation; ++j) {
            float s = j * d;
            float s2 = s * s;
            float s3 = s2 * s;
            Vec4f S(s3, s2, s, 1);
            Bezier_Matrix.Transform(S);
            G_new.Transform(S);
            triangleNet->SetVertex(i, j, Vec3f(S.x(), S.y(), S.z()));
        }
    }
    return triangleNet;
}
