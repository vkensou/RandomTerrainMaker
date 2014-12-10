#pragma once

#include "vector2.h"
#include "direct.h"
#include <cassert>
#include <functional>

class Ray
{
public:
	Vector2<double> startpoint{ 0, 0 };
	Direct direct{ 1, 0 };

	Ray(Vector2<double> &startpoint, Direct &direct)
		:startpoint(startpoint), direct(direct)
	{
		assert(direct.x != 0 || direct.y != 0);
	}

	double getSlope()
	{
		assert(direct.x != 0);
		return direct.y / direct.x;
	}

	double getX(double y)
	{
		if (y == startpoint.y || direct.x == 0)
			return startpoint.x;

		return (y - startpoint.y) / getSlope() + startpoint.x;
	}
	double getY(double x)
	{
		if (x == startpoint.x || direct.y == 0)
			return startpoint.y;

		return getSlope() * (x - startpoint.x) + startpoint.y;
	}

	void traverseLatticePoints(std::function<bool(int, int)> func, int num = 0, bool include_startpoint = true)
	{
		assert(direct.x != 0 || direct.y != 0);

		direct.normalize();
		double radian = direct.toRadian();
		bool z, xy;
		if (radian <= M_PI_4 || radian >= M_PI_4 * 7)
		{
			xy = true; z = true;
		}
		else if (radian >= 3 * M_PI_4 && radian <= 5 * M_PI_4)
		{
			xy = true; z = false;
		}
		else if (radian <= M_PI)
		{
			xy = false; z = true;
		}
		else
		{
			xy = false; z = false;
		}
		int i = xy == true ? startpoint.x : startpoint.y;
		int j;
		if (!include_startpoint)
		{
			if (z)
				i++;
			else
				i--;
		}
		if (xy)
			j = getY(i);
		else
			j = getX(i);

		bool r;
		int n = 0;
		do
		{
			if (xy)
				r = func(i, j);
			else
				r = func(j, i);

			if (z)
				i++;
			else
				i--;

			if (xy)
				j = getY(i);
			else
				j = getX(i);
			n++;
		} while (r && (num <= 0 || n < num));
	}
};