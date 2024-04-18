#include "RT_Vector.h"
#include <iostream>
#include <math.h>

RT_Vector::RT_Vector(float x, float y, float z)
{
    data[0] = x; 
    data[1] = y; 
    data[2] = z; 
}

RT_Vector::RT_Vector()
{
    data[0] = 0; 
    data[1] = 0; 
    data[2] = 0;
}

RT_Vector::~RT_Vector()
{
}

void RT_Vector::Print()
{
    std::cout << "(" << data[0]<< ", " << data[1]<< ", " << data[2] << ")\n" ;
}

RT_Vector RT_Vector::Copy()
{
    return RT_Vector(data[0], data[1],data[2]);
}

RT_Vector RT_Vector::Normalize()
{
    float len = Length();
    return RT_Vector(data[0]/len,data[1]/len,data[2]/len);
}

RT_Vector RT_Vector::Negate()
{
    return RT_Vector::ScalarProduct(-1.0f,*this);
}

float RT_Vector::Length()
{
    return sqrt(data[0]*data[0]+data[1]*data[1]+data[2]*data[2]);
}

RT_Vector RT_Vector::CrossProduct(RT_Vector vec_a, RT_Vector vec_b)
{
    float x = (vec_a.data[1]*vec_b.data[2])-(vec_a.data[2]*vec_b.data[1]);
    float y = (vec_a.data[2]*vec_b.data[0])-(vec_a.data[0]*vec_b.data[2]);
    float z = (vec_a.data[0]*vec_b.data[1])-(vec_a.data[1]*vec_b.data[0]);
    return RT_Vector(x,y,z);
}

float RT_Vector::DotProduct(RT_Vector vec_a, RT_Vector vec_b)
{
        return vec_a.data[0]*vec_b.data[0]+vec_a.data[1]*vec_b.data[1]+vec_a.data[2]*vec_b.data[2];
}

// Adds the content of two vectors
RT_Vector RT_Vector::Add(RT_Vector vec_a, RT_Vector vec_b)
{
    return RT_Vector(
        vec_a.data[0]+vec_b.data[0],
        vec_a.data[1]+vec_b.data[1],
        vec_a.data[2]+vec_b.data[2]
        );
}

RT_Vector RT_Vector::ScalarProduct(float scalar, RT_Vector vec)
{
    return RT_Vector(
        vec.data[0] * scalar,
        vec.data[1] * scalar, 
        vec.data[2] * scalar 
    );
}
