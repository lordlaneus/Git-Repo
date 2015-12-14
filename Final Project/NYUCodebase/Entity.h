
#pragma once

#include "ShaderProgram.h"
#include "Level.h"
#include "Sprite.h"
#include "Vector.h"
#include "Planet.h"
#include <vector>

using namespace std;
class Game;
class Player;
class Entity {
public:
	Game* g;
	float coolDown;
	float hurt;
	bool active = true;
	Vector position;
	Vector size;
	float rotation;
	Vector velocity;
	Vector accel;
	Vector maxVel;
	float friction;

	Sprite sprite;


	Entity();
	Entity(Game* g);
	enum Type{ player, projectile,enemy};
	virtual void update(float);
	void setTexture(GLuint, int, int, int);
	virtual void render(ShaderProgram*);
	virtual void playerCollision(Player* p);
	bool collides(Planet p);
	bool collides(Entity&); 
	bool collides(Entity&, float,float);
	vector<float> nVertices(Vector);
	Type type;

	
};