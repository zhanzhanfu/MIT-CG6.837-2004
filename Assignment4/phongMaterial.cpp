#include "phongMaterial.h"
#include "glCanvas.h"
#include <algorithm>

extern bool shade_back;

#ifdef SPECULAR_FIX
// OPTIONAL:  global variable allows (hacky) communication 
// with glCanvas::display
extern int SPECULAR_FIX_WHICH_PASS;
#endif



PhongMaterial::PhongMaterial(const Vec3f &diffuseColor, const Vec3f &specularColor, float exponent,
	const Vec3f &reflectiveColor, const Vec3f &transparentColor, float indexOfRefraction):Material(diffuseColor) {
	
	this->specularColor = specularColor;
	this->exponent = exponent;
	this->reflectiveColor = reflectiveColor;
	this->transparentColor = transparentColor;
	this->indexOfRefraction = indexOfRefraction;
}
PhongMaterial::~PhongMaterial() {

}

Vec3f PhongMaterial::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, 
	const Vec3f &lightColor) const{
	
	Vec3f normal = hit.getNormal();
	Vec3f direction = ray.getDirection();

	float d = dirToLight.Dot3(normal);
	if (shade_back) {
		d = std::abs(d);
	}
	else {
		d = std::max(d, (float)0);
	}

	
	Vec3f r = dirToLight - direction;
	r.Normalize();
	float s = normal.Dot3(r);
	if (shade_back) {
		s = std::abs(s);
	}
	else {
		s = std::max(s, (float)0);
	}

	Vec3f color = d * getDiffuseColor() + pow(s, exponent) * getSpecularColor();
	color = color * lightColor;

	return color;
	/*
	Vec3f direction = ray.getDirection();
	Vec3f normal = hit.getNormal();
	if (shade_back && direction.Dot3(normal) > 0) {
		normal.Negate();
	}

	float diffuse = dirToLight.Dot3(normal);
	Vec3f reflection_out = normal * 2.0f * normal.Dot3(dirToLight) - dirToLight;
	float specular = -1.0f * direction.Dot3(reflection_out);

	if (diffuse < 0.0f) {
		diffuse = 0.0f;
	}
	if (specular < 0.0f) {
		specular = 0.0f;
	}

	Vec3f color = getDiffuseColor();
	color = color * diffuse + specularColor * pow(specular, exponent);
	color = color * lightColor;

	return color;
	*/
}

void PhongMaterial::glSetMaterial(void) const {

	GLfloat one[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat zero[4] = { 0.0, 0.0, 0.0, 0.0 };
	GLfloat specular[4] = {
	  getSpecularColor().r(),
	  getSpecularColor().g(),
	  getSpecularColor().b(),
	  1.0 };
	GLfloat diffuse[4] = {
	  getDiffuseColor().r(),
	  getDiffuseColor().g(),
	  getDiffuseColor().b(),
	  1.0 };

	// NOTE: GL uses the Blinn Torrance version of Phong...      
	float glexponent = exponent;
	if (glexponent < 0) glexponent = 0;
	if (glexponent > 128) glexponent = 128;

#if !SPECULAR_FIX 

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);

#else

	// OPTIONAL: 3 pass rendering to fix the specular highlight 
	// artifact for small specular exponents (wide specular lobe)

	if (SPECULAR_FIX_WHICH_PASS == 0) {
		// First pass, draw only the specular highlights
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, zero);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);

	}
	else if (SPECULAR_FIX_WHICH_PASS == 1) {
		// Second pass, compute normal dot light 
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, one);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
	}
	else {
		// Third pass, add ambient & diffuse terms
		assert(SPECULAR_FIX_WHICH_PASS == 2);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
	}

#endif
}