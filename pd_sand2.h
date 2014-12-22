#pragma once

#include "particledeposition.h"
#include "elementspace2d.h"
#include "direct.h"
#include <qstring.h>
#include "wind.h"

class PD_Sand2
	:public ParticleDeposition
{
public:
	PD_Sand2(Terrain &terrain);

	void start() override;
	void step() override;
	void test() override;

private:
	//sands deposit ,which fly with wind 
	void sanddeposit();
	void sanddepositstep();

	//The sand blown by the wind
	void blowsand();
	void blowsandstep();
	void sandblowOutofTerrain(const IntPoint &point);
    bool canBeBlowed(const UIntPoint &point);

	//Sands flow from high to low
	void sandflow();
	bool sandflowstep();
	void unlockall();

    bool pointCanBeEroded(const UIntPoint &point);
    bool pointIsSettleable(const UIntPoint &point);

    void queryUnlockedHigherPoints(const IntPoint &point0, int radius, std::vector<IntPoint> &points);
    void queryUnlockedHigherPoints(const IntPoint &point0, const Radian &radian, std::vector<IntPoint> &points);

	void savetobmp(const QString &filename);
private:
	ElementSpace2D<bool> locks;
    Wind mwind;
    Radian angleofWindward;
    Radian angleofLeeward;
};

