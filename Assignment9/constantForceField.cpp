#include "constantForceField.h"

ConstantForceField::ConstantForceField(Vec3f force)
	:force(force)
{
}

ConstantForceField::~ConstantForceField()
{
}

Vec3f ConstantForceField::getAcceleration(const Vec3f& position, float mass, float t) const
{
	Vec3f a = force * (1.0f / mass);
	return a;
}
