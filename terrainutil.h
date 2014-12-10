#ifndef TERRAINUTIL_H
#define TERRAINUTIL_H

#include <vector>
#include <map>
#include "vector2.h"
#include <cmath>
#include "latticepoint.hpp"
#include "random.h"

static bool is2N(unsigned int num)
{
	if (num == 0)
	{
		return false;
	}
	else
	{
		return (num & (num - 1)) == 0;
	}
}

//the terrain's width or height must = 2 ^ n + 1
inline bool canbeTerrainWidth(unsigned int width)
{
	return (width >= 2) && is2N(width - 1);
}


template<typename T>
inline void setNum(T &n, T num)
{
	if (num > 255)
		n = 255;
	else
		n = num;
}

template<typename T>
inline void addNum(T &n, T num)
{
	setNum(n, n + num);
}

//periodic extension
inline int roundnum(int a, int w)
{
	a %= w;
	if (a < 0)a += w;
	return a;
}

//symmetric extension
inline int clipnum(int a, int w)
{
	int nr = (w - 1) * 2;
	int b = roundnum(a, nr);
	if (b >= w)
		return nr - b;
	else
		return b;
}

inline std::vector<Vector2<int>> &getPointsDistanceIs(int distance)
{
	static std::map<int, std::vector<Vector2<int>>> list;

	if (list.find(distance) == list.end())
	{
		std::vector<Vector2<int>> newlist;
		LatticePoint::getFromRing(distance, distance - 1, newlist);
		list.insert({ distance, newlist });
	}

	return list[distance];
}

#endif // TERRAINUTIL_H
