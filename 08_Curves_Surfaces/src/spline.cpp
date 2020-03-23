#include "spline.h"

Matrix Spline::Bezier_Matrix = Matrix(new float[16]{-1, 3, -3, 1, 3, -6, 3, 0, -3, 3, 0, 0, 1, 0, 0, 0});
Matrix Spline::BSpline_Matrix = Matrix(new float[16]{-1, 3, -3, 1, 3, -6, 0, 4, -3, 3, 3, 1, 1, 0, 0, 0}) * (1.0f / 6);

