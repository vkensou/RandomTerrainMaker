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
	auto nd = direct;
	double radian = nd.toRadian();
	EightDirect d8;
	d8 = radian;
	Vector2<int> d = d8;

    int xa = point.x + d.x, ya = point.y + d.y;
    int xb = point.x - d.x, yb = point.y - d.y;
    xa = clipnum(xa, getWidth());
    ya = clipnum(ya, getHeight());
    xb = clipnum(xb, getWidth());
    yb = clipnum(yb, getHeight());

    UIntPoint pa(xa, ya), pb(xb, yb);   //point after, point behind

    int ha = at(pa) - at(point), hb = at(point) - at(pb);

    if(ha > 0 && hb >= 0)
        return 1;
    if(hb < 0 && ha <= 0)
        return -1;
    return 0;
}

