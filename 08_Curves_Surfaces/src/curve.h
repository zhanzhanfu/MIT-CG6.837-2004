#pragma once

#include "spline.h"
#include "matrix.h"
#include <vector>

class Curve : public Spline {
public:
    Curve(int num) {
        num_vertices = num;
        vertices.resize(num_vertices);
    }

    void set(int i, const Vec3f &v) override { vertices[i] = v; }

    int getNumVertices() override { return num_vertices; }

    Vec3f getVertex(int i) override { return vertices[i]; }

    void Paint(ArgParser *args) override;

protected:

    void drawLines();

    void drawPoints();

    virtual void drawCurves(ArgParser *args) = 0;

    int num_vertices;
    std::vector<Vec3f> vertices;
};

class BezierCurve : public Curve {
public:
    BezierCurve(int num) : Curve(num) {}

    void OutputBezier(FILE *file) override;

    void OutputBSpline(FILE *file) override;

    void moveControlPoint(int selectedPoint, float x, float y) override;

    void addControlPoint(int selectedPoint, float x, float y) override;

    void deleteControlPoint(int selectedPoint) override;

    TriangleMesh *OutputTriangles(ArgParser *args) override;

private:
    void drawCurves(ArgParser *args) override;
};


class BSplineCurve : public Curve {
public:
    BSplineCurve(int num) : Curve(num) {}

    void OutputBezier(FILE *file) override;

    void OutputBSpline(FILE *file) override;

    void moveControlPoint(int selectedPoint, float x, float y) override;

    void addControlPoint(int selectedPoint, float x, float y) override;

    void deleteControlPoint(int selectedPoint) override;

    TriangleMesh *OutputTriangles(ArgParser *args) override;

private:
    void drawCurves(ArgParser *args) override;
};