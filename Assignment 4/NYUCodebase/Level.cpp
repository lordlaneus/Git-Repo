#include "Level.h"
#include <vector>
const int LEVEL_WIDTH = 8;
const int LEVEL_HEIGHT = 8;
const float TILE_SIZE = 16;

Level::Level(int w, int h, int ts, GLuint s)
{
	width = w;
	height = h;
	tileSize = ts;
	texture = s;
	// very temporary
	sheetX = 4;
	sheetY = 4;

	int data[LEVEL_WIDTH][LEVEL_HEIGHT]{
		{ 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 2, 2, 0, 2, 2, 2, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 2, 2, 2, 2, 2, 0, 1 },
		{ 1, 0, 0, 0, 2, 0, 0, 1 },
		{ 1, 0, 2, 0, 0, 0, 2, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1 }
	};
	for (int i = 0; i <8; i++)
	{
		tiles.push_back(std::vector<int>());
		for (int j = 0; j < 8; j++)
		{
			tiles[i].push_back(data[7 - j][i]);
		}
	}



};

void Level::render(ShaderProgram *program)
{
	std::vector<float> vertexData;
	std::vector<float> texCoordData;

	for (int y = 0; y < LEVEL_HEIGHT; y++)
	{
		for (int x = 0; x < LEVEL_WIDTH; x++)
		{
			renderTile(program, x, y);

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

			glUseProgram(program->programID);


			glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, &vertexData);
			glEnableVertexAttribArray(program->positionAttribute);

			glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, &texCoordData);
			glEnableVertexAttribArray(program->texCoordAttribute);

			glBindTexture(GL_TEXTURE_2D, texture);

			glDrawArrays(GL_TRIANGLES, 0, 3);
			glDrawArrays(GL_TRIANGLES, 3, 3);


			glDisableVertexAttribArray(program->positionAttribute);
			glDisableVertexAttribArray(program->texCoordAttribute);
		}
	}
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

