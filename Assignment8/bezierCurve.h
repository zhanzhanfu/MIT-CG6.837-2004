#pragma once
#include "curve.h"

class BezierCurve :public Curve {
public:
	BezierCurve(int num_v);
	virtual ~BezierCurve();

	virtual void Paint(ArgParser* args) override;
	virtual void OutputBezier(FILE* file) override;
	virtual void OutputBSpline(FILE* file) override;
	virtual void moveControlPoint(int selectedPoint, float x, float y) override;
	virtual void addControlPoint(int selectedPoint, float x, float y) override;
	virtual void deleteControlPoint(int selectedPoint) override;
	virtual TriangleMesh* OutputTriangles(ArgParser* args) override;

private:
	void drawCurve(ArgParser* args);

};