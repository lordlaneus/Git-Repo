
#pragma once

#include "Vector.h"

class Particle {
public:
	float u;
	float v;
	float spriteWidth;
	float spriteHeight;
	Particle();
	Vector start;
	Vector position;
	Vector velocity;
	Vector gravity;
	float lifetime;
	float size;
	float mass;
	bool on;
	void update(float);
};