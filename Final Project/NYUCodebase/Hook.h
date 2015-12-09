#pragma once

#include "Vector.h"
#include "Sprite.h"
#include "Player.h"

class Player;
class Hook{
public:
	int segments = 20;
	float maxLength = 40;
	float extended = 0;
	float castSpeed = 100;
	enum State {away, cast, reeling, recoiling};
	Planet* anchor;
	Player* player;
	Vector position;
	Vector velocity;
	Sprite sprite;
	State state;
	float size;

	Hook(Player*);
	void shoot(float, float);
	void update(float elapsed);
	void render(ShaderProgram*);
	void renderChain(ShaderProgram*);
};