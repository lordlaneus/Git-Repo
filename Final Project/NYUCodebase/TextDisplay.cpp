#include "TextDisplay.h"
#include "Util.h"
using namespace std;

TextDisplay::TextDisplay()
{

}
TextDisplay::TextDisplay(Sprite sprite, std::string text, float size, float spacing, float x, float y)
{
	this->position = Vector(x, y);
	this->sprite = sprite;
	this->text = text;
	this->size = size;
	this->spacing = spacing;
}

void TextDisplay::render(ShaderProgram* program)
{
	Util::drawText(program, sprite.texture, text, position.x, position.y, size, spacing);
}