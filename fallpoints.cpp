#include "fallpoints.h"
#include <fstream>

FallPoints::FallPoints(unsigned int width, unsigned int height)
    :mwidth(width), mheight(height)
{
}

void FallPoints::insert(UIntPoint point)
{
    points.push_back(point);
}

void FallPoints::save(const char *filename)
{
    std::ofstream file;
    file.open(filename);
    for(auto point:points)
    {
        file<<point.x<<","<<point.y<<"\n";
    }
}

void FallPoints::load(const char *filename)
{
    points.clear();
    std::ifstream file;
    file.open(filename);
    while(!file.eof())
    {
        unsigned int x, y;
        char d;
        file>>x>>d>>y;
        if(x < mwidth && y < mheight)
        {
            points.push_back({x, y});
        }
    }
}
