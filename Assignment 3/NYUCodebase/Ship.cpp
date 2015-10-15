#include "Ship.h"

Ship::Ship(int i)
{
	Entity();
	type = SHIP;
	y = floor(i/12) * 10+65;
	x = 10+i%12 * 12;
	width = 5;
	height = 5;
	setTexture(floor((i/12)%2));

}