#ifndef PD_SAND_H
#define PD_SAND_H

#include "particledeposition.h"
#include "elementspace2d.h"
#include <vector>

class PD_Sand : public ParticleDeposition
{
public:
    PD_Sand(Terrain &terrain);

    void start() override;
    void step() override;
    void setWindDirect(Vector2<double> winddirect);

private:
	//The sand blown by the wind
    void blowsand();
    void blowsandstep();

	//Sand from high to low
    void sandflow();
    bool sandflowstep();
    void unlockall();

    bool pointInWindwardSlope(const UIntPoint &point);
    bool pointInLeewardSlope(const UIntPoint &point);
    void sandblowOutofTerrain(const IntPoint &point);

	//put sands into terrain again, that were blown out of terrain.
    void putsands();
    UIntPoint getPutPosition();
    IntPoint getPutPositionS();

private:
    ElementSpace2D<bool> locks;
    Vector2<double> mwinddirect;
    int mwindpower;
    int mneedput;
    std::vector<int> mputedges; //left edge -- 0, right edge -- 1, up edge -- 2, down edge --3
};

#endif // PD_SAND_H
