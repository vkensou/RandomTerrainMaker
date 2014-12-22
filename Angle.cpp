#include "angle.h"
#include <cmath>

Degree::Degree()
    :degree(0)
{

}

Degree::Degree(double d)
    :degree(d)
{

}

Degree::Degree(const Radian &r)
    :degree(r * 180 / M_PI)
{
}

Radian::Radian()
    :radian(0)
{

}

Radian::Radian(double r)
    :radian(r)
{

}

Radian::Radian(const Degree &d)
    :radian(d * M_PI / 180)
{

}
