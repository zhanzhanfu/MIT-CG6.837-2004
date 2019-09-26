#include "verticalForceField.h"

VerticalForceField::VerticalForceField(float magnitude)
	:magnitude(magnitude)
{
}

VerticalForceField::~VerticalForceField()
{
}

Vec3f VerticalForceField::getAcceleration(const Vec3f& position, float mass, float t) const
{
	Vec3f force;
	force.Set(0, position.y() * magnitude, 0);
	Vec3f a = force * (-1.0f / mass);
	return a;
}
