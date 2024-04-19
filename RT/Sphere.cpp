#include <math.h>

#include "Sphere.h"
#include <cstddef>


class Scene;

bool Sphere::Intersect(Ray r, float *out_t)
{
    float A = 1.0f;
    float x_D = r.dir.data[0];
    float y_D = r.dir.data[1];
    float z_D = r.dir.data[2];

    float x_0 = r.pos.data[0];
    float y_0 = r.pos.data[1];
    float z_0 = r.pos.data[2];

    float x_c = pos.data[0];
    float y_c = pos.data[1];
    float z_c = pos.data[2];

    float B = 2*(x_D*(x_0-x_c)+y_D*(y_0-y_c)+z_D*(z_0-z_c));
    float C = (x_0 - x_c) * (x_0 -x_c) + (y_0 - y_c) * (y_0 -y_c) + (z_0 - z_c) * (z_0 -z_c)  - radius*radius;
    
    float D = (B*B)-(4*C); 
    if( D<0)
        return false;
    else
    {
        *out_t = ( 0.5f* (-B-sqrt(D)) <  0.5f* (-B+sqrt(D))) ?  0.5f* (-B-sqrt(D)) :  0.5f* (-B+sqrt(D));
        return true;
    }
}

void Sphere::GetColor(Ray r,float t ,int dpth, Color *col_ptr)
{
    // Calculate diffuse part;
    RT_Vector hitPos = r.PosAtT(t);
    RT_Vector normal = RT_Vector::Add(hitPos,pos.Negate());
    Ray normal_ray = Ray(hitPos,normal);
    float cumulated_intensity = 0.0f;
    for (std::size_t i = 0; i < scene_ptr->n_lights; i++)
    {
        cumulated_intensity += scene_ptr->light_ptrs[i]->GetIntensity(normal_ray);
    }

    Color ref_col = Color(0.0f,0.0f,0.0f);
    //Calculate reflection, if the depth allows it (and reflective part is greater than 0):
    if(dpth>0 && mat->refl_percentage>0.0f)
    {
        // calculate the reflection vector:
        RT_Vector ref = RT_Vector::Add(r.dir, RT_Vector::ScalarProduct( -2.0f* RT_Vector::DotProduct(r.dir,normal), normal) ); // d - 2 ( d . n ) n
        Ray r_ref = Ray(hitPos,ref);
        float t_ref;
        Object* ref_obj_ptr = NULL;

        scene_ptr->Raycast(r_ref,&t_ref, &ref_obj_ptr,this);
        if (ref_obj_ptr != NULL)
        {
            ref_obj_ptr->GetColor(r_ref,t_ref,dpth-1,&ref_col);
        }
        
        
    }
    

    col_ptr->r = mat->ambientCol.r + cumulated_intensity*mat->diffuseCol.r + (mat->refl_percentage) * ref_col.r;
    col_ptr->b = mat->ambientCol.b + cumulated_intensity*mat->diffuseCol.b + (mat->refl_percentage) * ref_col.b;
    col_ptr->g = mat->ambientCol.g + cumulated_intensity*mat->diffuseCol.g + (mat->refl_percentage) * ref_col.g;

}

Sphere::Sphere(RT_Vector p, float r, Material* m, Scene* scn_ptr)
{
    pos = p.Copy();
    radius = r;
    mat = m;
    scene_ptr = scn_ptr;
}

Sphere::~Sphere()
{

}
