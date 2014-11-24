#ifndef POINT_H
#define POINT_H

#include <math.h>

template<typename T>
struct Point
{
    Point(T x, T y)
        :x(x),y(y)
    {

    }

    T x{0};
    T y{0};

    double distanceTo(const Point<T> &point) const
    {
        return sqrt((x - point.x) * (x - point.x) + (y - point.y) * (y - point.y));
    }
};

typedef Point<int> IntPoint;
typedef Point<unsigned int> UIntPoint;

#endif // POINT_H
