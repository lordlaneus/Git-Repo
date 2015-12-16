#pragma once

#include "Gui.h"
#include "Sprite.h"
#include "Entity.h"


class Entity;
class Bar: public GUI
{
public:
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