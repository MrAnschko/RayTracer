#ifndef MATERIAL_H
#define MATERIAL_H

#include "Image.h"

class Material
{
    public:
        Color ambientCol;
        Color specularCol;
        Color diffuseCol;
        float refl_percentage;

        Material( Color am , Color spec, Color diffuse, float refl);
        ~Material();

};

#endif