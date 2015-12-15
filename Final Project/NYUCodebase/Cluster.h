
#pragma once

#include "ShaderProgram.h"
#include "Planet.h"
#include "Sprite.h"
#include <vector>

class Planet;
class Cluster{
public:
	float radius = 300;
	int x;
	int y;
	Sprite sprite;
	std::vector<Planet> planets;

	Cluster();
	Cluster(int, Sprite);


	Planet* checkCollision(Vector);
	int calculatePlanetType(float);
	void update(float tick);
	void render(ShaderProgram*);

};