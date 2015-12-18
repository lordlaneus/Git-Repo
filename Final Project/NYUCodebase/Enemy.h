#pragma once

#include "Entity.h"
#include "Game.h"
#include "ShaderProgram.h"
#include "Projectile.h"
#include "Bar.h"
class Enemy:public Entity
{
public:
	enum State{aggro,returning, alert,circle};
	
	State state = alert;
	Bar* bar;
	Vector home;
	Vector baseSize;
	int circleDir = 1;
	float startHealth = 25;
	float damage = 10;
	float avoidance = 20;
	float landBonus = M_PI;
	float maxPuff = 1.5;
	float puffStart = .07;
	float fireRate = 2.5;
	float coolDown = fireRate;
	float detectionRange = 50;
	float circleRange = 10;
	float projectileSpeed = 40;
	float speed = 10;

	Enemy();
	Enemy(std::string, Game*);
	Enemy(Game*, Vector position);

	void die();
	void fire();
	void playerCollision(Player* p);

	void takeDamage(float dmg);
	void update(float);
	void render(ShaderProgram* program, int offset);

	
};