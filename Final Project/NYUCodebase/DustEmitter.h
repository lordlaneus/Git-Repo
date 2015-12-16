#pragma once

#include "ParticleEmitter.h"
#include "Particle.h"
#include "ShaderProgram.h"

class Planet;
class DustEmitter : public ParticleEmitter
{
public:

	DustEmitter();
	void trigger(Planet*);
	void render(ShaderProgram*); 
	void reset(Particle&);
};