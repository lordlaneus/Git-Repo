#include "Entity.h"
#include <vector>
Entity::Entity()
{
	x = 0;
	y = 0;
	width = 1;
	height = 1;
	speedY = 0;
	speedX = 0;
	maxSpeed = -1;
	accelX = 0;
	accelY = 0;
	friction = 0;
	sprite = 0;

}

void Entity::makePlayer()
{ 
	type = PLAYER;
	x = 40;
	y = 100;
	width = 8;
	height = 12;
	speedY = 0;
	speedX = 0;
	maxSpeed = -1;
	accelX = 0;
	accelY = -50;
	friction = 0;
	sprite = 2;
}
bool Entity::checkCollision(Entity &e)
{
	if (e.y - e.height / 2 > y + height / 2){ return false; }
	if (e.y + e.height / 2 < y - height / 2){ return false; }
	if (e.x - e.width / 2 > x + width / 2){ return false; }
	if (e.x + e.width / 2 < x - width / 2){ return false; }
	return true;

}
void Entity::collide(std::vector<Entity> &e)
 {
	
}
void Entity::update(float tick)
{
	if (type = PLAYER)
	{
		///
	}
	if (maxSpeed > 0 && abs(speedX) > maxSpeed)
	{
		if (speedX < 0)
		{
			speedX = -maxSpeed;
		}
		else
		{
			speedX = maxSpeed;
		}
	}
	if (maxSpeed > 0 && speedY > maxSpeed)
	{
		if (speedY < 0)
		{
			speedY = -maxSpeed;
		}
		else
		{
			speedY = maxSpeed;
		}
	}

	speedX += accelX *tick;
	speedX > 0 ? speedX -= friction*tick : speedX < 0 ? speedX += friction*tick : speedX = 0;
	x += speedX*tick;
	speedY += accelY*tick;
	speedY > 0 ? speedY -= friction*tick : speedY < 0 ? speedY += friction*tick : speedY = 0;
	y += speedY*tick;
	
}
void Entity::setTexture(GLuint t, int s, int sx, int sy)
{
	texture = t;
	sprite = s;
	sheetX = sx;
	sheetY = sy;
}
void Entity::render(ShaderProgram *program)
{

	float u = (float)(((int)sprite) % sheetX) / (float)sheetX;
	float v = (float)(((int)sprite) / sheetX) / (float)sheetY;
	float spriteWidth = 1.0 / (float)sheetX;
	float spriteHeight = 1.0 / (float)sheetY;
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
	modelMatrix.Translate(x, y, 0);
	modelMatrix.Scale(width, height, 1);
	

	program->setModelMatrix(modelMatrix);

	//drawing
	glUseProgram(program->programID);

	
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glBindTexture(GL_TEXTURE_2D, texture);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLES, 3, 3);


	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}