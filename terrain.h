#ifndef TERRAIN_H
#define TERRAIN_H

#include "elementspace2d.h"

typedef double TerrainValue;
class Direct;

class Terrain
        :public ElementSpace2D<TerrainValue>
{
public:
	enum Extension { NONE, CYCLE, SYMMETRIC };

	Extension getExtension(){ return mextension; }
	void setExtension(Extension extension){
		mextension = extension;
	}
	IntPoint extend(const IntPoint &point);

    Terrain(unsigned int width, unsigned int height);

    void reset(unsigned int width, unsigned int height) override;

    // 1 //, 2 /-,3 /\, 4-/,5--,6-\,7\/,8\-,9\\.

    int getPointGradient(const UIntPoint &point, const Direct &direct);
private:
	Extension mextension{ NONE };

};

#endif // TERRAIN_H
