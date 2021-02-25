#include "sphere.h"

Sphere::Sphere(int positionX, int positionY)
{
    this->positionX = positionX;
    this->positionY = positionY;
}

// dodanie wierzchołka do siatki, ustalenie pozycji na kuli
int Sphere::addVertex(Point3D p)
{
    double length = sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
    vertices.push_back(Point3D(p.x/length, p.y/length, p.z/length));
    return index++;
}

// zwraca indeks punktu w środku p1 i p2
int Sphere::getMiddlePoint(int p1, int p2)
{
    // sprawdzamy czy juz mamy
    bool firstIsSmaller = p1 < p2;
    int64_t smallerIndex = firstIsSmaller ? p1 : p2;
    int64_t greaterIndex = firstIsSmaller ? p2 : p1;
    int64_t key = (smallerIndex << 32) + greaterIndex;

    if (this->middlePointIndexCache.find(key)->first == key)
        return this->middlePointIndexCache.find(key)->second;

    Point3D point1 = this->vertices[p1];
    Point3D point2 = this->vertices[p2];
    Point3D middle = Point3D(
        (point1.x + point2.x) / 2.0,
        (point1.y + point2.y) / 2.0,
        (point1.z + point2.z) / 2.0);

    // dodanie wierzchołka
    int i = addVertex(middle);
    this->middlePointIndexCache.insert(std::pair<int64_t, int>(key, i));
    return i;
}

void Sphere::create(int recursionLevel)
{
    this->middlePointIndexCache.clear();
    this->index = 0;

    // stworzenie 12 wierzcholkow dzwudziestoscianu
    double t = (1.0 + sqrt(5.0)) / 2.0;

    addVertex(Point3D(-1,  t,  0));
    addVertex(Point3D( 1,  t,  0));
    addVertex(Point3D(-1, -t , 0));
    addVertex(Point3D( 1, -t,  0));

    addVertex(Point3D( 0, -1,  t));
    addVertex(Point3D( 0,  1,  t));
    addVertex(Point3D( 0, -1, -t));
    addVertex(Point3D( 0,  1, -t));

    addVertex(Point3D( t,  0, -1));
    addVertex(Point3D( t,  0,  1));
    addVertex(Point3D(-t,  0, -1));
    addVertex(Point3D(-t,  0,  1));

    // stworzenie 20 scian
    faces.push_back(TriangleIndices(0, 11, 5));
    faces.push_back(TriangleIndices(0, 5, 1));
    faces.push_back(TriangleIndices(0, 1, 7));
    faces.push_back(TriangleIndices(0, 7, 10));
    faces.push_back(TriangleIndices(0, 10, 11));

    faces.push_back(TriangleIndices(1, 5, 9));
    faces.push_back(TriangleIndices(5, 11, 4));
    faces.push_back(TriangleIndices(11, 10, 2));
    faces.push_back(TriangleIndices(10, 7, 6));
    faces.push_back(TriangleIndices(7, 1, 8));

    faces.push_back(TriangleIndices(3, 9, 4));
    faces.push_back(TriangleIndices(3, 4, 2));
    faces.push_back(TriangleIndices(3, 2, 6));
    faces.push_back(TriangleIndices(3, 6, 8));
    faces.push_back(TriangleIndices(3, 8, 9));

    faces.push_back(TriangleIndices(4, 9, 5));
    faces.push_back(TriangleIndices(2, 4, 11));
    faces.push_back(TriangleIndices(6, 2, 10));
    faces.push_back(TriangleIndices(8, 6, 7));
    faces.push_back(TriangleIndices(9, 8, 1));

    // podzial
    for (int i = 0; i < recursionLevel; i++)
    {
        QVector<TriangleIndices> faces2;
        for (auto &tri : faces)
        {
            // zamiana trojkatnej sciany na 4 trojkaty
            int a = getMiddlePoint(tri.v1, tri.v2);
            int b = getMiddlePoint(tri.v2, tri.v3);
            int c = getMiddlePoint(tri.v3, tri.v1);

            faces2.push_back(TriangleIndices(tri.v1, a, c));
            faces2.push_back(TriangleIndices(tri.v2, b, a));
            faces2.push_back(TriangleIndices(tri.v3, c, b));
            faces2.push_back(TriangleIndices(a, b, c));
        }
        faces = faces2;
    }
}

