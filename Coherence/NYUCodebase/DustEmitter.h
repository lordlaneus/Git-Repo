#pragma once

#include "ParticleEmitter.h"
#include "Particle.h"
#include "ShaderProgram.h"

class Tile;
class DustEmitter : public ParticleEmitter
{
public:

	DustEmitter();
	void trigger(Tile*);
	void render(ShaderProgram*); 
	void reset(Particle&);
};