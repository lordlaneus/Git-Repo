#pragma once
#include "ShaderProgram.h"
#include "Sprite.h"
#include "Vector.h"
#include "Entity.h"
#include "Room.h"


class Game;
class Entity;
class Player;
class Room;
class Tile {
public:
	Room* Room;
	bool on;
	Vector position;
	float size;
	Sprite sprite;
	

	Tile();
};