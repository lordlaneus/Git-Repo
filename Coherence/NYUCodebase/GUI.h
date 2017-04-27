#pragma once

#include"ShaderProgram.h"
#include "Vector.h"

class GUI
{
public:
	float showTime = -1;
	bool active = true;
	bool visible= true;
	Vector position;
	Vector size;
	virtual void render(ShaderProgram*) = 0;
	virtual void update(float elapsed);
	virtual void show(float duration=-1);
};