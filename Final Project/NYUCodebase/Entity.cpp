#include "Entity.h"
#include <vector>
Entity::Entity()
{
	position.x = 0;
	position.y = 0;
	rotation = 0;
	width = 1;
	height = 1;
	velocity.y = 0;
	velocity.x = 0;
	maxVel.x = -1;
	maxVel.y = -1;
	accel.x = 0;
	accel.y = 0;
	friction = 0;
	sprite.index = 0;

	ctop = false;
	cright = false;
	cleft = false;
	cdown = false;
}

void Entity::update(float tick){

	velocity.x += accel.x *tick;
	velocity.x > 0 ? velocity.x -= friction*tick : velocity.x < 0 ? velocity.x += friction*tick : velocity.x = 0;
	position.x += velocity.x*tick;
	velocity.y += accel.y*tick;
	velocity.y > 0 ? velocity.y -= friction*tick : velocity.y < 0 ? velocity.y += friction*tick : velocity.y = 0;
	position.y += velocity.y*tick;
	
}
void Entity::setTexture(GLuint t, int s, int sx, int sy)
{
	sprite.texture = t;
	sprite.index = s;
	sprite.sheetW = sx;
	sprite.sheetH = sy;
}
void Entity::render(ShaderProgram *program)
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
		modelMatrix.Scale(-width, height, 1);
	}
	else
	{
		modelMatrix.Scale(width, height, 1);
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