#pragma once

#include "Gui.h"
#include "Sprite.h"
class MsgBox : public GUI
{
public:
	std::string text;
	std::string subText1;
	std::string subText2;
	std::string subText3;
	Sprite bg;
	Sprite font;
	MsgBox();
	MsgBox(Sprite, Sprite);
	void render(ShaderProgram*);

};