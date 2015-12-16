
#include "DustEmitter.h"
#include "Util.h"
using namespace std;

DustEmitter::DustEmitter()
{
	maxLifetime = 1;
	velocityDev.x = 10;
	velocityDev.y = 10;
	startSize = 1;
	endSize = 0;
}
void DustEmitter::trigger(Planet* p)
{
	sprite = p->sprite;
	ParticleEmitter::trigger();
}

void DustEmitter::reset(Particle& p)
{
	ParticleEmitter::reset(p);
	p.u = (float)(((int)sprite.index) % sprite.sheetW + Util::randFloat()*0.9) / (float)sprite.sheetW;
	p.v = (float)(((int)sprite.index + Util::randFloat()*0.9) / sprite.sheetW) / (float)sprite.sheetH;
	p.spriteWidth = 0.1 / (float)sprite.sheetW;
	p.spriteHeight = 0.1 / (float)sprite.sheetH;
}

void DustEmitter::render(ShaderProgram* program)
{
	std::vector<float> vertices;
	std::vector<float> texCoords;


	Matrix modelMatrix;
	modelMatrix.Translate(position.x, position.y, 0);

	program->setModelMatrix(modelMatrix);

	for (int i = 0; i < particles.size() && i<pCount; i++) {
		if (particles[i].on)
		{
			Particle p = particles[i];
			float m = (particles[i].lifetime / maxLifetime);
			float size = (startSize + m*(endSize - startSize));

			texCoords.insert(texCoords.end(), {
				p.u, p.v + p.spriteHeight,
				p.u, p.v,
				p.u + p.spriteWidth, p.v + p.spriteHeight,

				p.u + p.spriteWidth, p.v + p.spriteHeight,
				p.u, p.v,
				p.u + p.spriteWidth, p.v
			});

			vertices.insert(vertices.end(), {
				p.position.x - size, p.position.y + size,
				p.position.x - size, p.position.y - size,
				p.position.x + size, p.position.y + size,
				p.position.x + size, p.position.y + size,
				p.position.x - size, p.position.y - size,
				p.position.x + size, p.position.y - size
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