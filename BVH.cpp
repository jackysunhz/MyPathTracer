#include <algorithm>
#include <cassert>
#include "BVH.h"

/* 
* Private Methods
*/
BVHNode* BVH::buildTree(std::vector<Object*> objects)
{
    BVHNode* node = new BVHNode();

    if (objects.size() == 1) {
        auto obj = objects[0];
        node->bounds = obj->getBounds();
        node->object = obj;
        node->left = nullptr;
        node->right = nullptr;
        node->area = obj->getArea();
        return node;
    }
    else if (objects.size() == 2) {
        node->left = buildTree(std::vector{objects[0]});
        node->right = buildTree(std::vector{objects[1]});
        node->bounds = Union(node->left->bounds, node->right->bounds);
        node->area = node->left->area + node->right->area;
        return node;
    }
    else {
        BoundingBox centroidBB;//BB of all of the objects' centroids
        for (int i = 0; i < objects.size(); ++i) {
            centroidBB = Union(centroidBB, objects[i]->getBounds().Centroid());
        }
        int splitDim = centroidBB.maxDim();//determine axis to split
        switch (splitDim) {//sort the objects along whatever axis to split
        case 0:
            std::sort(objects.begin(), objects.end(), [](auto o1, auto o2) {
                return o1->getBounds().Centroid().x < o2->getBounds().Centroid().x;
            });
            break;
        case 1:
            std::sort(objects.begin(), objects.end(), [](auto o1, auto o2) {
                return o1->getBounds().Centroid().y < o2->getBounds().Centroid().y;
            });
            break;
        case 2:
            std::sort(objects.begin(), objects.end(), [](auto o1, auto o2) {
                return o1->getBounds().Centroid().z < o2->getBounds().Centroid().z;
            });
            break;
        }
        auto begin_iter = objects.begin();
        auto mid_iter = objects.begin() + (objects.size() / 2);
        auto end_iter = objects.end();
        auto leftobjects = std::vector<Object*>(begin_iter, mid_iter);
        auto rightobjects = std::vector<Object*>(mid_iter, end_iter);
        node->left = buildTree(leftobjects);
        node->right = buildTree(rightobjects);
        node->bounds = Union(node->left->bounds, node->right->bounds);
        node->area = node->left->area + node->right->area;
        return node;
    }
}

Intersection BVH::intersect(BVHNode* node, const Ray& ray) const
{
    if (node->bounds.IntersectP(ray, ray.direction_inv)) {//ray enters the bounding box
        if (node->object) {//currently at leaf node
            return node->object->GetIntersection(ray);
        }
        //if not at leaf node, return child intersection with smaller distance
        Intersection leftIntersection = intersect(node->left, ray);
        Intersection rightIntersection = intersect(node->right, ray);
        return (leftIntersection.distance < rightIntersection.distance) ? leftIntersection : rightIntersection;
    }
    else {
        return Intersection();
    }

}

void BVH::sample(BVHNode* node, float p, Intersection &inter, float &pdf)
{
    if(node->left == nullptr || node->right == nullptr){
        node->object->Sample(inter, pdf);
        pdf *= node->area;
        return;
    }
    if(p < node->left->area) sample(node->left, p, inter, pdf);
    else sample(node->right, p - node->left->area, inter, pdf);
}

/* 
* Public Methods
*/
BVH::BVH(std::vector<Object*> p): primitives(std::move(p))
{
    if (primitives.empty())
        return;
    root = buildTree(primitives);
}

BVH::~BVH() = default;

Intersection BVH::GetIntersection(const Ray& ray) const
{
    Intersection isect;
    if (!root)
        return isect;
    isect = BVH::intersect(root, ray);
    return isect;
}

void BVH::GetSample(Intersection &inter, float &pdf)
{
    float p = std::sqrt(get_random_float()) * root->area;
    sample(root, p, inter, pdf);
    pdf /= root->area;
}