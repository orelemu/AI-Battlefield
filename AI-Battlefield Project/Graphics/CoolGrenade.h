#pragma once
#include "definitions.h"
#include "Bullet.h"

class Game;

class CoolGrenade
{
private:
	double x, y;
	double targetX, targetY;
	double dir_angle;
	double dirX, dirY;
	double speed;
	bool isMoving;
	bool isExploding;
	int team;
	Bullet* bullets[GRENADE_NUM_BULLETS];
public:
	CoolGrenade(double xx, double yy, double targetX, double targetY, int team);
	void move(Game* game);
	void show();
	void setIsMoving(bool value) { isMoving = value; }
	bool getIsMoving() { return isMoving; }
	void setIsExploding(bool value) { isExploding = value; }
	bool getIsExploding() { return isExploding; }

};

