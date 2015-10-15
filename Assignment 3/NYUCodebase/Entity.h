
#pragma once

#include "ShaderProgram.h"
#include <vector>
enum Type{TANK,SHIP,BULLET};
class Entity {
public:
	Entity();
	bool checkCollision(Entity&);
	void collide(std::vector<Entity>&);
	void update(float);
	void setTexture(int);
	void render(ShaderProgram*,GLuint,int,int);

	Type type;
	float x;
	float y;
	float width;
	float height;
	float speedX;
	float speedY;
	float accelX;
	float accelY;
	float maxSpeed;
	float friction;

	int sprite;
};