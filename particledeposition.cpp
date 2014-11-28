#include "particledeposition.h"
#include "terrainutil.h"

ParticleDeposition::ParticleDeposition(Terrain &terrain)
    :TerrainModeling(terrain)
    ,fallpoints(mterrain.getWidth(), mterrain.getHeight())
{
}

void ParticleDeposition::start()
{
    mstepindex = 0;
    mterrain.reset();
    fallpoints.load("fallpoints.txt");
}

void ParticleDeposition::step()
{
    mstepindex++;
    for(unsigned int i = 0;i < fallpoints.size();i++)
    {
        placeOneParticle(fallpoints.at(i));
    }
}

void ParticleDeposition::generate()
{
    for(int i = 0;i<10;i++)
    {
        step();
    }
}

bool ParticleDeposition::isfinished()
{
    return false;
}

void ParticleDeposition::querylowerpoints(const IntPoint &point0, int radius, std::vector<IntPoint> &points)
{
    double h0 = mterrain.at(point0);
    int x0 = point0.x, y0 = point0.y;
    for(int j = y0 - radius;j <= y0 + radius;j++)
    {
        for(int i = x0 - radius;i <= x0 + radius;i++)
        {
            IntPoint point(i, j);
            int distance = point0.distanceTo(point);
            if(distance <= radius)
                addlowerpoint(point, h0, points);
        }
    }
}

void ParticleDeposition::addlowerpoint(const IntPoint &point, double h0, std::vector<IntPoint> &points)
{
    if(mterrain.pointInTerrain(point))
    {
        if(h0 - mterrain.at(point) >= 1)
        {
            points.push_back(point);
        }
    }
}

void ParticleDeposition::placeOneParticle(const UIntPoint &position)
{
    IntPoint point0 = {position.x ,position.y};

    while(true)
    {
        std::vector<IntPoint> points;

        querylowerpoints(point0, random(1, 5), points);

        if(points.size() == 0)
        {
            break;
        }

        int k = random(0, points.size() - 1);

        point0 = points[k];
    }

    mterrain.at(point0) += 1;
}
