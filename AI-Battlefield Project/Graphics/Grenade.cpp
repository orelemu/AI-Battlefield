#pragma once
#include "Grenade.h"
#include "glut.h"

Grenade::Grenade(double r, double c, int team)
{
	int i;
	row = r;
	col = c;
	double alpha = 2*PI/GRENADE_NUM_BULLETS; // bullet separation angle
	for (i = 0;i < GRENADE_NUM_BULLETS;i++)
	{
		bullets[i] = new Bullet(c, r, i * alpha, team);
	}
}

void Grenade::explode()
{	
	int i;

	for (i = 0;i < GRENADE_NUM_BULLETS;i++)
	{
		bullets[i]->setIsMoving(true);
	}
}

void Grenade::expand(Game* game)
{
	int i;

	for (i = 0;i < GRENADE_NUM_BULLETS;i++)
	{
		bullets[i]->move(game);
	}

}

void Grenade::show()
{
	int i;

	for (i = 0;i < GRENADE_NUM_BULLETS;i++)
	{
		bullets[i]->show();
	}

}

void Grenade::SimulateExplosion(Game* game)
{
	int i;

	for (i = 0;i < GRENADE_NUM_BULLETS;i++)
	{
		bullets[i]->SimulateExplosion(game);
	}

}
