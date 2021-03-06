#include "Menu.h"
#include "Util.h"
using namespace std;


Menu::Menu()
{
	position = Vector(80, 70);
	size = Vector(80, 80);
	margin = 30;
	visible = false;
	showTitle = true;
	fontSize = 6;
	fontSpacing = 1;
	lineSpacing = 10;
}

void Menu::addOption(string option)
{
	options.push_back(option);
}
void Menu::down()
{
	selected++;
	if (selected >= options.size())
	{
		selected = 0;
	}
}
string Menu::selection()
{
	return options[selected];
}
void Menu::up()
{
	selected--;
	if (selected <0)
	{
		selected = options.size() - 1;
	}
}
void Menu::render(ShaderProgram* program)
{


	if (bg)
	{
		bg->render(program, position.x, position.y - size.y / 2, size.x, size.y);
	}
	

	float optionsStart = position.y - fontSize * 2;
	if (showTitle)
	{
		Util::drawText(program, font.texture, title, position.x, position.y - fontSize * 2, fontSize*1.2, fontSpacing);
		optionsStart = position.y - fontSize * 5;
	}

	for (int i = 0; i < options.size(); i++)
	{
		Util::drawText(program, font.texture, options[i], position.x-size.x/2+margin, optionsStart-(i*fontSize+i*lineSpacing), fontSize, fontSpacing,false);
		
	}
	font.render(program, position.x-size.x/2+margin-fontSize, optionsStart- (selected*fontSize + selected*lineSpacing), fontSize, fontSize,0);
}

