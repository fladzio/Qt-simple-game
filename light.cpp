#include "light.h"

Light::Light()
{
   this->x = 0;
   this->y = 0;
   this->z = 0;
   this->intensity = 0;
}

Light::Light(double x, double y, double z, double intensity)
{
   this->x = x;
   this->y = y;
   this->z = z;
   this->intensity = intensity;
}
