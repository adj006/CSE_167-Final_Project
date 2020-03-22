#include "Vector4.h"
#include <math.h>
#include <iostream>

using namespace std;

Vector4::Vector4(double x, double y, double z, double w)
{
	v4[0] = x;
	v4[1] = y;
	v4[2] = z;
	v4[3] = w;
}

Vector4 Vector4::operator+(const Vector4& v)
{
	Vector4 temp;

	temp.v4[0] = v4[0] + v.v4[0];
	temp.v4[1] = v4[1] + v.v4[1];
	temp.v4[2] = v4[2] + v.v4[2];
	temp.v4[3] = v4[3] + v.v4[3];
	return temp;
}

Vector4 Vector4::operator-(const Vector4& v)
{
	Vector4 temp;

	temp.v4[0] = v4[0] - v.v4[0];
	temp.v4[1] = v4[1] - v.v4[1];
	temp.v4[2] = v4[2] - v.v4[2];
	temp.v4[3] = v4[3] - v.v4[3];
	return temp;
}

void Vector4::dehomogenize()
{
	double d = v4[3];
	v4[0] = v4[0] / d;
	v4[1] = v4[1] / d;
	v4[2] = v4[2] / d;
	v4[3] = v4[3] / d;
}

void Vector4::print(string comment)
{
	cout << comment << " - [ " << v4[0] << ", " << v4[1] << ", " << v4[2] << ", " << v4[3] << " ]" << endl;
}