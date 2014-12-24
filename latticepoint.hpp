#pragma once

#include <cassert>
#include <vector>
#include "vector2.h"

class LatticePoint
{
private:
	static void pushAxisPoints(int x, std::vector<Vector2<int>> &points)
	{
		points.push_back({ x, 0 });
		points.push_back({ -x, 0 });
		points.push_back({ 0, x });
		points.push_back({ 0, -x });
	}

	static void pushDiagonalPoints(int i, std::vector<Vector2<int>> &points)
	{
		points.push_back({ i, i });
		points.push_back({ i, -i });
		points.push_back({ -i, i });
		points.push_back({ -i, -i });
	}

	static void pushQuadrantsPoints(int i, int j, std::vector<Vector2<int>> &points)
	{
		points.push_back({ i, j });
		points.push_back({ i, -j });
		points.push_back({ -i, j });
		points.push_back({ -i, -j });

		points.push_back({ j, i });
		points.push_back({ j, -i });
		points.push_back({ -j, i });
		points.push_back({ -j, -i });
	}

public:
	//get lattice points in circle:(0, 0) radius
	static void getFromCircle(int radius, std::vector<Vector2<int>> &points)
	{
		assert(radius >= 0);

		points.push_back({ 0, 0 });
        for (int i = 1; i < radius; i++)
		{
			pushAxisPoints(i, points);
		}
		int x1 = floor(radius / sqrt(2)), y1 = x1;
		for (int i = 1; i <= x1; i++)
		{
			pushDiagonalPoints(i, points);
		}
		if (radius <= 2)return;
		x1++;
		if (x1 * x1 + y1 * y1 > radius * radius)
		{
			y1--;
		}
		int x2 = radius - 1, y2 = floor(sqrt(radius * radius - x2 * x2));
		for (int j = 1; j <= y2 - 1; j++)
		{
			for (int i = j + 1; i <= x2; i++)
			{
				pushQuadrantsPoints(i, j, points);
			}
		}
		for (int j = y2; j <= y1; j++)
		{
			int xe = floor(sqrt(radius * radius - j * j));
			for (int i = j + 1; i <= xe; i++)
			{
				pushQuadrantsPoints(i, j, points);
			}
		}
	}

	//get lattice points in circle:(0, 0) (smallradius, bigradius]
	static void getFromRing(int bigradius, int smallradius, std::vector<Vector2<int>> &points)
	{
		assert(bigradius == smallradius + 1 && smallradius >= 0);

        pushAxisPoints(bigradius, points);
		int x1b = floor(bigradius / sqrt(2));
		int x1s = floor(smallradius / sqrt(2));
		int x1 = x1b, y1 = x1;
		if (x1b > x1s)
			pushDiagonalPoints(x1, points);
		if (bigradius <= 2)return;
		x1++;
		if (x1 * x1 + y1 * y1 > bigradius * bigradius)
		{
			y1--;
		}
		int x2b = bigradius - 1, y2b = floor(sqrt(bigradius * bigradius - x2b * x2b));
		int x2s = smallradius - 1, y2s = floor(sqrt(smallradius * smallradius - x2s * x2s));
		for (int j = 1; j <= y2s; j++)
		{
			pushQuadrantsPoints(x2b, j, points);
		}
		for (int j = y2b > y2s ? y2b : y2b + 1; j <= y1; j++)
		{
			int xs = floor(sqrt(smallradius * smallradius - j * j));
			xs = xs < j ? j : xs;
			int xe = floor(sqrt(bigradius * bigradius - j * j));
			for (int i = xs + 1; i <= xe; i++)
			{
				pushQuadrantsPoints(i, j, points);
			}
		}
	}

	static int getNumbleofCircle(double radius)
	{
		assert(radius >= 0);

		unsigned int r = (int)radius;
		unsigned int sigma = 0;

		for (unsigned int i = 1; i <= r; i++)
		{
			sigma += (int)sqrt(radius * radius - i * i);
		}

		return 1 + 4 * r + 4 * sigma;
	}

	static int getNumberofRing(double bigradius, double smallradius)
	{
		assert(bigradius > smallradius && smallradius >= 0);

		return getNumbleofCircle(bigradius) - getNumbleofCircle(smallradius);
	}
};
