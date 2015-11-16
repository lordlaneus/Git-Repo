#include "Cluster.h"
using namespace std;

Cluster::Cluster(int size,Sprite s)
{
	x = 0;
	y = 0;
	sprite = s;
	for (int i = 0; i < size; i++)
	{
		float x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 100));
		float y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 100));
		float mass = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 30));
		planets.push_back(Planet(sprite, x, y, mass));
	}
}
void Cluster::update(float tick){

}
void Cluster::render(ShaderProgram *program)
{
	
	std::vector<float> vertexData;
	std::vector<float> texCoordData;
		for (int i = 0; i < planets.size(); i++)
		{
			float u = (float)(((int)planets[i].sprite.index) % sprite.sheetW) / (float)sprite.sheetW;
			float v = (float)(((int)planets[i].sprite.index) / sprite.sheetW) / (float)sprite.sheetH;

			float spriteWidth = 1.0 / (float)sprite.sheetW;
			float spriteHeight = 1.0 / (float)sprite.sheetH;
			vertexData.insert(vertexData.end(), {
				planets[i].x - planets[i].size / 2, planets[i].y - planets[i].size / 2,
				planets[i].x - planets[i].size / 2, planets[i].y + planets[i].size / 2,
				planets[i].x + planets[i].size / 2, planets[i].y + planets[i].size / 2,

				planets[i].x - planets[i].size / 2, planets[i].y - planets[i].size / 2,
				planets[i].x + planets[i].size / 2, planets[i].y + planets[i].size / 2,
				planets[i].x + planets[i].size / 2, planets[i].y - planets[i].size / 2
			});
			texCoordData.insert(texCoordData.end(), {
				u, v,
				u, v + (spriteHeight),
				u + spriteWidth, v + (spriteHeight),
				u, v,
				u + spriteWidth, v + (spriteHeight),
				u + spriteWidth, v
			});
		}
	Matrix modelMatrix;
	program->setModelMatrix(modelMatrix);


	glUseProgram(program->programID);


	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);

	glBindTexture(GL_TEXTURE_2D, sprite.texture);

	float v1 = texCoordData.data()[0];
	float v2 = texCoordData.data()[1];
	float v3 = texCoordData.data()[2];
	float v4 = texCoordData.data()[3];
	float v5 = texCoordData.data()[4];
	float v6 = texCoordData.data()[5];

	glDrawArrays(GL_TRIANGLES, 0, planets.size()* 6);


	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}