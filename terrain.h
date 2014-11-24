#ifndef TERRAIN_H
#define TERRAIN_H
#include "point.h"

class Terrain
{
public:
    Terrain(unsigned int width, unsigned int height);
    ~Terrain();

    unsigned int getWidth(){return mwidth;}
    unsigned int getHeight(){return mheight;}
    double * getData(){return mdata;}
    void reset();
    void reset(unsigned int width, unsigned int height);

    double &at(unsigned int x, unsigned int y)
    {
        return mdata[y * mheight + x];
    }
    double &at(const IntPoint &point){return at(point.x, point.y);}
    double &at(const UIntPoint &point){return at(point.x, point.y);}

    bool pointInTerrain(int x, int y)
    {
        return (x >= 0 && x < mwidth && y >= 0 && y < mheight);
    }    
    bool pointInTerrain(const IntPoint &point){return pointInTerrain(point.x, point.y);}
    bool pointInTerrain(const UIntPoint &point){return pointInTerrain(point.x, point.y);}
private:
    unsigned int mwidth;
    unsigned int mheight;
    double *mdata;
};

#endif // TERRAIN_H
