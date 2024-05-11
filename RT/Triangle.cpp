#include "Triangle.h"
#include <limits>

bool Triangle::Intersect(Ray r, float *out_t)
{
    // modified version from Wikipedia
    // Moeller-Trumbore intersection algorithm



    constexpr float epsilon = std::numeric_limits<float>::epsilon();

    RT_Vector edge1 = point_2 - point_1;
    RT_Vector edge2 = point_3 - point_1;
    RT_Vector ray_cross_e2 = RT_Vector::CrossProduct(r.dir, edge2);
    float det = RT_Vector::DotProduct(edge1, ray_cross_e2);

    if (det > -epsilon && det < epsilon)
        return false;    // This ray is parallel to this triangle.

    float inv_det = 1.0 / det;
    RT_Vector s = r.pos - point_1;
    float u = inv_det * RT_Vector::DotProduct(s, ray_cross_e2);

    if (u < 0 || u > 1)
        return false;

    RT_Vector s_cross_e1 = RT_Vector::CrossProduct(s, edge1);
    float v = inv_det * RT_Vector::DotProduct(r.dir, s_cross_e1);

    if (v < 0 || u + v > 1)
        return false;

    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = inv_det * RT_Vector::DotProduct(edge2, s_cross_e1);

    if (t > epsilon) // ray intersection
    {
        *out_t = t;
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;}

BoundingBox *Triangle::GetBB()
{
    return &bb;
}

void Triangle::GetColor(Ray r, float t, int dpth, Color *col_ptr)
{
    // Basically the same as from the PLane, so copy/pasete:
    
    // Calculate diffuse part;
    RT_Vector hitPos = r.PosAtT(t);
    RT_Vector n =normal.Normalize();
    Ray normal_ray = Ray(hitPos,n);
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

Triangle::Triangle(RT_Vector p1, RT_Vector p2, RT_Vector p3, Material *m, Scene *sc_ptr)
{
    point_1 = p1;
    point_2 = p2;
    point_3 = p3;
    mat = m;
    scene_ptr = sc_ptr;

        // find minimum values among points
    float min_x = point_1.data[0] < point_2.data[0] ? point_1.data[0]:point_2.data[0] ;
    min_x = min_x < point_3.data[0] ? min_x : point_3.data[0];
    float min_y = point_1.data[1] < point_2.data[1] ? point_1.data[1]:point_2.data[1] ;
    min_y = min_y < point_3.data[1] ? min_y : point_3.data[1];
    float min_z = point_1.data[2] < point_2.data[2] ? point_1.data[2]:point_2.data[2] ;
    min_z = min_z < point_3.data[2] ? min_z : point_3.data[2];
    RT_Vector min = RT_Vector(min_x,min_y,min_z); 

    float max_x = point_1.data[0] > point_2.data[0] ? point_1.data[0]:point_2.data[0] ;
    max_x = max_x > point_3.data[0] ? min_x : point_3.data[0];
    float max_y = point_1.data[1] > point_2.data[1] ? point_1.data[1]:point_2.data[1] ;
    max_y = max_y > point_3.data[1] ? min_y : point_3.data[1];
    float max_z = point_1.data[2] > point_2.data[2] ? point_1.data[2]:point_2.data[2] ;
    max_z = max_z > point_3.data[2] ? min_z : point_3.data[2];
    RT_Vector max = RT_Vector(min_x,min_y,min_z);

    bb.min_expanse = min;
    bb.max_expanse = max;
}

Triangle::~Triangle()
{
}
