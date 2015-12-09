#include "Game.h"
#include "Util.h"
using namespace std;
Game::Game()
{

	
	addSprite("planet", "Planet Sheet.png", 0, 2, 2);
	addSprite("player", "Player Sheet.png", 1, 4, 4);
	addSprite("dust", "Dust Sheet.png", 0, 1, 1);
	addSprite("hook", "Hook Sheet.png", 0, 1, 2);
	addSprite("font", "font.png", 0, 1, 1);

	player = new Player(this, sprites["player"]);
	cluster = Cluster(500, sprites["planet"]);

	dustEmitter.pCount = 20;
	dustEmitter.maxLifetime = .6;
	dustEmitter.velocityDev.x = 10;
	dustEmitter.velocityDev.y = 10;
	dustEmitter.startSize = 2;
	dustEmitter.endSize = 0;
	dustEmitter.sprite = sprites["dust"];
	int i = dustEmitter.sprite.index;
}
void Game::update(float elapsed)
{
	cluster.update(elapsed);
	player->update(elapsed);
	dustEmitter.update(elapsed);


}
void Game::render(ShaderProgram *program)
{

	cluster.render(program);
	player->render(program);
	dustEmitter.render(program);
	Util::drawText(program, sprites["font"].texture, to_string(player->health), 5, 95, 5, 2);
}

void Game::addSprite(string name, const char* path, int i, int w, int h)
{
	sprites[name] = Sprite(Util::loadImage(path), i, w, h);
}

void Game::emitDust(Vector position, Planet p)
{
	dustEmitter.position = position;
	dustEmitter.gravity = p.position;
	dustEmitter.mass = p.mass();

	dustEmitter.velocity = position - p.position;
	dustEmitter.velocity.normalize(5);

	dustEmitter.trigger();
}
