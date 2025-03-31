#include "IdleState.h"
#include "Support.h"
#include "Game.h"
#include "CollectAmmoState.h"
#include "CollectHealthState.h"
#include "DeadState.h"

NPC* findAliveTeammate(NPC* p, Game* game) {
	NPC* teammate = nullptr;
	for (int i = 0; i < NUM_NPCS && teammate == nullptr; i++) {
		NPC* otherNPC = game->NPCs[i];
		bool isNotSameNPC = p != otherNPC;
		bool isSameTeam = game->NPCs[i]->getTeam() == p->getTeam();
		bool isAlive = !otherNPC->getIsDead();
		if (isNotSameNPC && isSameTeam && isAlive)
			teammate = otherNPC;
	}
	return teammate;
}

bool isHealthPickupAvailable(Game* game) {
	bool isHealthPickupAvailable = false;
	for (int i = 0; i < NUM_HEALTH_PACKS && !isHealthPickupAvailable; i++) {
		isHealthPickupAvailable = !game->healthPickups[i]->getIsTaken();
	}
	return isHealthPickupAvailable;
}

bool isAmmoPickupAvailable(Game* game) {
	bool isAmmoPickupAvailable = false;
	for (int i = 0; i < NUM_AMMO_PACKS && !isAmmoPickupAvailable; i++) {
		isAmmoPickupAvailable = !game->ammoPickups[i]->getIsTaken();
	}
	return isAmmoPickupAvailable;
}

// enter current state
void IdleState::OnEnter(NPC* p, Game* game)
{
	if (p->getIsDead()) {
		Transition(p, game);
		return;
	}
	Support* pAsSupport = (Support*) p;
	if ((!pAsSupport->hasHealthPacks() && isHealthPickupAvailable(game)) ||
		(!pAsSupport->hasAmmoPacks() && isAmmoPickupAvailable(game))) {
		Transition(p, game);
		return;
	}
	Fighter* fighter = (Fighter*) findAliveTeammate(p, game);
	if (fighter == nullptr) return;
	if (p->isCloseToMe(fighter->getX(), fighter->getY(), 5 + p->getRandFactor())) return;
	p->setIsMoving(true);
	p->setTarget(fighter->getX(), fighter->getY());
	p->moveTowardTarget(game);
}


void IdleState::Transition(NPC* p, Game* game)
{
	// exititng from current state
	OnExit(p);
	// create next state and pass it to NPC
	if (p->getIsDead()) {
		p->setCurrentState(new DeadState());
		return;
	}

	Support* pAsSupport = (Support*)p;
	if (!pAsSupport->hasHealthPacks() && isHealthPickupAvailable(game)) {
		bool isHealthPickupAvailable = false;
		for (int i = 0; i < NUM_HEALTH_PACKS && !isHealthPickupAvailable; i++) {
			isHealthPickupAvailable = !game->healthPickups[i]->getIsTaken();
		}
		if (isHealthPickupAvailable) p->setCurrentState(new CollectHealthState());
	}
	if (!pAsSupport->hasAmmoPacks() && isAmmoPickupAvailable(game)) {
		bool isAmmoPickupAvailable = false;
		for (int i = 0; i < NUM_HEALTH_PACKS && !isAmmoPickupAvailable; i++) {
			isAmmoPickupAvailable = !game->ammoPickups[i]->getIsTaken();
		}
		if (isAmmoPickupAvailable) p->setCurrentState(new CollectAmmoState());
	}

	// entering new state
	p->getCurrentState()->OnEnter(p, game);
}

// exiting current state
void IdleState::OnExit(NPC* p)
{
	p->setIsMoving(false);

}

