#pragma once

#include "ShaderProgram.h"
#include "Sprite.h"
#include "Cluster.h"
#include "Player.h"
#include "GUI.h"
#include "Bar.h"
#include "MsgBox.h"
#include "TextDisplay.h"
#include "ParticleEmitter.h"
#include "Projectile.h"
#include <map>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>

class Player;
class Game {
public:
	int totalEnemies = 10;
	enum State { title, play, paused,done, message,gameOver};
	Game();
	State state = title;
	Cluster cluster;
	Player* player;
	Mix_Music *bgm;
	std::map<std::string, Sprite> sprites;
	std::map < std::string, Mix_Chunk*> sounds;
	Bar health;
	MsgBox msgBox;
	TextDisplay enemyCount;
	Projectile ball;
	std::vector<GUI*> gui;
	std::vector<Entity*> entities;
	ParticleEmitter dustEmitter;
	ParticleEmitter fireEmitter;

	void addSprite(std::string, const char*, int, int, int);
	void addSound(std::string, const char*);
	void reset();
	void update(float elapsed);
	void renderBG(ShaderProgram*);
	void render(ShaderProgram*);
	void pause();
	void showMsg(std::string msg, std::string subMsg1 = "", std::string subMsg2 = "", std::string subMsg3 = "");
	void triggerParticles(ParticleEmitter& pe, Vector position);
	void triggerParticles(ParticleEmitter& pe, Vector position, Planet p,float speed =5);
	void playSound(std::string);
};