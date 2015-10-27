
#pragma once

#include "ShaderProgram.h"
#include <vector>
enum Type{PLAYER, ENEMY};
class Entity {
public:
	Entity();
	void makePlayer();

	bool checkCollision(Entity&);
	void collide(std::vector<Entity>&);
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
	float maxSpeed;
	float friction;

	GLuint texture;
	int sprite;
	int sheetX;
	int sheetY;
};