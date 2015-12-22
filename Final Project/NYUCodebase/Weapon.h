#pragma once
#include "Sprite.h"
#include "Animation.h"
#include "Player.h"
#include "Vector.h"
#include "Entity.h"
class Weapon: public Entity
{
public:
	const float baseDamage = 10;
	const float baseHeight = 8;
	const float baseRange = 12;
	const float baseWidth = 3;
	const float chargeMult = 1;
	const float maxLifetime = .5;
	const float rangeChargeMult =.25;
	
	bool active = false;
	Animation anim;
	float charge;
	float damage;
	Vector dir;
	float lifetime;
	Player* player;
	float range;

	Weapon();
	Weapon(Sprite s, Player* player);

	void swing(Vector direction, Vector velocity,float charge);

	void render(ShaderProgram*);
	void update(float);
};