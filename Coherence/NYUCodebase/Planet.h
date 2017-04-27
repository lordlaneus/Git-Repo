
#pragma once

#include "ShaderProgram.h"
#include "Sprite.h"
#include "Cluster.h"
#include "Vector.h"
#include <vector>
#include "Game.h"

class Cluster;
class Planet {
public:
	enum Type{moon, star};
	Type type = moon;
	bool on;
	float damage = 20;
	Vector position;
	float size;
	float density;
	Sprite sprite;
	Cluster* cluster;

	Planet();
	Planet(std::string line, Cluster* cluster, Sprite sprite);
	Planet(Cluster*,Sprite, float=0, float=0, float=1, float=2);
	
	float mass();

	void render(ShaderProgram*);
	bool validIn(Cluster);

};