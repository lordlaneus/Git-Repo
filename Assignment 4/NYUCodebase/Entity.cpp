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
	maxSpeedX = -1;
	maxSpeedY = -1;
	accelX = 0;
	accelY = 0;
	friction = 0;
	sprite = 0;

	ctop = false;
	cright = false;
	cleft = false;
	cdown = false;
}

void Entity::makePlayer()
{ 
	type = PLAYER;
	x = 30;
	y = -10;
	width = 8;
	height = 12;
	speedY = 0;
	speedX = 0;
	maxSpeedX = 50;
	accelX = 0;
	accelY = -100;
	friction = 25;
	sprite = 2;
}
void Entity::checkCollisions(Level l)
{
	if (collidePoint(l, x, y - height / 2))
	{
		speedY = 0;
		y -=  fmod(y - height / 2, l.tileSize);	
	}
	else if (collidePoint(l, x, y + height / 2))
	{
		speedY = 0;
		y -= fmod(y + height / 2, l.tileSize) + l.tileSize;
	}

	if (collidePoint(l, x - width/2 , y))
	{
		speedX = 0;
		accelX = 0;
		x -= fmod(x - width / 2, l.tileSize) - l.tileSize;
	}
	else if(collidePoint(l, x + width / 2, y))
	{
		speedX = 0;
		accelX = 0;
		x -= fmod(x + width / 2, l.tileSize);
	}
	
	
}
float Entity::collidePoint(Level l, float x, float y)
{
	{
		int xt = x / (int)l.tileSize;
		int yt = (-y) / (int)l.tileSize;
		if (xt >= 0 && yt >= 0 && xt < l.tiles.size() & yt < l.tiles.size())
		{
			if (l.tiles[yt][xt] != 0)
			{
				return true;
			}
		}
	}
	return false;
}
void Entity::update(float tick){
	if (type = PLAYER)
	{
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