#define MIN_DIST 0.01f
#define RAY_DEPTH 3
#define GRID_EXPANSE 30 
#define GRID_RESOLUTION 60
//#define USE_GRID
//#define USE_CACHE
#include <limits>
#include <iostream>

#include "Scene.h"
#include "Object.h"
#include "Camera.h"
#include "RT_Vector.h"
#include "AR_List.h"

#include <cstddef>
#include <cmath>
#include <list>

Scene::Scene(Object **objs, int n_objs,Light** l_ptrs, int n_ls, Camera *c , int block_cache_size)
{
    cam_ptr = c;
    object_ptrs = objs;
    n_Objects = n_objs;
    light_ptrs = l_ptrs;
    n_lights = n_ls;
    cache_size  = std::min(block_cache_size,n_Objects);
#ifndef USE_CACHE
    cache_size = 0;
#endif
    cache_end=cache_size-1 ;

    // when the scene is created make sure that each object and each light knows which scene it is in.
    for (std::size_t i = 0; i < n_Objects; i++)
    {
        object_ptrs[i]->scene_ptr = this;
    }
    for (std::size_t i = 0; i < n_lights; i++)
    {
        light_ptrs[i]->scene_ptr = this;
    }
    RT_Vector min(-1* GRID_EXPANSE , -1* GRID_EXPANSE ,-1* GRID_EXPANSE );
    RT_Vector max( GRID_EXPANSE , GRID_EXPANSE , GRID_EXPANSE );
    RT_Vector res( GRID_RESOLUTION , GRID_RESOLUTION , GRID_RESOLUTION );
    Grid g = Grid(min,max,res);
    grid = g;
    #ifdef USE_GRID
    for (int i = 0; i < n_Objects; i++)
    {
        g.AddObject(object_ptrs[i]);
    }
    #endif

    
}

void Scene::Render(const char *path)
{
    //create image:
    Image img = Image(cam_ptr->res[0],cam_ptr->res[1]);

    // Create starting ray.
    RT_Vector f = 
        RT_Vector::Add(
            cam_ptr->screenCenter,
            RT_Vector::Add(
                RT_Vector::ScalarProduct(-0.5f *cam_ptr->res[0] ,cam_ptr->scrnx),
                RT_Vector::ScalarProduct(0.5f*cam_ptr->res[1],cam_ptr->scrny)
            )
        );

    Ray curr_ray = Ray(cam_ptr->position,f);

    // shoot ray for each pixel:
    for( float q_x = 0.0f; q_x < cam_ptr->res[0]; q_x++)
    {
        for(float q_y = 0.0f ; q_y < cam_ptr->res[1]; q_y++)
        { 
            RT_Vector curr_dir = RT_Vector::Add(f, RT_Vector::ScalarProduct(q_x, cam_ptr->scrnx)); // shift by x <- could be separated into outer loop, but I was too lazy. 
            curr_dir = RT_Vector::Add(curr_dir, RT_Vector::ScalarProduct(-1.0f*q_y, cam_ptr->scrny)); // shift by y
            curr_ray.dir = curr_dir.Normalize();

            Color col = Color(0.0f,0.0f,0.0f);
            Raytrace(curr_ray, RAY_DEPTH ,&col); // this will also get the color, hopefully

            img.SetColor(col,(int)q_x,(int)q_y);

        }
    }

    img.Export(path);
}

// For one ray compute the color.
void Scene::Raytrace(Ray r, int Depth, Color* col_ptr)
{
    if( Depth <= 0)
        return;
    // Find closest hit;
    float t_min = std::numeric_limits<float>::infinity();
    Object* closestObject = NULL;
    Raycast(r,&t_min,&closestObject,NULL);
    if(closestObject != NULL )
        closestObject->GetColor(r, t_min, Depth-1, col_ptr);
}

