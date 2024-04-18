#include "Light.h"
#include "RT_Vector.h"
#include <iostream>

#define EPSILON 0.01f

float Light::GetIntensity(Ray ray)
{

    
    RT_Vector light_dir = RT_Vector::Add(pos,RT_Vector::ScalarProduct(-1.0f, ray.pos));
    float intensity = RT_Vector::DotProduct(light_dir,ray.dir) / light_dir.Length();
    intensity = intensity<0? 0 : intensity;
    float t;
    Object* obj_ptr = NULL;
    scene_ptr->Raycast(Ray(ray.pos,light_dir),&t,&obj_ptr,NULL);
    if(obj_ptr!=NULL && t <=  (light_dir.Length() - EPSILON ) )
    {
         return 0.0f;
    }
    
    return intensity*startingIntensity / RT_Vector::DotProduct(light_dir,light_dir);
}

Light::Light(RT_Vector p, float start_int)
{
    pos = p;
    startingIntensity = start_int;
}

Light::~Light()
{
}
