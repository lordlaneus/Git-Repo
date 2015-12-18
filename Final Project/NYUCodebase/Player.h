
#pragma once

#include "ShaderProgram.h"
#include "Entity.h"
#include "Sprite.h"
#include "Game.h"
#include "Hook.h"
#include "Weapon.h"
#include "KeyStack.h"
#include "Planet.h"
#include "Animation.h"

class Hook;
class Weapon;
class Game;
class Planet;
class Player: public Entity{
public:
	enum State{ inAir, onGround };
	Hook* hook;
	Planet* planet;
	Weapon* sword;
	KeyStack wasd;
	State state = onGround;
	bool dying;
	float deathTime = 1;
	Animation walkAnim;
	Animation death;
	bool shiftDown = false;
	bool shifted = false;
	float energyRegen = 25;
	float shiftCost = 75;
	float hurt = 0;
	float energy = 100;
	float maxEnergy = 100;
	float walkSpeed = 75;
	float jumpPower = 50;
	float walking = 0;
	float mass = 50;
	float shiftingTime;
	Player();
	Player(Game *g, Sprite);

	bool collidesP(Planet& p);
	void collide(Entity& e);
	void attack(Vector);
	void die();
	void jump();
	void land(Planet& p);
	void shift();
	void takeDamage(float dmg);
	void unshift();
	void walk();
	void pop(Planet& p);
	void render(ShaderProgram*);
	void update(float elapsed);
	void changeSprite();
};