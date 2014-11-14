#ifndef PLATEDRIFT_H
#define PLATEDRIFT_H
#include "terrainmodeling.h"

class PlateDrift
        :public TerrainModeling
{
public:
    PlateDrift(Terrain &terrain);
    ~PlateDrift();
    void start() override;
    void step() override;
    void generate() override;
    bool isfinished() override;

private:
    struct Cell
    {
        union {
            struct {
                int l,ld,d,rd,r,ru,u,lu;
            };
            int id[8];
        }x[2];
        int ys{0};
        bool isd(int i)
        {
            int n = 0;
            for(int j = 0;j<8;j++)
            {
                if(x[i].id[j]!=false)
                    n++;
            }
            if(n>=2)
                return true;
            else
                return false;
        }
    };
    Cell *cells{nullptr};
    int index{0}, index2{1};
    void clear();
    void debugcells(int);
    void debugterrain();
};

#endif // PLATEDRIFT_H
