#include "Player.h"

using namespace std;

Player::Player()
{
	Entity();

}

Player::Player(Game* g, Sprite sprite)
{
	Entity();
	onGround = false;
	hook = new Hook(this);
	hook->sprite = g->sprites["hook"];
	this->g = g;
	this->sprite = sprite;
	position.x = 80;
	position.y = 50;
	width = 5;
	height = 5;

}
bool Player::collides(Planet p)
{
	if (p.on)
	{
		return false;
	}
	return position.distance(p.position) < p.size / 2 + height / 4;
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
} 
void Player::land(Planet &p)
{
	if (p.type == Planet::moon)
	{
		hook->state = Hook::away;
		onGround = true;
		walk();
		accel.clear();
		velocity.clear();
		planet = &p;
	}
	else if (p.type == Planet::star)
	{

		hook->state = Hook::away;
		onGround = true;
		accel.clear();
		velocity.clear();
		planet = &p;
		takeDamage(10); 
		jump();
	}
}
void Player::takeDamage(float dmg)
{
	health -= dmg;
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
	v = v*(p.size / 2 + height / 4);
	position = p.position+ v;
	if (!p.on)
	{
		v.normalize(height / 2);
		g->emitDust(position-v, p);
	}

}

void Player::render(ShaderProgram* program)
{
	Entity::render(program);
	hook->render(program);
}
void Player::update(float elapsed)
{
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
		newPos.rotate(walkSpeed*walking*elapsed);
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
			if (collides(g->cluster.planets[i]))
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