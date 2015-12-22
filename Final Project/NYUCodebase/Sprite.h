
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
	int count();
	void set(GLuint t, int i, int w, int h)
	{
		texture = t;
		index = i;
		sheetH = h;
		sheetW = w;
	}
	void render(ShaderProgram*, float x, float y, float w, float h, float rotaion = 0, int index = -1, bool fliped = false);

	Sprite operator[](int);
};