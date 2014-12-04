#ifndef PD_DLA_H
#define PD_DLA_H

#include "terrainmodeling.h"
#include "particledeposition.h"
#include "fallpoints.h"

#include <vector>
#include "vector2.h"

class PD_DLA : public ParticleDeposition
{
public:
    PD_DLA(Terrain &terrain);
    void start() override;
    void step() override;

private:
    FallPoints fallpoints;
};

#endif // PD_DLA_H
