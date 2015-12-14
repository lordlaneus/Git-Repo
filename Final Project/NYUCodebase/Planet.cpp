#include "Planet.h"
#include "Cluster.h"
#include <vector>

using namespace std;

Planet::Planet()
{

}
Planet::Planet(Sprite sprite, float x, float y, float size, float density)
{

	this->sprite = sprite;
	position.x = x;
	on = false;
	position.y = y;
	this->size = size;
	this->density = density;

}
bool Planet::validIn(Cluster c)
{
	for (int i = 0; i < c.planets.size(); i++)
	{
		Planet p = c.planets[i];
		if (p.position.distance(position) < p.size / 2 + size / 2 + 10)
		{
			return false;
		}
	}
	return true;
}
float Planet::mass()
{
	return size*density;
}

void Planet::render(ShaderProgram *program){

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
		modelMatrix.Scale(size, size, 1);


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