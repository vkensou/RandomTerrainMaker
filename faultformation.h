#ifndef FAULTFORMATION_H
#define FAULTFORMATION_H
#include "terrainmodeling.h"

class FaultFormation
        :public TerrainModeling
{
public:
    FaultFormation(Terrain &terrain);
    void start() override;
    void step() override;


private:
    double mdelta0, mdeltan;
};

#endif // FAULTFORMATION_H
