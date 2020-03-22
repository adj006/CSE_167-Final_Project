#include "Camera.h"

using namespace std;

Camera::Camera()
{
	cam.identity();
}

Camera::Camera(Vector3 e1, Vector3 up1, Vector3 d1)
{
	cam.identity();

	e = e1;
	up = up1;
	d = d1;

	x = Vector3(0, 0, 0);
	y = Vector3(0, 0, 0);
	z = Vector3(0, 0, 0);

	Vector3 tempVecZ = e - d;
	tempVecZ.normalize();
	z = tempVecZ;

	Vector3 tempVecX;
	tempVecX = tempVecX.cross(up, z);
	tempVecX.normalize();
	x = tempVecX;

	y = tempVecX.cross(z, x);

	cam.identity();

	cam.m[0][0] = x.v3[0];
	cam.m[1][0] = x.v3[1];
	cam.m[2][0] = x.v3[2];

	cam.m[0][1] = y.v3[0];
	cam.m[1][1] = y.v3[1];
	cam.m[2][1] = y.v3[2];

	cam.m[0][2] = z.v3[0];
	cam.m[1][2] = z.v3[1];
	cam.m[2][2] = z.v3[2];

	cam.m[0][3] = e.v3[0];
	cam.m[1][3] = e.v3[1];
	cam.m[2][3] = e.v3[2];
}

GLdouble * Camera::getGLMatrix()
{
	GLdouble tempArr[16];

	tempArr[0] = cam.m[0][0];
	tempArr[1] = cam.m[1][0];
	tempArr[2] = cam.m[2][0];
	tempArr[3] = cam.m[3][0];

	tempArr[4] = cam.m[0][1];
	tempArr[5] = cam.m[1][1];
	tempArr[6] = cam.m[2][1];
	tempArr[7] = cam.m[3][1];

	tempArr[8] = cam.m[0][2];
	tempArr[9] = cam.m[1][2];
	tempArr[10] = cam.m[2][2];
	tempArr[11] = cam.m[3][2];

	tempArr[12] = cam.m[0][3];
	tempArr[13] = cam.m[1][3];
	tempArr[14] = cam.m[2][3];
	tempArr[15] = cam.m[3][3];

	return tempArr;
}

void Camera::inverse()
{
	Matrix4 t;
	t.identity();
	t.makeTranslate(-e.v3[0], -e.v3[1], -e.v3[2]);

	Matrix4 r;
	r.identity();
	r = cam;
	r.m[0][3] = 0;
	r.m[1][3] = 0;
	r.m[2][3] = 0;
	r.m[3][0] = 0;
	r.m[3][1] = 0;
	r.m[3][2] = 0;
	r.transpose();

	cam = r * t;
}