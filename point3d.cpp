#include "point3d.h"

Point3D::Point3D()
{
   this->x = 0;
   this->y = 0;
   this->z = 0;
}

Point3D::Point3D(double x, double y, double z)
{
   this->x = x;
   this->y = y;
   this->z = z;
}

void Point3D::normalize()
{
    double length = sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
    this->x = this->x / length;  this->y = this->y / length;  this->z = this->z / length;
}
