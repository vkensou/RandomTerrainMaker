#include "terrain.h"
#include <memory.h>
#include <assert.h>
#include "terrainutil.h"

Terrain::Terrain(unsigned int width, unsigned int height)
    :mwidth(width), mheight(height), mdata(nullptr)
{
    reset(width, height);
}

Terrain::~Terrain()
{
    if(mdata != nullptr)
    {
        delete mdata;
    }
}

void Terrain::reset()
{
    memset(mdata, 0, mwidth * mheight * sizeof(double));
}

void Terrain::reset(unsigned int width, unsigned int height)
{
    assert(canbeTerrainWidth(width));
    assert(canbeTerrainWidth(height));

    mwidth = width;
    mheight = height;
    if(mdata != nullptr)
    {
        delete mdata;
    }

    mdata = new double[mwidth * mheight];
    reset();
}
