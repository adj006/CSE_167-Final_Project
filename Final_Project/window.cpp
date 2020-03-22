#define _CRT_SECURE_NO_WARNINGS

#include "Window.h"
#include <cmath>

using namespace std;

int Window::width = 1520;   // set window width in pixels here
int Window::height = 800;   // set window height in pixels here

double time = 0.0;				//time is for the frequency of the sin curve of patch
int eMap = 0;
double delta = 0.00001;		//offset to calculate the normals of the patch
double rX = -100;				//rains x-variable
double rY = 100.0;				//rains y-variable
double rZ = -100;				//rains z-Variable
double vRain = 0.0;				//velocity of the rain
int iRain = 0;					//rain toggle indicator

//point offset u/v to calculate normal
Point pU;		
Point pV;

//tangent of the curve on the u-axis/v-axis
Vector3 tanU;
Vector3 tanV;

//normals of the correspoinding vertecies drawed with gl_QUADS
Vector3 n0;		
Vector3 n1;		
Vector3 n2;		
Vector3 n3;		

Matrix4 glmatrix;

Vector3 e(-10.0, 20.0, -30.0);	//center of projection
Vector3 up(0.0, 20.0, 0.0);	//up-vector
Vector3 d(0.0, 0.0, 0.0);	//look at

Camera camera(e, up, d);

Point Points[4][4] = {
		{
			{ -30, 10, 0 },
			{ -25, 10, 0 },
			{ -15, 10, 0 },
			{ -10, 10, 0 }
		},
		{
			{ -30, 5, 0 },
			{ -25, 5, 0 },
			{ -15, 5, 0 },
			{ -10, 5, 0 }
		},
		{
			{ -30, -5, 0 },
			{ -25, -5, 0 },
			{ -15, -5, 0 },
			{ -10, -5, 0 }
		},
		{
			{ -30, -10, 0 },
			{ -25, -10, 0 },
			{ -15, -10, 0 },
			{ -10, -10, 0 }
		}
};

Point Points2[4][4] = {
		{
			{ Points[0][3].x, 10, 0 },
			{ -5, 10, 0 },
			{ 5, 10, 0 },
			{ 10, 10, 0 }
		},
		{
			{ Points[1][3].x, 5, 0 },
			{ -5, 5, 0 },
			{ 5, 5, 0 },
			{ 10, 5, 0 }
		},
		{
			{ Points[2][3].x, -5, 0 },
			{ -5, -5, 0 },
			{ 5, -5, 0 },
			{ 10, 0, 0 }
		},
		{
			{ Points[3][3].x, -10, 0 },
			{ -5, -10, 0 },
			{ 5, -10, 0 },
			{ 10, -10, 0 }
		}
};

Point Points3[4][4] = {
		{
			{ Points2[0][3].x, 10, 0 },
			{ 15, 10, 0 },
			{ 25, 10, 0 },
			{ 30, 10, 0 }
		},
		{
			{ Points2[1][3].x, 5, 0 },
			{ 15, 5, 0 },
			{ 25, 5, 0 },
			{ 30, 5, 0 }
		},
		{
			{ Points2[2][3].x, -5, 0 },
			{ 15, -5, 0 },
			{ 25, -5, 0 },
			{ 30, -5, 0 }
		},
		{
			{ Points2[3][3].x, -10, 0 },
			{ 15, -10, 0 },
			{ 25, -10, 0 },
			{ 30, -10, 0 }
		}
};

// the level of detail of the surface
unsigned int LOD = 100;

Point CalculateU(float t, int row, Point Points[4][4]) {

	// the final point
	Point p;

	// the t value inverted
	float it = 1.0f - t;

	// calculate blending functions
	float b0 = t*t*t;
	float b1 = 3 * t*t*it;
	float b2 = 3 * t*it*it;
	float b3 = it*it*it;

	// sum the effects of the Points and their respective blending functions
	p.x = b0*Points[row][0].x +
		b1*Points[row][1].x +
		b2*Points[row][2].x +
		b3*Points[row][3].x;

	p.y = b0*Points[row][0].y +
		b1*Points[row][1].y +
		b2*Points[row][2].y +
		b3*Points[row][3].y;

	p.z = b0*Points[row][0].z +
		b1*Points[row][1].z +
		b2*Points[row][2].z +
		b3*Points[row][3].z;

	return p;
}

