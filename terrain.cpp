#include "terrain.h"
#include <memory.h>
#include <assert.h>
#include "terrainutil.h"
#include "direct.h"
#include "eightdirect.h"

Terrain::Terrain(unsigned int width, unsigned int height)
    :ElementSpace2D(width, height)
{
}

void Terrain::reset(unsigned int width, unsigned int height)
{
    assert(canbeTerrainWidth(width));
    assert(canbeTerrainWidth(height));

    ElementSpace2D::reset(width, height);
}

int Terrain::getPointGradient(const UIntPoint &point, const Direct &direct)
{
	assert(pointInSpace(point));
	auto nd = direct;
	double radian = nd.toRadian();
	EightDirect d8;
	d8 = radian;
	Vector2<int> d = d8;

	auto pa = extend(IntPoint(point.x + d.x, point.y + d.y));
	auto pb = extend(IntPoint(point.x - d.x, point.y - d.y));

    int ha = at(pa) - at(point), hb = at(point) - at(pb);

	if (ha >= 0 && hb >= 0)
	{
		if (ha != 0 || hb != 0)
			return 1;
	}
	else if (ha <= 0 && hb <= 0)
	{
		if (ha != 0 || hb != 0)
			return -1;
	}
	else
		return 0;
}

IntPoint Terrain::extend(const IntPoint &point)
{
	IntPoint p1;
	if (mextension == CYCLE)
	{
		p1.x = roundnum(point.x, getWidth());
		p1.y = roundnum(point.y, getHeight());
	}
	else if (mextension == SYMMETRIC)
	{
		p1.x = clipnum(point.x, getWidth());
		p1.y = clipnum(point.y, getHeight());
	}
	else if (mextension = NONE)
	{
		if (point.x < 0)
			p1.x = 0;
		else if (point.x >= getWidth())
			p1.x = getWidth() - 1;
		else
			p1.x = point.x;

		if (point.y < 0)
			p1.y = 0;
		else if (point.y >= getHeight())
			p1.y = getHeight() - 1;
		else
			p1.y = point.y;
	}
	return p1;
}