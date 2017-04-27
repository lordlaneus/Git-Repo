
#pragma once

#include "ShaderProgram.h"
#include "Tile.h"
#include "Sprite.h"
#include "Game.h"
#include <vector>

class Tile;
class Game;
class Room {
public:
	Game* g;
	Sprite sprite;
	int tileSize = 32;
	Vector size;
	float gravity = 10;
	std::vector<std::vector<Tile>> tiles;

	Room();
	Room(Game*, Sprite);
	Room(Game* g, int, Sprite);
	
	Tile* checkCollision(Vector);
	bool load(std::string path);
	void update(float tick);
	void render(ShaderProgram*);

};