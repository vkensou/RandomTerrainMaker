#ifndef ELEMENTSPACE2D_H
#define ELEMENTSPACE2D_H

#include <memory.h>
#include <assert.h>
#include "vector2.h"
#include <functional>

template <typename ElementType>
class ElementSpace2D
{
public:
	ElementSpace2D(unsigned int width = 0, unsigned int height = 0);
	virtual ~ElementSpace2D();

	unsigned int getSize(){ return mwidth * mheight; }
	unsigned int getWidth(){ return mwidth; }
	unsigned int getHeight(){ return mheight; }
	ElementType * getData(){ return mdata; }
	void clearup();
	virtual void reset(unsigned int width, unsigned int height);

	ElementType &at(unsigned int x, unsigned int y);
	const ElementType &at(unsigned int x, unsigned int y) const;

	ElementType &at(const IntPoint &point){ return at(point.x, point.y); }
	ElementType &at(const UIntPoint &point){ return at(point.x, point.y); }

	ElementType &at(unsigned int i);
	const ElementType &at(unsigned int i) const;

	bool pointInSpace(int x, int y)
	{
		return (x >= 0 && x < mwidth && y >= 0 && y < mheight);
	}
	bool pointInSpace(const IntPoint &point){ return pointInSpace(point.x, point.y); }
	bool pointInSpace(const UIntPoint &point){ return pointInSpace(point.x, point.y); }

	void for_each(std::function<void(unsigned int i, ElementType &value)> func);

	void for_each(std::function<void(unsigned int x, unsigned int y, ElementType &value)> func);

	void fill(ElementType value);

private:
	unsigned int mwidth;
	unsigned int mheight;
	ElementType *mdata{ nullptr };
	void destroyelements();
};

template <typename ElementType>
ElementSpace2D<ElementType>::ElementSpace2D(unsigned int width, unsigned int height)
{
	reset(width, height);
}

template <typename ElementType>
ElementSpace2D<ElementType>::~ElementSpace2D()
{
	destroyelements();
}

template <typename ElementType>
void ElementSpace2D<ElementType>::clearup()
{
	memset(mdata, 0, mwidth * mheight * sizeof(ElementType));
}

template <typename ElementType>
void ElementSpace2D<ElementType>::reset(unsigned int width, unsigned int height)
{
	destroyelements();

	mwidth = width;
	mheight = height;

	mdata = new ElementType[mwidth * mheight];
	clearup();
}

template <typename ElementType>
ElementType &ElementSpace2D<ElementType>::at(unsigned int x, unsigned int y)
{
	assert(x < mwidth);
	assert(y < mheight);

	return mdata[y * mheight + x];
}

template <typename ElementType>
const ElementType &ElementSpace2D<ElementType>::at(unsigned int x, unsigned int y) const
{
	return at(x, y);
}

template <typename ElementType>
ElementType &ElementSpace2D<ElementType>::at(unsigned int i)
{
	assert(i < mwidth * mheight);

	return mdata[i];
}

template <typename ElementType>
const ElementType &ElementSpace2D<ElementType>::at(unsigned int i) const
{
	return at(i);
}

template <typename ElementType>
void ElementSpace2D<ElementType>::for_each(std::function<void(unsigned int, ElementType &)> func)
{
	for (unsigned int i = 0; i < mwidth * mheight; i++)
	{
		func(i, at(i));
	}
}

template <typename ElementType>
void ElementSpace2D<ElementType>::for_each(std::function<void(unsigned int, unsigned int, ElementType &)> func)
{
	for (unsigned int y = 0; y < mheight; y++)
	{
		for (unsigned int x = 0; x < mwidth; x++)
		{
			func(x, y, at(x, y));
		}
	}
}

template <typename ElementType>
void ElementSpace2D<ElementType>::fill(ElementType value)
{
	std::function<void(unsigned int, ElementType &)> func = [value](unsigned int, ElementType &v)
	{
		v = value;
	};

	for_each(func);
}

template <typename ElementType>
void ElementSpace2D<ElementType>::destroyelements()
{
	if (mdata != nullptr)
	{
		delete mdata;
	}
}


#endif // ELEMENTSPACE2D_H
