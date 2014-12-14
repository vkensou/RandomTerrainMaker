#pragma once

#include "particledeposition.h"
#include "elementspace2d.h"
#include "direct.h"

class PD_Sand2
	:public ParticleDeposition
{
public:
	PD_Sand2(Terrain &terrain);

	void start() override;
	void step() override;
	void setWindDirect(Direct winddirect);

private:
	//sands deposit ,which fly with wind 
	void sanddeposit();
	void sanddepositstep();

	//The sand blown by the wind
	void blowsand();
	void blowsandstep();
	void sandblowOutofTerrain(const IntPoint &point);

	//Sands flow from high to low
	void sandflow();
	bool sandflowstep();
	void unlockall();

	bool pointInWindwardSlope(const UIntPoint &point);
	bool pointInLeewardSlope(const UIntPoint &point);

private:
	ElementSpace2D<bool> locks;
	Direct mwinddirect;
	int mwindpower;
	int deposit;
	int erosiveness;
};

