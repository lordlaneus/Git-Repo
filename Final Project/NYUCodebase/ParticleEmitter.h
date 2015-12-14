
#pragma once

#include "Vector.h"
#include <vector>
#include "Particle.h"
#include "Sprite.h"
#include "ShaderProgram.h"

class ParticleEmitter {
public:
	ParticleEmitter(int pCount=20);
	
	void update(float elapsed);
	void render(ShaderProgram*);
	void reset(Particle &p);
	void trigger();
	void start();


	Vector position;
	Vector gravity;
	bool loop = false;
	float mass;
	Vector velocity;
	Vector velocityDev;
	Sprite sprite;
	float lifeDev = 0;
	float maxLifetime;
	float startSize;
	float endSize;
	float sizeDev;
	int pCount;
	
	std::vector<Particle> particles;

	
};