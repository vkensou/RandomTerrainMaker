#ifndef TERRAINMODELING_H
#define TERRAINMODELING_H
#include "terrain.h"

class TerrainModeling
{
public:
    TerrainModeling(Terrain &terrain);
    virtual ~TerrainModeling(){}

    virtual void start() = 0;
    virtual void step() = 0;
    virtual void generate();
    virtual bool isfinished(){return mstepindex >= mstepcount;}
    void erosion();

protected:
    Terrain &mterrain;
    unsigned int mstepcount;
    unsigned int mstepindex;
};

#endif // TERRAINMODELING_H
