#include "Sprite.h"
void Sprite::render(ShaderProgram* program, float x, float y, float width, float height, float rotation, bool flipped)
{

	float u = (float)(((int)index) % sheetW) / (float)sheetW;
	float v = (float)(((int)index) / sheetW) / (float)sheetH;
	float spriteWidth = 1.0 / (float)sheetW;
	float spriteHeight = 1.0 / (float)sheetH;
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
	modelMatrix.Translate(x, y, 0);
	modelMatrix.Rotate(rotation);
	if (flipped)
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

	glBindTexture(GL_TEXTURE_2D, texture);

	glDrawArrays(GL_TRIANGLES, 0, 6);


	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}