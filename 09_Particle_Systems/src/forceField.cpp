#include "forceField.h"


Vec3f GravityForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    return gravity;
}

Vec3f ConstantForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    return force * (1.0f / mass);
}

Vec3f RadialForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    return magnitude * (-1 * position) * (1.0f / mass);
}

Vec3f VerticalForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    return magnitude * Vec3f(0, -position.y(), 0) * (1.0f / mass);
}

Vec3f WindForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    Vec3f force = magnitude * Vec3f(sin(10 * t), cos(10 * t), 0);
    return force * (1.0f / mass);
}
