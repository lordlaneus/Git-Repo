
#pragma once

#include "ShaderProgram.h"
class Sprite{
public:

	bool flipped = false;
	GLuint texture;
	int index;
	int sheetH;
	int sheetW;
	Sprite()
	{

	}
	Sprite(GLuint t, int i, int w, int h)
	{
		texture = t;
		index = i;
		sheetH = h;
		sheetW = w;
	}
	void set(GLuint t, int i, int w, int h)
	{
		texture = t;
		index = i;
		sheetH = h;
		sheetW = w;
	}
	void render(ShaderProgram*, float, float, float, float, float = 0, bool =false);
};