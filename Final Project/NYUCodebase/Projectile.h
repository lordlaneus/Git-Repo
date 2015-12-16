#pragma once
#include "Entity.h"
#include "ParticleEmitter.h"
#include "Game.h"

class Player;
class Projectile : public Entity
{
public:
	float maxLifetime = 5;
	float lifetime = 0;
	float damage = 5;
	ParticleEmitter pe;
	Projectile();
	Projectile(Game*, Vector);
	void update(float);
	void render(ShaderProgram*);
	void playerCollision(Player* p);
};