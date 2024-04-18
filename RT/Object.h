#ifndef OBJECT_H
#define OBJECT_H
 
#include "Scene.h"
#include "Ray.h"
#include "Image.h"

class Scene;

class Object{
    public:
        virtual bool Intersect(Ray r,float* out_t);
        virtual void GetColor(Ray r, float t, int dpth, Color* col_ptr);
        Scene* scene_ptr;
};

#endif