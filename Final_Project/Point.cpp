#include "Point.h"

using namespace std;



Point::Point(float fx, float fy, float fz)
{
	x = fx;
	y = fy;
	z = fz;
}

Point& Point::operator=(const Point& pnts)
{
	if (this != &pnts)
	{
		x = pnts.x;
		y = pnts.y;
		z = pnts.z;
	}
	return *this;
}

/*
Point Point::operator-(const Point& p)
{
	Point tmp;
	tmp.x = x - p.x;
	tmp.y = y - p.y;
	tmp.z = y - p.z;
	return tmp;
}
*/