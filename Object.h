#pragma once
#include "global.h"
#include "BoundingBox.h"
#include "Ray.h"
#include "Intersection.h"

class Object {
public:
    Object() {}
    virtual ~Object() {}
    virtual Intersection getIntersection(Ray _ray) = 0;
    virtual BoundingBox getBounds()=0;
    virtual float getArea()=0;
    virtual void Sample(Intersection &inter, float &pdf)=0;
    virtual bool hasEmit()=0;
};


