#include "Plane.h"
#include "RT_Vector.h"
#define SMALL 0.01

bool Plane::Intersect(Ray r, float *out_t)
{
    if( RT_Vector::DotProduct(r.dir,normal)*RT_Vector::DotProduct(r.dir,normal) < SMALL )
    {
        return false;
    }
    float numerator = -1.0f*(RT_Vector::DotProduct(r.pos,normal) + distance);
    float denominator = RT_Vector::DotProduct(normal,r.dir);
    *out_t = numerator/denominator;
    return true;

}

void Plane::GetColor(Ray r, float t, int dpth, Color *col_ptr)
{
    // Calculate diffuse part;
    RT_Vector hitPos = r.PosAtT(t);
    RT_Vector n =normal.Normalize();
    Ray normal_ray = Ray(hitPos,n);
    float cumulated_intensity = 0.0f;
    for (size_t i = 0; i < scene_ptr->n_lights; i++)
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

Plane::Plane(RT_Vector n, float d, Material *m, Scene *sc_ptr)
{
    normal = n.Normalize();
    distance = d;
    mat = m;
    scene_ptr = sc_ptr;
}

Plane::~Plane()
{
}
