#pragma once

#include <vector>
#include "Vector.h"
#include "Object.h"
#include "BVH.h"
#include "Ray.h"

class Scene
{
private:
    Vector3f backgroundColor = Vector3f(0.26f, 0.67f, 0.84f);
    Vector3f eyePos = Vector3f(278, 273, -800);
    int width = 784;
    int height = 784;
    int fov = 40;
    int maxDepth = 1;
    float RussianRoulette = 0.8;
    BVH* bvh;
    std::vector<Object* > objects;
public:
    Scene() {}
    Scene(int w, int h) : width(w), height(h) {}
    void Add(Object* object) { objects.push_back(object); }
    const std::vector<Object*>& GetObjects() const { return objects; }
    void BuildBVH() { this->bvh = new BVH(objects); }
    Intersection GetIntersection(const Ray& ray) const { return this->bvh->GetIntersection(ray); }
    Vector3f GetEyePos() const { return eyePos; }
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    void SetResolution(int w, int h) { width = w, height = h; }
    int GetFov() const { return fov; }
    void SetFov(int _fov) { fov = _fov; }
    void SetRR(float RR) { RussianRoulette = RR; }

    void SampleLight(Intersection& pos, float& pdf) const;
    Vector3f Trace(const Ray& ray, int depth) const;
    Vector3f Shade(const Intersection& inter, const Vector3f& wi) const;
};