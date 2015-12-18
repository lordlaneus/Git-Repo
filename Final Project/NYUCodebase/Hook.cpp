#include "Hook.h"
#include <vector>
using namespace std;

Hook::Hook(Player* p)
{
	player = p;
	size = 1;
	state = away;
}
void Hook::shoot(Vector target,float strength, float length, float size)
{
	
	velocity = target;
	velocity.normalize(castSpeed);
	this->strength = strength;
	maxLength = length;
	this->size = size;
	position = player->position;
	state = cast;
}
void Hook::update(float elapsed)
{
	if (state == cast)
	{
		position = position + velocity*elapsed;
		anchor = player->g->cluster->checkCollision(position);
		if (anchor)
		{
			state = reeling;
		}
		if (position.distance(player->position) > maxLength)
		{
			state = recoiling;
		}
	}
	else if (state == recoiling)
	{
		velocity =player->position - position;
		if (velocity.length() < 5)
		{
			state = away;
		}
		velocity.normalize(castSpeed * 2);
		position = position + velocity*elapsed;
	}
	else if (state == reeling)
	{
		Vector pull = position - player->position;
		pull.normalize(strength);
		player->velocity = player->velocity + pull * elapsed;
	}

}
void Hook::render(ShaderProgram* program)
{
	if (state == away)
	{
		return;
	}
	renderChain(program);
	float u = (float)(((int)2) % sprite.sheetW) / (float)sprite.sheetW;
	float v = (float)(((int)2) / sprite.sheetW) / (float)sprite.sheetH;
	float spriteWidth = 1.0 / (float)sprite.sheetW;
	float spriteHeight = 1.0 / (float)sprite.sheetH;
	GLfloat texCoords[] = {
		u, v + spriteHeight,
		u + spriteWidth, v,
		u, v,
		u + spriteWidth, v,
		u, v + spriteHeight,
		u + spriteWidth, v + spriteHeight
	};
	float vertices[] = { -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f };
	Matrix modelMatrix;
	modelMatrix.Translate(position.x, position.y, 0);
	modelMatrix.Scale(size*2, size*2, 1);
	program->setModelMatrix(modelMatrix);

	//drawing
	glUseProgram(program->programID);


	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glBindTexture(GL_TEXTURE_2D, sprite.texture);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLES, 3, 3);


	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}
void Hook::renderChain(ShaderProgram* program)
{
	vector<float> vertices;
	vector<float> texCoords;


	Matrix modelMatrix;
	program->setModelMatrix(modelMatrix);

	for (int i = 0; i < segments; i++) {
		float m = ((float)i/ segments);
		float x = player->position.x + (position.x - player->position.x)*m;
		float y = player->position.y + (position.y - player->position.y)*m;
		vertices.insert(vertices.end(), {
			x - size, y + size,
			x - size, y - size,
			x + size, y + size,
			x + size, y + size,
			x - size, y - size,
			x + size, y - size
		});
		texCoords.insert(texCoords.end(), {
			0.5f, 0.5f,
			0.5f, 1.0f,
			1.0f, 0.5f,

			1.0f, 0.5f,
			0.5f, 1.0f,
			1.0f, 1.0f

		});
	}
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
	glEnableVertexAttribArray(program->texCoordAttribute);

	glBindTexture(GL_TEXTURE_2D, sprite.texture);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 2);
}