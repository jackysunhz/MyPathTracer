#pragma once
#include "Material.h"

Material::Material(MaterialType t, Vector3f e, Vector3f a, float i, float r, float m)
    : m_type(t), emission(e), albedo(a), ior(i), roughness(r), metalness(m) {}

MaterialType Material::getType() { return m_type; }

Vector3f Material::getEmission() { return emission; }

bool Material::hasEmission() { return emission.norm() > EPSILON; }

Vector3f Material::reflect(const Vector3f& I, const Vector3f& N) const { return I - 2 * dotProduct(I, N) * N; }

Vector3f Material::sample(const Vector3f& wi, const Vector3f& N) {
    switch (m_type) {
        case DIFFUSE: {
            // uniform sample on the hemisphere
            float x_1 = get_random_float(), x_2 = get_random_float();
            float z = std::fabs(1.0f - 2.0f * x_1);
            float r = std::sqrt(1.0f - z * z), phi = 2 * M_PI * x_2;
            Vector3f localRay(r * std::cos(phi), r * std::sin(phi), z);
            return toWorld(localRay, N);

            break;
        }
        case MICROFACET: {
            float r0 = get_random_float();
            float r1 = get_random_float();
            float a2 = roughness * roughness;
            float theta = acosf(sqrtf((1 - r0) / ((a2 - 1) * r0 + 1)));
            float phi = 2 * M_PI * r1;
            float z = cos(theta);
            float r = std::sqrt(1.0f - z * z);
            Vector3f wm = toWorld(Vector3f(r * cosf(phi), r * sinf(phi), z), N);
            Vector3f wo = reflect(wi, wm);
            return wo;

            break;
        }
    }
}

float Material::pdf(const Vector3f& wi, const Vector3f& wo, const Vector3f& N) {
    switch (m_type) {
        case DIFFUSE: {
            // uniform sample probability 1 / (2 * PI)
            if (dotProduct(wo, N) > 0.0f)
                return 0.5f / M_PI;
            else
                return 0.0f;
            break;
        }
        case MICROFACET: {
            Vector3f h = (wo - wi).normalized();
            float a2 = roughness * roughness;
            float cosTheta = dotProduct(h, N);
            float exp = (a2 - 1) * cosTheta * cosTheta + 1;
            float D = a2 / (M_PI * exp * exp);
            return D * cosTheta / (4 * dotProduct(wo, h));
            break;
        }
    }
}

Vector3f Material::brdf(const Vector3f& wi, const Vector3f& wo, const Vector3f& N) {
    switch (m_type) {
        case DIFFUSE: {
            // calculate the contribution of diffuse model
            float cosalpha = dotProduct(N, wo);
            if (cosalpha > 0.0f) {
                Vector3f diffuse = albedo / M_PI;
                return diffuse;
            }
            else
                return Vector3f(0.0f);
            break;
        }
        case MICROFACET: {
            if (dotProduct(N, wo) < 0) {
                return Vector3f(0.0f);
            }
            Vector3f h = (wo - wi).normalized();
            Vector3f F = F_Schlick(dotProduct(-wi, h));
            Vector3f f_lambert = albedo / M_PI;
            float D = D_GGX(N, h, roughness);
            float k = (1 + roughness) * (1 + roughness) * 0.125f;
            float G = G1_GGX(-wi, h, N) * G1_GGX(wo, h, N);
            Vector3f f_cooktorrance = D * G * F / (4 * dotProduct(N, -wi) * dotProduct(N, wo));
            return (Vector3f(1) - F) * f_lambert + f_cooktorrance;
            break;
        }
    }
}

float Material::F_exact(const Vector3f& wi, const Vector3f& N, float ior) const {
    float etai = 1.0003, etat = ior;
    if (dotProduct(wi, N) < 0) { std::swap(etai, etat); }
    float c = fabs(dotProduct(wi, N));
    float g2 = powf((etat / etai), 2) - 1 + c * c;
    if (g2 < 0) { return 1; }//total internal refletion
    float g = sqrtf(g2);
    float dif = g - c, sum = g + c;
    float lhs = 0.5 * powf(dif, 2) / powf(sum, 2);
    float rhs = powf(c * sum - 1, 2) / powf(c * dif + 1, 2) + 1;
    return lhs * rhs;
}

Vector3f Material::F_Schlick(float cosTheta) {
    Vector3f F0 = lerp(Vector3f(0.04f), albedo, metalness);
    return F0 + (Vector3f(1.f) - F0) * pow(1.0 - cosTheta, 5.0);
}

float Material::D_GGX(const Vector3f& N, const Vector3f& H, float a) {
    float NdotH = dotProduct(N, H);
    if (NdotH <= 0) { return 0; }
    float a2 = a * a;
    float nom = a2;
    float tanTheta = crossProduct(N, H).norm() / NdotH;
    float denom = a2 + tanTheta * tanTheta;
    denom = M_PI * powf(NdotH, 4) * denom * denom;
    return nom / denom;
}

float Material::G1_GGX(const Vector3f& v, const Vector3f& h, const Vector3f& N) {
    float NdotV = dotProduct(N, v);
    float lhs = std::max(dotProduct(v, h) / NdotV, 0.f);
    float tanTheta = crossProduct(N, v).norm() / NdotV;
    float rhs = 2 / (1 + sqrtf(1 + roughness * roughness * tanTheta * tanTheta));
    return lhs * rhs;
}