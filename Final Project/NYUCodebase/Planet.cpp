#include "Planet.h"
#include <vector>

using namespace std;

Planet::Planet(Sprite sprite, float x, float y, float size, float density)
{

	this->sprite = sprite;
	this-> x = x;
	this->y = y;
	this->size = size;
	this->density = density;

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
		modelMatrix.Translate(x, y, 0);
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