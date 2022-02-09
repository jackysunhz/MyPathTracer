#pragma once
#include "Vector.h"
#include "Material.h"
class Object;

struct Intersection {
    Intersection(){
        happened=false;
        coords=Vector3f();
        normal=Vector3f();
        distance= std::numeric_limits<double>::max();
        obj =nullptr;
        m=nullptr;
    }
    bool happened;
    Vector3f coords;//position
    Vector3f tcoords;//texture coords
    Vector3f normal;
    Vector3f emit;//emission at the intersection surface
    double distance;//distance to the camera(for depth test)
    Object* obj;
    Material* m;
};
