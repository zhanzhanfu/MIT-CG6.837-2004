#pragma once
#include "curve.h"

class BSplineCurve :public Curve {
public:
	BSplineCurve(int num_v);
	virtual ~BSplineCurve();

	virtual void Paint(ArgParser* args) override;
	virtual void OutputBezier(FILE* file) override;
	virtual void OutputBSpline(FILE* file) override;
	virtual TriangleMesh* OutputTriangles(ArgParser* args) override;

private:
	void drawCurve(ArgParser* args);
};