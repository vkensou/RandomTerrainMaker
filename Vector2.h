#ifndef POINT_H
#define POINT_H

#include <math.h>

template<typename T>
struct Vector2
{
    Vector2(T x, T y)
        :x(x),y(y)
    {
    }

    T x{0};
    T y{0};

    double distanceTo(const Vector2<T> &point) const;

    void normalize();
};

template<typename T>
double Vector2<T>::distanceTo(const Vector2<T> &point) const
{
    return sqrt((x - point.x) * (x - point.x) + (y - point.y) * (y - point.y));
}

template<typename T>
void Vector2<T>::normalize()
{
    if(x != 0 || y != 0)
    {
        double length = sqrt(x * x + y * y);
        x/=length;
        y/=length;
    }
}

typedef Vector2<int> IntPoint;
typedef Vector2<unsigned int> UIntPoint;

#endif // POINT_H
