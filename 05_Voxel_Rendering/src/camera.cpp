#include "camera.h"

//OrthographicCamera
Ray OrthographicCamera::generateRay(Vec2f point) {
    Vec3f ro = center + (point.x() - 0.5) * horizontal * size + (point.y() - 0.5) * up * size;
    return Ray(ro, direction);
}

float OrthographicCamera::getTMin() const {
    return -INFINITY;
}

//PerspectiveCamera
Ray PerspectiveCamera::generateRay(Vec2f point) {
    //angle 已在 scene_parser.C 中处理为弧度值
    float d = 0.5 / tan(angle / 2);
    Vec3f v = d * direction + (point.x() - 0.5) * horizontal + (point.y() - 0.5) * up;
    v.Normalize();
    return Ray(center, v);
}

float PerspectiveCamera::getTMin() const {
    return 0;
}
