#pragma once

#include "ParticleEmitter.h"
#include "Planet.h"

class DustEmitter : public ParticleEmitter
{
public:

	DustEmitter();
	void trigger(Planet);
	void render(ShaderProgram*); 
	void reset(Particle&);
};