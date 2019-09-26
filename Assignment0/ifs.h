#pragma once
#include "matrix.h"
#include "image.h"
class IFS {
public:
	IFS();
	~IFS();
	void Input(char* input_file);
	void Render(Image* image, int num_points, int num_iters);
	
private:
	int n;
	Matrix* matrix;
	float* prob;


};