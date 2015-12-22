#pragma once

#include "Entity.h"
#include "Game.h"
#include "ShaderProgram.h"
#include "Projectile.h"
#include "Bar.h"
class Enemy:public Entity
{
public:

	const float avoidance = 20;
	const float baseWidth = 5;
	const float baseHeight = 5;
	const float landBonus = 1.5;

	const float rollerDamage = 15;
	const float rollerMaxHealth = 20;
	const float rollerSpeed = 40;

	const float spiterCircleRange = 15;
	const float spitterDamage = 10;
	const float spitterfireRate = 2.5;
	const float spitterHealth = 25;
	const float spitterMaxPuff = 1.5;
	const float spitterProjectileSpeed = 40;
	const float spitterSpeed = 15;


	enum State{aggro,returning, alert,circle};
	
	

	Vector baseSize;
	Bar* bar;
	int circleDir = 1;
	float circleRange;
	float coolDown = fireRate;
	float detectionRange = 50;
	float damage;
	float fireRate;
	Vector home;
	float maxPuff = 1;
	float projectileSpeed;
	float puffStart = .07;
	float speed;
	State state = alert;

	Enemy();
	Enemy(std::string, Game*);
	Enemy(Game*, Vector position);

	
	void die();
	void fire();
	void makeChaser();
	void makeSpitter();
	void playerCollision(Player* p);

	void takeDamage(float dmg);
	virtual void update(float);
	void render(ShaderProgram* program, int offset);

	
};