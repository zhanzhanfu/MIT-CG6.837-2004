#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "ray.h"
#include "hit.h"
#include "vectors.h"
#include <algorithm>

class Material {

public:

    Material(const Vec3f &d_color) : diffuseColor(d_color) {}

    virtual ~Material() {};

    virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &l, const Vec3f &lightColor) const = 0;

    virtual void glSetMaterial() const = 0;

    virtual bool reflect(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &reflected) const = 0;

    virtual bool refract(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &refracted) const = 0;

    virtual Vec3f getDiffuseColor() { return diffuseColor; }

protected:
    Vec3f diffuseColor;
};

class PhongMaterial : public Material {
public:
    PhongMaterial(const Vec3f &d_color, const Vec3f &s_color, float e, const Vec3f &ref_color, const Vec3f &trans_color, float index) :
            Material(d_color), specularColor(s_color), exponent(e), reflectiveColor(ref_color), transparentColor(trans_color),
            indexOfRefraction(index) {}

    virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &l, const Vec3f &lightColor) const;

    void glSetMaterial() const override;

    virtual bool reflect(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &reflected) const;

    virtual bool refract(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &refracted) const;

private:

    Vec3f specularColor;
    float exponent;
    Vec3f reflectiveColor;
    Vec3f transparentColor;
    float indexOfRefraction;
};


#endif