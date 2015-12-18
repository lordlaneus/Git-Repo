#pragma once

#include "Gui.h"
#include "Sprite.h"
#include "Entity.h"


class Entity;
class Bar: public GUI
{
public:
	enum Color{red=1, blue = 2, green =3};
	Color color = red;
	bool fixed = true;
	Entity* e;
	float max;
	float current;
	Sprite sprite;
	Bar();
	Bar(Sprite sprite, float, float width = 100, float height = 10, Entity* = NULL);
	void render(ShaderProgram*);
	void update(float); 

};