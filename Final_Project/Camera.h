#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

using namespace std;

class Camera
{
public:
	Vector3 e;
	Vector3 up;
	Vector3 d;

	Vector3 x;
	Vector3 y;
	Vector3 z;

	Matrix4 cam;

	Camera();

	Camera(Vector3 e1, Vector3 up1, Vector3 d1);
	GLdouble * Camera::getGLMatrix();
	void inverse();
};

#endif