#include "Projectile.h"
#include "Util.h"
#include "Planet.h"
#include "Vector.h"
using namespace std;
Projectile::Projectile()
{

}
Projectile::Projectile(Game* g, Vector pos) :
Entity(g)
{

	type = projectile;
	position = pos;
	size = Vector(2,2);
	pe = ParticleEmitter(20);
	sprite = g->sprites["particles"];
	pe.sprite = sprite;
	pe.loop = true;
	pe.startSize = 2;
	pe.endSize = 1;
	pe.velocity = Vector(0, 0);
	pe.velocityDev = Vector(10, 10);
	pe.maxLifetime = .1;
	pe.lifeDev = .1;
	pe.position = position;
	pe.gravity = position;
	pe.mass = 20;

	pe.start();
}
void Projectile::render(ShaderProgram* program)
{
	Entity::render(program);
	pe.render(program);
}

void Projectile::update(float elapsed)
{
	Entity::update(elapsed);
	pe.position = position;
	pe.update(elapsed);
	lifetime += elapsed;
	Planet* p = g->cluster->checkCollision(position);
	if (lifetime > maxLifetime || (p && p->type!=Planet::star))
	{
		active = false;
	}
}
void Projectile::playerCollision(Player* p)
{
	p->takeDamage(damage);
	active = false;
}