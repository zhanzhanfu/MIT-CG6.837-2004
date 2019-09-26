#pragma once
#include "spline.h"

class Curve :public Spline {
public:
	Curve()	{}
	virtual ~Curve(){}
protected:
	int curve_tessellation;
	void drawPoints();
	void drawLine();
	
};
