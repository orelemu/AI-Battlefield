#pragma once
#include "definitions.h"


class PickupPack
{
private:
	int type; // AMMO / HEALTH
	int x, y;
	bool isTaken = false;
public:
	PickupPack(int x, int y, int type);
	void show();
	void take() { isTaken = true; }
	bool getIsTaken() { return isTaken; }
	int getX() { return x; }
	int getY() { return y; }
};