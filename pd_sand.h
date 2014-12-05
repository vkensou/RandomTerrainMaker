#ifndef PD_SAND_H
#define PD_SAND_H

#include "particledeposition.h"
#include "elementspace2d.h"

class PD_Sand : public ParticleDeposition
{
public:
    PD_Sand(Terrain &terrain);

    void start() override;
    void step() override;

private:
    void blowsand();
    void blowsandstep();
    void sandflow();
    bool sandflowstep();
    void unlockall();
    bool pointInWindwardSlope(const UIntPoint &point);
    bool pointInLeewardSlope(const UIntPoint &point);
    void sandblowOutofTerrain(const IntPoint &point);
    void putsands();
private:
    ElementSpace2D<bool> locks;
    Vector2<double> mwinddirect;
    int mwindpower;
    int mneedput;

};

#endif // PD_SAND_H
