#ifndef _POINT_H_
#define _POINT_H_

#include "Vector3.h"

class Point
{
public:
	float x;
	float y;
	float z;
	Point(float fx = 0, float fy = 0, float fz = 0);
	Point& operator=(const Point& p);
	//Point operator-(const Point& p);
};

#endif