#pragma once
#include "definitions.h"
#include "Room.h"
#include "NPC.h"
#include "PickupPack.h"

class Game
{
public:
    Game();
	bool isGameStarted;
	int maze[MSZ][MSZ];
	double securityMap[MSZ][MSZ];
	Room* rooms[NUM_ROOMS];
	NPC* NPCs[NUM_NPCS];
	PickupPack* ammoPickups[NUM_AMMO_PACKS];
	PickupPack* healthPickups[NUM_HEALTH_PACKS];
};