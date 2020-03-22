#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "Vector4.h"
#include <GL/glut.h>
#include <vector>

class Particle
{
public:
	Vector4 position;
	double fallingDelta;
	Particle();
	Particle(double delta);
	void update();
	void draw();
};

#endif