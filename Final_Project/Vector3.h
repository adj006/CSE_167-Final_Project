#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include <string>
#include "Point.h"

class Vector3
{
public:
	double v3[3];
	Vector3::Vector3(double x = 0, double y = 0, double z = 0); //constructor with three 
	//parameters for the vector
	//coordinates
	Vector3 operator+(const Vector3& v); //overload operator '+' for addition
	Vector3 operator-(const Vector3& v); //overload operator '-' for subtraction
	Vector3 operator*(const Vector3& v); //overload operator '*' for multiplication
	//Vector3& operator=(const Point& p);	 //overload operator '=' for equality
	void negate(); //negation
	void scale(double s); //scale(multiplication with scalar value)
	double dot(const Vector3& v1, const Vector3& v2); //dot product, returns result
	Vector3& cross(const Vector3& v1, const Vector3& v2); //cross product, returns result
	//and puts it in calling vector
	double length(); //length of the vector
	void normalize(); //normalize the vector(make it so that its length is 
	//equal to one)
	void print(std::string comment); //print x, y and z components of the vector
	//after a comment string
};

#endif