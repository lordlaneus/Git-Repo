#include "Level.h"
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;
const float TILE_SIZE = 16;
bool Level::readHeader(std::ifstream &stream) {
	string line;
	width = -1;
	height = -1;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "width") {
			width = atoi(value.c_str());
		}
		else if (key == "height"){
			height = atoi(value.c_str());
		}
		else if (key == "tilewidth"){
			tileSize = atoi(value.c_str());
		}
	}
	if (width == -1 || height == -1) {
		return false;
	}
	vector<vector<int>> temp(height, vector<int>(width, 0));
	tiles = temp;
}
bool Level::readLayerData(std::ifstream &stream) {
	string line;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "data") {
			for (int y = 0; y < height; y++) {
				getline(stream, line);
				istringstream lineStream(line);
				string tile;
				for (int x = 0; x < width; x++) {
					getline(lineStream, tile, ',');
					unsigned char val = (unsigned char)atoi(tile.c_str());
					if (val > 0) {
						// be careful, the tiles in this format are indexed from 1 not 0
						tiles[y][x] = val - 1;
					}
					else {
						tiles[y][x] = 0;
					}
				}
			}
		}
	}
	return true;
}
Level::Level(string filepath)
{
	ifstream file(filepath);
	string line;
	while (getline(file,line))
	{
		if (line == "[header]")
		{
			if (!readHeader(file))
			{
				return;
			}
		}
		else if(line == "[layer]")
		{
			readLayerData(file);
		}
	}
}

void Level::setTexture(GLuint t, int x, int y)
{
	texture = t;
	sheetX = x;
	sheetY = y;

}
void Level::render(ShaderProgram *program)
{
	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	
	for (int y = 0; y < width; y++)
	{
		for (int x = 0; x < width; x++)
		{
			//renderTile(program, x, y);

			float u = (float)(((int)tiles[y][x]) % sheetX) / (float)sheetX;
			float v = (float)(((int)tiles[y][x]) / sheetX) / (float)sheetY;

			float spriteWidth = 1.0 / (float)sheetX;
			float spriteHeight = 1.0 / (float)sheetY;
			vertexData.insert(vertexData.end(), {
				TILE_SIZE * x, -TILE_SIZE * y,
				TILE_SIZE * x, (-TILE_SIZE * y) - TILE_SIZE,
				(TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
				TILE_SIZE * x, -TILE_SIZE * y,
				(TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
				(TILE_SIZE * x) + TILE_SIZE, -TILE_SIZE * y
			});
			texCoordData.insert(texCoordData.end(), {
				u, v,
				u, v + (spriteHeight),
				u + spriteWidth, v + (spriteHeight),
				u, v,
				u + spriteWidth, v + (spriteHeight),
				u + spriteWidth, v
			});
		}
	}
	Matrix modelMatrix;
	program->setModelMatrix(modelMatrix);


			glUseProgram(program->programID);


			glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
			glEnableVertexAttribArray(program->positionAttribute);

			glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
			glEnableVertexAttribArray(program->texCoordAttribute);

			glBindTexture(GL_TEXTURE_2D, texture);
			float a1x = vertexData.data()[0];
			float a1y = vertexData.data()[1];
			float a2x = vertexData.data()[2];
			float a2y = vertexData.data()[3];
			float a3x = vertexData.data()[4];
			float a3y = vertexData.data()[5];
			glDrawArrays(GL_TRIANGLES,0,width*width*6);
			

			glDisableVertexAttribArray(program->positionAttribute);
			glDisableVertexAttribArray(program->texCoordAttribute);
}
void Level::renderTile(ShaderProgram *program, int x, int y)
{
	int sprite = tiles[x][y];

	float u = (float)(((int)sprite) % sheetX) / (float)sheetX;
	float v = (float)(((int)sprite) / sheetX) / (float)sheetY;
	float spriteWidth = 1.0 / (float)sheetX;
	float spriteHeight = 1.0 / (float)sheetY;
	GLfloat texCoords[] = {
		u, v + spriteHeight,
		u + spriteWidth, v,
		u, v,
		u + spriteWidth, v,
		u, v + spriteHeight,
		u + spriteWidth, v + spriteHeight
	};
	float vertices[] = { -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f };
	// our regular sprite drawing
	Matrix modelMatrix;
	modelMatrix.Translate(x*tileSize, y*tileSize, 0);
	modelMatrix.Scale(tileSize, tileSize, 1);

	//program->setViewMatrix(view)
	program->setModelMatrix(modelMatrix);

	//drawing
	glUseProgram(program->programID);


	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glBindTexture(GL_TEXTURE_2D, texture);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLES, 3, 3);


	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}

