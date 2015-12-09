
#pragma once

#include "ShaderProgram.h"
#include "Level.h"
#include "Sprite.h"
#include "Vector.h"
#include <vector>


class Entity {
public:
	Entity();
	void update(float);
	void setTexture(GLuint, int, int, int);
	void render(ShaderProgram*);

	Vector position;
	float rotation;
	float width;
	float height;
	Vector velocity;
	Vector accel;
	Vector maxVel;
	float friction;

	Sprite sprite;

	bool ctop, cleft, cright, cdown;
};