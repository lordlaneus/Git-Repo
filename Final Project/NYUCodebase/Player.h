
#pragma once

#include "ShaderProgram.h"
#include "Entity.h"
#include "Sprite.h"
#include "Game.h"
#include "Hook.h"
#include "Weapon.h"
#include "KeyStack.h"
#include "Planet.h"
#include "Particle.h"
#include "Animation.h"

class Hook;
class Weapon;
class Game;
class Planet;
class Player: public Entity{
public:

	const float chargeCost = 40;
	const float deathTime = 1;
	const float energyRegen = 25;
	const float heavyGrappleCost = 25;
	const float jumpPower = 50;
	const float maxEnergy = 100;
	const float shiftCost = 75;
	const float walkSpeed = 75;
	const float zipCost = 40;
	const float zipLength = .2;

	bool charging;
	float chargeTime;
	Animation deathAnim;
	bool dying;
	float energy = 100;
	Hook* hook;
	float hurt = 0;
	float mass = 50;
	Planet* planet;
	bool shiftDown = false;
	bool shifted = false;
	float shiftingTime;
	bool onGround;
	Animation walkAnim;
	KeyStack wasd;
	float walking;
	Weapon* weapon;
	bool zDown;
	Animation zipAnim;
	Vector zipPosition;
	float zipTime;


	
	Player();
	Player(Game *g, Sprite);

	void attack(Vector);
	void changeSprite();
	void charge();
	bool collidesP(Planet& p);
	void collide(Entity& e);
	void die();
	bool drainEnergy(float amount);
	void jump();
	void land(Planet& p);
	void pop(Planet& p);
	void shift();
	void takeDamage(float dmg);
	void unshift();
	void walk();
	void zip();

	void render(ShaderProgram*);
	void update(float elapsed);
};