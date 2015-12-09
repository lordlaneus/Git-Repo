#pragma once

#include "ShaderProgram.h"
#include "Sprite.h"
#include "Cluster.h"
#include "Player.h"
#include "ParticleEmitter.h"
#include <map>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>

class Player;
class Game {
public:
	enum State { title, play, menu,done };
	Game();
	State state = title;
	Cluster cluster;
	Player* player;
	std::map<std::string, Sprite> sprites;
	ParticleEmitter dustEmitter;

	void addSprite(std::string, const char*, int, int, int);
	void update(float elapsed);
	void render(ShaderProgram*);
	void emitDust(Vector position, Planet p);
};