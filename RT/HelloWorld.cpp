#define BLOCKER_CACHE_SIZE 5

#include <iostream>
#include <vector>
#include <string>

#include <cstddef>
#include "RT_Vector.h"
#include "Camera.h"
#include "Sphere.h"
#include "Scene.h"
#include "Plane.h"
#include "Triangle.h"
#include "Grid.h"


using namespace std;

int main()
{
    //Define Parameters for the Camera:
    RT_Vector up = RT_Vector(0,0,1); // the Up vector of the camera
    RT_Vector pos = RT_Vector(-3,0,2); // the position of the camera

    RT_Vector sDir = RT_Vector(1,0,0); // the direction of the screen.
    float hfov = 1;
    float vfov = 1;
    float hres = 1080;
    float vres = 1080;

    // testing the grid.

    RT_Vector grid_start = RT_Vector(-20,-20,-20);
    RT_Vector grid_end = RT_Vector(+20,+20,+20);
    RT_Vector grid_resolution = RT_Vector(40,40,40);
    Grid g = Grid(grid_start,grid_end,grid_resolution);
    
    
    //Define the Camera
    Camera cam = Camera(
        pos,
        sDir,
        up, // could be done better since it needs to be orthogonal to sCenter. If it isnt orthogonal I will make it.
        hfov, // fov in radians
        vfov, // fov in radians
        hres,
        vres
    );
    


    
    //Define a sphere 1 
    RT_Vector sp_pos_1 = RT_Vector(2,0,0); // Position of the sphere (in world coordinates)
    
    Color sp_1_am_col = Color(0.5,0,0); // Color of the sphere
    Color sp_1_spec_col = Color(1,0,0); // Color of the sphere
    Color sp_1_diff_col = Color(0.0,0.5f,0); // Color of the sphere
    Material mat_1 = Material( sp_1_am_col ,  sp_1_spec_col,  sp_1_diff_col, 0.5f);
    
    Sphere sp_1 = Sphere(sp_pos_1,0.5f,&mat_1,NULL); //Sphere
    //Define a sphere 2

    RT_Vector sp_pos_2 = RT_Vector(6.0f,0,0); // Position of the sphere (in world coordinates)

    Color sp_2_am_col = Color(0.0f,0.5f,0); // Color of the sphere
    Color sp_2_spec_col = Color(1,1,1); // Color of the sphere
    Color sp_2_diff_col = Color(0.5,0.0f,0.0f); // Color of the sphere
    Material mat_2 = Material( sp_2_am_col ,  sp_2_spec_col,  sp_2_diff_col, 0.3f);
    
    Sphere sp_2 = Sphere(sp_pos_2,1.0,&mat_2,NULL); //Sphere

    //Define Plane 1
    Color pln_1_am_col = Color(0.1f,0.1f,0.1f); // Color of the sphere
    Color pln_1_spec_col = Color(0,0,0); // Color of the sphere
    Color pln_1_diff_col = Color(0.1f,0.1f,0.1f); // Color of the sphere
    Material pl_1_mat = Material( pln_1_am_col ,  pln_1_spec_col, pln_1_diff_col, 0.5f);
    Plane pln_1 = Plane(RT_Vector(0,1.0,0.1f),-3.0f,&pl_1_mat,NULL);

    //Define Plane 2
    Color pln_2_am_col = Color(0.1f,0.1f,0.1f); // Color of the sphere
    Color pln_2_spec_col = Color(0,0,0); // Color of the sphere
    Color pln_2_diff_col = Color(0.1f,0.1f,0.1f); // Color of the sphere
    Material pl_2_mat = Material( pln_2_am_col ,  pln_2_spec_col, pln_2_diff_col, 0.0f);
    Plane pln_2 = Plane(RT_Vector(0,0,1.0f),0.0f,&pl_2_mat,NULL);

    //Define Plane 3
    Plane pln_3 = Plane(RT_Vector(0,1.0,0.1f),3.0f,&pl_1_mat,NULL);

    g.AddObject(sp_1);
    //Define a triangle: 
    RT_Vector tr_p1 = RT_Vector(5,0,0);
    RT_Vector tr_p2 = RT_Vector(4,0,-3);
    RT_Vector tr_p3 = RT_Vector(5,2,0);

    Triangle tr_1 = Triangle(tr_p1,tr_p2, tr_p3, &pl_2_mat, NULL);

    //Define Lights
    int n_lights = 2;
    Light light1(RT_Vector(20,0,30),1000);
    Light light2(RT_Vector(0,30,30),1000);
    Light** lights=(Light **)(malloc(sizeof(Object*)*n_lights));
    lights[0] = &light1;
    lights[1] = &light2;

    //Define the Scene
    int n_objects = 4; //Define the number of objects in the scene
    Object** objs = (Object**)(malloc(sizeof(Object*)*n_objects)); // Allocate memory for Objects
    objs[0] = &sp_1; //
    objs[1] = &sp_2; //
    objs[2] = &pln_1;
    objs[3] = &tr_1;
    // objs[3] = &pln_2;
    // objs[4] = &pln_3;

    Scene scn = Scene(objs,n_objects,lights,n_lights,&cam, BLOCKER_CACHE_SIZE);
    scn.Render("test2.bmp");

    //Free the objects
    free(objs);
    free(lights);
}