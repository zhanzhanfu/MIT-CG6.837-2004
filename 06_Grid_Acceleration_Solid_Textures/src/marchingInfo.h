#pragma once

#include "vectors.h"

class MarchingInfo {
public:
    MarchingInfo() : t_cur(INFINITY), t_next_x(INFINITY), t_next_y(INFINITY), t_next_z(INFINITY) {}

    void nextCell();

    float t_cur;
    int i, j, k;
    float t_next_x, t_next_y, t_next_z;
    float dt_x, dt_y, dt_z;
    int sign_x, sign_y, sign_z;
    Vec3f normal;

};