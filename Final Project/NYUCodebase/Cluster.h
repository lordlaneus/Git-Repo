
#pragma once

#include "ShaderProgram.h"
#include "Planet.h"
#include "Sprite.h"
#include <vector>

const float WIDTH = 5;
const float HEIGHT = 5;
class Planet;
class Cluster{
public:
	Cluster(int, Sprite);
	int x;
	int y;
	Sprite sprite;
	std::vector<Planet> planets;
	void update(float tick);
	void render(ShaderProgram*);

};