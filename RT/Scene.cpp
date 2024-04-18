#define MIN_DIST 0.01f
#define RAY_DEPTH 5

#include <limits>
#include <iostream>

#include "Scene.h"
#include "Object.h"
#include "Camera.h"
#include "RT_Vector.h"

Scene::Scene(Object **objs, int n_objs,Light** l_ptrs, int n_ls, Camera *c)
{
    cam_ptr = c;
    object_ptrs = objs;
    n_Objects = n_objs;
    light_ptrs = l_ptrs;
    n_lights = n_ls;

    // when the scene is created make sure that each object and each light knows which scene it is in.
    for (size_t i = 0; i < n_Objects; i++)
    {
        object_ptrs[i]->scene_ptr = this;
    }
    for (size_t i = 0; i < n_lights; i++)
    {
        light_ptrs[i]->scene_ptr = this;
    }
    
    
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

void Scene::Raytrace(Ray r, int Depth, Color* col_ptr)
{
    if( Depth <= 0)
        return;
    // Find closest hit;
    float t_min = std::numeric_limits<float>::infinity();
    float curr_t = 1000.0; // float of the current hit of the object
    Object* closestObject = NULL;
    Raycast(r,&t_min,&closestObject,NULL);
    
    if(closestObject != NULL )
        closestObject->GetColor(r, t_min, Depth-1, col_ptr);
}

void Scene::Raycast(Ray r, float* t, Object** object_ptr, Object* ignore_obj)
{
    float t_min = std::numeric_limits<float>::infinity();
    float curr_t = std::numeric_limits<float>::infinity();
    for(int i = 0; i < n_Objects; i++)
    {
        if(object_ptrs[i] != ignore_obj)
        {
            if(object_ptrs[i]->Intersect(r,&curr_t))
            {
                if( curr_t> MIN_DIST && curr_t < t_min)
                {
                    t_min = curr_t;
                    *object_ptr = object_ptrs[i];
                }
            }
        }
    }
    *t = t_min;
}

Scene::~Scene()
{
    
}