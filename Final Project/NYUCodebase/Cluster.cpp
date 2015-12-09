#include "Cluster.h"
#include "Util.h"
using namespace std;

Cluster::Cluster()
{

}
Cluster::Cluster(int size,Sprite s)
{
	x = 0;
	y = 0;
	sprite = s;
	for (int i = 0; i < size; i++)
	{
		Planet p(sprite);
		if (Util::randFloat() < .2)
		{
			p.type = Planet::star;
			p.sprite.index = 1;
		}
		do
		{
			p.position.x = static_cast <float> (Util::randFloat() * 1000)-500;
			p.position.y = static_cast <float> (Util::randFloat() * 1000)-500;
			p.size = static_cast <float> (Util::randFloat() * 30) + 5;
		} while (!p.validIn(*this));
		planets.push_back(p);
	}
}
void Cluster::update(float tick){

}
Planet* Cluster::checkCollision(Vector v)
{
	for (int i = 0; i < planets.size(); i++)
	{
		if (v.distance(planets[i].position) < planets[i].size / 2)
		{
			return &planets[i];
		}
	}
	return NULL;
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
			texCoordData.insert(texCoordData.end(), {
				u, v,
				u + spriteWidth, v+spriteHeight,
				u, v+spriteHeight,

				u + spriteWidth, v + spriteHeight,
				u, v,
				u + spriteWidth, v
			});
			vertexData.insert(vertexData.end(), {
				planets[i].position.x - planets[i].size / 2, planets[i].position.y - planets[i].size / 2,
				planets[i].position.x + planets[i].size / 2, planets[i].position.y + planets[i].size / 2,
				planets[i].position.x - planets[i].size / 2, planets[i].position.y + planets[i].size / 2,

				planets[i].position.x + planets[i].size / 2, planets[i].position.y + planets[i].size / 2,
				planets[i].position.x - planets[i].size / 2, planets[i].position.y - planets[i].size / 2,
				planets[i].position.x + planets[i].size / 2, planets[i].position.y - planets[i].size / 2
			});
			int a;
			
		}
	Matrix modelMatrix;
	program->setModelMatrix(modelMatrix);


	glUseProgram(program->programID);


	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);

	glBindTexture(GL_TEXTURE_2D, sprite.texture);

	glDrawArrays(GL_TRIANGLES, 0, planets.size()* 6);


	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}