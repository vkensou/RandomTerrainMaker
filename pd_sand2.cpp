#include "pd_sand2.h"
#include "terrainutil.h"
#include <assert.h>
#include "ray.hpp"

PD_Sand2::PD_Sand2(Terrain &terrain)
	:ParticleDeposition(terrain)
	, mwinddirect(1, 0)
{
}

void PD_Sand2::start()
{
	mstepindex = 0;

	//mterrain.fill(0);
	//for (int i = 0; i < 1000; i++)
	//	placeOneParticle({ mterrain.getWidth() / 2, mterrain.getHeight() / 2 }, 2);

	std::function<void(unsigned int i, TerrainValue &value)> func = [](unsigned int, TerrainValue& v)
	{
		v = random(50, 51);
	};
	mterrain.for_each(func);

	setWindDirect({ 1, 0 });
	mwindpower = 10;
	deposit = mterrain.getSize();
	erosiveness = mterrain.getSize();
	
	deposit = 0;

	locks.reset(mterrain.getWidth(), mterrain.getHeight());
	locks.fill(false);

	mterrain.setExtension(Terrain::CYCLE);
}

void PD_Sand2::step()
{
	mstepindex++;
	sanddeposit();
	blowsand();
	sandflow();
}

void PD_Sand2::setWindDirect(Direct winddirect)
{
	assert(winddirect.x != 0 || winddirect.y != 0);
	mwinddirect = winddirect;
	mwinddirect.normalize();
}

void PD_Sand2::blowsand()
{
	for (int i = 0; i < erosiveness; i++)
	{
		blowsandstep();
	}
}

void PD_Sand2::blowsandstep()
{
	UIntPoint p0(random(0, mterrain.getWidth() - 1), random(0, mterrain.getHeight() - 1));
	if (!pointInWindwardSlope(p0))
	{
		return;
	}
	TerrainValue h0 = mterrain.at(p0);
	if (h0 == 0)
	{
		return;
	}
	mterrain.at(p0)--;
	h0--;
	auto windpower = random(0, mwindpower);
	windpower = mwindpower;
	Ray ray(Vector2<double>(p0.x, p0.y), mwinddirect);

	auto func = [&](int x, int y)
	{
		auto p1 = mterrain.extend({ x, y });
		TerrainValue h1 = mterrain.at(p1);
		if (h1 >= h0)
			windpower -= 1;
		//else if (h1 > h0)
		//	windpower -= 2;
		else if (h1 < h0)
			windpower -= 2;

		h0 = h1;
		if (windpower < 0)
		{
			placeOneParticle(UIntPoint(p1.x, p1.y), 2);
			return false;
		}
		return true;
	};

	ray.traverseLatticePoints(func, 0, false);
}

void PD_Sand2::sandflow()
{
	while (sandflowstep());
}

bool PD_Sand2::sandflowstep()
{
	bool r = false;
	std::function<void(unsigned int, unsigned int, TerrainValue &)> func = [this, &r](unsigned int x, unsigned int y, TerrainValue &)
	{
		if (locks.at(x, y) == false)
		{
			std::vector<IntPoint> points;

			queryhigherpoints(IntPoint(x, y), 2, points);

			auto iter = points.begin();
			for (; iter != points.end();)
			{
				if (locks.at(*iter) == true)
				{
					iter = points.erase(iter);
				}
				else
				{
					iter++;
				}
			}

			if (points.size() == 0)
			{
				return;
			}

			int k = random(0, points.size() - 1);
			IntPoint p = points[k];
			mterrain.at(p) -= 1;
			mterrain.at(x, y) += 1;
			locks.at(p) = true;
			locks.at(x, y) = true;
			r = true;
		}
	};

	mterrain.for_each(func);
	unlockall();
	return r;
}

void PD_Sand2::unlockall()
{
	locks.fill(false);
}

bool PD_Sand2::pointInWindwardSlope(const UIntPoint &point)
{
	return mterrain.getPointGradient(point, mwinddirect) > 0;
}

bool PD_Sand2::pointInLeewardSlope(const UIntPoint &point)
{
	return mterrain.getPointGradient(point, mwinddirect) < 0;
}

void PD_Sand2::sandblowOutofTerrain(const IntPoint &point)
{

}

void PD_Sand2::sanddeposit()
{
	for (int i = 0; i < deposit; i++)
	{
		sanddepositstep();
	}
}

void PD_Sand2::sanddepositstep()
{
	unsigned int x0 = random(0, mterrain.getWidth() - 1), y0 = random(0, mterrain.getHeight() - 1);
	if (pointInWindwardSlope({ x0, y0 }))
	{
		placeOneParticle({ x0, y0 }, 2);
	}
}