#include "phongMaterial.h"
#include <math.h>
#include <algorithm>
#include "glCanvas.h"
extern bool Blinn;


#ifdef SPECULAR_FIX
// OPTIONAL:  global variable allows (hacky) communication 
// with glCanvas::display
extern int SPECULAR_FIX_WHICH_PASS;
#endif



PhongMaterial::PhongMaterial(const Vec3f &d_Color, const Vec3f &s_Color, float ex):Material(d_Color) {
	specularColor = s_Color;
	exponent = ex;
}
PhongMaterial::~PhongMaterial() {

}

Vec3f PhongMaterial::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const{
	Vec3f normal = hit.getNormal();
	//
	float d = dirToLight.Dot3(normal);
	if (d < 0) {
		return Vec3f(0,0, 0);
	}
	Vec3f Diffuse = d * getDiffuseColor();
	//
	
	Vec3f rayDir = ray.getDirection();
	rayDir.Normalize();
	Vec3f Specular;
	if (Blinn) {
		Vec3f r = dirToLight - rayDir;
		r.Normalize();
		Specular = pow(max(normal.Dot3(r), (float)0), exponent)*getSpecularColor();
	}
	else {
		Vec3f r = 2 * d * normal - dirToLight;
		Specular = pow(max(-rayDir.Dot3(r), (float)0), exponent)*getSpecularColor();
	}
	
	Vec3f result = (Diffuse + Specular)*lightColor;


	return result;


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