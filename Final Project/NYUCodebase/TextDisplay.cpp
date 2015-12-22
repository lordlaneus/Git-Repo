#include "TextDisplay.h"
#include "Util.h"
using namespace std;

TextDisplay::TextDisplay()
{

}
TextDisplay::TextDisplay(Sprite sprite, string label, string text, float size, float spacing, float x, float y)
{
	this->position = Vector(x, y);
	this->sprite = sprite;
	this->label = label;
	this->text = text;
	this->size = size;
	this->spacing = spacing;
}

void TextDisplay::render(ShaderProgram* program)
{
	Util::drawText(program, sprite.texture, label+text, position.x, position.y, size, spacing, centered);
}