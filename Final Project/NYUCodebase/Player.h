
#pragma once

#include "ShaderProgram.h"
#include "Entity.h"
#include "Sprite.h"
#include "Game.h"
#include "Hook.h"
#include "Weapon.h"
#include "KeyStack.h"

class Game;
class Hook;
class Weapon;
class Player: public Entity {
public:
	Hook* hook;
	Planet* planet;
	Weapon* sword;
	KeyStack wasd;
	bool onGround;
	float hurt = 0;
	float maxHealth = 100;
	float health = 100;
	float walkSpeed = 75;
	float jumpPower = 50;
	float walking = 0;
	float mass = 50;
	Player();
	Player(Game *g, Sprite);

	bool collidesP(Planet p);
	void collide(Entity& e);
	void attack(Vector);
	void die();
	void jump();
	void land(Planet& p);
	void takeDamage(float dmg);
	void walk();
	void pop(Planet p);
	void render(ShaderProgram*);
	void update(float elapsed);
	void changeSprite();
};