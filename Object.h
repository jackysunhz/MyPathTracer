//
// Created by LEI XU on 5/13/19.
//
#pragma once
#ifndef RAYTRACING_OBJECT_H
#define RAYTRACING_OBJECT_H

//#include "Vector.h"
#include "global.h"
#include "BoundingBox.h"
#include "Ray.h"
#include "Intersection.h"

class Object
{
public:
    Object() {}
    virtual ~Object() {}
    /*virtual bool intersect(const Ray& ray) = 0;
    virtual bool intersect(const Ray& ray, float &, uint32_t &) const = 0;*/
    virtual Intersection GetIntersection(Ray _ray) = 0;
    //virtual void getSurfaceProperties(const Vector3f &, const Vector3f &, const uint32_t &, const Vector2f &, Vector3f &, Vector2f &) const = 0;
    //virtual Vector3f evalDiffuseColor(const Vector2f &) const =0;
    virtual BoundingBox getBounds()=0;
    virtual float getArea()=0;
    virtual void Sample(Intersection &inter, float &pdf)=0;
    virtual bool hasEmit()=0;
};



#endif //RAYTRACING_OBJECT_H
