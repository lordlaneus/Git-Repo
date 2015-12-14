#pragma once
#include "Game.h"
#include "Entity.h";
#include "ShaderProgram.h"
#include "Projectile.h"
class Enemy:public Entity
{
public:
	enum State{aggro,returning, alert};
	
	State state = alert;
	Vector home;
	Vector baseSize;
	float maxPuff = 1.5;
	float puffStart = .07;
	float health = 3;
	float fireRate = 3;
	float coolDown = fireRate;
	float detectionRange = 50;
	float circleRange = 10;
	float speed = 10;

	Enemy(Game*, Vector position);

	void die();
	void fire();
	void playerCollision(Player* p);

	void takeDamage(float dmg, Vector v);
	void update(float);
	void render(ShaderProgram*);

	
};