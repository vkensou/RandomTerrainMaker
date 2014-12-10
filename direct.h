#pragma once

#define _USE_MATH_DEFINES

#include "vector2.h"
#include <cmath>

class Direct
	:public Vector2 < double >
{
	Direct(double x, double y)
		:Vector2(x, y)
	{
	}
	
	Direct(double radian)
	{
		x = cos(radian);
		y = sin(radian);
	}

	double toRadian()
	{
		normalize();
		return atan2(y, x);
	}
};