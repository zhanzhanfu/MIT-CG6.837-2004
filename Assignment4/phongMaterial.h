#pragma once
#include "material.h"


class PhongMaterial :public Material {
private:
	
public:
	PhongMaterial(const Vec3f &diffuseColor,const Vec3f &specularColor,float exponent,
		const Vec3f &reflectiveColor,const Vec3f &transparentColor,float indexOfRefraction);
	~PhongMaterial();
	

	virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,
		const Vec3f &lightColor) const override;
	virtual void glSetMaterial(void) const override;
};