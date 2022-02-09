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

// BVHAccel Declarations
inline int leafNodes, totalLeafNodes, totalPrimitives, interiorNodes;

class BVH {
private:
    const int leafNodeObjNum = 1;
    std::vector<Object*> primitives;
    BVHNode* root;

    //construct the BVH tree
    BVHNode* buildTree(std::vector<Object*>objects);

    //calls sample method on lower level objects
    void sample(BVHNode* node, float p, Intersection& pos, float& pdf);

    //recursively finds the intersection given a ray
    Intersection intersect(BVHNode* node, const Ray& ray)const;

public:
    //constructor
    BVH(std::vector<Object*> p);

    //destructor
    ~BVH();

    //this just calls BVH::intersect
    Intersection GetIntersection(const Ray &ray) const;

    //this just calls BVH::sample
    void GetSample(Intersection &pos, float &pdf);
};

struct BVHNode {
    BoundingBox bounds;
    BVHNode *left;//left child
    BVHNode *right;//right child
    Object* object;
    float area;

    //struct constructor
    BVHNode(): area(0), left(nullptr), right(nullptr), object(nullptr) {
        bounds = BoundingBox();
    }
};
