#include "terrain.h"
#include <memory.h>
#include <assert.h>
#include "terrainutil.h"

Terrain::Terrain(unsigned int width, unsigned int height)
    :ElementSpace2D(width, height)
{
}

void Terrain::reset(unsigned int width, unsigned int height)
{
    assert(canbeTerrainWidth(width));
    assert(canbeTerrainWidth(height));

    ElementSpace2D::reset(width, height);
}

