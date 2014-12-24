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

//    for (int i = 0; i < 10000; i++)
//    {
//        placeOneParticle({mterrain.getWidth() / 2, mterrain.getHeight() / 4 * 1}, 2, 1);
//    }
//    for (int i = 0; i < 10000; i++)
//    {
//        placeOneParticle({mterrain.getWidth() / 2, mterrain.getHeight() / 4 * 2}, 1, 1);
//    }
//    for (int i = 0; i < 10000; i++)
//    {
//        placeOneParticle({mterrain.getWidth() / 2, mterrain.getHeight() / 4 * 3}, 1, 2);
//    }
    mterrain.setExtension(Terrain::CYCLE);
    for (int i = 0; i < 10000; i++)
    {
        placeOneParticle({0, 0}, 1, 1);
    }
}

void ParticleDeposition::step()
{
    mstepindex++;
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

void ParticleDeposition::placeOneParticle(const UIntPoint &position, int radius, int height)
{
    IntPoint point0(position.x ,position.y);

    std::vector<IntPoint> lastpoints;
    lastpoints.push_back(point0);

    std::vector<IntPoint> nextpoints;
    int r = 1;
    int h = mterrain.at(point0);
    for(;;)
    {
        nextpoints.clear();
        h -= height;
        querylowerpoints(point0, r, radius, h, nextpoints);
        if(nextpoints.size() == 0)
            break;
        r += radius;
        lastpoints.swap(nextpoints);
    }

    int k = random(0, lastpoints.size() - 1);
    mterrain.at(lastpoints[k])++;
}

void ParticleDeposition::placeOneParticle(const UIntPoint &position, const Radian &radian)
{
    assert(radian > 0 && radian < M_PI_2);
    int radius, height;
    if(radian == M_PI_4)
    {
        radius = 1;
        height = 1;
    }
    else if(radian < M_PI_4)
    {
        radius = randomIntegerFromReal(1 / tan(radian));
        height = 1;
    }
    else
    {
        radius = 1;
        height = randomIntegerFromReal(tan(radian));
    }
    placeOneParticle(position, radius, height);
}

void ParticleDeposition::queryNearbyPointsIf(const IntPoint &point0, int radius, int width, std::vector<IntPoint> &points,
                                             std::function<void(const IntPoint &point, std::vector<IntPoint> &points)> func)
{
	assert(radius > 0);
    for(int i = 0; i < width; i++)
    {
        std::vector<Vector2<int>> &allpoints = getPointsDistanceIs(radius + i);
        for (int j = 0; j < allpoints.size(); j++)
        {
            IntPoint point = { point0.x + allpoints[j].x, point0.y + allpoints[j].y };
            if (mterrain.getExtension() != Terrain::NONE)
            {
                point = mterrain.extend(point);
            }
            func(point, points);
        }
        if(points.size() > 0)return;
    }
}

void ParticleDeposition::querylowerpoints(const IntPoint &point0, int radius, int width, int height, std::vector<IntPoint> &points)
{
    auto func = [&](const IntPoint &point, std::vector<IntPoint> &points)
    {
        if(mterrain.pointInSpace(point) && mterrain.at(point) <= height)
        {
            points.push_back(point);
        }
    };

    queryNearbyPointsIf(point0, radius, width, points, func);
}

void ParticleDeposition::queryhigherpoints(const IntPoint &point0, int radius, int width, int height, std::vector<IntPoint> &points)
{
    double h0 = mterrain.at(point0);
    auto func = [&](const IntPoint &point, std::vector<IntPoint> &points)
    {
        if(mterrain.pointInSpace(point))
        {
            if(mterrain.at(point) - h0 > height)
            {
                points.push_back(point);
            }
        }
    };

    queryNearbyPointsIf(point0, radius, width, points, func);
}

int ParticleDeposition::getPointRandomly(std::vector<IntPoint> &points)
{
    std::vector<double> distances;
    std::vector<double> weights;
    double sum = 0;
    for(unsigned int i = 0; i < points.size(); i++)
    {
        distances.push_back(sqrt(points[i].x * points[i].x + points[i].y * points[i].y));
        sum += distances[i];
        weights.push_back(sum);
    }

    double r = random(sum);
    int i = 0;
    for(i = 0; i < points.size(); i++)
    {
        if(r <= weights[i])
            break;
    }
    return i;
}

