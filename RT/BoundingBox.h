#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "RT_Vector.h"

class BoundingBox
{
    public:
        bool bbIntersect(BoundingBox *other_box);
        BoundingBox(RT_Vector min, RT_Vector max);
        BoundingBox();
        ~BoundingBox();
        RT_Vector min_expanse;
        RT_Vector max_expanse;
};



#endif