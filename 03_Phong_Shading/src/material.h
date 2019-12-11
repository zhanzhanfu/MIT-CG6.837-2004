#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vectors.h"
#include <algorithm>
// ====================================================================
// ====================================================================

// You will extend this class in later assignments

class Material {

public:

    // CONSTRUCTORS & DESTRUCTOR
    Material(const Vec3f &d_color) { diffuseColor = d_color; }

    virtual ~Material() {};

    // ACCESSORS
    virtual Vec3f getDiffuseColor() const { return diffuseColor; }

    virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,
                        const Vec3f &lightColor) const = 0;

protected:

    // REPRESENTATION
    Vec3f diffuseColor;

};

// ====================================================================
// ====================================================================

class PhongMaterial : public Material {
public:
    PhongMaterial(const Vec3f &d_color, const Vec3f &s_color, float e) :
            Material(d_color), specularColor(s_color), exponent(e) {}

    ~PhongMaterial() override {}

    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const override {
        Vec3f normal = hit.getNormal();
        Vec3f v = -1 * ray.getDirection();
        Vec3f l = dirToLight;
        //diff
        float diff = std::max(normal.Dot3(l), 0.0f);
        Vec3f diffuse = diff * diffuseColor * lightColor;
        //spec
        Vec3f h = v + l;
        h.Normalize();
        float spec = pow(max(normal.Dot3(h), 0.0f), exponent);
        Vec3f specular = spec * specularColor * lightColor;
        //ans
        Vec3f color = diffuse + specular;
        return color;
    }


    Vec3f specularColor;
    float exponent;
};


#endif