#include "camera.h"

//OrthographicCamera
Ray OrthographicCamera::generateRay(Vec2f point) {
    Vec3f ro = center + (point.x() - 0.5) * horizontal * size + (point.y() - 0.5) * up * size;
    return Ray(ro, direction);
}

float OrthographicCamera::getTMin() const {
    return -INFINITY;
}


