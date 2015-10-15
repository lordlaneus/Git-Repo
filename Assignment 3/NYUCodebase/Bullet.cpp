#include "Bullet.h"

Bullet::Bullet(float x)
{
	Entity();
	type = BULLET;
	y = 10;
	this->x = x;
	width = 1;
	height = 1;
	speedY = 50;
	friction = 1;
	setTexture(3);

}