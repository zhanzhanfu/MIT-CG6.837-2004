#pragma once

#include "vectors.h"

class ForceField {
public:
    ForceField() {}

    virtual ~ForceField() {}

    virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const = 0;

};

class GravityForceField : public ForceField {
public:
    GravityForceField(const Vec3f &gravity) : gravity(gravity) {}

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override;

private:
    Vec3f gravity;
};

class ConstantForceField : public ForceField {
public:
    ConstantForceField(const Vec3f &force) : force(force) {}

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override;

private:
    Vec3f force;
};

class RadialForceField : public ForceField {
public:
    RadialForceField(float magnitude) : magnitude(magnitude) {}

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override;

private:
    float magnitude;
};

class VerticalForceField : public ForceField {
public:
    VerticalForceField(float magnitude) : magnitude(magnitude) {}

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override;

private:
    float magnitude;
};

class WindForceField : public ForceField {
public:
    WindForceField(float magnitude) : magnitude(magnitude) {}

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override;

private:
    float magnitude;
};
