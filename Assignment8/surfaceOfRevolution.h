#pragma once
#include "surface.h"


class SurfaceOfRevolution :public Surface {
public:
	SurfaceOfRevolution(Curve* c);
	virtual ~SurfaceOfRevolution();
	

	virtual void Paint(ArgParser* args) override;
	virtual TriangleMesh* OutputTriangles(ArgParser* args) override;
	virtual void OutputBSpline(FILE* file) override;

private:
	void drawCurve(ArgParser* args);

	Curve* curve;
};