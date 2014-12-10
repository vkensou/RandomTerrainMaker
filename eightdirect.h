#pragma once

#include "vector2.h"
#define _USE_MATH_DEFINES
#include <cmath>

class EightDirect
{
public:
	enum D8
	{
		LEFTUP, UP, RIGHTUP, LEFT, NO, RIGHT, LEFTDOWN, DOWN, RIGHTDOWN
	};
	
	D8 direct{ NO };

	EightDirect();
	~EightDirect();

	EightDirect &rotate(int a)
	{
		static int dir[9] = { 1, 2, 3, 0, -1, 4, 7, 6, 5 };
		static D8 rot[8] = { LEFT, LEFTUP, UP, RIGHTUP, RIGHT, RIGHTDOWN, DOWN, LEFTDOWN };

		if (direct != NO && a != 0)
		{
			int k = dir[direct];
			k += a;
			k %= 8;
			if (k<0)k += 8;
			direct = rot[k];
		}
		return *this;
	}

	operator Vector2<int>()
	{
		int dx, dy;
		dx = direct % 3 - 1;
		dy = direct / 3 - 1;
		return Vector2<int>(dx, dy);
	}

	EightDirect &operator=(double radian)
	{
		double degree = round(radian * 180 / M_PI);
		while (degree >= 360)degree -= 360;
		while (degree < 0)degree += 360;

		if (degree < 22.5)
			direct = LEFT;
		else if (degree < 67.5)
			direct = LEFTUP;
		else if (degree < 112.5)
			direct = UP;
		else if (degree < 157.5)
			direct = RIGHTUP;
		else if (degree < 202.5)
			direct = RIGHT;
		else if (degree < 247.5)
			direct = RIGHTDOWN;
		else if (degree < 292.5)
			direct = DOWN;
		else if (degree < 337.5)
			direct = LEFTDOWN;
		else
			direct = LEFT;
	}

private:

};

EightDirect::EightDirect()
{
}

EightDirect::~EightDirect()
{
}