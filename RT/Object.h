#ifndef OBJECT_H
#define OBJECT_H
 
#include "Scene.h"
#include "Ray.h"
#include "Image.h"
#include "BoundingBox.h"

class Scene;

class Object{
    public:
        virtual bool Intersect(Ray r,float* out_t);
        virtual void GetColor(Ray r, float t, int dpth, Color* col_ptr);
        virtual BoundingBox* GetBB(); // method to get the bounding box of an object.
        Scene* scene_ptr;
};

#endif