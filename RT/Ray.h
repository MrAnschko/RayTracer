#ifndef RAY_H
#define RAY_H
#include "RT_Vector.h"


class Ray
{
public:
    RT_Vector pos; //the position of the 
    RT_Vector dir;

    RT_Vector PosAtT(float t);
    
    Ray(RT_Vector p, RT_Vector d);
    ~Ray();
};

#endif