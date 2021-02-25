#ifndef DRAW_H
#define DRAW_H

#include <point3d.h>
#include <texture.h>
#include <QDebug>
#include <QPoint>
#include <QColor>
#include <QImage>

class Draw
{
public:
    Draw();
    QColor checkPixel(QImage *img, double px, double py);
    QPoint getProjectPixel(Point3D p1, int f_ogniskowa, Point3D POV, int positionX, int positionY);
    void project(Point3D p1, Point3D p2, QImage *img, int f_ogniskowa, Point3D POV, int positionX, int positionY);
    void drawLine(QPoint p1, QPoint p2, QImage *img, QColor c);
    void insertPixel(QPoint p, QImage *img, QColor c);
    void imgClear(QImage *img);
    void texture(QImage *img, Texture *texture, QVector<QPoint> points, double lightValue, int triangleIndex);
};

#endif // DRAW_H
