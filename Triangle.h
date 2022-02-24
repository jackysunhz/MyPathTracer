#pragma once
#include "BVH.h"
#include "Intersection.h"
#include "Material.h"
#include "OBJ_Loader.h"
#include "Object.h"
#include <cassert>
#include <array>

class Triangle : public Object {
private:
    Vector3f v0, v1, v2; // vertices A, B ,C , counter-clockwise order
    Vector3f e1, e2;     // 2 edges v1-v0, v2-v0;
    Vector3f normal;
    float area;
    Material* m;
public:
    //constructor
    Triangle(Vector3f _v0, Vector3f _v1, Vector3f _v2, Material* _m = nullptr)
        : v0(_v0), v1(_v1), v2(_v2), m(_m) {
        e1 = v1 - v0;
        e2 = v2 - v0;
        normal = normalize(crossProduct(e1, e2));
        area = crossProduct(e1, e2).norm()*0.5f;
    }

    Intersection getIntersection(Ray ray) {
        /* Find ray - triangle intersection using Moller Trumbore Algorithm
           Basic idea: express a point in triangle using baricentric coordinates and equate with O + tD
           where O is the ray origin and D is the ray direction*/
        Intersection inter;
        if (dotProduct(ray.direction, normal) > 0)
            return inter;
        double u, v, t_tmp = 0;
        Vector3f pvec = crossProduct(ray.direction, e2);
        double det = dotProduct(e1, pvec);
        if (fabs(det) < EPSILON)
            return inter;
        double det_inv = 1. / det;
        Vector3f tvec = ray.origin - v0;
        u = dotProduct(tvec, pvec) * det_inv;
        if (u < 0 || u > 1)
            return inter;
        Vector3f qvec = crossProduct(tvec, e1);
        v = dotProduct(ray.direction, qvec) * det_inv;
        if (v < 0 || u + v > 1)
            return inter;
        //at this point a valid intersection is guaranteed
        t_tmp = dotProduct(e2, qvec) * det_inv;
        inter.obj = this;
        inter.normal = normal;
        inter.m = m;
        inter.happened = true;
        inter.distance = t_tmp;//ray direction is normalized, so distance is numerically equal to t
        inter.coords = ray.origin + ray.direction * t_tmp;
        return inter;
    }
   
    BoundingBox getBounds() { return Union(BoundingBox(v0, v1), v2); }

    float getArea() { return area; }

    bool hasEmit() { return m->HasEmission(); }

    void Sample(Intersection& inter, float& pdf) {
        float x = std::sqrt(get_random_float());
        float y = get_random_float();
        inter.coords = v0 * (1.0f - x) + v1 * (x * (1.0f - y)) + v2 * (x * y);
        inter.normal = this->normal;
        pdf = 1.0f / area;
    }
};





