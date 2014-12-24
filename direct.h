#pragma once

#include "vector2.h"
#include "angle.h"
#include <cmath>
#include <cassert>

class Direct
	:public Vector2 < double >
{
public:
	Direct(double x, double y)
		:Vector2(x, y)
	{
		assert(x != 0 || y != 0);
		normalize();
	}
	
    Direct(const Radian &radian)
	{
		x = cos(radian);
		y = sin(radian);
	}

    Radian toRadian()
	{
		normalize();
		if (x == 0)
		{
			if (y > 0)
				return M_PI_2;
			else if (y < 0)
				return 3 * M_PI_2;
		}

		if (y >= 0)
		{
			return atan2(y, x);
		}
		else
		{
			return 2 * M_PI + atan2(y, x);
		}
	}
};
