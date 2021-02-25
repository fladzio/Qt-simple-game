#ifndef TEXTURE_H
#define TEXTURE_H

#include <QImage>

class Texture
{
public:
    Texture() {};
    Texture(QImage *texture, int recursionLevel);
    QImage *texture;
    struct TrianglePoints
    {
      public:
        QPoint p1, p2, p3;
        TrianglePoints(QPoint p1, QPoint p2, QPoint p3)
        {
            this->p1 = p1;
            this->p2 = p2;
            this->p3 = p3;
        }
    };
    QVector<TrianglePoints> triangles;
};

#endif // TEXTURE_H
