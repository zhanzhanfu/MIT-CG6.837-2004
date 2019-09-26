#pragma once
#include "vectors.h"


class MarchingInfo {
public:

	MarchingInfo();
	~MarchingInfo();
	void nextCell();


	float tmin;
	int i, j, k;
	float tnext_x, tnext_y, tnext_z;
	float dt_x, dt_y, dt_z;
	int sign_x, sign_y, sign_z;
	Vec3f normal;

};