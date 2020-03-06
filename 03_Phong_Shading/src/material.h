#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "ray.h"
#include "hit.h"
#include <algorithm>

class Material {

public:

    Material(const Vec3f &d_color) { diffuseColor = d_color; }

    virtual ~Material() {};

    virtual Vec3f getDiffuseColor() const { return diffuseColor; }

    virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,
                        const Vec3f &lightColor) const = 0;

    virtual void glSetMaterial() const = 0;

protected:

    Vec3f diffuseColor;

};

// ====================================================================
// ====================================================================

class PhongMaterial : public Material {
public:
    PhongMaterial(const Vec3f &d_color, const Vec3f &s_color, float e) :
            Material(d_color), specularColor(s_color), exponent(e) {}

    ~PhongMaterial() override {}

    virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &l, const Vec3f &lightColor) const;


    virtual void glSetMaterial() const;

    Vec3f getSpecularColor() const { return specularColor; }

private:
    Vec3f specularColor;
    float exponent;
};


#endif