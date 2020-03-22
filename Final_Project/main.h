#ifndef _MAIN_H_
#define _MAIN_H_

//#include "Node.h"
//#include "Group.h"
//#include "MatrixTransform.h"
//#include "Geode.h"
//#include "Cube.h"
//#include "Sphere.h"
//#include "Robot.h"
//#include "PointCloud.h"
//#include "Light.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string.h>
#include <GL/glut.h>
#include "Window.h"
#include "Matrix4.h"
#include "Camera.h"
#include "Point.h"
#include "Particle.h"

#define M_PI atan(1)*4

namespace Globals
{
	extern Matrix4 m;
	extern GLuint texture[5];
	extern vector<Particle> particles;
	//extern int bound;
	//extern int pointCloud;
};

#endif