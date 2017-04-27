#pragma once

#include "Gui.h"
#include "Sprite.h"
class TextDisplay : public GUI
{
public:
	bool centered;
	float size;
	float spacing;
	std::string text;
	std::string label;
	Sprite sprite;
	TextDisplay();
	TextDisplay(Sprite sprite, std::string label, std::string text, float size, float spacing,float x, float y);
	void render(ShaderProgram*);

};