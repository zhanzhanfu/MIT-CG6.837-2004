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
    Material(const Vec3f &d_color, const Vec3f &s_color, float e, const Vec3f &ref_color, const Vec3f &trans_color,
             float index) : diffuseColor(d_color), specularColor(s_color), reflectiveColor(ref_color),
                            transparentColor(trans_color), exponent(e), indexOfRefraction(index) {}

    virtual ~Material() {};

    // ACCESSORS
    Vec3f getDiffuseColor() const { return diffuseColor; }

    Vec3f getSpecularColor() const {return specularColor;}

    Vec3f getReflectiveColor() const { return reflectiveColor; }

    Vec3f getTransparentColor() const { return transparentColor; }

    float getExponent() const { return exponent; }

    float getIndexOfRefraction() const { return indexOfRefraction; }

protected:

    // REPRESENTATION
    Vec3f diffuseColor;
    Vec3f specularColor;
    Vec3f reflectiveColor;
    Vec3f transparentColor;
    float exponent;
    float indexOfRefraction;

};

// ====================================================================
// ====================================================================

class PhongMaterial : public Material {
public:
    PhongMaterial(const Vec3f &d_color, const Vec3f &s_color, float e, const Vec3f &ref_color,
                  const Vec3f &trans_color, float index) : Material(d_color, s_color, e, ref_color,
                                                                    trans_color, index) {}

    ~PhongMaterial() override {}

};


#endif