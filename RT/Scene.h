#ifndef SCENE_H
#define SCENE_H

#include "Object.h"
#include "Camera.h"
#include "Light.h"
class Object;
class Light;

class Scene{
    private:
        void Raytrace(Ray r, int Depth, Color* col_ptr);
        int cache_end;
        int cache_size;
    public:
        Object **object_ptrs; // Pointer to the memory that holds the objects. Maybe should define a Data structure for that but I figured well do that anyway
        int n_Objects; // the number of objects in the scene
        Light **light_ptrs;
        int n_lights;
        Camera *cam_ptr; // pointer to the camera of the scene
        void Render(const char* path); // A function to write all the colors into an image nd export them at the given path.
        void Raycast(Ray r, float* t_out, Object** obj_hit, Object* ignore_obj);
        bool RaycastHit(Ray r, float* maxDepth); // Test whether a Raycast hits any object before the depth is reached. doesn't care what object
        void AddObjectAtIndexToCache(int index); //there is no such thing as a cache, actually but this rearranges the memory accordingly. also the cache is not ordered.
        Object* GetCacheObj(int index); // method to Access the cache at an point index.
        Scene(Object **obj_ptrs, int n_objs, Light** light_ptrs, int n_lights, Camera *c,int block_cache_size);
        ~Scene();
};
#endif