#include "draw.h"

Draw::Draw()
{
}

void Draw::project(Point3D p1, Point3D p2, QImage *img, int f_ogniskowa, Point3D POV, int positionX, int positionY)
{
    int x1, x2, y1, y2;
    if(p1.z > POV.z && p2.z > POV.z)
    {
        x1 = p1.x * f_ogniskowa / p1.z + positionX;
        y1 = p1.y * f_ogniskowa / p1.z + positionY;
        x2 = p2.x * f_ogniskowa / p2.z + positionX;
        y2 = p2.y * f_ogniskowa / p2.z + positionY;
        drawLine(QPoint(x1, y1), QPoint(x2, y2), img, QColor(255,255,255));
    }
}

QPoint Draw::getProjectPixel(Point3D p, int f_ogniskowa, Point3D POV, int positionX, int positionY)
{
    int x, y;
    if(p.z > POV.z)
    {
        x = p.x * f_ogniskowa / p.z + positionX;
        y = p.y * f_ogniskowa / p.z + positionY;
        return QPoint(x, y);
    }
}

void Draw::insertPixel(QPoint p, QImage *img, QColor c)
{
    int img_height = img->size().height();
    int img_width = img->size().width();
    if (p.x() >= 0 && p.x() < img_width && p.y() >= 0 && p.y() < img_height) {
        unsigned char* ptr;
        ptr = img->bits();
        ptr[img_width * 4 * p.y() + 4 * p.x()] = c.blue();
        ptr[img_width * 4 * p.y() + 4 * p.x() + 1] = c.green();
        ptr[img_width * 4 * p.y() + 4 * p.x() + 2] = c.red();
    }
}

void Draw::drawLine(QPoint p1, QPoint p2, QImage *img, QColor c)
{
    //Bresenham's line algorithm
    int dx =  abs(p2.x() - p1.x());
    int sx = p1.x() < p2.x() ? 1 : -1;
    int dy = -abs(p2.y() - p1.y());
    int sy = p1.y() < p2.y() ? 1 : -1;
    int err = dx + dy;
    int err2;

    int x1 = p1.x(); int y1 = p1.y();
    int x2 = p2.x(); int y2 = p2.y();

    while (true)
    {
        insertPixel(QPoint(x1, y1), img, c);
        if (x1 == x2 && y1 == y2)
            break;

        err2 = 2*err;
        if (err2 >= dy)
        {
            err += dy;
            x1 += sx;
        }
        if (err2 <= dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void Draw::imgClear(QImage *img)
{
    unsigned char* ptr;
    ptr = img->bits();
    int img_height = img->size().height();
    int img_width = img->size().width();

    for (int i = 0; i < img_height; i++) {
        for (int j = 0; j < img_width; j++) {
            ptr[img_width * 4 * i + 4 * j] = 0;
            ptr[img_width * 4 * i + 4 * j + 1] = 0;
            ptr[img_width * 4 * i + 4 * j + 2] = 0;
        }
    }
}

QColor Draw::checkPixel(QImage *img, double px, double py)
{
    int x = px;
    int y = py;

    int img_width = img->size().width();
    int img_height = img->size().height();

    if (x >= 0 && x < img_width - 1 && y >= 0 && y < img_height - 1) {
        unsigned char* ptr;
        ptr = img->bits();
        int r, g, b;

            r = ptr[img_width*4*y + 4*x + 2];
            g = ptr[img_width*4*y + 4*x + 1];
            b = ptr[img_width*4*y + 4*x];

        return QColor(r, g, b);
    }

    return nullptr;
}

void Draw::texture(QImage *img, Texture *texture, QVector<QPoint> points, double lightValue, int triangleIndex)
{
    double W = (points[1].x() - points[0].x()) * (points[2].y() - points[0].y()) - (points[1].y() - points[0].y()) * (points[2].x() - points[0].x());

/* SCAN LINE */
    int y_min = img->size().height();
    int y_max = 0;

    for(int i = 0; i < 3; i++)
    {
        if(points[i].y() < y_min) y_min = points[i].y();
        if(points[i].y() > y_max) y_max = points[i].y();
    }

    for(int y = y_min; y <= y_max; y++)
    {
        QVector <int> x_tab;
        for(int p = 0; p < 3; p++)
        {
            int next;
            if(p == 2)
                next = 0;
            else
                next = p + 1;

            if((points[p].y() > y && points[next].y() <= y) || (points[p].y() <= y && points[next].y() > y))
            {
                if(points[next].y() - points[p].y() != 0)
                {
                    int x = points[p].x() + (y - points[p].y()) * (double)(((double)points[next].x() - (double)points[p].x()) / ((double)points[next].y() - (double)points[p].y()));
                    x_tab.push_back(x);
                }
            }
        }

        std::sort(x_tab.begin(), x_tab.end());

        if(x_tab.size() > 1)
        {
            for(int x = x_tab[0] + 1; x <= x_tab[1]; x++)
            {
                    double v, w, u;
                    v = ((x - points[0].x()) * (points[2].y() - points[0].y()) - (y - points[0].y()) * (points[2].x() - points[0].x())) / W;
                    w = ((points[1].x() - points[0].x()) * (y - points[0].y()) - (points[1].y() - points[0].y()) * (x - points[0].x())) / W;
                    u = 1.0 - v - w;
                    double ptx = u * texture->triangles[triangleIndex].p1.x() + v * texture->triangles[triangleIndex].p2.x() + w * texture->triangles[triangleIndex].p3.x();
                    double pty = u * texture->triangles[triangleIndex].p1.y() + v * texture->triangles[triangleIndex].p2.y() + w * texture->triangles[triangleIndex].p3.y();

                    QColor c = checkPixel(texture->texture, ptx, pty);
                    int r = c.red() * lightValue;
                    int g = c.green() * lightValue;
                    int b = c.blue() * lightValue;
                    c.setRgb(r, g, b);

                    if (c != nullptr)
                        insertPixel(QPoint(x, y), img, c);
            }
        }
    }
}
