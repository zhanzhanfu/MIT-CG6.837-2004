#include "rayTracer.h"
const float EPSILON = 0.01f;
extern bool useTransparentShadows;
RayTracer::RayTracer(SceneParser *s, int max_bounces, float cutoff_weight, bool shadows) {
	this->sp = s;
	this->max_bounces = max_bounces;
	this->cutoff_weight = cutoff_weight;
	this->shadows = shadows;

}
RayTracer::~RayTracer() {

}

Vec3f RayTracer::traceRay(Ray &ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit &hit) const {
	
	if (bounces > max_bounces) {
		return Vec3f(0, 0, 0);
	}
	Group *group = sp->getGroup();
	Vec3f pixel = sp->getBackgroundColor();

	if (group->intersect(ray, hit, tmin)) {
		
		if (bounces==1) {
			RayTree::SetMainSegment(ray, 0, hit.getT());
		}
		
		pixel = sp->getAmbientLight() * hit.getMaterial()->getDiffuseColor();
		Vec3f normal = hit.getNormal();
		normal.Normalize();
		Vec3f point = hit.getIntersectionPoint();
		// direct light
		
		int numLights = sp->getNumLights();
		for (int i=0;i<numLights;i++)
		{
			Light *light = sp->getLight(i);
			Vec3f dirToLight, lightColor;
			float disToLight;
			light->getIllumination(point, dirToLight, lightColor, disToLight);
			Ray ray2(point, dirToLight);
			Hit hit2(disToLight, NULL, Vec3f());
			if (shadows) {
				bool flag = group->intersect(ray2, hit2, EPSILON);
				RayTree::AddShadowSegment(ray2, 0, hit2.getT());
				if (!flag) {
					pixel += hit.getMaterial()->Shade(ray, hit, dirToLight, lightColor);
				}
			}
			else {
				pixel += hit.getMaterial()->Shade(ray, hit, dirToLight, lightColor);
			}
		}

		Vec3f k_reflective = hit.getMaterial()->getReflectiveColor();
		Vec3f k_transparent = hit.getMaterial()->getTransparentColor();
		if (bounces <= max_bounces && weight >= cutoff_weight && k_reflective.Length()>0) {
			Vec3f re_dir = mirrorDirection(normal, ray.getDirection());
			Ray ray3 = Ray(point, re_dir);
			Hit hit3 = Hit(INT_MAX, NULL, Vec3f());
			Vec3f re_color = traceRay(ray3, EPSILON, bounces + 1,
				weight*k_reflective.Length(), indexOfRefraction, hit3);
			RayTree::AddReflectedSegment(ray3, 0.0f, hit3.getT());

			pixel += re_color * k_reflective;
		}

		if (bounces<=max_bounces && weight>=cutoff_weight && k_transparent.Length()>0)
		{
			float index_refraction;
			if (normal.Dot3(ray.getDirection()) < 0) {//from air to obj
				index_refraction = hit.getMaterial()->getIndexOfRefraction();
			}
			else {
				index_refraction = 1.0f / hit.getMaterial()->getIndexOfRefraction();
				//index_refraction = 1.0f / indexOfRefraction;
				normal = -1.0f*normal;
			}
			
			Vec3f transmitted_direction;
			if (transmittedDirection(normal, ray.getDirection(), 1.0f, index_refraction, transmitted_direction)) {
				Ray ray4 = Ray(point, transmitted_direction);
				Hit hit4 = Hit(INT_MAX, NULL, Vec3f());
				Vec3f transmitted_color = traceRay(ray4, EPSILON, bounces + 1, 
					weight*k_transparent.Length(), index_refraction, hit4);
				RayTree::AddTransmittedSegment(ray4, 0, hit4.getT());
				pixel += k_transparent * transmitted_color;
			}
		}
		
	}
	
	return pixel;
}

Vec3f RayTracer::mirrorDirection(const Vec3f &normal, const Vec3f &incoming) const {
	Vec3f out = incoming - normal * 2.0f *incoming.Dot3(normal);
	out.Normalize();
	return out;
}

bool RayTracer::transmittedDirection(const Vec3f &normal, const Vec3f &incoming, float index_i, float index_t, Vec3f &transmitted) const {
	bool flag = false;
	float ni = -1.0f * normal.Dot3(incoming);

	if (index_t > 0) {
		float index_r = index_i / index_t;
		float temp = 1.0f - pow(index_r, 2)*(1.0f - pow(ni, 2));
		if (temp >= 0) {
			transmitted = (float)(index_r* ni - sqrt(temp)) * normal + index_r * incoming;
			transmitted.Normalize();
			flag = true;
		}
	}
	return flag;
}