Point CalculateV(float t, Point* pnts) {
	Point p;

	// the t value inverted
	float it = 1.0f - t;

	// calculate blending functions
	float b0 = t*t*t;
	float b1 = 3 * t*t*it;
	float b2 = 3 * t*it*it;
	float b3 = it*it*it;

	// sum the effects of the Points and their respective blending functions
	p.x = b0*pnts[0].x +
		b1*pnts[1].x +
		b2*pnts[2].x +
		b3*pnts[3].x;

	p.y = b0*pnts[0].y +
		b1*pnts[1].y +
		b2*pnts[2].y +
		b3*pnts[3].y;

	p.z = b0*pnts[0].z +
		b1*pnts[1].z +
		b2*pnts[2].z +
		b3*pnts[3].z;

	return p;
}

Point Calculate(float u, float v, Point Points[4][4]) {

	Point temp[4];

	// calculate each point on our final v curve
	temp[0] = CalculateU(u, 0, Points);
	temp[1] = CalculateU(u, 1, Points);
	temp[2] = CalculateU(u, 2, Points);
	temp[3] = CalculateU(u, 3, Points);

	return CalculateV(v, temp);
}

void Window::processNormalKeys(unsigned char key, int x, int y)
{
	Matrix4 trans;
	trans.identity();

	Matrix4 rot;
	rot.identity();

	switch (key)
	{
		/*
		* moves Model in the x-direction
		*/
	case 'x':
		trans.makeTranslate(-1, 0, 0);
		Globals::m = trans * Globals::m;
		break;
	case 'X':
		trans.makeTranslate(1, 0, 0);
		Globals::m = trans * Globals::m;
		break;

		/*
		* moves Model in the y-direction
		*/
	case 'y':
		trans.makeTranslate(0, -1, 0);
		Globals::m = trans * Globals::m;
		break;
	case 'Y':
		trans.makeTranslate(0, 1, 0);
		Globals::m = trans * Globals::m;
		break;

		/*
		* moves Model in the z-direction
		*/
	case 'z':
		trans.makeTranslate(0, 0, -1);
		Globals::m = trans * Globals::m;
		break;
	case 'Z':
		trans.makeTranslate(0, 0, 1);
		Globals::m = trans * Globals::m;
		break;

		/*
		* rotates the Model on the y-axis
		*/
	case 's':
		rot.makeRotateY(-5);
		Globals::m = Globals::m * rot;
		break;
	case 'S':
		rot.makeRotateY(5);
		Globals::m = Globals::m * rot;
		break;

		/*
		* Toggles the envirnment mapping
		*/
	case 'e':
		if (eMap == 0)
			eMap = 1;
		else
			eMap = 0;
		break;

		/*
		* Controls the velocity of the rain
		*/
	case 'r':
		vRain -= 2.5;
		break;
	case 'R':
		vRain += 2.5;
		break;

		/*
		* Toggle on/off the rain
		*/
	case 'o':
		if (iRain == 0)
			iRain = 1;
		else
			iRain = 0;
		break;
	}
}

/*
void Window::specialKeysProcess(int key, int x, int y)
{

	switch (key)
	{
	case GLUT_KEY_F1:
		glutDisplayFunc(displayCallback_cube);
		glutIdleFunc(idleCallback_cube);
		break;
	case GLUT_KEY_F2:
		glutDisplayFunc(displayCallback_house);
		glutIdleFunc(idleCallback_house);
		whichCamera = 1;
		break;
	case GLUT_KEY_F3:
		glutDisplayFunc(displayCallback_house);
		glutIdleFunc(idleCallback_house);
		whichCamera = 2;
		break;
	case GLUT_KEY_F4:
		parser("Bunny.xyz");
		glutDisplayFunc(displayCallback_bunny);
		glutIdleFunc(idleCallback_bunny);
		firstTime = 1;
		getMin();
		getMax();

		break;
	case GLUT_KEY_F5:
		parser("Dragon.xyz");
		glutDisplayFunc(displayCallback_dragon);
		glutIdleFunc(idleCallback_dragon);
		firstTime = 1;
		getMin();
		getMax();

		break;
	}
}
*/

