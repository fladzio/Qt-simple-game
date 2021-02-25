#ifndef LIGHT_H
#define LIGHT_H


class Light
{
public:
    Light();
    Light(double x, double y, double z, double intensity);

    double x;
    double y;
    double z;
    double intensity;
};

#endif // LIGHT_H
