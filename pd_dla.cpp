#include "PD_DLA.h"
#include "terrainutil.h"

PD_DLA::PD_DLA(Terrain &terrain)
    :ParticleDeposition(terrain)
    ,fallpoints(mterrain.getWidth(), mterrain.getHeight())
{
}

void PD_DLA::start()
{
    mstepindex = 0;
    mterrain.clearup();
    fallpoints.load("fallpoints.txt");
}

void PD_DLA::step()
{
    mstepindex++;
    for(unsigned int i = 0;i < fallpoints.size();i++)
    {
        placeOneParticle(fallpoints.at(i));
    }
}
