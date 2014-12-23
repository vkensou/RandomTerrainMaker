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

    for (int i = 0; i < 10000; i++)
        placeOneParticle({mterrain.getWidth() / 2, mterrain.getHeight() / 2}, 3, 1);
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

    while(true)
    {
        std::vector<IntPoint> points;

        querylowerpoints(point0, radius, height, points);

        if(points.size() == 0)
        {
            break;
        }

        int k = getPointRandomly(points);

        point0 = points[k];
    }

    mterrain.at(point0) += 1;
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

void ParticleDeposition::queryNearbyPointsIf(const IntPoint &point0, int radius, std::vector<IntPoint> &points,
                                             std::function<void(const IntPoint &point, std::vector<IntPoint> &points)> func)
{
	assert(radius > 0);

	for (int i = 1; i <= radius; i++)
	{
		std::vector<Vector2<int>> &allpoints = getPointsDistanceIs(i);
		for (int j = 0; j < allpoints.size(); j++)
		{
			IntPoint point;
			if (mterrain.getExtension() != Terrain::NONE)
			{
				point = mterrain.extend({ point0.x + allpoints[j].x, point0.y + allpoints[j].y });
			}
			else
			{
				point = { point0.x + allpoints[j].x, point0.y + allpoints[j].y };
			}
			func(point, points);
		}
		if (points.size() > 0)return;
	}
}

void ParticleDeposition::querylowerpoints(const IntPoint &point0, int radius, int height, std::vector<IntPoint> &points)
{
    double h0 = mterrain.at(point0);
    auto func = [h0, height, this](const IntPoint &point, std::vector<IntPoint> &points)
    {
        if(mterrain.pointInSpace(point))
        {
            if(h0 - mterrain.at(point) >= height)
            {
                points.push_back(point);
            }
        }
    };

    queryNearbyPointsIf(point0, radius, points, func);
}

void ParticleDeposition::queryhigherpoints(const IntPoint &point0, int radius, int height, std::vector<IntPoint> &points)
{
    double h0 = mterrain.at(point0);
    auto func = [h0, height, this](const IntPoint &point, std::vector<IntPoint> &points)
    {
        if(mterrain.pointInSpace(point))
        {
            if(mterrain.at(point) - h0 > height)
            {
                points.push_back(point);
            }
        }
    };

    queryNearbyPointsIf(point0, radius, points, func);
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

