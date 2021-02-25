#ifndef POINT3D_H
#define POINT3D_H

#include <math.h>

class Point3D
{
public:
    Point3D();
    Point3D(double x, double y, double z);
    void normalize();

    double x;
    double y;
    double z;
};

#endif // POINT3D_H
