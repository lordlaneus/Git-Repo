#pragma once

#include "GUI.h"
#include "Sprite.h"

class Cursor :public GUI
{
public: 
	Sprite sprite;
	void render(ShaderProgram* program)
	{
		sprite.render(program, position.x, position.y, size.x, size.y);
	}
};