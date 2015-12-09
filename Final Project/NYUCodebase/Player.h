
#pragma once

#include "ShaderProgram.h"
#include "Entity.h"
#include "Sprite.h"
#include "Game.h"
#include "Hook.h"
#include "KeyStack.h"

class Game;
class Hook;
class Player: public Entity {
public:
	Game* g;
	Hook* hook;
	Planet* planet;
	KeyStack wasd;
	bool onGround;
	float hurt = 0;
	float Maxhealth = 100;
	float health = 100;
	float walkSpeed = 2.5;
	float jumpPower = 50;
	float walking = 0;
	float mass = 50;
	Player();
	Player(Game *g, Sprite);

	bool collides(Planet p);
	void jump();
	void land(Planet& p);
	void takeDamage(float dmg);
	void walk();
	void pop(Planet p);
	void render(ShaderProgram*);
	void update(float elapsed);
	void changeSprite();
};