//----------------------------------------------------------------------------
// Callback method called when system is idle.
void Window::idleCallback()
{
	/*
	* boundry check for time frequency
	*/
	if (time == 360)
		time = 0;

	for (int i = 0; i <= 100; i++)
	{
		for (int j = 0; j <= 100; j++)
		{
			//cout << "hello" << endl;
			//Rain
			/*
			* boundry checks for rain
			*/
			if (Globals::particles[i*j].position.v4[1] < -150.0)
				Globals::particles[i*j].position.v4[1] = 150.0;
			else
				Globals::particles[i*j].update();
			
		}
	}

	displayCallback();         // call display routine to show the object
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when graphics window is resized by the user
void Window::reshapeCallback(int w, int h)
{
	cerr << "Window::reshapeCallback called" << endl;
	width = w;
	height = h;
	glViewport(0, 0, w, h);  // set new viewport size
	//glViewport(GL_FULLS);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, double(width) / (double)height, 1.0, 1000.0); // set perspective projection viewing frustum
	//glTranslatef(-10, -50, -50);    // move camera back 30 units so that it looks at the origin (or else it's in the origin)
	glMatrixMode(GL_MODELVIEW);
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when window readraw is necessary or when glutPostRedisplay() was called.
void Window::displayCallback()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);  // make sure we're in Modelview mode

	//setting the camera in object coordinates
	camera.inverse();
	//glLoadMatrixd(camera.cam.getPointer());

	// Tell OpenGL what ModelView matrix to use:
	glmatrix = Globals::m;
	Matrix4 tmp;
	tmp = camera.cam * glmatrix;
	
	//glmatrix.transpose();
	tmp.transpose();

	glLoadMatrixd(tmp.getPointer());
	camera.inverse();

	//glColor3f(1.0, 0.0, 0.0);		// This makes the cube green; the parameters are for red, green and blue. 
	//glDisable(GL_COLOR_MATERIAL);
	
	// Draw all Bexier curves
	//glBegin(GL_QUADS);
	//glBegin(GL_QUADS);
	
	// set the current drawing color to white to use original texture colors
	//glColor3f(1, 1, 1);




	//cout << time << endl;
	//for animation
	Points[0][0].z = -8*sin(time); time += .01;
	Points[1][0].z = -8*sin(time); time += .01;
	Points[2][0].z = -8*sin(time); time += .01;
	Points[3][0].z = -8*sin(time); time += .01;

	Points[0][1].z = 8*sin(time); time += .01;
	Points[1][1].z = 8*sin(time); time += .01;
	Points[2][1].z = 8*sin(time); time += .01;
	Points[3][1].z = 8*sin(time); time += .01;
	Points[0][2].z = -8*sin(time); time += .01;
	Points[1][2].z = -8*sin(time); time += .01;
	Points[2][2].z = -8*sin(time); time += .01;
	Points[3][2].z = -8*sin(time); time += .01;
	
	Points[0][3].z = 8*sin(time); time += .01;
	Points[1][3].z = 8*sin(time); time += .01;
	Points[2][3].z = 8*sin(time); time += .01;
	Points[3][3].z = 8*sin(time); time += .01;
	
	
	Points2[0][0].z = Points[0][3].z;
	Points2[1][0].z = Points[1][3].z;
	Points2[2][0].z = Points[2][3].z;
	Points2[3][0].z = Points[3][3].z;
	
	//Points2[0][1].y = 8 * sin(time); time += .1;
	//Points2[1][1].y = 8 * sin(time); time += .1;
	//Points2[2][1].y = 8 * sin(time); time += .1;
	//Points2[3][1].y = 8 * sin(time); time += .1;
	
	/*
		1st row
	*/
	Vector3 v;
	v.v3[0] = Points[0][3].x - Points[0][2].x;
	v.v3[1] = Points[0][3].y - Points[0][2].y;
	v.v3[2] = Points[0][3].z - Points[0][2].z;
	
	Points2[0][1].x = v.v3[0] + Points2[0][0].x;
	Points2[0][1].y = v.v3[1] + Points2[0][0].y;
	Points2[0][1].z = v.v3[2] + Points2[0][0].z;
	
	/*
		2nd row
	*/
	v.v3[0] = Points[1][3].x - Points[1][2].x;
	v.v3[1] = Points[1][3].y - Points[1][2].y;
	v.v3[2] = Points[1][3].z - Points[1][2].z;
	
	Points2[1][1].x = v.v3[0] + Points2[1][0].x;
	Points2[1][1].y = v.v3[1] + Points2[1][0].y;
	Points2[1][1].z = v.v3[2] + Points2[1][0].z;
	
	/*
		3rd row
	*/
	v.v3[0] = Points[2][3].x - Points[2][2].x;
	v.v3[1] = Points[2][3].y - Points[2][2].y;
	v.v3[2] = Points[2][3].z - Points[2][2].z;
	
	Points2[2][1].x = v.v3[0] + Points2[2][0].x;
	Points2[2][1].y = v.v3[1] + Points2[2][0].y;
	Points2[2][1].z = v.v3[2] + Points2[2][0].z;
	
	/*
		4th row
	*/
	v.v3[0] = Points[3][3].x - Points[3][2].x;
	v.v3[1] = Points[3][3].y - Points[3][2].y;
	v.v3[2] = Points[3][3].z - Points[3][2].z;
	
	Points2[3][1].x = v.v3[0] + Points2[3][0].x;
	Points2[3][1].y = v.v3[1] + Points2[3][0].y;
	Points2[3][1].z = v.v3[2] + Points2[3][0].z;
	
	
	
	Points2[0][2].z = 8 * sin(time); time += .01;
	Points2[1][2].z = 8 * sin(time); time += .01;
	Points2[2][2].z = 8 * sin(time); time += .01;
	Points2[3][2].z = 8 * sin(time); time += .01;
	
	Points2[0][3].z = -8 * sin(time); time += .01;
	Points2[1][3].z = -8 * sin(time); time += .01;
	Points2[2][3].z = -8 * sin(time); time += .01;
	Points2[3][3].z = -8 * sin(time); time += .01;
	
	Points3[0][0].z = Points2[0][3].z;
	Points3[1][0].z = Points2[1][3].z;
	Points3[2][0].z = Points2[2][3].z;
	Points3[3][0].z = Points2[3][3].z;
	
	//Points3[0][1].y = 8 * sin(time); time += .1;
	//Points3[1][1].y = 8 * sin(time); time += .1;
	//Points3[2][1].y = 8 * sin(time); time += .1;
	//Points3[3][1].y = 8 * sin(time); time += .1;
	
	
	
	/*
	1st row
	*/
	v.v3[0] = Points2[0][3].x - Points2[0][2].x;
	v.v3[1] = Points2[0][3].y - Points2[0][2].y;
	v.v3[2] = Points2[0][3].z - Points2[0][2].z;
	
	Points3[0][1].x = v.v3[0] + Points3[0][0].x;
	Points3[0][1].y = v.v3[1] + Points3[0][0].y;
	Points3[0][1].z = v.v3[2] + Points3[0][0].z;
	
	/*
	2nd row
	*/
	v.v3[0] = Points2[1][3].x - Points2[1][2].x;
	v.v3[1] = Points2[1][3].y - Points2[1][2].y;
	v.v3[2] = Points2[1][3].z - Points2[1][2].z;
	
	Points3[1][1].x = v.v3[0] + Points3[1][0].x;
	Points3[1][1].y = v.v3[1] + Points3[1][0].y;
	Points3[1][1].z = v.v3[2] + Points3[1][0].z;
	
	/*
	3rd row
	*/
	v.v3[0] = Points2[2][3].x - Points2[2][2].x;
	v.v3[1] = Points2[2][3].y - Points2[2][2].y;
	v.v3[2] = Points2[2][3].z - Points2[2][2].z;
	
	Points3[2][1].x = v.v3[0] + Points3[2][0].x;
	Points3[2][1].y = v.v3[1] + Points3[2][0].y;
	Points3[2][1].z = v.v3[2] + Points3[2][0].z;
	
	/*
	4th row
	*/
	v.v3[0] = Points2[3][3].x - Points2[3][2].x;
	v.v3[1] = Points2[3][3].y - Points2[3][2].y;
	v.v3[2] = Points2[3][3].z - Points2[3][2].z;
	
	Points3[3][1].x = v.v3[0] + Points3[3][0].x;
	Points3[3][1].y = v.v3[1] + Points3[3][0].y;
	Points3[3][1].z = v.v3[2] + Points3[3][0].z;
	
	Points3[0][2].z = 8 * sin(time); time += .01;
	Points3[1][2].z = 8 * sin(time); time += .01;
	Points3[2][2].z = 8 * sin(time); time += .01;
	Points3[3][2].z = 8 * sin(time); time += .01;

	// use the parametric time value 0 to 1
	for (int i = 0; i < LOD - 1; ++i) {

		// calculate the parametric u value
		float u0 = (float)i / (LOD - 1);
		float u1 = (float)(i + 1) / (LOD - 1);

		for (int j = 0; j < LOD - 1; ++j) {

			// calculate the parametric v value
			float v0 = (float)j / (LOD - 1);
			float v1 = (float)(j + 1) / (LOD - 1);

			/*
			* calculate the points and normals on the surface of patch 1
			*/
			//1st point
			Point p0 = Calculate(u0, v0, Points);
			pU = Calculate(u0 + delta, v0, Points);
			pV = Calculate(u0, v0 + delta, Points);

			//tanU
			tanU.v3[0] = pU.x - p0.x;
			tanU.v3[1] = pU.y - p0.y;
			tanU.v3[2] = pU.z - p0.z;
			//tanU = pU - p0;

			//tanV
			tanV.v3[0] = pV.x - p0.x;
			tanV.v3[1] = pV.y - p0.y;
			tanV.v3[2] = pV.z - p0.z;
			//tanV = pV - p0;

			tanU.normalize();
			tanV.normalize();

			//1st normal
			n0 = tanU.cross(tanU, tanV);


			//2nd point
			Point p1 = Calculate(u1, v0, Points);
			pU = Calculate(u1 + delta, v0, Points);
			pV = Calculate(u1, v0 + delta, Points);

			//tanU
			tanU.v3[0] = pU.x - p1.x;
			tanU.v3[1] = pU.y - p1.y;
			tanU.v3[2] = pU.z - p1.z;
			//tanU = pU - p1;

			//tanV
			tanV.v3[0] = pV.x - p1.x;
			tanV.v3[1] = pV.y - p1.y;
			tanV.v3[2] = pV.z - p1.z;
			//tanV = pV - p1;

			tanU.normalize();
			tanV.normalize();

			//2nd normal
			n1 = tanU.cross(tanU, tanV);


			//3rd point
			Point p2 = Calculate(u1, v1, Points);
			pU = Calculate(u1 + delta, v1, Points);
			pV = Calculate(u1, v1 + delta, Points);

			//tanU
			tanU.v3[0] = pU.x - p2.x;
			tanU.v3[1] = pU.y - p2.y;
			tanU.v3[2] = pU.z - p2.z;
			//tanU = pU - p2;

			//tanV
			tanV.v3[0] = pV.x - p2.x;
			tanV.v3[1] = pV.y - p2.y;
			tanV.v3[2] = pV.z - p2.z;
			//tanV = pV - p2;

			tanU.normalize();
			tanV.normalize();

			//3rd normal
			n2 = tanU.cross(tanU, tanV);


			//4th point
			Point p3 = Calculate(u0, v1, Points);
			pU = Calculate(u0 + delta, v1, Points);
			pV = Calculate(u0, v1 + delta, Points);

			//tanU
			tanU.v3[0] = pU.x - p3.x;
			tanU.v3[1] = pU.y - p3.y;
			tanU.v3[2] = pU.z - p3.z;

			//tanV
			tanV.v3[0] = pV.x - p3.x;
			tanV.v3[1] = pV.y - p3.y;
			tanV.v3[2] = pV.z - p3.z;

			tanU.normalize();
			tanV.normalize();

			//4th normal
			n3 = tanU.cross(tanU, tanV);

			glColor3f(1.0, 0.0, 0.0);		// This makes the cube green; the parameters are for red, green and blue.
			glBegin(GL_QUADS);
			/*
			* draw points and normals
			*/
			//bottom left corner
			glNormal3f(n0.v3[0], n0.v3[1], n0.v3[2]);
			glVertex3f(p0.x, p0.y, p0.z);

			//bottom right corner
			glNormal3f(n1.v3[0], n1.v3[1], n1.v3[2]);
			glVertex3f(p1.x, p1.y, p1.z);

			//top right corner
			glNormal3f(n2.v3[0], n2.v3[1], n2.v3[2]);
			glVertex3f(p2.x, p2.y, p2.z);

			//top left corner
			glNormal3f(n3.v3[0], n3.v3[1], n3.v3[2]);
			glVertex3f(p3.x, p3.y, p3.z);

			//glEnd();

			/*
			* calculate the points and normals on the surface of patch 2
			*/
			//1st point
			Point p0_2 = Calculate(u0, v0, Points2);
			pU = Calculate(u0 + delta, v0, Points2);
			pV = Calculate(u0, v0 + delta, Points2);

			//tanU
			tanU.v3[0] = pU.x - p0_2.x;
			tanU.v3[1] = pU.y - p0_2.y;
			tanU.v3[2] = pU.z - p0_2.z;
			//tanU = pU - p0;

			//tanV
			tanV.v3[0] = pV.x - p0_2.x;
			tanV.v3[1] = pV.y - p0_2.y;
			tanV.v3[2] = pV.z - p0_2.z;
			//tanV = pV - p0;

			tanU.normalize();
			tanV.normalize();

			//1st normal
			n0 = tanU.cross(tanU, tanV);


			//2nd point
			Point p1_2 = Calculate(u1, v0, Points2);
			pU = Calculate(u1 + delta, v0, Points2);
			pV = Calculate(u1, v0 + delta, Points2);

			//tanU
			tanU.v3[0] = pU.x - p1_2.x;
			tanU.v3[1] = pU.y - p1_2.y;
			tanU.v3[2] = pU.z - p1_2.z;
			//tanU = pU - p1;

			//tanV
			tanV.v3[0] = pV.x - p1_2.x;
			tanV.v3[1] = pV.y - p1_2.y;
			tanV.v3[2] = pV.z - p1_2.z;
			//tanV = pV - p1;

			tanU.normalize();
			tanV.normalize();

			//2nd normal
			n1 = tanU.cross(tanU, tanV);


			//3rd point
			Point p2_2 = Calculate(u1, v1, Points2);
			pU = Calculate(u1 + delta, v1, Points2);
			pV = Calculate(u1, v1 + delta, Points2);

			//tanU
			tanU.v3[0] = pU.x - p2_2.x;
			tanU.v3[1] = pU.y - p2_2.y;
			tanU.v3[2] = pU.z - p2_2.z;
			//tanU = pU - p2;

			//tanV
			tanV.v3[0] = pV.x - p2_2.x;
			tanV.v3[1] = pV.y - p2_2.y;
			tanV.v3[2] = pV.z - p2_2.z;
			//tanV = pV - p2;

			tanU.normalize();
			tanV.normalize();

			//3rd normal
			n2 = tanU.cross(tanU, tanV);


			//4th point
			Point p3_2 = Calculate(u0, v1, Points2);
			pU = Calculate(u0 + delta, v1, Points2);
			pV = Calculate(u0, v1 + delta, Points2);

			//tanU
			tanU.v3[0] = pU.x - p3_2.x;
			tanU.v3[1] = pU.y - p3_2.y;
			tanU.v3[2] = pU.z - p3_2.z;

			//tanV
			tanV.v3[0] = pV.x - p3_2.x;
			tanV.v3[1] = pV.y - p3_2.y;
			tanV.v3[2] = pV.z - p3_2.z;

			tanU.normalize();
			tanV.normalize();

			//4th normal
			n3 = tanU.cross(tanU, tanV);

			glColor3f(1.0, 1.0, 1.0);		// This makes the cube green; the parameters are for red, green and blue.
			//glBegin(GL_QUADS);

			/*
			* draw points and normals
			*/
			//bottom left corner
			glNormal3f(n0.v3[0], n0.v3[1], n0.v3[2]);
			glVertex3f(p0_2.x, p0_2.y, p0_2.z);

			//bottom right corner
			glNormal3f(n1.v3[0], n1.v3[1], n1.v3[2]);
			glVertex3f(p1_2.x, p1_2.y, p1_2.z);

			//top right corner
			glNormal3f(n2.v3[0], n2.v3[1], n2.v3[2]);
			glVertex3f(p2_2.x, p2_2.y, p2_2.z);

			//top left corner
			glNormal3f(n3.v3[0], n3.v3[1], n3.v3[2]);
			glVertex3f(p3_2.x, p3_2.y, p3_2.z);

			/*
			* calculate the points and normals on the surface of patch 2
			*/
			//1st point
			Point p0_3 = Calculate(u0, v0, Points3);
			pU = Calculate(u0 + delta, v0, Points3);
			pV = Calculate(u0, v0 + delta, Points3);

			//tanU
			tanU.v3[0] = pU.x - p0_3.x;
			tanU.v3[1] = pU.y - p0_3.y;
			tanU.v3[2] = pU.z - p0_3.z;
			//tanU = pU - p0;

			//tanV
			tanV.v3[0] = pV.x - p0_3.x;
			tanV.v3[1] = pV.y - p0_3.y;
			tanV.v3[2] = pV.z - p0_3.z;
			//tanV = pV - p0;

			tanU.normalize();
			tanV.normalize();

			//1st normal
			n0 = tanU.cross(tanU, tanV);


			//2nd point
			Point p1_3 = Calculate(u1, v0, Points3);
			pU = Calculate(u1 + delta, v0, Points3);
			pV = Calculate(u1, v0 + delta, Points3);

			//tanU
			tanU.v3[0] = pU.x - p1_3.x;
			tanU.v3[1] = pU.y - p1_3.y;
			tanU.v3[2] = pU.z - p1_3.z;
			//tanU = pU - p1;

			//tanV
			tanV.v3[0] = pV.x - p1_3.x;
			tanV.v3[1] = pV.y - p1_3.y;
			tanV.v3[2] = pV.z - p1_3.z;
			//tanV = pV - p1;

			tanU.normalize();
			tanV.normalize();

			//2nd normal
			n1 = tanU.cross(tanU, tanV);


			//3rd point
			Point p2_3 = Calculate(u1, v1, Points3);
			pU = Calculate(u1 + delta, v1, Points3);
			pV = Calculate(u1, v1 + delta, Points3);

			//tanU
			tanU.v3[0] = pU.x - p2_3.x;
			tanU.v3[1] = pU.y - p2_3.y;
			tanU.v3[2] = pU.z - p2_3.z;
			//tanU = pU - p2;

			//tanV
			tanV.v3[0] = pV.x - p2_3.x;
			tanV.v3[1] = pV.y - p2_3.y;
			tanV.v3[2] = pV.z - p2_3.z;
			//tanV = pV - p2;

			tanU.normalize();
			tanV.normalize();

			//3rd normal
			n2 = tanU.cross(tanU, tanV);


			//4th point
			Point p3_3 = Calculate(u0, v1, Points3);
			pU = Calculate(u0 + delta, v1, Points3);
			pV = Calculate(u0, v1 + delta, Points3);

			//tanU
			tanU.v3[0] = pU.x - p3_3.x;
			tanU.v3[1] = pU.y - p3_3.y;
			tanU.v3[2] = pU.z - p3_3.z;

			//tanV
			tanV.v3[0] = pV.x - p3_3.x;
			tanV.v3[1] = pV.y - p3_3.y;
			tanV.v3[2] = pV.z - p3_3.z;

			tanU.normalize();
			tanV.normalize();

			//4th normal
			n3 = tanU.cross(tanU, tanV);

			glColor3f(0.0, 1.0, 0.0);		// This makes the cube green; the parameters are for red, green and blue.
			//glBegin(GL_QUADS);

			/*
			* draw points and normals
			*/
			//bottom left corner
			glNormal3f(n0.v3[0], n0.v3[1], n0.v3[2]);
			glVertex3f(p0_3.x, p0_3.y, p0_3.z);

			//bottom right corner
			glNormal3f(n1.v3[0], n1.v3[1], n1.v3[2]);
			glVertex3f(p1_3.x, p1_3.y, p1_3.z);

			//top right corner
			glNormal3f(n2.v3[0], n2.v3[1], n2.v3[2]);
			glVertex3f(p2_3.x, p2_3.y, p2_3.z);

			//top left corner
			glNormal3f(n3.v3[0], n3.v3[1], n3.v3[2]);
			glVertex3f(p3_3.x, p3_3.y, p3_3.z);

		}
	}

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnd();
	
	/*
	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, Globals::texture[0]);
	// Make sure no bytes are padded:
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Select GL_MODULATE to mix texture with polygon color for shading:
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Use bilinear interpolation:
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	////Set this texture to be the one we are working with
	//glBindTexture(GL_TEXTURE_2D, Globals::texture[3]);
	//glBegin(GL_QUADS);
	////front wall
	//glColor3f(1.0, 1.0, 1.0);
	//
	//glNormal3f(0, 0, 1);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glEnable(GL_COLOR_MATERIAL);

	glBegin(GL_QUADS);

	// specify texture coordinates for each vertex
	// note that textures are stored "upside down"

	//left wall
	glColor3f(1.0, 1.0, 1.0);

	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(-10, 0, 10);
	glTexCoord2f(1, 1); glVertex3f(-10, 0, -10);
	glTexCoord2f(1, 0); glVertex3f(-10, 20, -10);
	glTexCoord2f(0, 0); glVertex3f(-10, 20, 10);

	glEnd();

	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, Globals::texture[1]);

	glBegin(GL_QUADS);
	//back wall
	glColor3f(1.0, 1.0, 1.0);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0, 1); glVertex3f(-10, 0, -10);
	glTexCoord2f(1, 1); glVertex3f(10, 0, -10);
	glTexCoord2f(1, 0); glVertex3f(10, 20, -10);
	glTexCoord2f(0, 0); glVertex3f(-10, 20, -10);
	glEnd();

	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, Globals::texture[2]);
	glBegin(GL_QUADS);
	//right wall
	glColor3f(1.0, 1.0, 1.0);

	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(10, 0, -10);
	glTexCoord2f(1, 1); glVertex3f(10, 0, 10);
	glTexCoord2f(1, 0); glVertex3f(10, 20, 10);
	glTexCoord2f(0, 0); glVertex3f(10, 20, -10);
	glEnd();
	//glTexCoord2f(0, 1); glVertex3f(-10, 0, 10);
	//glTexCoord2f(1, 1); glVertex3f(10, 0, 10);
	//glTexCoord2f(1, 0); glVertex3f(10, 20, 10);
	//glTexCoord2f(0, 0); glVertex3f(-10, 20, 10);
	//glEnd();

	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, Globals::texture[4]);
	glBegin(GL_QUADS);
	//top wall
	glColor3f(1.0, 1.0, 1.0);

	glNormal3f(0, -1, 0);
	glTexCoord2f(0, 1); glVertex3f(-10, 20, 10);
	glTexCoord2f(1, 1); glVertex3f(10, 20, 10);
	glTexCoord2f(1, 0); glVertex3f(10, 20, -10);
	glTexCoord2f(0, 0); glVertex3f(-10, 20, -10);
	glEnd();
	*/

	//if (iRain == 1)
	//{
	glDisable(GL_LIGHTING);
	for (int i = 0; i <= 100; i++)
	{
		for (int j = 0; j <= 100; j++)
		{
			//cout << "hello" << endl;
			//Rain
			Globals::particles[i*j].draw();
		}
	}
	glEnable(GL_LIGHTING);
	//}


	glFlush();
	glutSwapBuffers();
}
