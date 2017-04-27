#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"
using namespace std;

Player::Player()
{
	

}
Player::~Player()
{
	delete hook;
	delete weapon;
}
Player::Player(Game* g, Sprite sprite) 
{
	this->g = g;
	this->sprite = sprite;
	//tile = &g->room->tiles[0][0];// this initial value shouldn't be used, but it could prevent a crash
	health = maxHealth;
	hook = new Hook(this);
	weapon = new Weapon(g->sprites["attack"],this);
	hook->sprite = g->sprites["particles"];
	this->sprite = sprite;

	position.x = 0;
	position.y = 15; // so that the player pops to the top of the starting world

	size = Vector(4, 7);


	//configure animations
	deathAnim.loop = false;
	deathAnim.addFrame(2, 1);
	deathAnim.addFrame(4, 1);
	deathAnim.addFrame(5, 1);
	deathAnim.addFrame(6, 1);
	deathAnim.addFrame(7, 1);

	walkAnim.duration = 0.5;
	walkAnim.addFrame(8, 1);
	walkAnim.addFrame(9, 1);
	walkAnim.addFrame(10, 1);
	walkAnim.addFrame(11, 1);
}
bool Player::collidesT(Tile& t)
{
	//todo
	return false;
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
	chargeTime = 0;
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
	draining = true;
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
		Vector jumpVector = Vector(0.0f, 1.0f);
		jumpVector.normalize(jumpPower);
		velocity = velocity + jumpVector;
	}
	else if (hook->state==Hook::reeling)
	{
		hook->state = Hook::recoiling;
	}
} 
void Player::land(Tile &t)
{
		g->playSound("land");
		hook->state = Hook::away;
		onGround = true;
		tile = &t;
		walk();
		accel.clear();
		velocity.clear();


}
void Player::takeDamage(float dmg)
{
	if (hurt > 0)
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
		case SDLK_a:
			walking = -1;
			break;
		case SDLK_d:
			walking = 1;
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
void Player::pop(Tile& t)
{
	
	Vector v = position - t.position;
	float l = v.length();
	v.normalize();
	position = t.position+ v;
	if (!t.on)
	{
		v.normalize(size.y/ 2);
		g->dustEmitter.sprite.index = t.sprite.index;
		g->triggerDust(position-v, t,10);
	}

}


void Player::update(float elapsed)
{
	draining = false;
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
	if (charging)
	{
		if (drainEnergy(chargeCost*elapsed))
		{
		chargeTime += elapsed;
		}
	}

	
	if (hurt > 0)
	{
	hurt -= elapsed;
	}
	if (dying)
	{
		velocity = velocity + accel* elapsed;
		position = position + velocity * elapsed;
		if (hurt <= 0)
		{
			g->currentMenu = &g->gameOverMenu;
			g->gameOverMenu.show();
			g->state = Game::menu;

		}
	}
	else
	{
		accel = g->room->gravity;
		velocity = velocity + accel *elapsed;
		position = position + velocity * elapsed;
	}
	if (!draining && energy<maxEnergy)
	{
		energy += energyRegen* elapsed;// refills energyy provided no calls have been made to drainEnergy()
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

	Entity::render(program);
	hook->render(program);
	if (weapon->active)
	{
		weapon->render(program);
	}
	program->setInvert(false);
	if (g->cPressed)
	{
		Vector direction(1, 1);
		Entity* closest = NULL;
		float minDistance = FLT_MAX;
		for (int i = 0; i < g->entities.size(); i++)
		{
			if (g->entities[i]->type == Entity::enemy &&  g->entities[i]->position.distance(position) < minDistance)
			{
				closest = g->entities[i];
				minDistance = (g->entities[i]->position.distance(position));
			}
		}
		if (closest)
		{
			Vector temp = (closest->position - position).normalize(5);

				g->sprites["gui"][1].render(program, position.x + temp.x, position.y + temp.y, 3, 3, temp.angle());
		}
	}

}