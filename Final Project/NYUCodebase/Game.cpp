#include "Game.h"
#include "Util.h"
#include "Bar.h"
#include "Enemy.h"
using namespace std;
Game::Game()
{
	addSprite("bg", "resources\\background.png",0,1,1);
	addSprite("planet", "resources\\Planet Sheet.png", 0, 4, 2);
	addSprite("enemy", "resources\\Enemy Sheet.png", 0, 4, 1);
	addSprite("player", "resources\\Player Sheet.png", 1, 4, 2);
	addSprite("dust", "resources\\Particles.png", 0, 4, 2);
	addSprite("hook", "resources\\Hook Sheet.png", 0, 2, 1);
	addSprite("gui", "resources\\gui.png", 0, 1, 2);
	addSprite("msg", "resources\\messageBox.png", 0, 1, 1);
	addSprite("font", "resources\\font.png", 0, 1, 1);
	addSprite("attack", "resources\\attack.png", 0, 8, 1);

	addSound("hurt", "resources\\hurt.wav");
	addSound("land", "resources\\land.wav");
	addSound("death", "resources\\death.wav");
	addSound("enemyHurt", "resources\\enemy hurt.wav");
	addSound("fire", "resources\\fire.wav");
	addSound("explode", "resources\\explode.wav");
	addSound("bgm", "resources\\bgm.wav");
	playSound("bgm");

	dustEmitter.maxLifetime = .6;
	dustEmitter.velocityDev.x = 10;
	dustEmitter.velocityDev.y = 10;
	dustEmitter.startSize = 2;
	dustEmitter.endSize = 0;
	dustEmitter.sprite = sprites["dust"];

	fireEmitter = dustEmitter;
	fireEmitter.sprite.index = 7;
	fireEmitter.startSize = 5;

	health = Bar(sprites["gui"], 100, 25, 2);
	health.position = Vector(3, 95);
	gui.push_back(&health);

	msgBox = MsgBox(sprites["msg"], sprites["font"]);
	gui.push_back(&msgBox);

	enemyCount = TextDisplay(sprites["font"], "Enemeis Left:", 3, 1,120, 95);
	gui.push_back(&enemyCount);
	reset();
	
}
void Game::reset()
{

	state = title;
	player = new Player(this, sprites["player"]);
	cluster = Cluster(100, sprites["planet"]);

	for (int i = 0; i < entities.size(); i++)
	{
		delete entities[i];
	}
	entities.clear();
	for (int i = 0; i < totalEnemies;i++)
	{
		entities.push_back(new Enemy(this, Vector(Util::randFloat() * 400 - 200, Util::randFloat() * 400 - 200)));
	}
}
void Game::update(float elapsed)
{
	cluster.update(elapsed);
	player->update(elapsed);
	dustEmitter.update(elapsed);
	fireEmitter.update(elapsed);
	int enemiesLeft = 0;
	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->update(elapsed);
		if (entities[i]->type == Entity::enemy)
		{
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

	enemyCount.text = "Enemeis Left: " + to_string(enemiesLeft);
	if (enemiesLeft==0)
	{
		showMsg("Victory!", "\"r\" to reset.", "\"q\" to quit.");
		state = Game::gameOver;
	}


}
void Game::renderBG(ShaderProgram *program)
{
	float texture[] = { 
		0, 0,
		1, 1,
		0, 1,
		1, 1,
		0, 0,
		1, 0};
	float vertices[] = {
		0, 0,
		160, 100,
		0, 100, 
		160, 100,
		0, 0,
		160, 0};
	Matrix model;
	program->setModelMatrix(model);
	glUseProgram(program->programID);


	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texture);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glBindTexture(GL_TEXTURE_2D, sprites["bg"].texture);

	glDrawArrays(GL_TRIANGLES, 0, 6);


	glDisableVertexAttribArray(program->positionAttribute);

}
void Game::render(ShaderProgram *program)
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
	}
	//gui
	Matrix viewMatrix;
	program->setViewMatrix(viewMatrix);
	for (int i = 0; i < gui.size(); i++)
	{
		if (gui[i]->show)
		{
			gui[i]->render(program);
		}
	}
}
void Game::pause()
{
	showMsg("Paused","\"esc\" to continue.", "\"r\" to reset.", "\"q\" to quit.");
	state = paused;

}

void Game::addSound(string name, const char* path)
{
	sounds[name] = Mix_LoadWAV(path);
}
void Game::addSprite(string name, const char* path, int i, int w, int h)
{
	sprites[name] = Sprite(Util::loadImage(path), i, w, h);
}
void Game::showMsg(string msg, string subMsg1, string subMsg2, string subMsg3)
{
	msgBox.text = msg;
	msgBox.subText1 = subMsg1;
	msgBox.subText2 = subMsg2;
	msgBox.subText3 = subMsg3;
	msgBox.show = true;
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

void Game::playSound(string name)
{
	Mix_PlayChannel(-1, sounds[name], 0);
}