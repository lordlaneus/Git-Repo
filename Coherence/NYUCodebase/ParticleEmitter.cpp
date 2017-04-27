#include "ParticleEmitter.h"
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "Util.h"

using namespace std;

ParticleEmitter::ParticleEmitter(int pCount)
{
	this->pCount = pCount;
	for (int i = 0; i < pCount; i++)
	{
		particles.push_back(Particle());
	}
}
void ParticleEmitter::update(float elapsed)
{
	for (int i = 0; i < particles.size(); i++) {
		particles[i].update(elapsed);
		if (particles[i].lifetime > maxLifetime)
		{
			if (loop)
			{
				reset(particles[i]);
			}
			else
			{
				particles[i].on = false;
			}
			
		}

	}
}
void ParticleEmitter::render(ShaderProgram* program)
{
	std::vector<float> vertices;
	std::vector<float> texCoords;


	Matrix modelMatrix;
	modelMatrix.Translate(position.x, position.y, 0);

	program->setModelMatrix(modelMatrix);

	for (int i = 0; i < particles.size() && i<pCount; i++) {
		if (particles[i].on)
		{ 
		float m = (particles[i].lifetime / maxLifetime);
		float size = (startSize + m*(endSize - startSize));
		vertices.insert(vertices.end(), {
			particles[i].position.x - size, particles[i].position.y + size,
			particles[i].position.x - size, particles[i].position.y - size,
			particles[i].position.x + size, particles[i].position.y + size,
			particles[i].position.x + size, particles[i].position.y + size,
			particles[i].position.x - size, particles[i].position.y - size,
			particles[i].position.x + size, particles[i].position.y - size
		});
		float u = (float)(((int)sprite.index) % sprite.sheetW) / (float)sprite.sheetW;
		float v = (float)(((int)sprite.index) / sprite.sheetW) / (float)sprite.sheetH;
		float spriteWidth = 1.0 / (float)sprite.sheetW;
		float spriteHeight = 1.0 / (float)sprite.sheetH;
		texCoords.insert(texCoords.end(), {
			u, v+spriteHeight,
			u ,v,
			u+spriteWidth, v + spriteHeight,

			u + spriteWidth, v + spriteHeight,
			u, v,
			u + spriteWidth, v
		});
		}
	}
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
	glEnableVertexAttribArray(program->texCoordAttribute);

	glBindTexture(GL_TEXTURE_2D, sprite.texture);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 2);
}


void ParticleEmitter::reset(Particle &p)
{
	p.velocity = velocity;
	p.velocity.x += Util::randFloat()*velocityDev.x - velocityDev.x / 2;
	p.velocity.y += Util::randFloat()*velocityDev.y - velocityDev.y / 2;
	p.size = startSize;
	p.lifetime = Util::randFloat()*lifeDev;
	p.position.x = 0;
	p.position.y = 0;
	p.gravity = gravity;
	p.mass = mass;
	p.on = true;
	p.start = position;
}
void ParticleEmitter::trigger()
{
	for (int i = 0; i < particles.size(); i++) {
		reset(particles[i]);
	}
}
void ParticleEmitter::start()
{
	for (int i = 0; i < particles.size(); i++) {
		reset(particles[i]);
		particles[i].lifetime = maxLifetime* i / particles.size();
		float a = particles[i].lifetime;

		float b = 4;
	}
}