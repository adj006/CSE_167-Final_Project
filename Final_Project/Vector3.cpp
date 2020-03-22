#include "Vector3.h"
#include <math.h>
#include <iostream>

using namespace std;

Vector3::Vector3(double x, double y, double z)
{
	v3[0] = x;
	v3[1] = y;
	v3[2] = z;
}

Vector3 Vector3::operator+(const Vector3& v)
{
	Vector3 temp;

	temp.v3[0] = v3[0] + v.v3[0];
	temp.v3[1] = v3[1] + v.v3[1];
	temp.v3[2] = v3[2] + v.v3[2];
	return temp;
}

Vector3 Vector3::operator-(const Vector3& v)
{
	Vector3 temp;
	temp.v3[0] = v3[0] - v.v3[0];
	temp.v3[1] = v3[1] - v.v3[1];
	temp.v3[2] = v3[2] - v.v3[2];
	return temp;
}

Vector3 Vector3::operator*(const Vector3& v)
{
	Vector3 temp;

	temp.v3[0] = v3[0] * v.v3[0];
	temp.v3[1] = v3[1] * v.v3[1];
	temp.v3[2] = v3[2] * v.v3[2];
	return temp;
}

/*
Vector3& Vector3::operator=(const Point& p)
{
	this->v3[0] = p.x;
	this->v3[1] = p.y;
	this->v3[2] = p.z;
	return *this;
}
*/

void Vector3::negate()
{
	v3[0] = -v3[0];
	v3[1] = -v3[1];
	v3[2] = -v3[2];
}

void Vector3::scale(double s)
{
	v3[0] = s * v3[0];
	v3[1] = s * v3[1];
	v3[2] = s * v3[2];
}

double Vector3::dot(const Vector3& v1, const Vector3& v2)
{
	return ((v1.v3[0] * v2.v3[0]) + (v1.v3[1] * v2.v3[1]) + (v1.v3[2] * v2.v3[2]));
}

Vector3& Vector3::cross(const Vector3& v1, const Vector3& v2)
{
	Vector3  v = Vector3(v1.v3[1] * v2.v3[2] - v1.v3[2] * v2.v3[1], v1.v3[2] * v2.v3[0] - v1.v3[0] * v2.v3[2], v1.v3[0] * v2.v3[1] - v1.v3[1] * v2.v3[0]);
	return v;
}

double Vector3::length()
{
	return (sqrt(pow(v3[0], 2) + pow(v3[1], 2) + pow(v3[2], 2)));
}

void Vector3::normalize()
{
	if (0 != length())
	{
		double length = this->length();
		v3[0] = v3[0] / length;
		v3[1] = v3[1] / length;
		v3[2] = v3[2] / length;
	}
}

void Vector3::print(string comment)
{
	cout << comment << " - [ " << v3[0] << ", " << v3[1] << ", " << v3[2] << " ]" << '\n';
}