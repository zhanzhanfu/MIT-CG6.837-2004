#pragma once
#include "spline.h"
#include "curve.h"
class Surface :public Spline {
public:
	Surface(){}
	virtual Surface(){}
protected:
	void drawPoints();
	void drawLine();
	

	int curve_tessellation;



	

};