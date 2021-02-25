#include "texture.h"

Texture::Texture(QImage *texture, int recursionLevel)
{
    this->texture = texture;

    int off = 2;
    int len = texture->width() - off;

    triangles.push_back(TrianglePoints(QPoint(off, len), QPoint(off, off), QPoint(len, len)));
    triangles.push_back(TrianglePoints(QPoint(len, off), QPoint(off, off), QPoint(len, len)));

    // podzial
    for (int i = 0; i < recursionLevel; i++)
    {
        QVector<TrianglePoints> triangles2;
        for (auto &tri : triangles)
        {
            // zamiana trojkatnej sciany na 4 trojkaty
            QPoint a = QPoint((tri.p1.x() + tri.p2.x()) / 2, (tri.p1.y() + tri.p2.y()) / 2);
            QPoint b = QPoint((tri.p2.x() + tri.p3.x()) / 2, (tri.p2.y() + tri.p3.y()) / 2);
            QPoint c = QPoint((tri.p3.x() + tri.p1.x()) / 2, (tri.p3.y() + tri.p1.y()) / 2);

            triangles2.push_back(TrianglePoints(tri.p1, a, c));
            triangles2.push_back(TrianglePoints(tri.p2, b, a));
            triangles2.push_back(TrianglePoints(tri.p3, c, b));
            triangles2.push_back(TrianglePoints(a, b, c));
        }
        triangles = triangles2;
    }
}
