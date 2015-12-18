#pragma once
#include "Sprite.h"
#include "Animation.h"
#include "Player.h"
#include "Vector.h"
#include "Entity.h"
class Weapon: public Entity
{
public:
	Player* player;
	float range = 12;
	bool active = false;
	Vector dir;
	Animation anim;
	float damage = 10;
	float maxLifetime = .5;
	float lifetime = 0;

	Weapon();
	Weapon(Sprite s, Player* player);

	void swing(Vector direction, Vector velocity);

	void render(ShaderProgram*);
	void update(float);
};