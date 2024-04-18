#ifndef PLANE_H
#define PLANE_H

#include "Object.h"
#include "Material.h"
#include "Scene.h"

class Plane : public Object{
    public:
        RT_Vector normal;
        float distance;
        Material* mat;
        bool Intersect(Ray r,float* out_t) override; //method to check if there is an intersection with the ray the return value returns if an intersection took place, the out_t returns the t value at which the untersection took place
        
        void GetColor(Ray r, float t, int dpth, Color *col_ptr) override; // Ray is the rey that hit, dpth is the point at which it hit
        Plane(RT_Vector n, float d, Material* m, Scene* sc_ptr);
        ~Plane();
};

#endif