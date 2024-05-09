#ifndef GRID_H
#define GRID_H

#include <vector>
#include <list>
#include "Object.h"
#include "RT_Vector.h"

class Grid{
    public:
        void AddObject(Object obj);
        std::list<Object> ObjectsInCellPos(RT_Vector pos);
        std::list<Object> ObjectsInCellIndex(int x,int y,int z);
        Grid(RT_Vector min_pos, RT_Vector max_pos, RT_Vector resolution);
        ~Grid();
    private:
        std::vector<std::list<Object>> grid_data; // a vector for all the data  for [x][y][z] = [x+x_res*y+x_res*y_res*z]
        RT_Vector minCorner; // The min corner of the 0 0 0 Cell 
        RT_Vector cell_size; // The scale of the cells in each dimension
        RT_Vector grid_size; // The size of the Grid in each dimension
        RT_Vector resolution; // the number of cells in each dimension

};


#endif