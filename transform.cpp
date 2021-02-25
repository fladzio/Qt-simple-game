#include "transform.h"

Transform::Transform(double z)
{
    this->translation.setToIdentity();
    this->scale.setToIdentity();
    this->rotationx.setToIdentity();
    this->rotationy.setToIdentity();
    this->rotationz.setToIdentity();
    this->translation(2,3) = z;
}

Point3D Transform::transformPoint(Point3D p)
{
    Point3D ret;
    final = translation * rotationz * rotationy * rotationx * scale;
    ret.x = final(0,0) * p.x + final(0,1) * p.y + final(0,2) * p.z + final(0,3);
    ret.y = final(1,0) * p.x + final(1,1) * p.y + final(1,2) * p.z + final(1,3);
    ret.z = final(2,0) * p.x + final(2,1) * p.y + final(2,2) * p.z + final(2,3);

    return ret;
}
