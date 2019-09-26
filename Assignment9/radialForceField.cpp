#include "radialForceField.h"

RadialForceField::RadialForceField(float magnitude) 
	:magnitude(magnitude)
{
}

RadialForceField::~RadialForceField()
{
}

Vec3f RadialForceField::getAcceleration(const Vec3f& position, float mass, float t) const
{
	Vec3f force = position * magnitude;
	Vec3f a = force * (-1.0f / mass);
	return a;
}
