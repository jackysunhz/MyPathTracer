#pragma once
#include <iostream>
#include <cmath>
#include <random>
#include "Vector.h"

#undef M_PI
#define M_PI 3.141592653589793f

const float EPSILON = 0.00016f;

inline float clamp(const float& lo, const float& hi, const float& v) 
{ 
    return std::max(lo, std::min(hi, v)); 
}

inline  bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1)
{
    float discr = b * b - 4 * a * c;
    if (discr < 0) return false;
    else if (discr == 0) x0 = x1 = - 0.5 * b / a;
    else {
        float q = (b > 0) ?
                  -0.5 * (b + sqrt(discr)) :
                  -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }
    if (x0 > x1) std::swap(x0, x1);
    return true;
}

inline Vector3f toWorld(const Vector3f& a, const Vector3f& N) 
{
    Vector3f B, C;
    if (std::fabs(N.x) > std::fabs(N.y)) {
        float invLen = 1.0f / std::sqrt(N.x * N.x + N.z * N.z);
        C = Vector3f(N.z * invLen, 0.0f, -N.x * invLen);
    }
    else {
        float invLen = 1.0f / std::sqrt(N.y * N.y + N.z * N.z);
        C = Vector3f(0.0f, N.z * invLen, -N.y * invLen);
    }
    B = crossProduct(C, N);
    return a.x * B + a.y * C + a.z * N;
}

inline float get_random_float()
{
    static std::random_device dev;
    static std::mt19937 rng(dev());
    static std::uniform_real_distribution<float> dist(0.f, 1.f);
    return dist(rng);
}

inline float deg2rad(const float& deg) 
{ 
    return deg * M_PI / 180.0; 
}

inline void UpdateProgress(float progress)
{
    printf("\r      ");
    printf("\r%.2f %%", progress * 100);
}
