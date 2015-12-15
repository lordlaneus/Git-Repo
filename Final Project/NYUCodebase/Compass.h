#pragma once

#include "GUI.h"
#include "Sprite.h"

class Compass : public GUI
{
public:
	Sprite sprite;
	Vector direction;

	void render(ShaderProgram*);
};