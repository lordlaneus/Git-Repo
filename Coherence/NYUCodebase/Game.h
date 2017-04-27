#pragma once

#include "ShaderProgram.h"


#include "Entity.h"
#include "Tile.h"
#include "Sprite.h"
#include "Room.h"
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

class Room;
class Entity;
class Player;
class Game {
public:
	const int enemyChasers = 15;
	const int enemyRollers = 10;
	const int enemySpitters = 5;
	const float startupLength = 3;//3

	enum State { ready, startup, title, play, menu, message, done};


	Menu gameOverMenu;
	Menu mainMenu;
	Menu pauseMenu;
	Menu playMenu;
	Menu victoryMenu;


	Room* room;
	bool cPressed;
	Menu* currentMenu;
	Cursor cursor;
	//Compass compass;
	DustEmitter dustEmitter;
	TextDisplay enemyCount;
	Bar energy;
	std::vector<Entity*> entities;
	bool exploring = false;
	ParticleEmitter fireEmitter;
	std::vector<GUI*> gui;
	Bar health;
	std::vector<GUI*> indicators;
	float lastFrameTime;
	MsgBox msgBox;
	std::map < std::string, Mix_Music*> music;
	
	Player* player;
	std::map < std::string, Mix_Chunk*> sounds;
	std::map<std::string, Sprite> sprites;
	float startupTimer = 0;
	State state;
	float timer;
	TextDisplay timerDisplay;

	Game();

	void addMusic(std::string, const char*);
	void addSound(std::string, const char*);
	void addSprite(std::string, const char*, int, int, int);
	
	void killAll();
	void menuSelect();
	void pause();
	void playMusic(std::string);
	void playSound(std::string);
	void reset();
	void showMsg(std::string msg, std::string subMsg1 = "", std::string subMsg2 = "", std::string subMsg3 = "");
	void start(ShaderProgram*);
	void triggerDust(Vector position, Tile t,float speed);
	void triggerFire(Vector position);
	void triggerParticles(ParticleEmitter& pe, Vector position, Tile t,float speed =5);

	void update(float elapsed);
	void renderBG(ShaderProgram*);
	void renderFade(ShaderProgram*, int);
	void renderGui(ShaderProgram*);
	void render(ShaderProgram*);
};