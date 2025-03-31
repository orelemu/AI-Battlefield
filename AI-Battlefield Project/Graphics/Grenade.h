#pragma once
#include "Bullet.h"

class Game;

class Grenade
{
private:
	double row, col;
	Bullet* bullets[GRENADE_NUM_BULLETS];
	int team;
	double angle;
	bool isMoving;

public:
	Grenade(double r, double c, int team);
	void explode();
	void expand(Game* game);
	void show();
	void SimulateExplosion(Game* game);
	void setIsMoving(bool isMove) { isMoving = isMove; }
};

