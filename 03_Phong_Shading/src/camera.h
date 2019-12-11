#pragma once

#include "ray.h"

class Camera {
public:
    Camera() {}

    virtual ~Camera() {}

    virtual Ray generateRay(Vec2f point) = 0;

    virtual float getTMin() const = 0;
};

//OrthographicCamera
class OrthographicCamera : public Camera {
public:
    OrthographicCamera(Vec3f &c, Vec3f &d, Vec3f &u, float s) : center(c), direction(d), up(u), size(s) {
        direction.Normalize();
        Vec3f::Cross3(horizontal, direction, up);
        Vec3f::Cross3(up, horizontal, direction);
        horizontal.Normalize();
        up.Normalize();
    }

    ~OrthographicCamera() override {}

    Ray generateRay(Vec2f point) override;

    float getTMin() const override;

    Vec3f center;
    Vec3f direction;
    Vec3f up;
    Vec3f horizontal;
    float size;

};

//PerspectiveCamera
class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(Vec3f &c, Vec3f &d, Vec3f &u, float a) : center(c), direction(d), up(u), angle(a) {
        direction.Normalize();
        Vec3f::Cross3(horizontal, direction, up);
        Vec3f::Cross3(up, horizontal, direction);
        horizontal.Normalize();
        up.Normalize();
    }

    ~PerspectiveCamera() override {}

    Ray generateRay(Vec2f point) override;

    float getTMin() const override;

    Vec3f center;
    Vec3f direction;
    Vec3f up;
    Vec3f horizontal;
    float angle;
};



