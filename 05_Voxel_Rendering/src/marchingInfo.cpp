#include "marchingInfo.h"


void MarchingInfo::nextCell() {
    int flag = 0;
    if (t_next_x <= t_next_y && t_next_x <= t_next_z)
        flag = 1;
    if (t_next_y <= t_next_x && t_next_y <= t_next_z)
        flag = 2;
    if (t_next_z <= t_next_x && t_next_z <= t_next_y)
        flag = 3;
    switch (flag) {
        case 1:
            i += sign_x;
            t_cur = t_next_x;
            t_next_x += dt_x;
            normal = Vec3f(-1, 0, 0) * sign_x;
            break;
        case 2:
            j += sign_y;
            t_cur = t_next_y;
            t_next_y += dt_y;
            normal = Vec3f(0, -1, 0) * sign_y;
            break;
        case 3:
            k += sign_z;
            t_cur = t_next_z;
            t_next_z += dt_z;
            normal = Vec3f(0, 0, -1) * sign_z;
            break;
    }

}