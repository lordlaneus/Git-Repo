#include "Room.h"
#include "Util.h"
using namespace std;

Room::Room()
{

}
Room::Room(Game*, Sprite sprite)
{
	this->g;
	this->sprite = sprite;
}
Room::Room(Game* g, int size, Sprite s)
{
	int a = 5;
}
void Room::update(float tick) {
	//in case I want to add dynamic planets in the future
}
Tile* Room::checkCollision(Vector v)
{
	//todo
	return NULL;
}

bool Room::load(string path)
{
	ifstream file("resources\\" + path);
	string line;
	//header
	getline(file, line);
	if (line != "clh") return false;
	getline(file, line);
	size.x = atoi(line.c_str());
	getline(file, line);
	size.y = atoi(line.c_str());
	getline(file, line);
	tileSize = atoi(line.c_str());
	//level data;
	while (file.is_open())
	{
		
	}

}
void Room::render(ShaderProgram *program)
{

	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	for (int i = 0; i < tiles.size(); i++)
	{
		for (int j = 0; j < tiles[0].size(); j++)
		{
			float u = (float)(((int)tiles[i][j].sprite.index) % sprite.sheetW) / (float)sprite.sheetW;
			float v = (float)(((int)tiles[i][j].sprite.index) / sprite.sheetW) / (float)sprite.sheetH;
			float spriteWidth = 1.0 / (float)sprite.sheetW;
			float spriteHeight = 1.0 / (float)sprite.sheetH;
			texCoordData.insert(texCoordData.end(), {
				u, v,
				u + spriteWidth, v + spriteHeight,
				u, v + spriteHeight,

				u + spriteWidth, v + spriteHeight,
				u, v,
				u + spriteWidth, v
			});
			vertexData.insert(vertexData.end(), {
				tiles[i][j].position.x - tileSize / 2, tiles[i][j].position.y + tileSize / 2,
				tiles[i][j].position.x + tileSize / 2, tiles[i][j].position.y - tileSize / 2,
				tiles[i][j].position.x - tileSize / 2, tiles[i][j].position.y - tileSize / 2,

				tiles[i][j].position.x + tileSize / 2, tiles[i][j].position.y - tileSize / 2,
				tiles[i][j].position.x - tileSize / 2, tiles[i][j].position.y + tileSize / 2,
				tiles[i][j].position.x + tileSize / 2, tiles[i][j].position.y + tileSize / 2
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

	glBindTexture(GL_TEXTURE_2D, sprite.texture);

	glDrawArrays(GL_TRIANGLES, 0, tiles[0].size() * tiles.size() * 6);


	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}