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
			particles[i].on = false;
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

	for (int i = 0; i < particles.size(); i++) {
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
		texCoords.insert(texCoords.end(), {
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 0.0f,

			1.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f

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
	p.lifetime = 0;
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
	int a =5;
}