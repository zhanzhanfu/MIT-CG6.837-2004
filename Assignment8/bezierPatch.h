#pragma once
#include "surface.h"


class BezierPatch :public Surface {
public:
	BezierPatch();
	virtual ~BezierPatch();
	virtual void Paint(ArgParser* args) override;
	virtual TriangleMesh* OutputTriangles(ArgParser* args) override;


private:
	void drawCurve(ArgParser* args);
};