#pragma once

#include <atomic>
#include <vector>
#include <memory>
#include <ctime>
#include "Object.h"
#include "Ray.h"
#include "BoundingBox.h"
#include "Intersection.h"
#include "Vector.h"

struct BVHNode;
struct BVHPrimitiveInfo;

// BVHAccel Declarations
inline int leafNodes, totalLeafNodes, totalPrimitives, interiorNodes;
class BVH {
private:
    // BVHAccel Private Data
    const int leafNodeObjNum = 1;
    std::vector<Object*> primitives;
    BVHNode* root;

    // BVHAccel Private Methods
    BVHNode* buildTree(std::vector<Object*>objects);
    void sample(BVHNode* node, float p, Intersection& pos, float& pdf);
    Intersection intersect(BVHNode* node, const Ray& ray)const;

public:
    // BVHAccel Public Types
    // BVHAccel Public Methods
    BVH(std::vector<Object*> p);
    ~BVH();
    Intersection GetIntersection(const Ray &ray) const;
    void GetSample(Intersection &pos, float &pdf);
};


struct BVHNode {
    BoundingBox bounds;
    BVHNode *left;
    BVHNode *right;
    Object* object;
    float area;
    BVHNode(): area(0), left(nullptr), right(nullptr), object(nullptr)
    {
        bounds = BoundingBox();
    }
};
