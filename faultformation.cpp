#include "faultformation.h"
#include "terrainutil.h"

FaultFormation::FaultFormation(Terrain &terrain)
    :TerrainModeling(terrain), mdelta0(10), mdeltan(0)
{
}

void FaultFormation::start()
{
    mstepcount = 10;
    mstepindex = 0;
    mdelta0 = 5;
    mdeltan = 0;
    mterrain.reset();
}

int changesign(int num)
{
    if(qrand() % 2==0)
    {
        return -num;
    }
    else
    {
        return num;
    }
}


void FaultFormation::step()
{
    if(mstepindex == mstepcount) return;
    mstepindex++;

    int x0 = qrand() % mterrain.getWidth(), y0 = qrand() % mterrain.getHeight();
    int a1 = changesign(qrand()),a2 = changesign(qrand());
    double *data = mterrain.getData();
    double delta = mdelta0 + (mstepindex / mstepcount) * (mdeltan - mdelta0);
    for(int j = 0;j<(int)mterrain.getHeight();j++)
    {
        for(int i = 0;i<(int)mterrain.getWidth();i++)
        {
            if((a1 * (i - x0) + a2 * (j - y0))>=0)
            {
                addNum(data[j * mterrain.getWidth() + i], delta);
            }
        }
    }
}


