#ifndef _VECTOR4_H_
#define _VECTOR4_H_

#include <string>

class Vector4
{
protected:
	//double x;
	//double y;
	//double z;
	//double w;
	//double v4[4];
public:
	double v4[4];
	//Vector4() : v4[0](0), v4[1](0), v4[2](0), v4[3](0) {}
	Vector4(double x = 0, double y = 0, double z = 0, double w = 0); //constructor 
	//with four parameters for the vector coordinates
	Vector4 operator+(const Vector4& v); //overload operator '+' for addition
	Vector4 operator-(const Vector4& v); //overload operator '-' for subtraction
	void dehomogenize(); //dehomogenize the vector(scale it so that its fourth
	//component is equal to one)
	void print(std::string comment); //print x, y, z and w components of the vector 
	//after a comment string
};

#endif