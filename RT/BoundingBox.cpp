#include "BoundingBox.h"

bool BoundingBox::bbIntersect(BoundingBox *other_box)
{
    for (size_t i = 0; i < 3; i++)
    {
        if( !(
                (min_expanse.data[i] <= other_box->min_expanse.data[i] && other_box->min_expanse.data[i] <=  min_expanse.data[i])  ||
                (other_box->min_expanse.data[i] <= min_expanse.data[i] && min_expanse.data[i] <=  other_box->min_expanse.data[i])
            )
           )
        {
            return false;
        }
    }
    return true;
    
}

BoundingBox::BoundingBox(RT_Vector min, RT_Vector max)
{
    min_expanse = min;
    max_expanse = max;
}

BoundingBox::BoundingBox()
{
    min_expanse = RT_Vector();
    max_expanse = RT_Vector();
}

BoundingBox::~BoundingBox()
{
}
