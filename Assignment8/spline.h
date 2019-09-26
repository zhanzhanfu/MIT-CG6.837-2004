#pragma once
#include "vectors.h"
#include "arg_parser.h"
#include "triangle_mesh.h"
#include <GL/freeglut.h>
#include "matrix.h"
#include <math.h>
class Spline {
public:
	Spline(){}
	virtual ~Spline(){}


	virtual void set(int i, Vec3f v);

	// FOR VISUALIZATION
	virtual void Paint(ArgParser* args) {};

	// FOR CONVERTING BETWEEN SPLINE TYPES
	virtual void OutputBezier(FILE* file) {};
	virtual void OutputBSpline(FILE* file) {};

	// FOR CONTROL POINT PICKING
	virtual int getNumVertices() { return num_vertices; }
	virtual Vec3f getVertex(int i) { return vertices[i]; }
	virtual int getTypeId() { return type_id; }

	// FOR EDITING OPERATIONS
	virtual void moveControlPoint(int selectedPoint, float x, float y);
	virtual void addControlPoint(int selectedPoint, float x, float y);
	virtual void deleteControlPoint(int selectedPoint);

	// FOR GENERATING TRIANGLES
	virtual TriangleMesh* OutputTriangles(ArgParser* args) { return nullptr; };

public:
	int num_vertices;
	Vec3f* vertices;
	int type_id;
};