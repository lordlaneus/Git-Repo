#include "Enemy.h"

#include "Bar.h"
#include "Planet.h"
#include "Util.h"
using namespace std;

Enemy::Enemy()
{

}
Enemy::Enemy(Game* g, Vector position) :
Entity(g)
{
	size = Vector(5, 5);
	baseSize = size;
	maxHealth = startHealth;
	health = maxHealth;
	this->position = position;
	type = enemy;
	home = position;
	float coolDown = fireRate + Util::randFloat();
	sprite = g->sprites["enemy"];
	bar = new Bar(g->sprites["gui"], maxHealth, 4, 1);
	bar->e = this;
	bar->fixed = false;
	bar->visible = false;
	
	g->indicators.push_back(bar);

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
	p->velocity = (g->player->position - position).normalize(projectileSpeed);
	g->entities.push_back(p);
	g->playSound("fire");
	coolDown = fireRate + Util::randFloat();
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

		if (coolDown < 0)
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

		if (coolDown < 0)
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
	//avoid planets
	Planet* p = g->cluster->checkCollision(position);
	if (p&&p->type!=Planet::star)
	{
		if (state != circle)
		{
			position = position + (position - p->position).normalize(p->size / 2 - (position.distance(p->position)));
			if (g->cluster->checkCollision(position + velocity))
			{
				if (velocity.dot((position - p->position).rotate(M_PI/2)) > 0)
				{
					velocity = (position - p->position).rotate(M_PI / 2).normalize(speed*landBonus);
				}
				else
				{
					velocity = (position - p->position).rotate(-M_PI / 2).normalize(speed*landBonus);
				}
			}
		}
		else
		{
			if (g->cluster->checkCollision(position + velocity))
			{
				position = position + (position - p->position).normalize(p->size / 2 - (position.distance(p->position)));
				circleDir *= -1;
			}
		}
	}
}
void Enemy::render(ShaderProgram* program)
{
	if (hurt > 0)
	{
		if (state==aggro || state == circle)
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
		if (state == aggro || state == circle)
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