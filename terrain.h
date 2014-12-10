#ifndef TERRAIN_H
#define TERRAIN_H

#include "elementspace2d.h"

typedef double TerrainValue;
class Direct;

class Terrain
        :public ElementSpace2D<TerrainValue>
{
public:
    Terrain(unsigned int width, unsigned int height);

    void reset(unsigned int width, unsigned int height) override;

    int getPointGradient(const UIntPoint &point, const Direct &direct);
};

#endif // TERRAIN_H
