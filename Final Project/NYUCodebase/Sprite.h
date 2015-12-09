
#pragma once

#include "ShaderProgram.h"
class Sprite{
public:
	Sprite()
	{

	}
	Sprite(GLuint t, int i, int h, int w)
	{
		texture = t;
		index = i;
		sheetH = h;
		sheetW = w;
	}
	bool flipped = false;
	GLuint texture;
	int index;
	int sheetH;
	int sheetW;
	void set(GLuint t, int i, int h, int w)
	{
		texture = t;
		index = i;
		sheetH = h;
		sheetW = w;
	}
};