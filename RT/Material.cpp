#include "Material.h"

Material::Material(Color am, Color spec, Color diffuse, float refl)
{
    ambientCol = am;
    specularCol = spec;
    diffuseCol = diffuse;
    refl_percentage = refl;
}

Material::~Material()
{
}
