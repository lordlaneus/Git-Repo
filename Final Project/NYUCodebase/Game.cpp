
#include <fstream>
#include <iostream>

#include "Game.h"
#include "Util.h"
#include "Bar.h"
#include "DustEmitter.h"
#include "Enemy.h"
#include "Roller.h"


using namespace std;
Game::Game()
{
	//load images
	addSprite("logo", "resources\\logo.png", 0, 1, 1);
	addSprite("fade", "resources\\fade.png", 0, 4, 4);
	addSprite("gui", "resources\\gui.png", 0, 2, 2);
	addSprite("bg", "resources\\background.png", 0, 1, 1);
	addSprite("planet", "resources\\Planets.png", 0, 4, 4);
	addSprite("enemy", "resources\\Enemy Sheet.png", 0, 4, 2);
	addSprite("player", "resources\\Player Sheet.png", 1, 4, 4);
	addSprite("particles", "resources\\Particles.png", 0, 2,2);
	addSprite("bar", "resources\\bar.png", 0, 1, 4);
	addSprite("msg", "resources\\messageBox.png", 0, 1, 1);
	addSprite("font", "resources\\font.png", 16, 16, 16);
	addSprite("attack", "resources\\attack.png", 0, 8, 1);
	addSprite("tutorial", "resources\\tutorial.png", 0, 4, 3);

	//load sounds
	addSound("startup", "resources\\startup.wav");
	addSound("boop", "resources\\boop.wav");
	addSound("hurt", "resources\\hurt.wav");
	addSound("land", "resources\\land.wav");
	addSound("death", "resources\\death.wav");
	addSound("enemyHurt", "resources\\enemy hurt.wav");
	addSound("fire", "resources\\fire.wav");
	addSound("explode", "resources\\explode.wav");
	addSound("zap", "resources\\zap.wav");
	addMusic("bgm", "resources\\bgm.wav");

	//configure menus
	mainMenu.title = "Main Menu"; 
	mainMenu.font = sprites["font"];
	mainMenu.visible = true;
	mainMenu.showTitle = false;
	mainMenu.addOption("Play");
	mainMenu.addOption("Options");
	mainMenu.addOption("Quit");
	currentMenu = &mainMenu;

	playMenu.title = "Play Menu";
	playMenu.showTitle = false;
	playMenu.margin = 20;
	playMenu.font = sprites["font"];
	playMenu.addOption("Main Game");
	playMenu.addOption("Free Roam");
	playMenu.addOption("Tutorial");
	playMenu.addOption("Back");
	gui.push_back(&playMenu);

	victoryMenu.title = "Victory!";
	victoryMenu.font = sprites["font"];
	victoryMenu.margin = 15;
	victoryMenu.addOption("Restart");
	victoryMenu.addOption("Explore");
	gui.push_back(&victoryMenu);

	pauseMenu.title = "Paused";
	pauseMenu.margin = 15;
	pauseMenu.font = sprites["font"];
	pauseMenu.addOption("Resume");
	pauseMenu.addOption("Quit");
	gui.push_back(&pauseMenu);

	gameOverMenu = pauseMenu;
	gameOverMenu.title = "Game Over";
	gui.push_back(&gameOverMenu);


	//configure gui elements
	health = Bar(sprites["bar"], 100, 25, 2);
	health.position = Vector(3, 95);
	gui.push_back(&health);

	energy = health;
	energy.color = Bar::blue;
	energy.position = Vector(3, 92);
	gui.push_back(&energy);


	msgBox = MsgBox(sprites["msg"], sprites["font"]);
	gui.push_back(&msgBox);

	enemyCount = TextDisplay(sprites["font"], "Enemeis Left:","", 3, 1, 90, 95);
	gui.push_back(&enemyCount);

	timerDisplay = TextDisplay(sprites["font"], "Time:","", 3, 1, 110, 90);
	gui.push_back(&timerDisplay);
	
	//In case I decide to revert to an earlier implementation of the compass
/*	compass.sprite = sprites["gui"];
	compass.position = Vector(80, 90);
	compass.size = Vector(15, 15);
	gui.push_back(&compass);*/

	cursor.sprite = sprites["gui"][2];
	cursor.size = Vector(4, 4);
	gui.push_back(&cursor);

	//configures particle emitter for fire effect
	fireEmitter.sprite.index = 7;
	fireEmitter.maxLifetime = 1;
	fireEmitter.velocityDev.x = 20;
	fireEmitter.velocityDev.y = 20;
	fireEmitter.startSize = 5;
	fireEmitter.endSize = 0;
	fireEmitter.sprite = sprites["particles"];

	reset();
	state = ready;

}
void Game::reset()
{

	state = title;//half the time reset() is called, the immediate next line sets state to something else, but whatever
	currentMenu = &mainMenu;
	timer = 0;
	exploring = false;
	health.visible = true;
	energy.visible = true;
	enemyCount.visible = true;
	timerDisplay.position.x = 110;
	timerDisplay.size ;
	timerDisplay.visible = true;

	//resets level
	delete cluster;
	cluster = new Cluster(this, 100, sprites["planet"]);
	
	//reset player
	delete player;
	player = new Player(this, sprites["player"]);


	//clears gui elements
	for (int i = 0; i < indicators.size(); i++)
	{
		delete indicators[i];
	}
	indicators.clear();
	for (int i = 0; i < entities.size(); i++)
	{
		delete entities[i];
	}
	entities.clear();


	//spawns enemies
	for (int i = 0; i < enemySpitters; i++)
	{
		float dist = Util::randFloat()*(cluster->radius - 100) + 100;
		float angle = Util::randFloat()*M_PI * 2;
		Vector position(1, 1);
		position.rotate(angle);
		position.normalize(dist);
		Enemy* e = new Enemy(this, position);
		e->makeSpitter();
		entities.push_back(e);
	}
	for (int i = 0; i < enemyChasers; i++)
	{
		float dist = Util::randFloat()*(cluster->radius - 100) + 100;
		float angle = Util::randFloat()*M_PI * 2;
		Vector position(1, 1);
		position.rotate(angle);
		position.normalize(dist);
		Enemy* e = new Enemy(this, position);
		e->makeChaser();
		entities.push_back(e);
	}
	for (int i = 0; i < enemyRollers; i++)
	{
		float dist = Util::randFloat()*(cluster->radius - 100) + 100;
		float angle = Util::randFloat()*M_PI * 2;
		Vector position(1, 1);
		position.rotate(angle);
		position.normalize(dist);
		Roller* r = new Roller(this, position);
		entities.push_back(r);
	}
}
void Game::start(ShaderProgram *program)
{
	renderBG(program);
	float texture[] = {
		0, 0,
		1, 1,
		0, 1,
		1, 1,
		0, 0,
		1, 0 };
	float vertices[] = {
		5, -5,
		-5, 5,
		5, 5,
		-5, 5,
		5, -5,
		-5, -5 };
	Matrix model;
	model.Translate(80, 50, 0);
	model.Rotate(0.15);
	model.Scale(3, 5, 1);
	program->setModelMatrix(model);
	program->setAlpha(1);
	glUseProgram(program->programID);


	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texture);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glBindTexture(GL_TEXTURE_2D, sprites["logo"].texture);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->texCoordAttribute);
	glDisableVertexAttribArray(program->positionAttribute);

	Util::drawText(program, sprites["font"].texture, "Lord Laneus", 80, 80, 10, 2);
	Util::drawText(program, sprites["font"].texture, "Presents:", 80, 20, 10, 2);

	float m = startupTimer / 1.5;
	int fade = m * 16;
	if (fade <= 15)
	{
		renderFade(program, 15 - fade);
	}



}

