
#pragma once

#include "ShaderProgram.h"
#include <vector>
class Level {
public:
	Level(int, int, int, GLuint);
	void render(ShaderProgram*);
	void renderTile(ShaderProgram*, int x, int y);
	GLuint texture;
	int height;
	int width;
	int tileSize;
	std::vector<std::vector<int>> tiles;

	int sheetX;
	int sheetY;
};