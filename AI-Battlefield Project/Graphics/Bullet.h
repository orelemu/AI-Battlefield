#pragma once
#include "definitions.h"

class Game;

class Bullet
{
private: 
	double x, y;
	double dir_angle;
	double dirX, dirY;
	double speed;
	bool isMoving;
	int team;
public:
	Bullet(double xx, double yy, double angle, int team);
	void move(Game* game);
	void show();
	void setIsMoving(bool value) { isMoving = value; }
	bool getIsMoving() { return isMoving; }
	void SimulateExplosion(Game* game);

};

