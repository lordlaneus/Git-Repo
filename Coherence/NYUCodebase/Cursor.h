#pragma once

#include "GUI.h"
#include "Sprite.h"

class Cursor :public GUI
{
public: 
	Sprite sprite;
	void render(ShaderProgram* program)
	{
		sprite.render(program, position.x+80, position.y+50, size.x, size.y);
	}
};