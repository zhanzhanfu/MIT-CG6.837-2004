#pragma once

#include "curve.h"


class Surface : public Spline {
public:
    Surface() {}

};


class SurfaceOfRevolution : public Surface {
public:
    SurfaceOfRevolution(Curve *c) : curve(c) {}

    void set(int i, const Vec3f &v) override { curve->set(i, v); }

    int getNumVertices() override { return curve->getNumVertices(); }

    Vec3f getVertex(int i) override { return curve->getVertex(i); }

    void Paint(ArgParser *args) override;

    void moveControlPoint(int selectedPoint, float x, float y) override;

    void addControlPoint(int selectedPoint, float x, float y) override;

    void deleteControlPoint(int selectedPoint) override;

    TriangleMesh *OutputTriangles(ArgParser *args) override;

    void OutputBSpline(FILE *file) override;

private:
    Curve *curve;
};

class BezierPatch : public Surface {
public:
    BezierPatch() {
        num_vertices = 16;
        vertices.resize(num_vertices);
    }

    void set(int i, const Vec3f &v) override { vertices[i] = v; }

    int getNumVertices() override { return num_vertices; }

    Vec3f getVertex(int i) override { return vertices[i]; }

    void Paint(ArgParser *args) override;

    TriangleMesh *OutputTriangles(ArgParser *args) override;

private:
    int num_vertices;
    std::vector<Vec3f> vertices;
};