void Sphere::draw(QImage *img, Texture *texture, int f_ogniskowa, Point3D POV, Light light, Transform transform, bool drawEdges, bool drawTextures)
{
    QVector<Point3D> newVertices;
    Draw draw;

    for(auto &vertex : vertices)
        newVertices.push_back(transform.transformPoint(vertex));

    for(int i = 0; i < faces.size(); i++)
    {
        Point3D p1 = newVertices[faces[i].v1];
        Point3D p2 = newVertices[faces[i].v2];
        Point3D p3 = newVertices[faces[i].v3];

        Point3D w1, w2, wp, w1_w2_normal, w_light, w_light_mirror;

        // wektor od wierzcholka do obserwatora
        wp.x = POV.x - p1.x;
        wp.y = POV.y - p1.y;
        wp.z = POV.z - p1.z;
        wp.normalize();

        // wektor od wierzcholka do swiatła
        w_light.x = light.x - p1.x;
        w_light.y = light.y - p1.y;
        w_light.z = light.z - p1.z;
        w_light.normalize();

        // wektor od wierzcholka do wierzchokla obok
        w1.x = p2.x - p1.x;
        w1.y = p2.y - p1.y;
        w1.z = p2.z - p1.z;

        // wektor od wierzcholka do 2 wierzcholka obok
        w2.x = p3.x - p1.x;
        w2.y = p3.y - p1.y;
        w2.z = p3.z - p1.z;

        // wyznaczenie wektora normalnego(rownoleglego "na zewnatrz" bryly)
        w1_w2_normal.x = w1.y * w2.z - w2.y * w1.z;
        w1_w2_normal.y = w2.x * w1.z - w1.x * w2.z;
        w1_w2_normal.z = w1.x * w2.y - w2.x * w1.y;
        w1_w2_normal.normalize();

        // wektor lustrzany do wektora swiatła
        /*double scalar = w1_w2_normal.x * w_light.x + w1_w2_normal.y * w_light.y + w1_w2_normal.z * w_light.z;
        w_light_mirror.x = 2 * scalar * w1_w2_normal.x - w_light.x;
        w_light_mirror.y = 2 * scalar * w1_w2_normal.y - w_light.y;
        w_light_mirror.z = 2 * scalar * w1_w2_normal.z - w_light.z;
        w_light_mirror.normalize();*/

        // "wartosc" oswietlenia
        double lightValue = ((w_light.x * w1_w2_normal.x + w_light.y * w1_w2_normal.y + w_light.z * w1_w2_normal.z) * light.intensity);
        if(lightValue < 0)
            lightValue = 0;

        // jesli > 0 -> kat ostry -> widzimy sciane
        if((w1_w2_normal.x * wp.x + w1_w2_normal.y * wp.y + w1_w2_normal.z * wp.z) > 0)
        {
            // teskturowanie + oswietlenie
            if(drawTextures)
            {
                QVector<QPoint> points;
                QPoint qp1 = draw.getProjectPixel(p1, f_ogniskowa, POV, positionX, positionY); points.push_back(qp1);
                QPoint qp2 = draw.getProjectPixel(p2, f_ogniskowa, POV, positionX, positionY); points.push_back(qp2);
                QPoint qp3 = draw.getProjectPixel(p3, f_ogniskowa, POV, positionX, positionY); points.push_back(qp3);
                int k = i;
                if(i >= 32)
                    k = i % 32;
                draw.texture(img, texture, points, lightValue, k);
            }

            // krawedzie
            if(drawEdges)
            {
                draw.project(p1, p2, img, f_ogniskowa, POV, positionX, positionY);
                draw.project(p1, p3, img, f_ogniskowa, POV, positionX, positionY);
                draw.project(p2, p3, img, f_ogniskowa, POV, positionX, positionY);
            }
        }
    }
}
