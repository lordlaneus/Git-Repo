
#pragma once

#include "ShaderProgram.h"
#include "Planet.h"
#include "Sprite.h"
#include "Game.h"
#include <vector>

class Planet;
class Game;
class Cluster{
public:
	const float radius = 300;

	Game* g;
	Sprite sprite;
	std::vector<Planet> planets;

	Cluster();
	Cluster(Game*, Sprite);
	Cluster(Game* g, int, Sprite);


	Planet* checkCollision(Vector);
	int calculatePlanetType(float);
	Planet* closestPlanet(Vector);
	void update(float tick);
	void render(ShaderProgram*);

};