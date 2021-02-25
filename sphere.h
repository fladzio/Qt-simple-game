#ifndef SPHERE_H
#define SPHERE_H
#include "point3d.h"
#include "math.h"
#include "draw.h"
#include "transform.h"
#include "light.h"
#include <map>
#include <QDebug>
#include <QVector>

class Sphere
{
public:
    int positionX, positionY;
    Sphere() {};
    Sphere(int positionX, int positionY);
    struct TriangleIndices
    {
      public:
        int v1, v2, v3;
        TriangleIndices(int v1, int v2, int v3)
        {
            this->v1 = v1;
            this->v2 = v2;
            this->v3 = v3;
        }
    };
    QVector<Point3D> vertices;
    QVector<TriangleIndices> faces;
    std::map<int64_t, int> middlePointIndexCache;
    int index;

    int addVertex(Point3D p);
    int getMiddlePoint(int p1, int p2);
    void draw(QImage *img, Texture *texture, int f_ogniskowa, Point3D POV, Light light, Transform transform, bool drawEdges, bool drawTextures);
    void create(int recursionLevel);

};

#endif // SPHERE_H
