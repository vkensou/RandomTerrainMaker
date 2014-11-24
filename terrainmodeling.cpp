#include "terrainmodeling.h"

TerrainModeling::TerrainModeling(Terrain &terrain)
    :mterrain(terrain), mstepcount(0), mstepindex(0)
{
}

int ptoi(int i, int j, int width, int height)
{
    if(i<0)i=0;
    if(i>=width)i=width-1;
    if(j<0)j=height;
    if(j>=height)j=height-1;
    return j * width + i;
}

void TerrainModeling::erosion()
{
    double *temp = new double[mterrain.getWidth() * mterrain.getHeight()];
    double *data = mterrain.getData();
    temp[0] = data[0];
//    float k = 0.5;

    for(unsigned int j = 0;j<mterrain.getHeight();j++)
    {
        temp[j * mterrain.getWidth() + 0] = mterrain.at(0, j);
        temp[j * mterrain.getWidth() + mterrain.getWidth() - 1] = mterrain.at(mterrain.getWidth() - 1, j);
        for(unsigned int i = 1;i<mterrain.getWidth() - 1;i++)
        {
            temp[j * mterrain.getWidth() + i] = (mterrain.at(i - 1, j) + mterrain.at(i, j) + mterrain.at(i + 1, j)) / 3;
        }
    }

    for(unsigned int j = 0;j<mterrain.getHeight();j++)
    {
        for(unsigned int i = 0;i<mterrain.getWidth();i++)
        {
            data[j * mterrain.getWidth() + i] = (temp[ptoi(i,j-1,mterrain.getWidth(),mterrain.getHeight())] + 1 * temp[j * mterrain.getWidth() + i] + temp[ptoi(i, j+1,mterrain.getWidth(),mterrain.getHeight())]) / 3;
        }
    }

    delete temp;
}

void TerrainModeling::generate()
{
    start();
    while(!isfinished())
    {
        step();
    }
}