void Game::addMusic(string name, const char* path)
{
	music[name] = Mix_LoadMUS(path);
}
void Game::addSound(string name, const char* path)
{
	sounds[name] = Mix_LoadWAV(path);
}

void Game::addSprite(string name, const char* path, int i, int w, int h)
{
	sprites[name] = Sprite(Util::loadImage(path), i, w, h);
}
void Game::killAll()
{
	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->active = false;
	}
}
void Game::loadCluster(string path)
{
	ifstream file("resources\\" + path);
	string line;
	if (file.is_open())
	{
		getline(file, line);
		if (line == "[GWC]")
		{
			killAll();
			cluster = new Cluster(this, sprites["tutorial"]);
			while (getline(file, line))
			{
				if (line.substr(0, 2) == "P:")
				{
					cluster->planets.push_back(Planet(line.substr(3), cluster, cluster->sprite));
				}

				else if (line.substr(0, 2) == "S:")
				{
					Planet p(line.substr(3), cluster, cluster->sprite);
					p.type = Planet::star;
					cluster->planets.push_back(p);

				}
				else if (line.substr(0, 2) == "E:")
				{
					entities.push_back(new Enemy(line.substr(3), this));
				}
			}
		}
	}


}
void Game::menuSelect()//I hate this function, but I'm not sure else I should implement this
{
	Menu* oldMenu = currentMenu;
	if (currentMenu->title == "Main Menu")
	{
		switch (currentMenu->selected)
		{
		case 0:
			currentMenu = &playMenu;
			playMenu.show();
			currentMenu->visible = true;
			break;
		case 1:
			break;
		case 2:
			state = done;
			break;
		}
	}
	else if (currentMenu->title == "Play Menu")
	{
		switch (currentMenu->selected)
		{
		case 0:
			reset();
			exploring = false;
			state = play;
			msgBox.visible = false;
			lastFrameTime = (float)SDL_GetTicks() / 1000;
			break;
		case 1:
			reset();
			exploring = true;
			killAll();
			state = play;
			msgBox.visible = false;
			lastFrameTime = (float)SDL_GetTicks() / 1000;
			break;
		case 2:
			reset();
			exploring = true;
			loadCluster("tutorial.cluster");
			state = play;
			lastFrameTime = (float)SDL_GetTicks() / 1000;
			break;
		case 3:
			currentMenu = &mainMenu;
			currentMenu->visible = true;
			break;

		}
	}
	else if (currentMenu->title == "Paused")
	{
		switch (currentMenu->selected)
		{
		case 0:
			state = play;
			break;
		case 1:
			reset();
			break;
		}
	}
	else if (currentMenu->title == "Game Over")
	{
		switch (currentMenu->selected)
		{
		case 0:
			currentMenu->visible = false;
			reset();
			state = play;
			break;
		case 1:
			currentMenu->visible = false;
			reset();
		}
	}
	else if (currentMenu->title == "Victory!")
	{
		switch (currentMenu->selected)
		{
		case 0:
			currentMenu->visible = false;
			reset();
			break;
		case 1:
			exploring = true;
			currentMenu->visible = false;
			state = play;
		}
	}
	oldMenu->selected = 0;
	oldMenu->visible = false;
}
void Game::pause()
{
	currentMenu = &pauseMenu;
	pauseMenu.show();
	playSound("boop");
	state = menu;

}
void Game::playMusic(string name)
{
	Mix_PlayMusic(music[name], -1);
}
void Game::playSound(string name)
{
	Mix_PlayChannel(-1, sounds[name], 0);
}




