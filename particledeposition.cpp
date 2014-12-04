#include "particledeposition.h"
#include "terrainutil.h"

ParticleDeposition::ParticleDeposition(Terrain &terrain)
    :TerrainModeling(terrain)
{
}

void ParticleDeposition::start()
{
    mstepindex = 0;
    mterrain.fill(0);
}

void ParticleDeposition::step()
{
    mstepindex++;

    placeOneParticle({0, 0});

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

void ParticleDeposition::queryNearbyPointsIf(const IntPoint &point0, int radius, std::vector<IntPoint> &points,
                                             std::function<void(const IntPoint &point, std::vector<IntPoint> &points)> func)
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
                func(point, points);
        }
    }
}

void ParticleDeposition::querylowerpoints(const IntPoint &point0, int radius, std::vector<IntPoint> &points)
{
    double h0 = mterrain.at(point0);
    auto func = [h0, this](const IntPoint &point, std::vector<IntPoint> &points)
    {
        if(mterrain.pointInSpace(point))
        {
            if(h0 - mterrain.at(point) >= 1)
            {
                points.push_back(point);
            }
        }
    };

    queryNearbyPointsIf(point0, radius, points, func);
}

void ParticleDeposition::queryhigherpoints(const IntPoint &point0, int radius, std::vector<IntPoint> &points)
{
    double h0 = mterrain.at(point0);
    auto func = [h0, this](const IntPoint &point, std::vector<IntPoint> &points)
    {
        if(mterrain.pointInSpace(point))
        {
            if(mterrain.at(point) - h0 > 1)
            {
                points.push_back(point);
            }
        }
    };

    queryNearbyPointsIf(point0, radius, points, func);
}