void Scene::GridRaycast(Ray r, float *t_out, Object **object_ptr, Object *ignore_obj)
{
    float t_min = std::numeric_limits<float>::infinity();
    float curr_t = std::numeric_limits<float>::infinity();
    
    // initializing point of entry for ray.
    if(grid.WithinGrid(r.pos)){
        // ray starts from within the grid
    }
    else
    {
        // ray is from the outside of the grid
        // find the starting cell of the ray
        
        // possible min intersections:

        RT_Vector t_mins =  RT_Vector::HadamardProduct(grid.minCorner - r.pos, r.dir.Negate()); // t s corresponding to hits of the minimal  planes
        RT_Vector t_maxs =  RT_Vector::HadamardProduct(grid.minCorner - r.pos+ grid.grid_size, r.dir.Negate()); // t's corresponding to hits of the maximum planes
        // note that the t_mins needn't be smaller than the t_maxs


        // check that at least one of the t's is not zero in each direction
        // otherwise no hit with the grid
        if
        ( 
            t_mins.data[0] < 0 && t_maxs.data[0] < 0 
            || t_mins.data[1] < 0 && t_maxs.data[1] < 0 
            || t_mins.data[2] < 0 && t_maxs.data[2] < 0 
        )
        {
            *t_out = t_min;
            return;
        }

    }


    
    
}

// cast a single ray and find the closest object it hits 
void Scene::Raycast(Ray r, float* t, Object** object_ptr, Object* ignore_obj)
{   
    #ifdef USE_GRID
    GridRaycast(r,t,object_ptr,NULL);
    return;
    #endif
    float t_min = std::numeric_limits<float>::infinity();
    float curr_t = std::numeric_limits<float>::infinity();
    int index_of_min = -1;
#ifdef USE_CACHE
    // go over the cached objects first
    for(int i = 0; i < cache_size; i++)
    {   
        Object* curr_obj = GetCacheObj(i);
        
        if( curr_obj != ignore_obj)
        {
            if(curr_obj->Intersect(r,&curr_t))
            {
                if( curr_t> MIN_DIST && curr_t < t_min)
                {
                    t_min = curr_t;
                    *object_ptr = curr_obj;
                    
                    int pos = (i+cache_end+1) % cache_size;
                    if(pos<0)
                        pos += cache_size;
                    index_of_min = pos;
                }
            }
        }
    }
#endif
    
    for(int i = cache_size; i < n_Objects; i++)
    {
        if(object_ptrs[i] != ignore_obj)
        {
            if(object_ptrs[i]->Intersect(r,&curr_t))
            {
                if( curr_t> MIN_DIST && curr_t < t_min)
                {
                    t_min = curr_t;
                    *object_ptr = object_ptrs[i];
                    index_of_min = i;
                }
            }
        }
    }
#ifdef USE_CACHE
    if(index_of_min >= 0)
        AddObjectAtIndexToCache(index_of_min);
#endif
    *t = t_min;
}

bool Scene::RaycastHit(Ray r, float *maxDepth)
{
    float curr_t = std::numeric_limits<float>::infinity();
    Object* object_hit_ptr;
    
#ifdef USE_CACHE
    for(int i = 0; i < cache_size; i++)
    {
        if(GetCacheObj(i)->Intersect(r,&curr_t))
        {
            if( curr_t> MIN_DIST && curr_t < *maxDepth)
            {
                
                int pos = (i+cache_end+1) % cache_size;
                if(pos<0)
                    pos += cache_size;
                object_hit_ptr = GetCacheObj(pos);
                return true;
            }
            
        }
    }
#endif

    for(int i = cache_size; i < n_Objects; i++)
    {
        if(object_ptrs[i]->Intersect(r,&curr_t))
        {
            if( curr_t> MIN_DIST && curr_t < *maxDepth)
            {
                object_hit_ptr = object_ptrs[i];
                #ifdef USE_CACHE
                AddObjectAtIndexToCache(i);
                #endif
                return true;
            }
            
        }
    }
    return false;
}


void Scene::AddObjectAtIndexToCache(int index)
{
    // Exchanges the entries of last index and the object to be added    
    Object* prevEntry = object_ptrs[cache_end]; // Get previous last object in Queue
    object_ptrs[cache_end] = object_ptrs[index]; // Get put new object to the end of the queue
    cache_end--; // Shift end by one (so the last end is the beginning)
    cache_end = cache_end < 0 ? cache_size-1 : cache_end; // do loop de loop
    object_ptrs[index] = prevEntry; // put the object of the end of the queue to the place of the new object

}

Object *Scene::GetCacheObj(int index)
{
    int pos = (index+cache_end+1)% cache_size;
    if(pos<0)
        pos += cache_size;
    return object_ptrs[pos];
}

Scene::~Scene()
{
    
}