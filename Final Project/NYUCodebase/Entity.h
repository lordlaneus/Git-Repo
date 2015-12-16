
#pragma once

#include <vector>

#include "ShaderProgram.h"
#include "Sprite.h"
#include "Vector.h"


class Game;
class Planet;
class Player;
class Entity {
public:
	Game* g;
	float maxHealth;
	float health;
	float coolDown;
	float hurt;
	bool active = true;
	Vector position;
	Vector velocity;
	Vector accel;
	Vector size;
	float rotation;

	Sprite sprite;


	Entity();
	Entity(Game* g);
	enum Type{ player, projectile, enemy };
	virtual void update(float);
	void setTexture(GLuint, int, int, int);
	virtual void render(ShaderProgram*);
	virtual void playerCollision(Player* p);
	bool collides(Planet& p);
	bool collides(Entity&);
	bool collides(Entity&, float, float);
	std::vector<float> nVertices(Vector normal);
	Type type;
};