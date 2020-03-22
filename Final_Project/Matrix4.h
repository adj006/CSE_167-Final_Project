#ifndef _MATRIX4_H_
#define _MATRIX4_H_

#include "Vector3.h"
#include "Vector4.h"

#define M_PI atan(1)*4

class Matrix4
{
public:
	double m[4][4];   // matrix elements; first index is for rows, second for columns (row-major)
	Matrix4();
	Matrix4& operator=(const Matrix4& m2);
	double* getPointer();
	void identity();
	void transpose();
	void makeRotateY(double angle);
	Matrix4 operator*(const Matrix4& m2); //multiply matrix with matrix
	Vector4 operator*(const Vector4& v); //multiply matrix with vector
	void makeRotateX(double angle); //make rotation matrix about X axis with angle in degrees(note that 
	//the sin / cos functions in C++ expect radians so you need to convert to radians within the function)
	void makeRotateZ(double angle); //make rotation matrix about Z axis with angle in degrees
	void makeRotate(double angle, const Vector3& axis); //Make a rotation matrix about an arbitrary axis
	void makeScale(double sx, double sy, double sz); //make a non - uniform scaling matrix
	void makeTranslate(double tx, double ty, double tz); //make a translation matrix
	void print(std::string comment); //print the matrix(display all
	//16 matrix components numerically on the screen in a 4x4 array)

	void viewportMatrix(int x0, int x1, int y0, int y1); //viewport matrix
	void perProjectionMatrix(int a, int fov, double n, double f); //perspective projection matrix
	//void orthoProjectionMatrix(int d);	//orthographic projection matrix
};

#endif