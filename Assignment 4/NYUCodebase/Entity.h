
#pragma once

#include "ShaderProgram.h"
#include "Level.h"
#include <vector>
enum Type{PLAYER, ENEMY};
class Entity {
public:
	Entity();
	void makePlayer();

	void checkCollisions(Level);
	float collidePoint(Level, float, float);
	void update(float);
	void setTexture(GLuint, int, int, int);
	void render(ShaderProgram*);

	Type type;
	float x;
	float y;
	float width;
	float height;
	float speedX;
	float speedY;
	float accelX;
	float accelY;
	float maxSpeedX;
	float maxSpeedY;
	float friction;

	GLuint texture;
	int sprite;
	int sheetX;
	int sheetY;

	bool ctop, cleft, cright, cdown;
};