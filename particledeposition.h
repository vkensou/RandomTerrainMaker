#ifndef PARTICLEDEPOSITION_H
#define PARTICLEDEPOSITION_H

#include "terrainmodeling.h"
#include "fallpoints.h"

#include <vector>
#include "point.h"

class ParticleDeposition : public TerrainModeling
{
public:
    ParticleDeposition(Terrain &terrain);
    void start() override;
    void step() override;
    void generate() override;
    bool isfinished() override;

private:
    FallPoints fallpoints;
    void querylowerpoints(const IntPoint &point0, unsigned int radius, std::vector<IntPoint> &points);
    void addlowerpoint(const IntPoint &point, double h0, std::vector<IntPoint> &points);
};

#endif // PARTICLEDEPOSITION_H
