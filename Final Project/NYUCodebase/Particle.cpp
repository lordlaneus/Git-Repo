#include "Particle.h"
#include <vector>
#include <random>

using namespace std;

Particle::Particle()
{
	on = false;
}
void Particle::update(float elapsed)
{


	Vector pull = gravity - start;
	pull.normalize(mass);
	velocity = velocity + pull* elapsed;
	position = position + velocity * elapsed;
	lifetime += elapsed;
}