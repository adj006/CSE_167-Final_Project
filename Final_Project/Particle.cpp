#include "Particle.h"

using namespace std;

Particle::Particle()
{

}

Particle::Particle(double delta)
{
	fallingDelta = delta;
}

void Particle::update()
{
	position.v4[1] += fallingDelta;
}

void Particle::draw()
{
	//Rain
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(position.v4[0], position.v4[1], position.v4[2]);
	glVertex3f(position.v4[0], position.v4[1] - 2, position.v4[2]);
	glEnd();
}