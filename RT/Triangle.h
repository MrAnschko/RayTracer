#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object.h"
#include "Material.h"
#include "Scene.h"

class Triangle : public Object{
    private:
        RT_Vector normal;
        float distance;
    public:
        RT_Vector point_1;
        RT_Vector point_2;
        RT_Vector point_3;
        Material* mat;
        bool Intersect(Ray r,float* out_t) override; //method to check if there is an intersection with the ray the return value returns if an intersection took place, the out_t returns the t value at which the untersection took place
        
        void GetColor(Ray r, float t, int dpth, Color *col_ptr) override; // Ray is the rey that hit, dpth is the point at which it hit
        Triangle(RT_Vector p1, RT_Vector p2, RT_Vector p3, Material* m, Scene* sc_ptr);
        ~Triangle();
};

#endif