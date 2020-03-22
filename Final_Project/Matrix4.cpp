#include "Matrix4.h"
#include <math.h>
#include <string>
#include <iostream>

using namespace std;

Matrix4::Matrix4()
{
	for (int i = 0; i<4; ++i)
	{
		for (int j = 0; j<4; ++j)
		{
			m[i][j] = 0;
		}
	}
}

Matrix4& Matrix4::operator=(const Matrix4& m2)
{
	if (this != &m2)
	{
		for (int i = 0; i<4; ++i)
		{
			for (int j = 0; j<4; ++j)
			{
				m[i][j] = m2.m[i][j];
			}
		}
	}
	return *this;
}

// return pointer to matrix elements
double* Matrix4::getPointer()
{
	return &m[0][0];
}

// set matrix to identity matrix
void Matrix4::identity()
{
	for (int i = 0; i<4; ++i)
	{
		for (int j = 0; j<4; ++j)
		{
			if (i == j)
				m[i][j] = 1.0;
			else
				m[i][j] = 0.0;
		}
	}
}

// transpose the matrix (mirror at diagonal)
void Matrix4::transpose()
{
	Matrix4 temp;
	for (int i = 0; i<4; ++i)
	{
		for (int j = 0; j<4; ++j)
		{
			temp.m[j][i] = m[i][j];
		}
	}
	*this = temp;  // copy temporary values to this matrix
}

// Creates a rotation matrix which rotates about the y axis.v3.
// angle is expected in degrees
void Matrix4::makeRotateY(double angle)
{
	angle = angle / 180.0 * M_PI;  // convert from degrees to radians
	identity();
	m[0][0] = cos(angle);
	m[0][2] = sin(angle);
	m[2][0] = -sin(angle);
	m[2][2] = cos(angle);
}

Matrix4 Matrix4::operator*(const Matrix4& m2)
{
	Matrix4 m1;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				m1.m[i][j] += m[i][k] * m2.m[k][j];
			}
		}
	}
	return m1;

}

Vector4 Matrix4::operator*(const Vector4& v)
{
	Vector4 v1;
	for (int i = 0; i < 4; i++)
	{
		for (int k = 0; k < 4; k++)
		{
			v1.v4[i] += m[i][k] * v.v4[k];
		}
	}
	return v1;
}

void Matrix4::makeRotateX(double angle)
{
	angle = angle / 180.0 * M_PI;  // convert from degrees to radians
	identity();
	m[1][1] = cos(angle);
	m[2][1] = sin(angle);
	m[1][2] = -sin(angle);
	m[2][2] = cos(angle);
}

void Matrix4::makeRotateZ(double angle)
{
	angle = angle / 180.0 * M_PI;  // convert from degrees to radians
	identity();
	m[0][0] = cos(angle);
	m[1][0] = sin(angle);
	m[0][1] = -sin(angle);
	m[1][1] = cos(angle);
}

void Matrix4::makeRotate(double angle, const Vector3& axis)
{
	angle = angle / 180.0 * M_PI;
	identity();
	m[0][0] = 1 + (1 - cos(angle))*(pow(axis.v3[0], 2) - 1);
	m[0][1] = -axis.v3[2] * sin(angle) + (1 - cos(angle))*(axis.v3[0] * axis.v3[1]);
	m[0][2] = (axis.v3[1] * sin(angle)) + (1 - cos(angle))*(axis.v3[0])*(axis.v3[2]);
	m[1][0] = (axis.v3[2] * sin(angle)) + (1 - cos(angle))*(axis.v3[1])*(axis.v3[0]);
	m[1][1] = 1 + (1 - cos(angle)) * (pow(axis.v3[1], 2) - 1);
	m[1][2] = -axis.v3[0] * sin(angle) + (1 - cos(angle))*axis.v3[1] * axis.v3[2];
	m[2][0] = -axis.v3[1] * sin(angle) + (1 - cos(angle))*axis.v3[2] * axis.v3[0];
	m[2][1] = axis.v3[0] * sin(angle) + (1 - cos(angle))*axis.v3[2] * axis.v3[1];
	m[2][2] = 1 + (1 - cos(angle)) * (pow(axis.v3[2], 2) - 1);
}

void Matrix4::makeScale(double sx, double sy, double sz)
{
	identity();
	m[0][0] = sx;
	m[1][1] = sy;
	m[2][2] = sz;
}

void Matrix4::makeTranslate(double tx, double ty, double tz)
{
	identity();
	m[0][3] = tx;
	m[1][3] = ty;
	m[2][3] = tz;
}

void Matrix4::print(string comment)
{
	cout << comment << " - \n ";
	for (int i = 0; i < 4; i++)
	{
		cout << "| ";
		for (int j = 0; j < 4; j++)
		{
			cout << m[i][j];
			if (j < 3)
			{
				cout << ", ";
			}
		}
		cout << " |" << '\n';
	}
}

void Matrix4::viewportMatrix(int x0, int x1, int y0, int y1)
{
	identity();
	m[0][0] = (x1 - x0) / 2;
	m[1][1] = (y1 - y0) / 2;
	m[2][2] = 0.5;
	m[0][3] = (x0 + x1) / 2;
	m[1][3] = (y0 + y1) / 2;
	m[2][3] = 0.5;
}
void Matrix4::perProjectionMatrix(int a, int fov, double n, double f)
{
	m[0][0] = 1 / (a * tan((fov / 2) * (M_PI / 180)));
	m[1][1] = 1 / tan((fov / 2) * (M_PI / 180));
	m[2][2] = (n + f) / (n - f);
	m[2][3] = (2 * n * f) / (n - f);
	m[3][2] = -1;
}