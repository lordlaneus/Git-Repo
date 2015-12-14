#pragma once

#include "Gui.h"
#include "Sprite.h"
class TextDisplay : public GUI
{
public:
	float size;
	float spacing;
	std::string text;
	Sprite sprite;
	TextDisplay();
	TextDisplay(Sprite sprite, std::string, float size, float spacing,float x, float y);
	void render(ShaderProgram*);

};