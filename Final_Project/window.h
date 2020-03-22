#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "main.h"
#include "math.h"


class Window	  // OpenGL output window related routines
{
public:
	static int width, height; 	            // window size
	//static GLfloat specular[], shininess[], diffuse[];
	//static int perPixel;

	//static void parser();
	static void processNormalKeys(unsigned char key, int x, int y);
	//static void specialKeysProcess(int key, int x, int y);
	static void idleCallback(void);
	static void reshapeCallback(int, int);
	static void displayCallback(void);
	//static Vector3 getMax(int pointCloud);
	//static Vector3 getMin(int pointCloud);
	//static void createPointCloud();
	//static Vector3 trackBallMapping(int x, int y);
	//static void onMouseBtn(int btn, int s, int x, int y);
	//static void onMouseMove(int x, int y);
};

#endif

