#include "Bar.h"
#include <vector>
using namespace std;
Bar::Bar()
{
}
Bar::Bar(Sprite sprite, float max, float length, float height, Entity* e)
{
	this->e = e;

	this->sprite = sprite;
	position.clear();
	size = Vector(length, height);
	this->max = max;
	current = max;
}
void Bar::render(ShaderProgram *program)
{

	Matrix modelMatrix;
	modelMatrix.Translate(position.x, position.y, 0);
	modelMatrix.Scale(size.x, size.y, 1);
	program->setModelMatrix(modelMatrix);
	vector<float> vertices;
	vector<float> texCoords;
	//draw back;
	texCoords.insert(texCoords.end(),{
		0, 0,
		1, 0.25,
		0, 0.25,

		1, 0.25,
		0, 0,
		1, 0
	});
	vertices.insert(vertices.end(), {
		0,0,
		1,1,
		0,1,
		1,1,
		0,0,
		1,0
	});
	
	//draw front
	float perc = current / max;
	perc -= 15 / 16;
	float top = color*0.25;
	float btm = top + 0.25;
	if (perc < 0)
	{
		perc = 0;
	}
	texCoords.insert(texCoords.end(), {
		0, btm,
		perc, top,
		0, top,

		perc, top,
		0, btm,
		perc, btm
	});
	vertices.insert(vertices.end(), {
		0, 0,
		perc, 1,
		0, 1,
		perc, 1,
		0, 0,
		perc, 0
	});



	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
	glEnableVertexAttribArray(program->texCoordAttribute);

	glBindTexture(GL_TEXTURE_2D, sprite.texture);

	glDrawArrays(GL_TRIANGLES, 0,vertices.size()/2);
}

void Bar::update(float elapsed)
{
	GUI::update(elapsed);
	if (e)
	{
		if (!e->active)
		{
			active = false;
		}
		else
		{
			current = e->health;
			if (!fixed)
			{
				position = e->position;
				position.x -= e->size.x / 2;
				position.y += e->size.y / 2;
			}
		}
	}
}