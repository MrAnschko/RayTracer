#ifndef LIGHT_H
#define LIGHT_H

#include "RT_Vector.h"
#include "Ray.h"
#include "Scene.h"
#include <vector>
#include "Object.h"

class Scene;
class Object;

class Light{
    public:
        RT_Vector pos;
        float startingIntensity;
        float GetIntensity(Ray ray);
        Scene* scene_ptr;
        Light(RT_Vector p, float start_int);
        ~Light();

};


#endif