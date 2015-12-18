#pragma once

#include "Enemy.h"
#include "Planet.h"

class Roller : public Enemy
{
public:
	float dir;
	Planet* planet;

	Roller();
	Roller(Game*, Vector position);
	void pop(Planet* p);

	void update(float elapsed);
};