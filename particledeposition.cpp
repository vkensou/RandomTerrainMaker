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

	for (int i = 0; i < 100; i++)
		placeOneParticle({mterrain.getWidth() / 4, mterrain.getHeight() / 2}, 1);
	for (int i = 0; i < 100; i++)
		placeOneParticle({ mterrain.getWidth() / 2, mterrain.getHeight() / 2 }, 2);
	for (int i = 0; i < 100; i++)
		placeOneParticle({ mterrain.getWidth() * 3 / 4, mterrain.getHeight() / 2 }, 3);

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

void ParticleDeposition::placeOneParticle(const UIntPoint &position, int radius)
{
    IntPoint point0(position.x ,position.y);

    while(true)
    {
        std::vector<IntPoint> points;

		querylowerpoints(point0, radius, points);

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

