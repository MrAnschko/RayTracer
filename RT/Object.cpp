#include "Object.h"

bool Object::Intersect(Ray r, float *out_t)
{
    return false;
}

void Object::GetColor(Ray r, float t, int dpth, Color *col_ptr)
{

}

BoundingBox *Object::GetBB()
{
    return new BoundingBox();
}
