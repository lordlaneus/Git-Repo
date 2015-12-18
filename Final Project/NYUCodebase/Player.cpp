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
	weapon = new Weapon(g->sprites["attack"],this);
	hook->sprite = g->sprites["particles"];
	this->sprite = sprite;
	position.x = 0;
	position.y = 15;
	size = Vector(4, 7);

	deathAnim.loop = false;
	deathAnim.addFrame(2, 0.05);
	deathAnim.addFrame(4, 0.05);
	deathAnim.addFrame(5, 0.05);
	deathAnim.addFrame(6, 0.05);
	deathAnim.addFrame(7, 0.05);

	walkAnim.duration = 0.5;
	walkAnim.addFrame(8, 1);
	walkAnim.addFrame(9, 1);
	walkAnim.addFrame(10, 1);
	walkAnim.addFrame(11, 1);

	zipAnim.duration = 0.1;
	zipAnim.loop = false;
	zipAnim.addFrame(12, 1);
	zipAnim.addFrame(13, 1);
	zipAnim.addFrame(14, 1);
	zipAnim.addFrame(15, 1);
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
	charging = false;
	if (!weapon->active)
	{
		weapon->swing(dir,velocity,chargeTime);
	}
}
void Player::charge()
{
	charging = true;
	chargeTime = 1;
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
bool Player::drainEnergy(float amount)
{
	if (amount < energy)
	{
		energy -= amount;
		return true;
	}
	return g->exploring;
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
		g->triggerParticles(g->fireEmitter,position, p,20);
		planet = &p;
		hook->state = Hook::away;
		onGround = true;
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
	onGround = false;
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


void Player::zip()
{
	if (onGround && zipTime <= 0 && drainEnergy(zipCost))
	{
		zDown=false;
		g->playSound("zap");
		zipTime = zipLength;
		zipPosition = position;
		sprite.flipped = !sprite.flipped;
	}
}

void Player::update(float elapsed)
{

	if (weapon->active)
	{
		for (int i = 0; i < g->entities.size(); i++)
		{
			Entity*e = g->entities[i];
			if (weapon->collides(*e))
			{
				if (e->type == Entity::enemy)
				{
					dynamic_cast<Enemy*>(e)->takeDamage(weapon->damage);
				}
				else if (e->type == Entity::projectile)
				{
					dynamic_cast<Projectile*>(e)->active = false;
				}
			}
		}
	}
	bool splinch = false;
	if (shifted)
	{
		if (!drainEnergy(shiftCost*elapsed))
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
	if (charging)
	{
		if (drainEnergy(chargeCost*elapsed))
		{
		chargeTime += elapsed;
		}
	}
	if (!shifted&&!charging&&energy < maxEnergy)
	{

		energy += energyRegen* elapsed;
	}
	if (shiftingTime > 0)
	{
		shiftingTime -= elapsed;
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
	else if (onGround)
	{
		if (zDown)
		{
			zip();
		}
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
	if (zipTime > 0)
	{
		zipTime -= elapsed;
		Vector dest = planet->position - zipPosition;
		position = zipPosition + (dest *((zipLength - zipTime) / zipLength) * 2);
	}
	hook->update(elapsed);
	weapon->update(elapsed);
	changeSprite();
}
void Player::changeSprite()
{
	if (dying)
	{
		sprite.index = deathAnim.getFrame((deathTime - hurt) / deathTime);
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
	else if (!onGround)
	{
		sprite.index = 1;
	}
	else if (zipTime > 0)
	{
		sprite.index = zipAnim.getFrame(1 - zipTime / zipLength);
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
	if (weapon->active)
	{
		weapon->render(program);
	}
	program->setInvert(false);
	if (shiftingTime > 0)
	{

		glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
		g->sprites["gui"][3].render(program, position.x, position.y, 10 - shiftingTime * 40, 10 - shiftingTime * 40);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

}