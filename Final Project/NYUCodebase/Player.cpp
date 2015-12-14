#include "Player.h"
#include "Enemy.h"
using namespace std;

Player::Player():Entity()
{
	

}

Player::Player(Game* g, Sprite sprite) :
Entity(g)
{
	onGround = false;
	hook = new Hook(this);
	sword = new Weapon(g->sprites["attack"],this);
	hook->sprite = g->sprites["hook"];
	this->sprite = sprite;
	position.y = 15;
	size = Vector(4, 7);
	type = player;

}
bool Player::collidesP(Planet p)
{
	if (p.on)
	{
		return false;
	}
	return position.distance(p.position) < p.size / 2 + size.y / 4;
}


void Player::attack(Vector dir)
{
	sword->swing(dir);
}
void Player::die()
{
	g->showMsg("Game Over","\"r\" to reset.", "\"q\" to quit.");
	g->state = Game::gameOver;
}

void Player::jump()
{
	if (onGround)
	{
		onGround = false;
		Vector jumpVector = position - planet->position;
		jumpVector.normalize(jumpPower);
		velocity = velocity + jumpVector;
	}
	else if (hook->state==Hook::reeling)
	{
		hook->state = Hook::recoiling;
	}
} 
void Player::land(Planet &p)
{
	if (p.type == Planet::moon)
	{
		g->playSound("land");
		hook->state = Hook::away;
		onGround = true;

		planet = &p;
		walk();
		accel.clear();
		velocity.clear();
	}
	else if (p.type == Planet::star)
	{
		g->triggerParticles(g->fireEmitter,position, p,10);
		planet = &p;
		hook->state = Hook::away;
		onGround = true;
		accel.clear();
		velocity.clear();
		
		takeDamage(20); 
		jump();
	}
}
void Player::takeDamage(float dmg)
{
	if (hurt > 0)
	{
		return;
	}
	health -= dmg;
	g->health.current = health;
	if (health<= 0)
	{
		die();
		g->playSound("death");
		return;
	}
	g->playSound("hurt");
	hurt = 1;
	
}
void Player::walk()
{
	if (onGround)
	{
		switch (wasd.top())
		{
		case SDLK_w:
			walking = (position.x < planet->position.x) ? -1 : 1;
			break;
		case SDLK_s:
			walking = (position.x < planet->position.x) ? 1 : -1;
			break;
		case SDLK_a:
			walking = (position.y < planet->position.y) ? -1 : 1;
			break;
		case SDLK_d:
			walking = (position.y < planet->position.y) ? 1 : -1;
			break;
		default:
			walking = 0;
			break;
		}
	}
	else
	{
		walking = 0;
	}
}
void Player::pop(Planet p)
{
	
	Vector v = position - p.position;
	float l = v.length();
	v.normalize();
	v = v*(p.size / 2 + size.y / 4);
	position = p.position+ v;
	if (!p.on)
	{
		v.normalize(size.y/ 2);
		g->dustEmitter.sprite.index = p.sprite.index;
		g->triggerParticles(g->dustEmitter,position-v, p);
	}

}

void Player::render(ShaderProgram* program)
{
	Entity::render(program);
	hook->render(program);
	if (sword->active)
	{
		sword->render(program);
	}
	
}
void Player::update(float elapsed)
{
	if (sword->active)
	{
		for (int i = 0; i < g->entities.size(); i++)
		{
			Entity*e = g->entities[i];
			if (e->collides(*sword))
			{
				if (e->type == Entity::enemy)
				{
					dynamic_cast<Enemy*>(e)->takeDamage(1, e->position - position);
				}
				else if (e->type == Entity::projectile)
				{
					dynamic_cast<Projectile*>(e)->active = false;
				}
			}
		}
	}
	if (hurt > 0)
	{
	hurt -= elapsed;
	}
	Vector totalPull;
	float maxPull = 0;
	Planet majorPuller;
	for (int i = 0; i < g->cluster.planets.size(); i++)
	{

		Planet p = g->cluster.planets[i];

		Vector pull = p.position - position;
		pull.normalize();
		pull = pull / pow(position.distance(p.position), 1.5);
		pull = pull*p.size*p.density*mass;
		totalPull = totalPull + pull;
		if (pull.length() > maxPull)
		{
			majorPuller = p;
			maxPull = pull.length();
		}
	}

	if (onGround)
	{
		velocity.clear();
		pop(*planet);
		planet->on = true;
		Vector newPos = position - planet->position;
		newPos.rotate(walkSpeed*walking*elapsed/planet->size);
		position = planet->position + newPos;
		rotation = (planet->position - position).angle() + M_PI / 2;
	}
	else
	{
		accel = totalPull;
		rotation = (majorPuller.position - position).angle() + M_PI / 2;
		Entity::update(elapsed);
		for (int i = 0; i < g->cluster.planets.size(); i++)
		{
			if (collidesP(g->cluster.planets[i]))
			{
				
				land(g->cluster.planets[i]);
			}
			else
			{
				g->cluster.planets[i].on = false;
			}
		}
	}
	hook->update(elapsed);
	sword->update(elapsed);
	changeSprite();
}
void Player::changeSprite()
{
	if (walking < 0)
	{
		sprite.flipped = false;
	}
	else if (walking > 0)
	{
		sprite.flipped = true;
	}
	if (hurt > 0)
	{
		if (int(hurt*10)%2== 0)
		{
			sprite.index = 2;
		}
		else
		{
			sprite.index = 3;
		}
	}
	else if (!onGround)
	{
		sprite.index = 1;
	}
	else
	{
		sprite.index = 0;
	}
}