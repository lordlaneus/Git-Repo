#pragma once

#include "Gui.h"
#include "Sprite.h"
class Bar: public GUI
{
public:
	float max;
	float current;
	Sprite sprite;
	Bar();
	Bar(Sprite sprite, float, float width = 100, float height = 10);
	void render(ShaderProgram*);

};