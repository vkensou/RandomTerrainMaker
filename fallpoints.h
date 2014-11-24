#ifndef FALLPOINTS_H
#define FALLPOINTS_H
#include <vector>
#include "point.h"

class FallPoints
{
public:
    FallPoints(unsigned int width, unsigned int height);

    void insert(UIntPoint point);
    void save(const char *filename);
    void load(const char *filename);
    std::size_t size(){return points.size();}
    UIntPoint &at(unsigned int index){return points[index];}
//private:
    typedef std::vector<UIntPoint> PointList;
    PointList points;
private:
    unsigned int mwidth, mheight;
};

#endif // FALLPOINTS_H
