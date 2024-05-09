#ifndef SPHERE_H
#define SPHERE_H

#include "Object.h"
#include "Material.h"
#include "Scene.h"
#include "BoundingBox.h"


class Sphere : public Object{
    public:
        RT_Vector pos;
        float radius;
        Material* mat;
        bool Intersect(Ray r,float* out_t) override; //method to check if there is an intersection with the ray the return value returns if an intersection took place, the out_t returns the t value at which the untersection took place
        void GetColor(Ray r, float t, int dpth, Color *col_ptr) override; // Ray is the rey that hit, dpth is the point at which it hit
        BoundingBox* GetBB() override;
        Sphere(RT_Vector p, float r, Material* m, Scene* sc_ptr);
        ~Sphere();
};

#endif