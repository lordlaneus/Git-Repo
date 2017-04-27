#include "Enemy.h"

#include "sstream"
#include "Bar.h"
#include "Tile.h"
#include "Util.h"
using namespace std;

Enemy::Enemy()
{

}
Enemy::Enemy(string line, Game* g)
{
	this->g = g;
	type = enemy;
	maxPuff = 1;
	size = Vector(5, 5);
	baseSize = size;
	maxHealth = 1;
	bar = new Bar;
	sprite = g->sprites["enemy"];
	istringstream iss(line);
	iss>> position.x;
	iss >> position.y;
	iss >> speed;
	iss >> fireRate;

}
Enemy::Enemy(Game* g, Vector position) :
Entity(g)
{
	

	this->position = position;
	baseSize = Vector(baseWidth, baseHeight);
	type = enemy;
	home = position;
	float coolDown = fireRate + Util::randFloat();
	sprite = g->sprites["enemy"];
	bar = new Bar(g->sprites["bar"], maxHealth, 4, 1);
	bar->e = this;
	bar->fixed = false;
	bar->visible = false;
	
	g->indicators.push_back(bar);

}
void Enemy::makeChaser()
{
	size = size*0.66;
	circleRange = 0;
	fireRate = -1;
	damage = 20;
	maxHealth = 10;
	health = maxHealth;
	speed = 40;
	maxPuff = 1;
	sprite.index = 4;
}
void Enemy::makeSpitter()
{
	damage = spitterDamage;
	fireRate = spitterfireRate;
	maxHealth = spitterHealth;
	health = maxHealth;
	bar->max = maxHealth;
	maxPuff = spitterMaxPuff;
	projectileSpeed = spitterProjectileSpeed;
	speed = spitterSpeed;
	float coolDown = fireRate + Util::randFloat();
}
void Enemy::die()
{
	this->active = false;
	g->triggerFire(position);
	g->playSound("explode");
}
void Enemy::fire()
{
	Projectile* p = new Projectile(g, position, damage);
	p->velocity = (g->player->position - position).normalize(projectileSpeed);
	g->entities.push_back(p);
	g->playSound("fire");
	if (fireRate > 0)
	{
	coolDown = fireRate + Util::randFloat();
	}
}
void Enemy::playerCollision(Player* p)
{
	if (hurt <= 0)
	{
		p->takeDamage(5);
	}
}
void Enemy::takeDamage(float dmg)
{
	if (hurt <= 0)
	{
		g->playSound("enemyHurt");
		hurt = 1;
		health -= dmg;
		if (health <= 0)
		{
			die();
		}
	}
	bar->show(2.5);
}
void Enemy::update(float elapsed)
{
	Entity::update(elapsed);
	if (state == aggro)
	{
		coolDown -= elapsed;
		float m = coolDown / fireRate;
		float puff = m*(1 - maxPuff) / puffStart + maxPuff;
		if (puff < 1)
		{
			puff = 1;
		}
		size = baseSize*puff;

		if (coolDown < 0 && fireRate>0)
		{
			fire();
		}
		if (position.distance(g->player->position) > detectionRange)
		{
			state = returning;
			coolDown = fireRate;
		}
		if (position.distance(g->player->position) <= circleRange)
		{
			state = circle;
		}
		velocity = (g->player->position - position).normalize(speed);
			
		
	}
	else if (state == circle)
	{
		coolDown -= elapsed;
		float m = coolDown / fireRate;
		float puff = m*(1 - maxPuff) / puffStart + maxPuff;
		if (puff < 1)
		{
			puff = 1;
		}
		size = baseSize*puff;

		if (coolDown < 0 && fireRate>0)
		{
			fire();
		}
		if (position.distance(g->player->position) > detectionRange)
		{
			state = returning;
			coolDown = fireRate;
		}
		if (position.distance(g->player->position) > circleRange)
		{
			state = aggro;
		}
		velocity = (g->player->position - position).normalize(speed*circleDir).rotate(M_PI / 2);
	}
	else if (state == alert)
	{
		if (position.distance(g->player->position) < detectionRange)
		{
			state = aggro;
		}
		size = baseSize;
		velocity.clear();

		
	}
	else if (state == returning)
	{
		size = baseSize;
		velocity = (home - position).normalize(speed);
		if (position.distance(g->player->position) < detectionRange)
		{
			state = aggro;
		}
		else if (home.distance(position) < 5)
		{
			state = alert;
		}

	}
	if (hurt > 0)
	{
		velocity = (position - g->player->position).normalize(speed);
	}
}
void Enemy::render(ShaderProgram* program, int offset)
{
	if (hurt > 0)
	{
		if (state==aggro || state == circle)
		{

			Entity::render(program, -6);
		}
		else
		{
			Entity::render(program, -7);
		}
	}
	else
	{
		if (state == aggro || state == circle)
		{
			Entity::render(program,1);
		}
		else
		{
			Entity::render(program);
		}
	}
	
}