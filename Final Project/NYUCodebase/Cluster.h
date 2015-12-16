
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
	Game* g;
	float radius = 300;
	int x;
	int y;
	Sprite sprite;
	std::vector<Planet> planets;

	Cluster();
	Cluster(Game* g, int, Sprite);


	Planet* checkCollision(Vector);
	int calculatePlanetType(float);
	void update(float tick);
	void render(ShaderProgram*);

};