void Game::showMsg(string msg, string subMsg1, string subMsg2, string subMsg3)
{
	msgBox.text = msg;
	msgBox.subText1 = subMsg1;
	msgBox.subText2 = subMsg2;
	msgBox.subText3 = subMsg3;
	msgBox.visible = true;
	state = message;
}
void Game::triggerFire(Vector position)
{
	fireEmitter.position = position;
	fireEmitter.velocity = Vector(0, 0);
	fireEmitter.gravity = position;
	fireEmitter.mass = .5;
	fireEmitter.trigger();
}
void Game::triggerParticles(ParticleEmitter& pe, Vector position, Planet p, float speed)
{
	pe.position = (position*3+p.position) / 4;
	pe.gravity = p.position;
	pe.mass = p.mass();

	pe.velocity = position - p.position;
	pe.velocity.normalize(speed);

	pe.trigger();
}
void Game::triggerDust(Vector position, Planet p, float speed)
{
	dustEmitter.position = position;
	dustEmitter.gravity = p.position;
	dustEmitter.mass = p.mass() / 2;

	dustEmitter.velocity = position - p.position;
	dustEmitter.velocity.normalize(speed);

	dustEmitter.trigger(&p);
}

void Game::update(float elapsed)
{
	cluster->update(elapsed);//meaningless; clusters don't change
	player->update(elapsed);
	dustEmitter.update(elapsed);
	fireEmitter.update(elapsed);
	int enemiesLeft = 0;
	if (exploring)
	{
		health.visible = false;
		energy.visible = false;
		enemyCount.visible = false;
		timerDisplay.visible = false;
	}
	health.current = player->health;
	energy.current = player->energy;
	Vector compassReal = player->position;
	compassReal.y += 40;
	Entity* closest = NULL;
	float minDistance;
	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->update(elapsed);
		if (!entities[i]->active)
		{
			entities.erase(entities.begin() + i);
			i--;
			continue;
		}
		if (entities[i]->type == Entity::enemy)
		{
			if (enemiesLeft == 0 || entities[i]->position.distance(compassReal) < minDistance)
			{
				closest = entities[i];
				minDistance = (entities[i]->position.distance(player->position));
			}
			enemiesLeft++;
		}
		if (entities[i]->active && entities[i]->collides(*player))
		{
			entities[i]->playerCollision(player);
		}

	}
	if (closest)
	{
		//compass.direction = closest->position - compassReal;
	}
	else
	{
		//compass.visible = false;
	}

	for (int i = 0; i < indicators.size(); i++)
	{
		indicators[i]->update(elapsed);
	}
	for (int i = 0; i < gui.size(); i++)
	{
		gui[i]->update(elapsed);
	}

	enemyCount.text = to_string(enemiesLeft);
	if (enemiesLeft == 0 && !exploring)
	{
		timerDisplay.size *= 2;
		timerDisplay.position.x = 80;
		timerDisplay.centered = true;
		exploring = true;
		currentMenu = &victoryMenu;
		victoryMenu.show();
		state = Game::menu;
	}
	if (!exploring)
	{
		timer += elapsed;
	}
	string text = to_string((int)(timer)) + '.';
	text += to_string(((int)(timer * 10)) % 10);
	timerDisplay.text = text;


}
void Game::renderFade(ShaderProgram *program, int frame)
{
	Sprite sprite = sprites["fade"];
	float u = (float)(((int)frame) % sprite.sheetW) / (float)sprite.sheetW;
	float v = (float)(((int)frame) / sprite.sheetW) / (float)sprite.sheetH;
	float spriteWidth = 1.0 / (float)sprite.sheetW;
	float spriteHeight = 1.0 / (float)sprite.sheetH;
	GLfloat texture[] = {
		u, v + spriteHeight,
		u + spriteWidth, v,
		u, v,

		u + spriteWidth, v,
		u, v + spriteHeight,
		u + spriteWidth, v + spriteHeight
	};
	float vertices[] = {
		0, 0,
		160, 100,
		0, 100,
		160, 100,
		0, 0,
		160, 0 };
	Matrix model;
	program->setModelMatrix(model);
	glUseProgram(program->programID);


	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texture);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glBindTexture(GL_TEXTURE_2D, sprite.texture);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->texCoordAttribute);
	glDisableVertexAttribArray(program->positionAttribute);

}
void Game::renderBG(ShaderProgram *program)
{
	sprites["bg"].render(program, 80, 50, 160, 100);


}
void Game::renderGui(ShaderProgram* program)
{
	glUseProgram(program->programID);
	if (state == menu)
	{
		GLuint font = sprites["font"].texture;
		currentMenu->render(program);
	}
	else if(state == title)
	{

		renderBG(program);
		GLuint font = sprites["font"].texture;
		Util::drawText(program, font, "Gravity Well", 80, 80, 10, 2);
		currentMenu->render(program);
	}
	if (state != done && state!=title)
	{
		for (int i = 0; i < gui.size(); i++)
		{
			if (gui[i]->visible)
			{
				gui[i]->render(program);
			}
		}
	}
}
void Game::render(ShaderProgram *program)
{
	glUseProgram(program->programID);
	if (state != done)
	{
		Matrix view;
		program->setViewMatrix(view);
		renderBG(program);

		view.Translate(-player->position.x + 80, -player->position.y + 50, 0);
		program->setViewMatrix(view);

		cluster->render(program);
		player->render(program);
		dustEmitter.render(program);
		fireEmitter.render(program);

		for (int i = 0; i < entities.size(); i++)
		{
			if (entities[i]->active)
			{
				entities[i]->render(program);
			}
			else
			{
				delete entities[i];
				entities.erase(entities.begin() + i);
				i--;
			}
			for (int i = 0; i < indicators.size(); i++)
			{
				if (!indicators[i]->active)
				{
					delete indicators[i];
					indicators.erase(indicators.begin() + i);
					i--;
				}
				else if (indicators[i]->visible)
				{
					indicators[i]->render(program);
				}
			}
		}
	}
}