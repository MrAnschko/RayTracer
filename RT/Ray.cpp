#include "Ray.h"
#include "RT_Vector.h"

RT_Vector Ray::PosAtT(float t) // returns the position as vector if a value for r is set for the ray. 
{
    return RT_Vector::Add(pos, RT_Vector::ScalarProduct(t, dir));
}

Ray::Ray(RT_Vector p, RT_Vector d)
    : pos(p.Copy())
{
    dir = d.Normalize();
}

Ray::~Ray()
{
}