
#pragma once

#include "ShaderProgram.h"
#include "Level.h"
#include "Sprite.h"
#include <vector>


class Entity {
public:
	Entity();

	void checkCollisions(Level);
	float collidePoint(Level, float, float);
	void update(float);
	void setTexture(GLuint, int, int, int);
	void render(ShaderProgram*);

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

	Sprite sprite;

	bool ctop, cleft, cright, cdown;
};