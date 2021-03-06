#include "platedrift.h"
#include "terrainutil.h"
#include <log4cpp/Category.hh>
#include <string>
#include <sstream>

log4cpp::Category& logg =log4cpp::Category::getRoot().getInstance("TerrainMaker2D");

PlateDrift::PlateDrift(Terrain &terrain)
    :TerrainModeling(terrain)
{
}

PlateDrift::~PlateDrift()
{
    clear();
}

void PlateDrift::start()
{
    mstepindex = 0;
    mterrain.reset();
    clear();
    cells = new Cell[mterrain.getWidth() * mterrain.getHeight()];

    for(unsigned int i = 0;i<mterrain.getWidth() * mterrain.getHeight();i++)
    {
        cells[i].x[0].id[0] = cells[i].x[0].id[1] = cells[i].x[0].id[2] = cells[i].x[0].id[3] =
                cells[i].x[0].id[4] = cells[i].x[0].id[5] = cells[i].x[0].id[6] = cells[i].x[0].id[7] = 0;
        cells[i].x[1].id[0] = cells[i].x[1].id[1] = cells[i].x[1].id[2] = cells[i].x[1].id[3] =
                cells[i].x[1].id[4] = cells[i].x[1].id[5] = cells[i].x[1].id[6] = cells[i].x[1].id[7] = 0;
    }

//    for(unsigned int j = 0;j < mterrain.getHeight();j++)
//    {
//        for(unsigned int i = 0;i < mterrain.getWidth()/2;i++)
//        {
//            cells[j*mterrain.getWidth()+i].ys = 4;
//        }
//        for(unsigned int i = mterrain.getWidth()/2;i < mterrain.getWidth();i++)
//        {
//            cells[j*mterrain.getWidth()+i].ys = 0;
//        }
//    }

    for(unsigned int j = 0;j < mterrain.getHeight()/2;j++)
    {
        for(unsigned int i = 0;i < mterrain.getWidth();i++)
        {
            cells[j*mterrain.getWidth()+i].ys = 2;
        }
    }
    for(unsigned int j = mterrain.getHeight()/2;j < mterrain.getHeight();j++)
    {
        for(unsigned int i = 0;i < mterrain.getWidth();i++)
        {
            cells[j*mterrain.getWidth()+i].ys = 6;
        }
    }

//    debugcells(index);

}

void PlateDrift::step()
{
    mstepindex++;

    logg.debug("step %d", mstepindex);
    logg.debug("before:");
//    debugcells(index);
    for(unsigned int j = 0;j < mterrain.getHeight();j++)
    {
        for(unsigned int i = 0;i < mterrain.getWidth();i++)
        {
            if(j>0)
            {
                if(i > 0)
                {
                    cells[j*mterrain.getWidth()+i].x[index2].rd = cells[(j-1)*mterrain.getWidth()+i-1].x[index].rd;
                }
                else
                {
                    cells[j*mterrain.getWidth()+i].x[index2].rd =  0;
                }

                cells[j*mterrain.getWidth()+i].x[index2].d = cells[(j-1)*mterrain.getWidth()+i].x[index].d;

                if(i < mterrain.getWidth()-1)
                {
                    cells[j*mterrain.getWidth()+i].x[index2].ld = cells[(j-1)*mterrain.getWidth()+i+1].x[index].ld;
                }
                else
                {
                    cells[j*mterrain.getWidth()+i].x[index2].ld = 0;
                }

            }
            else
            {
                cells[j*mterrain.getWidth()+i].x[index2].rd = cells[j*mterrain.getWidth()+i].x[index2].d = cells[j*mterrain.getWidth()+i].x[index2].ld = false;
            }

            if(i > 0)
            {
                cells[j*mterrain.getWidth()+i].x[index2].r = cells[(j)*mterrain.getWidth()+i-1].x[index].r;
            }
            else
            {
                cells[j*mterrain.getWidth()+i].x[index2].r = 0;
            }

            if(i < mterrain.getWidth()-1)
            {
                cells[j*mterrain.getWidth()+i].x[index2].l = cells[(j)*mterrain.getWidth()+i+1].x[index].l;
            }
            else
            {
                cells[j*mterrain.getWidth()+i].x[index2].l = 0;
            }

            if(j<mterrain.getHeight()-1)
            {
                if(i > 0)
                {
                    cells[j*mterrain.getWidth()+i].x[index2].ru = cells[(j+1)*mterrain.getWidth()+i-1].x[index].ru;
                }
                else
                {
                    cells[j*mterrain.getWidth()+i].x[index2].ru = 0;
                }

                cells[j*mterrain.getWidth()+i].x[index2].u = cells[(j+1)*mterrain.getWidth()+i].x[index].u;

                if(i < mterrain.getWidth()-1)
                {
                    cells[j*mterrain.getWidth()+i].x[index2].lu = cells[(j+1)*mterrain.getWidth()+i+1].x[index].lu;
                }
                else
                {
                    cells[j*mterrain.getWidth()+i].x[index2].lu = 0;
                }
            }
            else
            {
                cells[j*mterrain.getWidth()+i].x[index2].ru = cells[j*mterrain.getWidth()+i].x[index2].u = cells[j*mterrain.getWidth()+i].x[index2].lu = false;
            }
        }
    }

    for(unsigned int i = 0;i<mterrain.getWidth() * mterrain.getHeight();i++)
    {
        cells[i].x[index2].id[cells[i].ys] = 1;
    }

    logg.debug("after:");
//    debugcells(index2);

    int temp = index;
    index = index2;
    index2 = temp;

    for(unsigned int i = 0;i<mterrain.getWidth() * mterrain.getHeight();i++)
    {
        if(cells[i].isd(index))addNum(mterrain.getData()[i], random(0.8, 1.0));
    }

//    debugterrain();
}

void PlateDrift::generate()
{
    for(int i = 0;i<10;i++)
    {
        step();
    }
}

bool PlateDrift::isfinished()
{
    return false;
}

void PlateDrift::clear()
{
    if(cells)
    {
        delete[] cells;
    }
}

void PlateDrift::debugcells(int id)
{
    std::ostringstream str;
    for(unsigned int j = 0;j < mterrain.getHeight();j++)
    {
        str.str("");
        for(unsigned int i = 0;i < mterrain.getWidth();i++)
        {
            str<<"(";
            for(int n = 0;n<8;n++)
            {
                str<<cells[j*mterrain.getWidth()+i].x[id].id[n]<<",";
            }
            str<<")";
        }
        logg.debug(str.str());
    }
}

void PlateDrift::debugterrain()
{
    std::ostringstream str;

    for(unsigned int j = 0;j < mterrain.getHeight();j++)
    {
        str.str("");
        for(unsigned int i = 0;i < mterrain.getWidth();i++)
        {
             str<<mterrain.getData()[i]<<",";
        }
        logg.debug(str.str());
    }
}
