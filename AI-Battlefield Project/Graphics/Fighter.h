#pragma once
#include "NPC.h"
#include "Bullet.h"
#include "CoolGrenade.h"

class Fighter : public NPC
{
private:
	int ammo;
	int grenadeAmmo;
	Bullet* bullet = nullptr;
	CoolGrenade* grenade = nullptr;
	//bool isShooting;
	//bool isThrowingGrenade;
public:
	Fighter(double xx, double yy, int team);
	void shoot(Game* game);
	void throwGrenade(Game* game);
	void show(Game* game);
	int getAmmo() { return ammo; }
	void setAmmo(int newAmmo) { ammo = newAmmo; }
	void setGrenadeAmmo(int newGrenadeAmmo) { grenadeAmmo = newGrenadeAmmo; }
	bool isOutOfAmmo() { return ammo <= 0 && grenadeAmmo <= 0; }
};


