#include "Weapon.h";
#define TAU 1.57079
using namespace std;
Weapon::Weapon()
{
	active = false;
}
Weapon::Weapon(Sprite sprite, Player* player)
{
	this->sprite = sprite;
	this->player = player;
	size = Vector(3, 8);
	anim.addFrame(0, 0.05);
	anim.addFrame(1, 0.05);
	anim.addFrame(2, 0.05);
	anim.addFrame(3, 0.05);
	anim.addFrame(4, 0.05);
	anim.addFrame(5, 0.05);
	anim.addFrame(6, 0.05);
	anim.addFrame(7, 0.05);
}

void Weapon::swing(Vector d)
{
	dir = (d- player->position);
	position = player->position;
	rotation = dir.angle();
	active = true;
	lifetime = 0;
}

void Weapon::render(ShaderProgram* program)
{
	float u = (float)(((int)sprite.index) % sprite.sheetW) / (float)sprite.sheetW;
	float v = (float)(((int)sprite.index) / sprite.sheetW) / (float)sprite.sheetH;
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
	// our regular sprite drawing
	Matrix modelMatrix;
	modelMatrix.Translate(position.x, position.y, 0);
	modelMatrix.Rotate(rotation);
	if (sprite.flipped)
	{
		modelMatrix.Scale(-size.x, size.y, 1);
	}
	else
	{
		modelMatrix.Scale(size.x, size.y, 1);
	}


	program->setModelMatrix(modelMatrix);

	//drawing
	glUseProgram(program->programID);


	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glBindTexture(GL_TEXTURE_2D, sprite.texture);

	glDrawArrays(GL_TRIANGLES, 0, 6);


	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);

}
void Weapon::update(float elapsed)
{
	if (active)
	{
		lifetime += elapsed;
		//position = player->position + dir.normalize(lifetime/maxLifetime*range);
		position = position + dir.normalize(range/maxLifetime)*elapsed;
		float m = lifetime / maxLifetime;
		sprite.index = anim.getFrame(m);
		if (lifetime > maxLifetime)
		{
			active = false;
		}
	}
}

