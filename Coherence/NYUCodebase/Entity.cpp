#include "Entity.h"
#include <vector>
#include "Tile.h"
#include <math.h>

#define TAU 1.57079
using namespace std;
Entity::Entity()
{

}
Entity::Entity(Game* g)
{

	this->g = g;
	size = Vector(1, 1);
}
bool Entity::collides(Entity&e)
{
	//should be functionally identical to the single return statement
	//but it's usefull for debbuging
	/*if (!collides(e, cos(rotation), sin(rotation)))
	{
		return false;
	}
	if (!collides(e, cos(rotation), cos(rotation)))
	{
		return false;
	}
	if (!collides(e, cos(e.rotation), sin(e.rotation)))
	{
		return false;
	}
	if (!collides(e, sin(e.rotation), cos(e.rotation)))
	{
		return false;
	}*/
	return (collides(e, cos(rotation), sin(rotation)) &&
		collides(e, sin(rotation), cos(rotation))&&
		collides(e, cos(e.rotation), sin(e.rotation)) &&
		collides(e, sin(e.rotation), cos(e.rotation)));

}
bool Entity::collides(Tile& p)
{
	//todo
	return false;
}
bool Entity::collides(Entity& e, float x, float y)
{
	Vector normal(x, y);
	vector<float> v1 = nVertices(normal);
	vector<float> v2 = e.nVertices(normal);
	bool less = false;
	bool more = false;
	for (int i = 0; i < 4; i++ )
	{
		for (int j = 0; j < 4; j++)
		{
			if (v1[i] < v2[j])
			{
				less = true;
			}
			else
			{
				more = true;
			}
		}
	}
	
	return (less&&more);
}
vector<float> Entity::nVertices(Vector normal)
{
	vector<float>nv;
	nv.push_back((position + (size / 2).rotate(rotation)).dot(normal));
	nv.push_back((position - (size / 2).rotate(rotation)).dot(normal));
	Vector invSize = size;
	invSize.x = -size.x;
	nv.push_back((position + (invSize / 2).rotate(rotation)).dot(normal));
	nv.push_back((position -(invSize / 2).rotate(rotation)).dot(normal));

	return nv;
}
void Entity::playerCollision(Player* p) 
{
}
void Entity::setTexture(GLuint t, int s, int sx, int sy)
{
	sprite.texture = t;
	sprite.index = s;
	sprite.sheetW = sx;
	sprite.sheetH = sy;
}
void Entity::update(float elapsed){

	if (hurt > 0)
	{
		hurt -= elapsed;
	}
	position.x += velocity.x*elapsed;
	position.y += velocity.y*elapsed;
	
}

void Entity::render(ShaderProgram *program, int offset)
{
	float u;
	float v;
	if (offset <0)
	{
			u = (float)((offset) % sprite.sheetW) / (float)sprite.sheetW;
			v = (float)((offset) / sprite.sheetW) / (float)sprite.sheetH;
	}
	else
	{
			u = (float)(((int)sprite.index + offset) % sprite.sheetW) / (float)sprite.sheetW;
			v = (float)(((int)sprite.index + offset) / sprite.sheetW) / (float)sprite.sheetH;
	}
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