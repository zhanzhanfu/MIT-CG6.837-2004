#include "gravityForceField.h"

GravityForceField::GravityForceField(Vec3f gravity)
	:gravity(gravity)
{
}

GravityForceField::~GravityForceField()
{
}

Vec3f GravityForceField::getAcceleration(const Vec3f& position, float mass, float t) const
{
	Vec3f a = gravity;
	return a;
}
