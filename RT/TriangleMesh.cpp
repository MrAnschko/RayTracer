#include "TriangleMesh.h"

#include <vector>
#include <limits>

bool TriangleMesh::Intersect(Ray r, float *out_t)
{
    float t_min = std::numeric_limits<float>::infinity();
    float curr_t = std::numeric_limits<float>::infinity();
    for(Triangle* tri : triangles)
    {
        tri->Intersect(r,&curr_t);
    } 
}

void TriangleMesh::GetColor(Ray r, float t, int dpth, Color *col_ptr)
{
}

TriangleMesh::TriangleMesh(std::vector<Triangle *> tris, Material *m, Scene *sc_ptr)
{
    triangles = tris;
    mat = m;
    scene_ptr = sc_ptr;
}


TriangleMesh::~TriangleMesh()
{
}
