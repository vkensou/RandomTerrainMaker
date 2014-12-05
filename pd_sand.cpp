#include "pd_sand.h"
#include "terrainutil.h"

PD_Sand::PD_Sand(Terrain &terrain)
    :ParticleDeposition(terrain)
    ,mwinddirect(0, 0)
{
}

void PD_Sand::start()
{
    mstepindex = 0;

    mterrain.for_each([](unsigned int, TerrainValue& v)
    {
        v = 50 + random(0, 1);
    });

    mwinddirect = {1, 0};
    mwinddirect.normalize();
    mwindpower = 24;

    locks.reset(mterrain.getWidth(), mterrain.getHeight());
    locks.fill(false);
}

void PD_Sand::step()
{
    mstepindex++;
    mneedput = 0;
    blowsand();
    sandflow();
    putsands();
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

    if(pointInLeewardSlope(p0))
        return;

    double h1 = mterrain.at(p0);
    mterrain.at(p0)--;

    IntPoint p1 = {p0.x + mwinddirect.x * mwindpower, p0.y + mwinddirect.y * mwindpower};

    if(!mterrain.pointInSpace(p1))
    {
        sandblowOutofTerrain(p1);
        return;
    }

    double h2 = mterrain.at(p1);
    if(h2 < h1)
    {
        p1.x -= mwinddirect.x * mwindpower * 0.5;
        p1.y -= mwinddirect.y * mwindpower * 0.5;
    }
    placeOneParticle({p1.x, p1.y});
}

void PD_Sand::sandflow()
{
    while(sandflowstep());
    unlockall();
}

bool PD_Sand::sandflowstep()
{
    bool r = false;
    mterrain.for_each([this, &r](unsigned int x, unsigned int y, TerrainValue &)
    {
        if(locks.at(x, y) == false)
        {
            std::vector<IntPoint> points;

            queryhigherpoints({x, y}, 2, points);

            if(points.size() == 0)
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
    });
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
    for()
}
