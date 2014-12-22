#include "pd_sand2.h"
#include "terrainutil.h"
#include <assert.h>
#include "ray.hpp"
#include <qimage.h>
#include <log4cpp/Category.hh>

log4cpp::Category & logg2 =log4cpp::Category::getRoot().getInstance("TerrainMaker2D");

PD_Sand2::PD_Sand2(Terrain &terrain)
	:ParticleDeposition(terrain)
    ,mwind(Direct(1, 0), 10, 1)
{
}

void PD_Sand2::start()
{
	mstepindex = 0;

    mterrain.setExtension(Terrain::CYCLE);

    mterrain.fill(0);

//    auto = [](TerrainValue &v)
//    {
//        v = random(50, 51);
//    };
//    mterrain.for_each(func);

    for (int i = 0; i < 1000; i++)
        placeOneParticle({ mterrain.getWidth() / 2, mterrain.getHeight() / 2 }, Degree(random(8, 20)));

	//when mwindpower == 20, will generates ripples
	locks.reset(mterrain.getWidth(), mterrain.getHeight());
	locks.fill(false);

    angleofWindward = Degree(20);
    angleofLeeward = Degree(34);
}

void PD_Sand2::step()
{
    mstepindex++;
    logg2.debug("step %d:", mstepindex);
    sanddeposit();
    blowsand();
    sandflow();
    mwind.setSediment(mwind.getSediment() * 0.9);
    int d = mwind.getDepositRate() * mterrain.getSize();
//    if(d == 0)mwind.setSediment(1);
//    if(mstepindex % 200 == 0)mwind.setSediment(1);
    logg2.debug("%lf", mwind.getSediment());
    logg2.debug("%d", d);
}

void PD_Sand2::blowsand()
{
    for (int i = 0; i < mwind.getErosionRate() * mterrain.getSize(); i++)
	{
		blowsandstep();
	}
}

void PD_Sand2::blowsandstep()
{
	UIntPoint p0(random(0, mterrain.getWidth() - 1), random(0, mterrain.getHeight() - 1));

    if(!pointCanBeEroded(p0))
        return;

    mterrain.at(p0)--;
    TerrainValue h0 = mterrain.at(p0);

    double windpower = random(0, mwind.getPower());
	//windpower = mwindpower;
    Vector2<double> startpoint = {p0.x, p0.y};
    Ray ray(startpoint, mwind.getDirect());

	auto func = [&](int x, int y)
	{
		auto p1 = mterrain.extend({ x, y });
		TerrainValue h1 = mterrain.at(p1);

        if (h1 >= h0)
            windpower -= 1;
//		//else if (h1 > h0)
//		//	windpower -= 2;
        else if (h1 < h0)
            windpower -= 2;

		h0 = h1;
		if (windpower < 0)
		{
//            mterrain.at(p1)++;
            placeOneParticle(UIntPoint(p1.x, p1.y), Degree(random(28, 34)));
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

            queryUnlockedHigherPoints(IntPoint(x, y), Radian(Degree(random(8, 20))), points);

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

bool PD_Sand2::pointCanBeEroded(const UIntPoint &point)
{
    auto g = mterrain.getPointGradient(point, mwind.getDirect());
    if(g == 1 || g == 2 || g == 3)
        return true;
    else
        return false;
}

bool PD_Sand2::pointIsSettleable(const UIntPoint &point)
{
    auto g = mterrain.getPointGradient(point, mwind.getDirect());
    if(g == 1 || g == 2 || g == 4)
        return true;
    else
        return false;
}

void PD_Sand2::queryUnlockedHigherPoints(const IntPoint &point0, int radius, std::vector<IntPoint> &points)
{
    double h0 = mterrain.at(point0);
    auto func = [h0, this](const IntPoint &point, std::vector<IntPoint> &points)
    {
        if(mterrain.pointInSpace(point))
        {
            if(locks.at(point) == false && mterrain.at(point) - h0 > 1)
            {
                points.push_back(point);
            }
        }
    };

    queryNearbyPointsIf(point0, radius, points, func);
}

void PD_Sand2::queryUnlockedHigherPoints(const IntPoint &point0, const Radian &radian, std::vector<IntPoint> &points)
{
    int radius = randomIntegerFromReal(1 / tan(radian));
    queryUnlockedHigherPoints(point0, radius, points);
}

void PD_Sand2::sandblowOutofTerrain(const IntPoint &point)
{

}

void PD_Sand2::sanddeposit()
{
    for (int i = 0; i < mwind.getDepositRate() * mterrain.getSize(); i++)
	{
		sanddepositstep();
	}
}

void PD_Sand2::sanddepositstep()
{
	unsigned int x0 = random(0, mterrain.getWidth() - 1), y0 = random(0, mterrain.getHeight() - 1);
    if (pointIsSettleable({ x0, y0 }))
    {
        placeOneParticle({ x0, y0 }, Degree(random(8, 20)));
    }
}

void PD_Sand2::test()
{
    int testid = 26;
	QString fmt("s%1_s%2.bmp");
	QString filename;
	start();
	filename = fmt.arg(testid).arg(0);
	savetobmp(filename);
    for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			step();
		}
		filename = fmt.arg(testid).arg((i + 1) * 10);
		savetobmp(filename);
	}
}

void PD_Sand2::savetobmp(const QString &filename)
{
	QImage gi("grey.bmp", "bmp");
	QImage testimage;
	testimage = gi.scaled(mterrain.getWidth(), mterrain.getHeight());
	testimage.fill(0);

	for (unsigned int j = 0; j < mterrain.getHeight(); j++)
	{
		for (unsigned int i = 0; i < mterrain.getWidth(); i++)
		{
			int v = mterrain.at(i, j);
            if (v > 0 && v % 3 == 1)
				testimage.setPixel(i, j, 255);
		}
	}

	testimage.save(filename);
}
