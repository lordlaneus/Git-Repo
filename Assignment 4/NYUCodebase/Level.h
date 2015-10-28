
#pragma once

#include "ShaderProgram.h"
#include <fstream>
#include <vector>
class Level {
public:
	Level(std::string);
	bool readHeader(std::ifstream&);
	bool readLayerData(std::ifstream&);
	void setTexture(GLuint, int, int);
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