#pragma once
#include "NPC.h"
#include "Fighter.h"

class Support : public NPC
{
private:
	int healthPacks;
	int ammoPacks;
public:
	Support(double xx, double yy, int team);
	bool giveAmmo(Fighter* p);
	bool giveHealth(NPC* p);
	void collectAmmo();
	void collectHealth();
	bool hasHealthPacks() { return healthPacks > 0; }
	bool hasAmmoPacks() { return ammoPacks > 0; }
};