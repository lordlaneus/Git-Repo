
#pragma once

#include "ShaderProgram.h"
#include "Sprite.h"
#include <vector>
class Cluster;
class Planet {
public:
	Planet();
	Planet(Sprite, float, float, float, float=1);
	void render(ShaderProgram*);
	float x;
	float y;
	float size;
	float density;
	Sprite sprite;
	Cluster* cluster;

};