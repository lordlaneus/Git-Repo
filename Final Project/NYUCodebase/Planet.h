
#pragma once

#include "ShaderProgram.h"
#include "Sprite.h"
#include "Vector.h"
#include <vector>
class Cluster;
class Planet {
public:
	enum Type{moon, star};
	Planet();
	Planet(Sprite, float=0, float=0, float=1, float=1);
	float mass();
	void render(ShaderProgram*);
	bool validIn(Cluster);

	Type type = moon;
	bool on;
	Vector position;
	float size;
	float density;
	Sprite sprite;
	Cluster* cluster;

};