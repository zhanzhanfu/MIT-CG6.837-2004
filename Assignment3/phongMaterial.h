#pragma once
#include "material.h"


class PhongMaterial :public Material {
private:
	Vec3f diffuseColor;
	Vec3f specularColor;
	float exponent;
public:
	PhongMaterial(const Vec3f &d_Color,const Vec3f &s_Color, float ex);
	virtual ~PhongMaterial();
	Vec3f getSpecularColor() const { return specularColor; }
	virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,
		const Vec3f &lightColor) const override;
	void glSetMaterial(void) const;
};