#ifndef TERRAINUTIL_H
#define TERRAINUTIL_H
#include <QTime>

static bool is2N(unsigned int num)
{
    if(num == 0)
    {
        return false;
    }
    else
    {
        return (num & (num-1)) == 0;
    }
}

//the terrain's width or height must = 2 ^ n + 1
inline bool canbeTerrainWidth(unsigned int width)
{
    return (width >= 2) && is2N(width-1);
}

//return [0,1]
inline double random()
{
    return qrand() / (double)RAND_MAX;
}

//return [0,n]
inline double random(double n)
{
    return qrand() / (double)RAND_MAX * n;
}

//return [a,b]
inline double random(double a, double b)
{
    return (qrand() / (double)RAND_MAX) * (b - a) + a;
}

//return [a,b]
inline int random(int a, int b)
{
    return (int)(qrand() / (RAND_MAX + 1.0) * (b - a + 1)) + a;
}

//return [0,n]
inline int random(int n)
{
    return qrand() / (RAND_MAX + 1.0) * (n + 1);
}

template<typename T>
inline void setNum(T &n, T num)
{
    if(num > 255)
        n = 255;
    else
        n = num;
}

template<typename T>
inline void addNum(T &n, T num)
{
    setNum(n, n+num);
}

inline int roundnum(int a, int w)
{
    a %= w;
    if(a<0)a+=w;
    return a;
}

inline int clipnum(int a, int w)
{
    int nr = (w - 1) * 2;
    int b = roundnum(a, nr);
    if(b >= w)
        return nr - b;
    else
        return b;
}
#endif // TERRAINUTIL_H
