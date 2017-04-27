#pragma once
#include "Entity.h"
#include "ParticleEmitter.h"
#include "Game.h"

class Player;
class Entity;
class Projectile : public Entity
{
public:
	float maxLifetime = 5;
	float lifetime = 0;
	float damage;
	ParticleEmitter pe;
	Projectile();
	Projectile(Game*, Vector, float damage);
	void update(float);
	void render(ShaderProgram*, int offset);
	void playerCollision(Player* p);
};