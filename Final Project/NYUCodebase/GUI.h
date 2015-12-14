#pragma once

#include"ShaderProgram.h";
#include "Vector.h";
class GUI
{
public:
	bool show = true;
	Vector position;
	Vector size;
	virtual void render(ShaderProgram*) = 0;
};