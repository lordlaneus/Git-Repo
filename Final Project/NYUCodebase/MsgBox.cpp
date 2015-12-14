#include "MsgBox.h"
#include "Util.h"
#include <vector>

using namespace std;

MsgBox::MsgBox()
{

}
MsgBox::MsgBox(Sprite bg, Sprite font)
{
	this->bg = bg;
	this->font = font;
	size = Vector(150, 80);
	position = Vector(5, 10);
	show = false;
}

void MsgBox::render(ShaderProgram* program)
{

	Matrix modelMatrix;
	modelMatrix.Translate(position.x, position.y, 0);
	modelMatrix.Scale(size.x, size.y, 1);
	program->setModelMatrix(modelMatrix);
	vector<float> vertices;
	vector<float> texCoords;
	//draw back;
	texCoords.insert(texCoords.end(), {
		0, 1,
		1, 0,
		0, 0,

		1, 0,
		0, 1,
		1, 1
	});
	vertices.insert(vertices.end(), {
		0, 0,
		1, 1,
		0, 1,
		1, 1,
		0, 0,
		1, 0
	});

	


	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
	glEnableVertexAttribArray(program->texCoordAttribute);

	glBindTexture(GL_TEXTURE_2D, bg.texture);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 2);

	Util::drawText(program, font.texture, text, 80, 70, 10, 2);
	Util::drawText(program, font.texture, subText1, 80, 40, 5, 2);
	Util::drawText(program, font.texture, subText2, 80, 30, 5, 2);
	Util::drawText(program, font.texture, subText3, 80, 20, 5, 2);

}