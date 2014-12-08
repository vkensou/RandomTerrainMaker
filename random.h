#pragma once

#include <stdlib.h>
#include <time.h>

//return [0,1]
inline double random()
{
	return rand() / (double)RAND_MAX;
}

//return [0,n]
inline double random(double n)
{
	return rand() / (double)RAND_MAX * n;
}

//return [a,b]
inline double random(double a, double b)
{
	return (rand() / (double)RAND_MAX) * (b - a) + a;
}

//return [a,b]
inline int random(int a, int b)
{
	return (int)(rand() / (RAND_MAX + 1.0) * (b - a + 1)) + a;
}

//return [0,n]
inline int random(int n)
{
	return rand() / (RAND_MAX + 1.0) * (n + 1);
}
