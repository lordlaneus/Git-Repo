#pragma once

#include "ShaderProgram.h"
#include "Sprite.h"
#include "Cluster.h"
#include "Player.h"
#include "GUI.h"
#include "Cursor.h"
#include "Bar.h"
#include "Menu.h"
#include "MsgBox.h"
#include "Compass.h"
#include "TextDisplay.h"
#include "ParticleEmitter.h"
#include "DustEmitter.h"
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
	int startupLength = 3;
	float startupTimer = 0;
	int totalEnemies = 10;
	bool exploring = false;
	enum State { ready, startup, title, play, paused, message, victory, gameOver, done, };
	Game();
	State state = title;
	Cluster cluster;
	Player* player;
	std::map<std::string, Sprite> sprites;
	std::map < std::string, Mix_Chunk*> sounds;
	std::map < std::string, Mix_Music*> music;
	Cursor cursor;
	Menu mainMenu;
	Menu gameOverMenu;
	Menu victoryMenu;
	Menu pauseMenu;
	Bar health;
	Compass compass;
	MsgBox msgBox;
	TextDisplay enemyCount;
	Projectile ball;
	std::vector<GUI*> indicators;
	std::vector<GUI*> gui;
	std::vector<Entity*> entities;
	DustEmitter dustEmitter;
	ParticleEmitter fireEmitter;

	void addSprite(std::string, const char*, int, int, int);
	void addSound(std::string, const char*);
	void addMusic(std::string, const char*);


	void start(ShaderProgram*);
	void reset();
	void update(float elapsed);
	void renderFade(ShaderProgram*, int);
	void renderBG(ShaderProgram*);
	void render(ShaderProgram*);
	void pause();
	void killAll();
	void showMsg(std::string msg, std::string subMsg1 = "", std::string subMsg2 = "", std::string subMsg3 = "");
	void triggerDust(Vector position, Planet p,float speed);
	void triggerParticles(ParticleEmitter& pe, Vector position);
	void triggerParticles(ParticleEmitter& pe, Vector position, Planet p,float speed =5);
	void playSound(std::string);
	void playMusic(std::string);
};