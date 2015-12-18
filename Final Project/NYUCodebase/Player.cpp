#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"
using namespace std;

Player::Player()
{
	

}

Player::Player(Game* g, Sprite sprite) 
{
	this->g = g;
	planet = &g->cluster->planets[0];
	maxHealth = 100;
	health = maxHealth;
	hook = new Hook(this);
	sword = new Weapon(g->sprites["attack"],this);
	hook->sprite = g->sprites["particles"];
	this->sprite = sprite;
	position.x = 0;
	position.y = 15;
	size = Vector(4, 7);

	death.loop = false;
	death.addFrame(2, 0.05);
	death.addFrame(4, 0.05);
	death.addFrame(5, 0.05);
	death.addFrame(6, 0.05);
	death.addFrame(7, 0.05);

	walkAnim.duration = 0.5;
	walkAnim.addFrame(8, 1);
	walkAnim.addFrame(9, 1);
	walkAnim.addFrame(10, 1);
	walkAnim.addFrame(11, 1);

}
bool Player::collidesP(Planet& p)
{
	if (p.on || shifted)
	{
		return false;
	}
	return position.distance(p.position) < p.size / 2 + size.y / 4;
}
void Player::attack(Vector dir)
{
	if (!sword->active)
	{
		sword->swing(dir,velocity);
	}
}
void Player::die()
{
	if (!dying)
	{
		g->playSound("death");
		hurt = deathTime;
	}
	dying = true;
}

void Player::jump()
{
	if (state==onGround)
	{
		state = inAir;
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
		state = onGround;

		planet = &p;
		walk();
		accel.clear();
		velocity.clear();
	}
	else if (p.type == Planet::star)
	{
		g->triggerParticles(g->fireEmitter,position, p,20);
		planet = &p;
		hook->state = Hook::away;
		state = onGround;
		accel.clear();
		velocity.clear();
		jump();
		takeDamage(p.damage);
		
	}
}
void Player::shift()
{
	shifted = true;
	shiftDown = true;
	state = inAir;
	shiftingTime = 0.25;
}
void Player::takeDamage(float dmg)
{
	if (hurt > 0 || shifted)
	{
		return;
	}
	if (!g->exploring)
	{
		health -= dmg;
		if (health <= 0)
		{
			die();
			return;
		}
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
void Player::pop(Planet& p)
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
		g->triggerDust(position-v, p,10);
	}

}


void Player::update(float elapsed)
{

	if (sword->active)
	{
		for (int i = 0; i < g->entities.size(); i++)
		{
			Entity*e = g->entities[i];
			if (sword->collides(*e))
			{
				if (e->type == Entity::enemy)
				{
					dynamic_cast<Enemy*>(e)->takeDamage(sword->damage);
				}
				else if (e->type == Entity::projectile)
				{
					dynamic_cast<Projectile*>(e)->active = false;
				}
			}
		}
	}
	bool splinch = false;
	if (shifted )
	{
		energy -= shiftCost * elapsed;
		if (energy <= 0 && !g->exploring)
		{
			shifted = false;
			splinch = true;
		}
		else if (!shiftDown)
		{
			shifted = false;
			for (int i = 0; i < g->cluster->planets.size(); i++)
			{
				if (collidesP(g->cluster->planets[i]))
				{
					if (g->cluster->planets[i].position.distance(position) < g->cluster->planets[i].size / 2 - 5)
					{
						shifted = true;
						break;
					}
					
				}
			}
		}
		
	}
	if (shiftingTime > 0)
	{
		shiftingTime -= elapsed;
	}
	else if(energy < maxEnergy)
	{
		
		energy += energyRegen* elapsed;
	}
	if (hurt > 0)
	{
	hurt -= elapsed;
	}
	Vector totalPull;
	float maxPull = 0;
	Planet majorPuller;
	for (int i = 0; i < g->cluster->planets.size(); i++)
	{

		Planet p = g->cluster->planets[i];

		Vector pull = p.position - position;
		pull.normalize();
		float distance = position.distance(p.position);
		//because gravity is weaker inside of a planet;
		if (distance < p.size/2)
		{
			distance = p.size - distance;
		}
		pull = pull / pow(distance, 1.5);
		pull = pull*p.size*p.density*mass;
		totalPull = totalPull + pull;
		if (pull.length() > maxPull)
		{
			majorPuller = p;
			maxPull = pull.length();
		}
	}
	if (dying)
	{
		velocity = velocity + accel* elapsed;
		position = position + velocity * elapsed;
		if (hurt <= 0)
		{
			g->currentMenu = &g->gameOverMenu;
			g->gameOverMenu.show();
			g->state = Game::gameOver;

		}
	}
	else if (state==onGround)
	{
		velocity.clear();
		pop(*planet);
		planet->on = true;
		Vector newPos = position - planet->position;
		newPos.rotate(walkSpeed*walking*elapsed/planet->size);
		position = planet->position + newPos;
		rotation = (planet->position - position).angle() + M_PI / 2;
	}
	else if (state==inAir)
	{
		accel = totalPull * !shifted;
		rotation = (majorPuller.position - position).angle() + M_PI / 2;
		velocity = velocity + accel *elapsed;
		position = position + velocity * elapsed;
		for (int i = 0; i < g->cluster->planets.size(); i++)
		{
			if (collidesP(g->cluster->planets[i]))
			{
				if (splinch && g->cluster->planets[i].type == Planet::moon)
				{
					if (g->cluster->planets[i].position.distance(position) < g->cluster->planets[i].size / 2 -5)
					{
						takeDamage(maxHealth * 2);
					}
				}
				land(g->cluster->planets[i]);
			}
			else
			{
				g->cluster->planets[i].on = false;
			}
		}
	}
	hook->update(elapsed);
	sword->update(elapsed);
	changeSprite();
}
void Player::changeSprite()
{
	if (dying)
	{
		sprite.index = death.getFrame((deathTime - hurt) / deathTime);
	}
	else if (hurt > 0)
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
	else if (state==inAir)
	{
		sprite.index = 1;
	}
	else if (walking < 0)
	{
		sprite.flipped = false;
		sprite.index = walkAnim.getFrame();
	}
	else if (walking > 0)
	{
		sprite.flipped = true;
		sprite.index = walkAnim.getFrame();
	}
	else
	{
		sprite.index = 0;

	}
}
void Player::render(ShaderProgram* program)
{

	
	if (shifted && shiftingTime <= 0)
	{
		program->setInvert(true);
	}
	Entity::render(program);
	hook->render(program);
	if (sword->active)
	{
		sword->render(program);
	}
	program->setInvert(false);
	if (shiftingTime > 0)
	{

		glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
		g->sprites["particles"].render(program, position.x, position.y, 10 - shiftingTime * 40, 10 - shiftingTime * 40, 0, 1, false);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

}