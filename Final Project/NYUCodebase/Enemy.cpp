#include "Enemy.h"

using namespace std;

Enemy::Enemy(Game* g, Vector position) :
Entity(g)
{
	size = Vector(5, 5);
	baseSize = size;
	this->position = position;
	type = enemy;
	home = position;
	Sprite s = g->sprites["dust"];
	float coolDown = fireRate;
	s.index = 1;
	sprite = g->sprites["enemy"];
	
}
void Enemy::die()
{
	this->active = false;
	g->triggerParticles(g->fireEmitter, position);
	g->playSound("explode");
}
void Enemy::fire()
{
	Projectile* p = new Projectile(g, position);
	p->velocity = (g->player->position - position).normalize(50);
	g->entities.push_back(p);
	g->playSound("fire");
	coolDown = fireRate;
}
void Enemy::playerCollision(Player* p)
{
	if (hurt <= 0)
	{
		p->takeDamage(5);
	}
}
void Enemy::takeDamage(float dmg, Vector v)
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
		velocity = velocity + v.normalize(1);
	}
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

		if (coolDown < 0)
		{
			fire();
		}
		if (position.distance(g->player->position) > detectionRange)
		{
			state = returning;
			coolDown = fireRate;
		}
		if (position.distance(g->player->position) < circleRange)
		{
			velocity = (g->player->position - position).normalize(speed).rotate(M_PI / 2);
		}
	else
		{
			velocity = (g->player->position - position).normalize(speed);
		}
		
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
void Enemy::render(ShaderProgram* program)
{
	if (hurt > 0)
	{
		if (state==aggro)
		{
			sprite.index = 3;
		}
		else
		{
			sprite.index = 2; 
		}
	}
	else
	{
		if (state == aggro)
		{
			sprite.index = 1;
		}
		else
		{
			sprite.index = 0;
		}
	}
	Entity::render(program);
}