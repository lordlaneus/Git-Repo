#include "Compass.h"

using namespace std;

void Compass::render(ShaderProgram* program)
{
	sprite.index = 0;
	sprite.render(program, position.x, position.y, size.x, size.y);
	sprite.index = 1;
	sprite.render(program, position.x, position.y, size.x, size.y,direction.angle());
}