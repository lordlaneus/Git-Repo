#include "Tank.h"

Tank::Tank()
{
	Entity();
	type = TANK;
	y =  10 ;
	x = 73;
	friction = 20;
	width = 10;
	height = 10;
	setTexture(2);

	maxSpeed = 25;

}