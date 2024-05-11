#ifndef GRID_H
#define GRID_H

#include "Object.h"
#include "RT_Vector.h"
#include "AR_List.h"

class Object;

class Grid{
    public:
        void AddObject(Object *obj_ptr);
        bool WithinGrid(RT_Vector p);
        bool IndexWithinGrid(RT_Vector p);
        AR_List_Util::AR_List<Object>* ObjectsInCellPos(RT_Vector pos);
        AR_List_Util::AR_List<Object>* ObjectsInCellIndex(int x,int y,int z);
        Grid(RT_Vector min_pos, RT_Vector max_pos, RT_Vector resolution);
        Grid();
        ~Grid();
        AR_List_Util::AR_List<Object>** grid_data; // an array for all the data  for [x][y][z] = [x+x_res*y+x_res*y_res*z]
        RT_Vector minCorner; // The min corner of the 0 0 0 Cell 
        RT_Vector cell_size; // The scale of the cells in each dimension
        RT_Vector grid_size; // The size of the Grid in each dimension
        RT_Vector resolution; // the number of cells in each dimension

};


#endif