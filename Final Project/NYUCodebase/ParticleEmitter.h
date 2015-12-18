
#pragma once

#include <vector>



#include "Particle.h"
#include "Sprite.h"
#include "ShaderProgram.h"
#include "Vector.h"

class ParticleEmitter {
public:
	ParticleEmitter(int pCount=20);
	
	void update(float elapsed);
	virtual void render(ShaderProgram*);
	virtual void reset(Particle &p);
	virtual void trigger();
	virtual void start();


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