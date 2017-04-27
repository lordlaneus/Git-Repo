
#pragma once

#include "ShaderProgram.h"
#include "Entity.h"
#include "Sprite.h"
#include "Game.h"
#include "Tile.h"
#include "Hook.h"
#include "Weapon.h"
#include "KeyStack.h"
#include "Particle.h"
#include "Animation.h"

class Hook;
class Weapon;
class Game;
class Tile;
class Entity;
class Player: public Entity{
public:
	const float compassCost = 10;
	const float chargeCost = 40;
	const float deathTime = 1;
	const float energyRegen = 25;
	const float heavyGrappleCost = 25;
	const float jumpPower = 50;
	const float maxEnergy = 100;
	const float maxHealth = 100;
	const float walkSpeed = 75;

	Tile* tile;
	bool charging;
	float chargeTime;
	Animation deathAnim;
	bool draining;
	bool dying;
	float energy = 100;
	Hook* hook;
	float hurt = 0;
	float mass = 50;
	bool onGround = false;
	Animation walkAnim;
	KeyStack wasd;
	float walking;
	Weapon* weapon;


	
	Player();
	~Player();
	Player(Game *g, Sprite);

	void attack(Vector);
	void changeSprite();
	void charge();
	bool collidesT(Tile& t);
	void collide(Entity& e);
	void die();
	bool drainEnergy(float amount);
	void jump();
	void land(Tile& t);
	void pop(Tile& t);
	void takeDamage(float dmg);
	void walk();

	void render(ShaderProgram*);
	void update(float elapsed);
};