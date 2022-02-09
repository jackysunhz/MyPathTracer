#pragma once
#include "Vector.h"

struct Ray {
    Vector3f origin;
    Vector3f direction;
    Vector3f direction_inverse;//this is only used for calculation convenience
    Ray(const Vector3f& ori, const Vector3f& dir): origin(ori), direction(dir) {
        direction_inverse = Vector3f(1./direction.x, 1./direction.y, 1./direction.z);
    }
};
