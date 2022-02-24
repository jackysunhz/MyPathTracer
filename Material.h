#pragma once
#include "Vector.h"
#include "global.h"

enum MaterialType { 
    DIFFUSE,
    MICROFACET
};

class Material {
private:
    MaterialType m_type;
    Vector3f albedo;
    Vector3f emission;
    float ior;//index of refraction
    float roughness;
    float metalness;

    //complete fresnel equation
    float f_exact(const Vector3f& wi, const Vector3f& N) const;

    //approximated fresnel term, by Schlick
    Vector3f f_Schlick(float cosTheta);

    //normal distribution term
    float d_GGX(const Vector3f& N, const Vector3f& H, float a);

    //geometric shadowing term
    float g1_GGX(const Vector3f& v, const Vector3f& h, const Vector3f& N);

    //microsurface reflection
    Vector3f brdf(const Vector3f& wi, const Vector3f& wo, const Vector3f& N);

public:
    //constructor
    Material(MaterialType t = DIFFUSE, Vector3f e = Vector3f(0), Vector3f albedo = Vector3f(0.725f, 0.71f, 0.68f), 
        float ior = 1.4f, float roughness = 0.5f, float metalness = 0.f);

    MaterialType GetType();
    Vector3f GetEmission();
    bool HasEmission();

    //sample a reflection direction
    Vector3f Sample(const Vector3f& wi, const Vector3f& N);

    //the probability associated with the sample
    float Pdf(const Vector3f& wi, const Vector3f& wo, const Vector3f& N);

    //the bidirectional reflectance distribution function
    Vector3f Bxdf(const Vector3f& wi, const Vector3f& wo, const Vector3f& N);

    //find the mirror-reflection direction
    Vector3f Reflect(const Vector3f& I, const Vector3f& N) const;
};





