#pragma once
#include "Ray.h"
#include "Vector.h"
#include <limits>
#include <array>

class BoundingBox
{
  public:
    Vector3f pMin, pMax;//two points to specify the bounding box

    //default constructor
    BoundingBox() {
        double minNum = std::numeric_limits<double>::lowest();
        double maxNum = std::numeric_limits<double>::max();
        pMax = Vector3f(minNum, minNum, minNum);
        pMin = Vector3f(maxNum, maxNum, maxNum);
    }

    //alt constructor 1
    BoundingBox(const Vector3f p) : pMin(p), pMax(p) {}

    //alt constructor 2
    BoundingBox(const Vector3f p1, const Vector3f p2) {
        pMin = Vector3f(fmin(p1.x, p2.x), fmin(p1.y, p2.y), fmin(p1.z, p2.z));
        pMax = Vector3f(fmax(p1.x, p2.x), fmax(p1.y, p2.y), fmax(p1.z, p2.z));
    }

    //return the diagonal vector
    Vector3f Diagonal() const {
        return pMax - pMin; 
    }

    //return the largest dimension(0 means x, 1 means y, 2 means z)
    int maxDim() const {
        Vector3f d = Diagonal();
        if (d.x > d.y && d.x > d.z)
            return 0;
        else if (d.y > d.z)
            return 1;
        else
            return 2;
    }

    //return the surface area of the BB
    double SurfaceArea() const {
        Vector3f d = Diagonal();
        return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
    }

    //return the geometric center of the BB
    Vector3f Centroid() { 
        return 0.5 * pMin + 0.5 * pMax; 
    }

    //return the intersection BB of *this* and another BB
    BoundingBox Intersect(const BoundingBox& b) {
        return BoundingBox(Vector3f(fmax(pMin.x, b.pMin.x), fmax(pMin.y, b.pMin.y),
                                fmax(pMin.z, b.pMin.z)),
                       Vector3f(fmin(pMax.x, b.pMax.x), fmin(pMax.y, b.pMax.y),
                                fmin(pMax.z, b.pMax.z)));
    }

    //return the relative position of a point
    Vector3f Offset(const Vector3f& p) const {
        Vector3f o = p - pMin;
        if (pMax.x > pMin.x)
            o.x /= pMax.x - pMin.x;
        if (pMax.y > pMin.y)
            o.y /= pMax.y - pMin.y;
        if (pMax.z > pMin.z)
            o.z /= pMax.z - pMin.z;
        return o;
    }

    //return if 2 BBs overlap
    bool Overlaps(const BoundingBox& b1, const BoundingBox& b2) {
        bool x = (b1.pMax.x >= b2.pMin.x) && (b1.pMin.x <= b2.pMax.x);
        bool y = (b1.pMax.y >= b2.pMin.y) && (b1.pMin.y <= b2.pMax.y);
        bool z = (b1.pMax.z >= b2.pMin.z) && (b1.pMin.z <= b2.pMax.z);
        return (x && y && z);
    }

    //return if a BB is inside another
    bool Inside(const Vector3f& p, const BoundingBox& b) {
        return (p.x >= b.pMin.x && p.x <= b.pMax.x && p.y >= b.pMin.y &&
                p.y <= b.pMax.y && p.z >= b.pMin.z && p.z <= b.pMax.z);
    }

    //return if a ray intersects with the current BB
    inline bool IntersectP(const Ray& ray, const Vector3f& invDir) const {
        Vector3f isDirPos(float(invDir.x > 0), float(invDir.y > 0), float(invDir.z > 0));
        Vector3f isDirNeg(float(invDir.x < 0), float(invDir.y < 0), float(invDir.z < 0));
        Vector3f t_in = (pMin * isDirPos + pMax * isDirNeg - ray.origin) * invDir;
        Vector3f t_out = (pMax * isDirPos + pMin * isDirNeg - ray.origin) * invDir;
        float _t_in = std::max({ t_in.x, t_in.y, t_in.z });
        float _t_out = std::min({ t_out.x, t_out.y, t_out.z });
        return _t_out > 0 + EPSILON && _t_in < _t_out + EPSILON;
    }
};

//Union operator on 2 BBs
inline BoundingBox Union(const BoundingBox& b1, const BoundingBox& b2)
{
    BoundingBox ret;
    ret.pMin = Vector3f::Min(b1.pMin, b2.pMin);
    ret.pMax = Vector3f::Max(b1.pMax, b2.pMax);
    return ret;
}

//Union operator on a BB and a point
inline BoundingBox Union(const BoundingBox& b, const Vector3f& p)
{
    BoundingBox ret;
    ret.pMin = Vector3f::Min(b.pMin, p);
    ret.pMax = Vector3f::Max(b.pMax, p);
    return ret;
}

