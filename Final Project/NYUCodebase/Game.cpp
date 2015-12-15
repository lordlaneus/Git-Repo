#include "Game.h"
#include "Util.h"
#include "Bar.h"
#include "DustEmitter.h"
#include "Enemy.h"
using namespace std;
Game::Game()
{
	addSprite("logo", "resources\\logo.png", 0, 1, 1);
	addSprite("fade", "resources\\fade.png", 0, 4, 4);
	addSprite("cursor", "resources\\cursor.png", 0, 1, 1);
	addSprite("bg", "resources\\background.png", 0, 1, 1);
	addSprite("planet", "resources\\Planet Sheet.png", 0, 4, 2);
	addSprite("enemy", "resources\\Enemy Sheet.png", 0, 4, 1);
	addSprite("player", "resources\\Player Sheet.png", 1, 5, 2);
	addSprite("particles", "resources\\Particles.png", 0, 1, 1);
	addSprite("hook", "resources\\Hook Sheet.png", 0, 2, 1);
	addSprite("gui", "resources\\gui.png", 0, 1, 2);
	addSprite("compass", "resources\\compass.png", 0, 2, 1);
	addSprite("msg", "resources\\messageBox.png", 0, 1, 1);
	addSprite("font", "resources\\font.png", 16, 16, 16);
	addSprite("attack", "resources\\attack.png", 0, 8, 1);

	addSound("startup", "resources\\startup.wav");
	addSound("boop", "resources\\boop.wav");
	addSound("hurt", "resources\\hurt.wav");
	addSound("land", "resources\\land.wav");
	addSound("death", "resources\\death.wav");
	addSound("enemyHurt", "resources\\enemy hurt.wav");
	addSound("fire", "resources\\fire.wav");
	addSound("explode", "resources\\explode.wav");
	addMusic("bgm", "resources\\bgm.wav");

	mainMenu.title = "Main Menu";
	mainMenu.position = Vector(80, 60);
	mainMenu.size = Vector(80, 80);
	mainMenu.margin = 30;
	mainMenu.fontSize = 6;
	mainMenu.fontSpacing = 1;
	mainMenu.font = sprites["font"];
	mainMenu.lineSpacing = 10;
	gameOverMenu = mainMenu;
	mainMenu.showTitle = false;
	mainMenu.addOption("Play");
	mainMenu.addOption("Roam");
	mainMenu.addOption("Quit");


	gameOverMenu.position = Vector(80, 80);
	gameOverMenu.size = Vector(80, 70);
	gameOverMenu.visible = false;
	gameOverMenu.bg = &sprites["msg"];
	gameOverMenu.title = "Game Over";
	gameOverMenu.margin = 25;
	victoryMenu = gameOverMenu;
	pauseMenu = gameOverMenu;
	gameOverMenu.addOption("Retry");
	gameOverMenu.addOption("Quit");
	gui.push_back(&gameOverMenu);

	victoryMenu.title = "Victory!";
	victoryMenu.margin = 15;
	victoryMenu.addOption("Restart");
	victoryMenu.addOption("Expolore");
	gui.push_back(&victoryMenu);

	pauseMenu.title = "Paused";
	pauseMenu.addOption("Resume");
	pauseMenu.addOption("Quit");
	gui.push_back(&pauseMenu);

	health = Bar(sprites["gui"], 100, 25, 2);
	health.position = Vector(3, 95);
	gui.push_back(&health);



	msgBox = MsgBox(sprites["msg"], sprites["font"]);
	gui.push_back(&msgBox);

	enemyCount = TextDisplay(sprites["font"], "Enemeis Left:", 3, 1, 120, 95);
	gui.push_back(&enemyCount);

	compass.sprite = sprites["compass"];
	compass.position = Vector(80, 90);
	compass.size = Vector(15, 15);
	gui.push_back(&compass);

	cursor.sprite = sprites["cursor"];
	cursor.size = Vector(4, 4);
	gui.push_back(&cursor);

	reset();
	state = ready;

	fireEmitter.sprite.index = 7;
	fireEmitter.maxLifetime = 1;
	fireEmitter.velocityDev.x = 10;
	fireEmitter.velocityDev.y = 10;
	fireEmitter.startSize = 5;
	fireEmitter.endSize = 0;
	fireEmitter.sprite = sprites["particles"];
}
void Game::reset()
{

	state = title;
	exploring = false;
	gameOverMenu.visible = false;
	health.visible = true;
	enemyCount.visible = true;
	cluster = Cluster(100, sprites["planet"]);
	delete player;
	player = new Player(this, sprites["player"]);
	health.e = player;

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

	for (int i = 0; i < totalEnemies; i++)
	{
		float dist = Util::randFloat()*(cluster.radius - 100) + 100;
		float angle = Util::randFloat()*M_PI * 2;
		Vector position(1, 1);
		position.rotate(angle);
		position.normalize(dist);
		entities.push_back(new Enemy(this, position));
	}
}
void Game::update(float elapsed)
{
	cluster.update(elapsed);
	player->update(elapsed);
	dustEmitter.update(elapsed);
	fireEmitter.update(elapsed);
	int enemiesLeft = 0;
	if (exploring)
	{
		health.visible = false;
		enemyCount.visible = false;
	}
	Vector compassReal = player->position;
	compassReal.y += 40;
	Entity* closest = NULL;
	float minDistance;
	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->update(elapsed);
		if (entities[i]->type == Entity::enemy)
		{
			if (enemiesLeft == 0 || entities[i]->position.distance(compassReal) < minDistance)
			{
				closest = entities[i];
				minDistance = (entities[i]->position.distance(player->position));
			}
			enemiesLeft++;
		}
		if (entities[i]->active && player->collides(*entities[i]))
		{
			entities[i]->playerCollision(player);
		}
		if (!entities[i]->active)
		{
			entities.erase(entities.begin() + i);
			i--;
		}
	}
	if (closest)
	{
		compass.direction = closest->position - compassReal;
	}
	else
	{
		compass.visible = false;
	}

	for (int i = 0; i < indicators.size(); i++)
	{
		indicators[i]->update(elapsed);
	}
	for (int i = 0; i < gui.size(); i++)
	{
		gui[i]->update(elapsed);
	}

	enemyCount.text = "Enemeis Left: " + to_string(enemiesLeft);
	if (enemiesLeft == 0 && !exploring)
	{
		currentMenu = &victoryMenu;
		victoryMenu.show();
		state = Game::victory;
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
void Game::render(ShaderProgram *program)
{
	if (state == title)
	{
		renderBG(program);
		GLuint font = sprites["font"].texture;
		Util::drawText(program, font, "Gravity Well", 80, 80, 10, 2);
		mainMenu.render(program);
		/*Util::drawText(program, font, "  Play", 80, 55, 6, 2);
		Util::drawText(program, font, "  Help", 80, 40, 6, 2);
		Util::drawText(program, font, "  Quit", 80, 25, 6, 2);*/
	}
	else if (state != done)
	{
		Matrix view;
		program->setViewMatrix(view);
		renderBG(program);

		view.Translate(-player->position.x + 80, -player->position.y + 50, 0);
		program->setViewMatrix(view);

		cluster.render(program);
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
		//gui
		Matrix viewMatrix;
		program->setViewMatrix(viewMatrix);
		for (int i = 0; i < gui.size(); i++)
		{
			if (gui[i]->visible)
			{
				gui[i]->render(program);
			}
		}
	}
}
void Game::pause()
{
	currentMenu = &pauseMenu;
	pauseMenu.show();
	playSound("boop");
	state = paused;

}
void Game::killAll()
{
	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->active = false;
	}
}
void Game::menuSelect()//I hate this function, but I'm not sure else I should implement this
{
	if (currentMenu->title == "Main Menu")
	{
		switch (currentMenu->selected)
		{
		case 1:
			killAll();
			exploring = true;
		case 0:
			state = play;
			msgBox.visible = false;
			lastFrameTime = (float)SDL_GetTicks() / 1000;
			break;
		case 2:
			state = done;
			break;
		}
	}
	else if (currentMenu->title == "Paused")
	{
		switch (currentMenu->selected)
		{
		case 0:
			state = play;
			currentMenu->visible = false;
			break;
		case 1:
			state = title;
			currentMenu->visible = false;
			currentMenu = &mainMenu;
			break;
		}
	}
	else if (currentMenu->title == "GAME OVER")
	{
		switch (currentMenu->selected)
		{
		case 0:
			state = title;
			currentMenu->visible = false;
			currentMenu = &mainMenu;
			break;
		case 1:
			state = done;
			break;
		}
	}
	else if (currentMenu->title == "Victory!")
	{
		switch (currentMenu->selected)
		{
		case 0:
			state = title;
			currentMenu->visible = false;
			currentMenu = &mainMenu;
			break;
		case 1:
			exploring = true;
			currentMenu->visible = false;
			state = play;
		}
	}
}
void Game::addSprite(string name, const char* path, int i, int w, int h)
{
	sprites[name] = Sprite(Util::loadImage(path), i, w, h);
}
void Game::addSound(string name, const char* path)
{
	sounds[name] = Mix_LoadWAV(path);
}

void Game::addMusic(string name, const char* path)
{
	music[name] = Mix_LoadMUS(path);
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
void Game::triggerParticles(ParticleEmitter& pe, Vector position)
{
	pe.position = position;
	pe.gravity = position;
	pe.mass = 0;
	pe.trigger();
}
void Game::triggerParticles(ParticleEmitter& pe, Vector position, Planet p, float speed)
{
	pe.position = position;
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
	dustEmitter.mass = p.mass();

	dustEmitter.velocity = position - p.position;
	dustEmitter.velocity.normalize(speed);

	dustEmitter.trigger(p);
}

void Game::playSound(string name)
{
	Mix_PlayChannel(-1, sounds[name], 0);
}
void Game::playMusic(string name)
{
	Mix_PlayMusic(music[name], -1);
}