#include "pd_sand.h"
#include "terrainutil.h"
#include <assert.h>

PD_Sand::PD_Sand(Terrain &terrain)
    :ParticleDeposition(terrain)
    ,mwinddirect(0, 0)
{
}

void PD_Sand::start()
{
    mstepindex = 0;

    std::function<void(unsigned int i, TerrainValue &value)> func = [](unsigned int, TerrainValue& v)
    {
        v = 50 + random(0, 1);
    };

    mterrain.for_each(func);

    setWindDirect({-1, 0});
    mwindpower = 5;

    locks.reset(mterrain.getWidth(), mterrain.getHeight());
    locks.fill(false);
//    mterrain.fill(0);
//    mterrain.at(mterrain.getWidth() / 2, mterrain.getHeight() / 2) = 255;
//    sandflow();
}

void PD_Sand::step()
{
    mstepindex++;
    mneedput = 0;
    blowsand();
    sandflow();
    putsands();
}

void PD_Sand::setWindDirect(Vector2<double> winddirect)
{
    assert(winddirect.x != 0 || winddirect.y != 0);
    mwinddirect = winddirect;
    mwinddirect.normalize();

    mputedges.clear();
    if(mwinddirect.x > 0){
        mputedges.push_back(0);
    }else if(mwinddirect.x < 0){
        mputedges.push_back(1);
    }

    if(mwinddirect.y > 0){
        mputedges.push_back(2);
    }else if(mwinddirect.y < 0){
        mputedges.push_back(3);
    }
}

void PD_Sand::blowsand()
{
    for(int i = 0;i < mterrain.getWidth() * mterrain.getHeight();i++)
    {
        blowsandstep();
    }
}

void PD_Sand::blowsandstep()
{
    UIntPoint p0(random(0, mterrain.getWidth() - 1), random(0, mterrain.getHeight() - 1));

	auto winddirect = mwinddirect;
	auto windpower = mwindpower;
    if(pointInLeewardSlope(p0))
	{ 
		return;
		//winddirect.x = -mwinddirect.x;
		//winddirect.y = -mwinddirect.y;
		//windpower = mwindpower / 5;
	}
        

    double h1 = mterrain.at(p0);
    mterrain.at(p0)--;

	IntPoint p1(p0.x + winddirect.x * windpower, p0.y + winddirect.y * windpower);

    if(mterrain.pointInSpace(p1))
    {
		double h2 = mterrain.at(p1);
		//if (h2 > h1)
		//{
		//	p1.x -= winddirect.x * 1;
		//	p1.y -= winddirect.y * 1;
		//}
		//else
		//{
		//	p1.x -= winddirect.x * 2;
		//	p1.y -= winddirect.y * 2;
		//}
		if(h2 < h1)
		{
			p1.x -= winddirect.x * windpower * 0.5;
			p1.y -= winddirect.y * windpower * 0.5;
		}
		placeOneParticle(UIntPoint(p1.x, p1.y), 2);
    }
	else
	{
		p1.x = roundnum(p1.x, mterrain.getWidth());
		p1.y = roundnum(p1.y, mterrain.getHeight());

		placeOneParticle(UIntPoint(p1.x, p1.y), 2);
        //sandblowOutofTerrain(p1);
	}
}

void PD_Sand::sandflow()
{
    while(sandflowstep());
}

bool PD_Sand::sandflowstep()
{
    bool r = false;
    std::function<void (unsigned int, unsigned int, TerrainValue &)> func = [this, &r](unsigned int x, unsigned int y, TerrainValue &)
    {
        if(locks.at(x, y) == false)
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
            mterrain.at(p)-=1;
            mterrain.at(x, y)+=1;
            locks.at(p) = true;
            locks.at(x, y) = true;
            r = true;
        }
    };

    mterrain.for_each(func);
    unlockall();
    return r;
}

void PD_Sand::unlockall()
{
    locks.fill(false);
}

bool PD_Sand::pointInWindwardSlope(const UIntPoint &point)
{
    return mterrain.getPointGradient(point, mwinddirect) > 0;
}

bool PD_Sand::pointInLeewardSlope(const UIntPoint &point)
{
    return mterrain.getPointGradient(point, mwinddirect) < 0;
}

void PD_Sand::sandblowOutofTerrain(const IntPoint &point)
{
    mneedput++;
}

void PD_Sand::putsands()
{
    for(int i = 0;i < mneedput;i++)
    {
        placeOneParticle(getPutPosition(), 2);
    }
}

UIntPoint PD_Sand::getPutPosition()
{
    while(true)
    {
        UIntPoint p0(random(0, mterrain.getWidth() - 1), random(0, mterrain.getHeight() - 1));
        if(pointInWindwardSlope(p0))
            return {p0.x, p0.y};
    }

    while(true)
    {
        IntPoint p0 = getPutPositionS();
        int power = random(0, mwindpower * 2);
        p0.x += mwinddirect.x * power;
        p0.y += mwinddirect.y * power;
		if (mterrain.pointInSpace(p0))
			return UIntPoint(p0.x, p0.y);
	}
}

IntPoint PD_Sand::getPutPositionS()
{
    assert(mputedges.size() > 0);
    int k = random(0, mputedges.size() - 1);
    switch (mputedges[k]) {
    case 0:
		return IntPoint(0, random(0, mterrain.getHeight() - 1));
    case 1:
		return IntPoint(mterrain.getWidth() - 1, random(0, mterrain.getHeight() - 1));
    case 2:
		return IntPoint(random(0, mterrain.getWidth() - 1), 0);
    case 3:
		return IntPoint(random(0, mterrain.getWidth() - 1), mterrain.getHeight() - 1);
    default:
        assert(false);
    }
}
