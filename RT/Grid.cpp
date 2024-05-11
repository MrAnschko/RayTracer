#include "Grid.h"
#include "Object.h"
#include "AR_List.h"

#include <vector>
#include <cmath>

// Funktion to add an Object to the grid.
void Grid::AddObject(Object *obj_ptr)
{ 
    BoundingBox *bb = obj_ptr->GetBB();
    RT_Vector bb_min = (obj_ptr->GetBB()->min_expanse);

    RT_Vector rel_min = bb_min  - minCorner;
    // if min is larger than the grid size then do not add it (might later be changed to save out of grid objects)
    if(rel_min.data[0] > grid_size.data[0] || rel_min.data[1] >grid_size.data[1] ||rel_min.data[2] > grid_size.data[2] )
        return;
 



    RT_Vector rel_max = obj_ptr->GetBB()->max_expanse - minCorner;
    // if max is out of grid then do not add it (might later be changed to save out of grid objects)
    if(rel_max.data[0] < 0 || rel_max.data[1] < 0 ||rel_max.data[2] < 0)
        return;

    // clip relative values such that the are no larger or smaller than the box:

    rel_min.data[0] = rel_min.data[0] > 0 ? rel_min.data[0]: 0; 
    rel_min.data[1] = rel_min.data[1] > 0 ? rel_min.data[1]: 0; 
    rel_min.data[2] = rel_min.data[2] > 0 ? rel_min.data[2]: 0;
    
    rel_max.data[0] = rel_max.data[0] > grid_size.data[0] ? grid_size.data[0] : rel_max.data[0]; 
    rel_max.data[1] = rel_max.data[1] > grid_size.data[1] ? grid_size.data[1] : rel_max.data[1]; 
    rel_max.data[2] = rel_max.data[2] > grid_size.data[2] ? grid_size.data[2] : rel_max.data[2];

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
                index = x + y * resolution.data[0] + z * resolution.data[0]*resolution.data[1];
                AR_List_Util::AR_List<Object> *tmp_l = (AR_List_Util::AR_List<Object> *) malloc(sizeof(AR_List_Util::AR_List<Object>));
                tmp_l->prev = tmp_l;
                tmp_l->data = obj_ptr;
                tmp_l->next = tmp_l;
                if(grid_data[index] == nullptr) {
                    grid_data[index] = tmp_l;
                }
                else
                {
                    
                    AR_List_Util::Append<Object>(grid_data[index],tmp_l);
                    AR_List_Util::AR_List<Object>* test =grid_data[index] ;
                }
            }
        }
    }
    
        
}

//checks whether a point is within the grid
bool Grid::WithinGrid(RT_Vector p)
{
    return p.data[0] >= this->minCorner.data[0] &&
            p.data[1] >= this->minCorner.data[1] &&
            p.data[2] >= this->minCorner.data[2] &&
            p.data[0] <= this->minCorner.data[0]+this->grid_size.data[0] &&
            p.data[1] <= this->minCorner.data[1]+this->grid_size.data[1] &&
            p.data[2] <= this->minCorner.data[2]+this->grid_size.data[2];
}

//checks whether an index is within the grid
bool Grid::IndexWithinGrid(RT_Vector p)
{
    return std::floor(p.data[0]) >= 0 &&
            std::floor(p.data[1]) >= 0 &&
            std::floor(p.data[2]) >= 0 &&
            std::floor(p.data[0]) < this->resolution.data[0] &&
            std::floor(p.data[1]) < this->resolution.data[1] &&
            std::floor(p.data[2]) < this->resolution.data[2];
}

//function to get all objects of the cell at position
AR_List_Util::AR_List<Object>* Grid::ObjectsInCellPos(RT_Vector pos)
{
    // get the cell;
    RT_Vector rel_pos = pos-minCorner;

    //return empty lists if the position is out of the grid.
    if(rel_pos.data[0] < 0 || rel_pos.data[1] < 0 ||rel_pos.data[2] < 0 )
        return nullptr;
    if(rel_pos.data[0] > grid_size.data[0] || rel_pos.data[1] >grid_size.data[1] ||rel_pos.data[2] > grid_size.data[2] )
        return nullptr;
    int x_index = std::floor(rel_pos.data[0] / cell_size.data[0]); 
    int y_index = std::floor(rel_pos.data[1] / cell_size.data[1]); 
    int z_index = std::floor(rel_pos.data[2] / cell_size.data[2]); 
    
    return ObjectsInCellIndex(x_index,y_index,z_index);
}

AR_List_Util::AR_List<Object>* Grid::ObjectsInCellIndex(int x, int y ,int z)
{
    return grid_data[(x+(int)resolution.data[0]*y+(int)resolution.data[0]*(int)resolution.data[1]*z)];
}

Grid::Grid(RT_Vector min_pos, RT_Vector max_pos, RT_Vector resolution)
{
    minCorner = min_pos.Copy();
    this->resolution = resolution.Copy();
    grid_size = max_pos -  min_pos;
    cell_size = RT_Vector(grid_size.data[0]/resolution.data[0],grid_size.data[1]/resolution.data[1],grid_size.data[2]/resolution.data[2]);
    int arr_size = (int)resolution.data[0]*(int)resolution.data[1]*(int)resolution.data[2]*sizeof(AR_List_Util::AR_List<Object>*);
    grid_data = (AR_List_Util::AR_List<Object>**) malloc(arr_size);
    for (size_t i = 0; i < resolution.data[0]*resolution.data[1]*resolution.data[2]; i++)
    {   
        grid_data[i] = nullptr;
    }
    
}

Grid::Grid()
{
    RT_Vector zero(0,0,0);
    minCorner = zero;
    resolution = zero;
    grid_size = zero;
    cell_size = zero;
    grid_data = nullptr;
}

Grid::~Grid()
{
}
