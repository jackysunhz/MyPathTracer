#pragma once

#include "Vector.h"
#include "global.h"

enum MaterialType { 
    DIFFUSE,
    MICROFACET
};

class Material{
private:
    MaterialType m_type;
    Vector3f albedo;
    Vector3f emission;
    float ior;
    float roughness;
    float metalness;

    float F_exact(const Vector3f& wi, const Vector3f& N, float ior) const;

    Vector3f F_Schlick(float cosTheta);

    float D_GGX(const Vector3f& N, const Vector3f& H, float a);

    float G1_GGX(const Vector3f& v, const Vector3f& h, const Vector3f& N);

public:
    Material(MaterialType t = DIFFUSE, Vector3f e = Vector3f(0), Vector3f albedo = Vector3f(0.725f, 0.71f, 0.68f), 
        float ior = 1.4f, float roughness = 0.5f, float metalness = 0.f);
    MaterialType getType();
    Vector3f getEmission();
    bool hasEmission();
    Vector3f sample(const Vector3f& wi, const Vector3f& N);
    float pdf(const Vector3f& wi, const Vector3f& wo, const Vector3f& N);
    Vector3f brdf(const Vector3f& wi, const Vector3f& wo, const Vector3f& N);
    Vector3f reflect(const Vector3f& I, const Vector3f& N) const;
};





