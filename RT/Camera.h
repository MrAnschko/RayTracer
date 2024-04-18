#ifndef CAMERA_H
#define CAMERA_H

#include "RT_Vector.h"
#include "Image.h"

class Camera
{
public:
    Camera(
        RT_Vector pos,
        RT_Vector sCenter,
        RT_Vector u, // could be done better since it needs to be orthogonal to sCenter. If it isnt orthogonal I will make it.
        float hfov, // fov in radians
        float vfov, // fov in radians
        float hres,
        float vres
        );
    RT_Vector position; // the position of the camera in world coordinates 
    RT_Vector up; //
    RT_Vector screenCenter; // center of the Screen (in local coordinates)
    RT_Vector scrnx; // vector orthogonal to up ant screen center.
    RT_Vector scrny; // vector orthogonal to screen center and parallel to up.

    float res[2]; // resolution in x and y direction
    float fov[2]; // fov in x (horizontal) and y(vertical) direction
    Image img;
    ~Camera();
};
#endif