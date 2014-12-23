#include "pd_sand2.h"
#include "terrainutil.h"
#include <assert.h>
#include "ray.hpp"
#include <qimage.h>
#include <log4cpp/Category.hh>

log4cpp::Category & logg2 =log4cpp::Category::getRoot().getInstance("TerrainMaker2D");

PD_Sand2::PD_Sand2(Terrain &terrain)
	:ParticleDeposition(terrain)
    ,mwind(Direct(1, 0), 15, 1)
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
        placeOneParticle({ mterrain.getWidth() / 2, mterrain.getHeight() / 2 }, Degree(35));

	//when mwindpower == 20, will generates ripples
	locks.reset(mterrain.getWidth(), mterrain.getHeight());
	locks.fill(false);

    angleofWindward = Degree(20);
    angleofLeeward = Degree(34);
}

void PD_Sand2::step()
{
    mstepindex++;
    sandout = 0;
    logg2.debug("step %d:", mstepindex);
    sanddeposit();
    blowsand();
    sandflow();
    mwind.setSediment(mwind.getSediment() * 0.9);
    int d = mwind.getDepositRate() * mterrain.getSize();
    if(d < 10)mwind.setSediment(10 / mterrain.getSize() / mwind.getErosionRate());
//    if(mstepindex % 200 == 0)mwind.setSediment(1);
    logg2.debug("%lf", mwind.getSediment());
    logg2.debug("%d", d);
    logg2.debug("%d", sandout);
//    if(mstepindex % 2 == 0)
//        mwind.setDirect(Direct(1,0));
//    else
//        mwind.setDirect(Direct(0,1));

}

void PD_Sand2::blowsand()
{
    for (int i = 0; i < /*mwind.getErosionRate() **/ mterrain.getSize(); i++)
	{
		blowsandstep();
	}
}

void PD_Sand2::blowsandstep()
{
	UIntPoint p0(random(0, mterrain.getWidth() - 1), random(0, mterrain.getHeight() - 1));

    if(!canBeBlowed(p0))
        return;

    mterrain.at(p0)--;
    TerrainValue h0 = mterrain.at(p0);

    double windpower = random(mwind.getPowerwithHeight(h0));

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

        if(h1 == 0 && h0 == 0)
        {
            sandblowOutofTerrain();
            return false;
        }

		h0 = h1;
        if (windpower <= 0)
		{
//            mterrain.at(p1)++;
            placeOneParticle(UIntPoint(p1.x, p1.y), Degree(35));
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

            queryUnlockedHigherPoints(IntPoint(x, y), Radian(Degree(35)), points);

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

void PD_Sand2::sandblowOutofTerrain()
{
    sandout++;
}

bool PD_Sand2::canBeBlowed(const UIntPoint &point)
{
    if(pointCanBeEroded(point) && random() < mwind.getPowerwithHeight(mterrain.at(point)) / Wind::MAX_POWER)
//    if(pointCanBeEroded(point))
        return true;
    else
        return false;
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
        placeOneParticle({ x0, y0 }, Degree(35));
    }
}

void PD_Sand2::test()
{
    test3();
    return;
    int testid = 27;
	QString fmt("s%1_s%2.bmp");
	QString filename;
	start();
	filename = fmt.arg(testid).arg(0);
	savetobmp(filename);
    for (int i = 0; i < 30; i++)
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
            if (v > 0 && v % 2 == 1)
				testimage.setPixel(i, j, 255);
		}
	}

    testimage.save(filename);
}

void PD_Sand2::test2()
{
    QString fmt("radius_%1.bmp");
    QString filename;
    mterrain.fill(0);
    int x0 = mterrain.getWidth() / 2, y0 = mterrain.getHeight() / 2;
    mterrain.at(x0, y0)++;
    for(int i = 1; i <= 100; i++)
    {
        mterrain.at(x0, y0)++;
        for(int j = 1; j <= i; j++)
        {
            std::vector<Vector2<int>> &allpoints = getPointsDistanceIs(j);
            for(unsigned int n = 0; n < allpoints.size(); n++)
            {
                mterrain.at(x0 + allpoints[n].x, y0 + allpoints[n].y)++;
            }
        }
    }
    filename = "tm.bmp";
    savetobmp(filename);
}

void PD_Sand2::test3()
{
    std::vector<Vector2<int>> &allpoints = getPointsDistanceLessThan(2);
    std::vector<double> distances;
    std::vector<double> weights;
    double sum = 0;
    for(unsigned int i = 0; i < allpoints.size(); i++)
    {
//        int x = allpoints[i].x, y = allpoints[i].y;
//        distance = sqrt(x )
        distances.push_back(sqrt(allpoints[i].x * allpoints[i].x + allpoints[i].y * allpoints[i].y));
        sum += distances[i];
        weights.push_back(sum);
    }
    auto randomWithWeight = [&]()
    {
        double r = random(sum);
        int i = 0;
        for(i = 0; i < allpoints.size(); i++)
        {
            if(r <= weights[i])
                break;
        }
        return i;
    };
    QString fmt("s_%1.bmp");
    QString filename;

    for(int o = 1; o <= 10; o++)
    {
        mterrain.fill(0);
        for(int i = 0; i < 1000000; i++)
        {
            double s = o;
            int x0 = mterrain.getWidth() / 2, y0 = mterrain.getHeight() / 2;
            for(;;)
            {
                auto k = randomWithWeight();
                Vector2<int> &d = allpoints[k];
                if(s < distances[k])
                    break;
                x0 += d.x;
                y0 += d.y;
                s -= distances[k];
                mterrain.at(x0, y0) = 1;
            }
        }
        filename = fmt.arg(o);
        savetobmp(filename);
    }
}
