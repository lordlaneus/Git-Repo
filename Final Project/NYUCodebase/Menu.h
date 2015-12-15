#pragma once

#include "GUI.h"
#include "Sprite.h"
#include <vector>
class Menu : public GUI
{
public:

	std::string title;
	bool showTitle;
	std::vector<std::string> options;
	int selected = 0;

	float margin;
	int fontSize;
	int fontSpacing;
	int lineSpacing;
	Sprite font;
	Sprite* bg;

	Menu();
	void addOption(std::string);
	void down();
	void up();
	void render(ShaderProgram*);
};