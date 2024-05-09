#include "Grid.h"
#include "Object.h"

#include <vector>
#include <cmath>

// Funktion to add an Object to the grid.
void Grid::AddObject(Object obj)
{
    RT_Vector start_index,stop_index;
    RT_Vector bb_min = (obj.GetBB()->min_expanse);
    RT_Vector rel_min = bb_min  - minCorner;
    // if min is larger than the grid size then do not add it (might later be changed to save out of grid objects)
    if(rel_min.data[0] > grid_size.data[0] || rel_min.data[1] >grid_size.data[1] ||rel_min.data[2] > grid_size.data[2] )
        return;
 


    RT_Vector rel_max = obj.GetBB()->max_expanse - minCorner;
    // if max is out of grid then do not add it (might later be changed to save out of grid objects)
    if(rel_max.data[0] < 0 || rel_max.data[1] < 0 ||rel_max.data[2] < 0)
        return;

    // clip relative values such that the are no larger or smaller than the box:

    rel_min.data[0] = rel_min.data[0] > 0 ? rel_min.data[0]: 0; 
    rel_min.data[1] = rel_min.data[1] > 0 ? rel_min.data[1]: 0; 
    rel_min.data[2] = rel_min.data[2] > 0 ? rel_min.data[2]: 0;
    
    rel_max.data[0] = rel_min.data[0] > grid_size.data[0] ? grid_size.data[0] : rel_min.data[0]; 
    rel_max.data[1] = rel_min.data[1] > grid_size.data[0] ? grid_size.data[0] : rel_min.data[1]; 
    rel_max.data[2] = rel_min.data[2] > grid_size.data[0] ? grid_size.data[0] : rel_min.data[2];

    int x_start = std::floor(rel_min.data[0] / cell_size.data[0]);
    int y_start = std::floor(rel_min.data[1] / cell_size.data[1]);
    int z_start = std::floor(rel_min.data[2] / cell_size.data[2]);
    
    int x_stop = std::ceil(rel_max.data[0] / cell_size.data[0]);
    int y_stop = std::ceil(rel_max.data[1] / cell_size.data[1]);
    int z_stop = std::ceil(rel_max.data[2] / cell_size.data[2]);

    // set starting index
    int index = x_start+
                resolution.data[0]*y_start
                +resolution.data[0]*resolution.data[1]*z_start;

    for (int z = z_start; z < z_stop; z++)
    {
        for (int y = y_start; y < y_stop; y++)
        {
            for (int x = x_start; x < x_stop; x++)
            {
                grid_data[index].push_back(obj);
                index++;
            }
            index += resolution.data[0];
        }
        index += resolution.data[0]*resolution.data[1];
    }
    
        
}


//function to get all objects of the cell at position
std::list<Object> Grid::ObjectsInCellPos(RT_Vector pos)
{
    // get the cell;
    RT_Vector rel_pos = pos-minCorner;

    //return empty lists if the position is out of the grid.
    if(rel_pos.data[0] < 0 || rel_pos.data[1] < 0 ||rel_pos.data[2] < 0 )
        return std::list<Object>();
    if(rel_pos.data[0] > grid_size.data[0] || rel_pos.data[1] >grid_size.data[1] ||rel_pos.data[2] > grid_size.data[2] )
        return std::list<Object>();
    int x_index = std::floor(rel_pos.data[0] / cell_size.data[0]); 
    int y_index = std::floor(rel_pos.data[1] / cell_size.data[1]); 
    int z_index = std::floor(rel_pos.data[2] / cell_size.data[2]); 
    
    return ObjectsInCellIndex(x_index,y_index,z_index);
}

std::list<Object> Grid::ObjectsInCellIndex(int x, int y ,int z)
{
    return grid_data[x+resolution.data[0]*y+resolution.data[0]*resolution.data[1]*z];
}

Grid::Grid(RT_Vector min_pos, RT_Vector max_pos, RT_Vector resolution)
{
    minCorner = min_pos;
    this->resolution = resolution;
    grid_size = max_pos -  min_pos;
    cell_size = RT_Vector(grid_size.data[0]/resolution.data[0],grid_size.data[1]/resolution.data[1],grid_size.data[2]/resolution.data[2]);
    grid_data.reserve(resolution.data[0]*resolution.data[1]*resolution.data[2]);
}

Grid::~Grid()
{
}
