
#define EPSILON 0.01


#include "Camera.h"
#include <stdio.h>      /* printf, fopen */
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include <math.h>

Camera::Camera(RT_Vector pos, RT_Vector sCenter,RT_Vector u, float hfov, float vfov, float hres, float vres)
{
    // Check that up and sCenter are orthogonal:
    if(RT_Vector::DotProduct(sCenter,u) >= 0.0f + EPSILON || RT_Vector::DotProduct(sCenter,u) <= 0.0f - EPSILON )
    {
        printf("Error: UP and  sCenter are not orthogonal"); // if they aren't exit the programm with an error
        exit(EXIT_FAILURE);
    }


    position = pos.Copy();
    screenCenter = sCenter.Copy();
    up = u.Copy();
    res[0] = hres;
    res[1] = vres;
    fov[0] = hfov;
    fov[1] = vfov;

    float s_dist = screenCenter.Length();

    scrnx = (RT_Vector::CrossProduct(sCenter,u));
    scrnx = scrnx.Normalize(); // Get Direction of scrn_x
    scrny = RT_Vector::CrossProduct(scrnx, sCenter).Normalize(); // scale to be one pixel large
    scrnx = RT_Vector::ScalarProduct(2 * s_dist * abs(tan(hfov))/hres,scrnx); // scale to be one pixel large.
    scrny = RT_Vector::ScalarProduct(2 * s_dist * abs(tan(vfov))/vres,scrny);


    Image img = Image(hres,vres);

}

Camera::~Camera()
{

}
