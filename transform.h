#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QGenericMatrix>
#include "point3d.h"

class Transform
{
public:
    Transform() {};
    Transform(double z);
    Point3D transformPoint(Point3D p);
    QGenericMatrix<4,4, double> translation, scale, rotationx, rotationy, rotationz, final;
};

#endif // TRANSFORM_H
