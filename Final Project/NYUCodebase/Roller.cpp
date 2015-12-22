#include "Roller.h"
#include "Util.h"
using namespace std;

Roller::Roller()
{

}

Roller::Roller(Game* g, Vector position):
Enemy(g,position)
{ 
	dir = 1;
	if (Util::randFloat() < 0.5)
	{
		dir = -1;
	}
	planet = g->cluster->closestPlanet(position);
	damage = rollerDamage;
	baseSize = Vector(baseWidth, baseHeight);
	maxHealth = rollerMaxHealth;
	health = maxHealth;
	bar->max = maxHealth;
	size = baseSize;
	speed = rollerSpeed;
	sprite.index = 3;
	pop(planet);
}

void Roller::pop(Planet* p)
{

	Vector v = position - p->position;
	float l = v.length();
	v.normalize(p->size / 2 + size.y / 4);
	position = p->position + v;

}

void Roller::update(float elapsed)
{
	if (hurt > 0)
	{
		hurt -= elapsed;
	}
	pop(planet);
	Vector newPos = position - planet->position;
	newPos.rotate(landBonus*speed*dir*elapsed / planet->size);
	position = planet->position + newPos;
	//rotation = (planet->position - position).angle() + M_PI / 2;
}