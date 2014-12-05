#include "terrain.h"
#include <memory.h>
#include <assert.h>
#include "terrainutil.h"

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

int Terrain::getPointGradient(const UIntPoint &point, const Vector2<double> &direct)
{
    auto d = direct;
    if(d.x > 0)
        d.x = 1;
    else if(d.x < 0)
        d.x = -1;

    if(d.y > 0)
        d.y = 1;
    else if(d.y < 0)
        d.y = -1;

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

