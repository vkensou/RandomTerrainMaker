#ifndef PARTICLEDEPOSITION_H
#define PARTICLEDEPOSITION_H

#include "terrainmodeling.h"

#include <vector>
#include "vector2.h"
#include <functional>
#include "angle.h"

class ParticleDeposition : public TerrainModeling
{
public:
	
	ParticleDeposition(Terrain &terrain);
	void start() override;
	void step() override;
	void generate() override;
	bool isfinished() override;


protected:
    void placeOneParticle(const UIntPoint &position, int radius, int height);
    void placeOneParticle(const UIntPoint &position, const Radian &radian);

	void queryNearbyPointsIf(const IntPoint &point0, int radius, std::vector<IntPoint> &points,
		std::function<void(const IntPoint &point, std::vector<IntPoint> &points)> func);
    void querylowerpoints(const IntPoint &point0, int radius, int height, std::vector<IntPoint> &points);
    void queryhigherpoints(const IntPoint &point0, int radius, int height, std::vector<IntPoint> &points);

};

#endif // PARTICLEDEPOSITION_H
