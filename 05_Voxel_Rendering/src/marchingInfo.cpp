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
            tmin = t_next_x;
            t_next_x += dt_x;
            break;
        case 2:
            j += sign_y;
            tmin = t_next_y;
            t_next_y += dt_y;
            break;
        case 3:
            k += sign_z;
            tmin = t_next_z;
            t_next_z += dt_z;
            break;
    }

}