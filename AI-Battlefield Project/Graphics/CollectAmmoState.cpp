#include "CollectAmmoState.h"
#include "IdleState.h"
#include "Support.h"
#include "Game.h"
#include "PickupPack.h"

PickupPack* findClosestAmmoPack(NPC* p, Game* game) {
	int distance = INT_MAX;
	PickupPack* closestPickupPack = nullptr;
	for (int i = 0; i < NUM_AMMO_PACKS; i++) {
		PickupPack* pickupPack = game->ammoPickups[i];
		if (pickupPack->getIsTaken()) {
			continue;
		}
		int localDistance = abs(p->getY() - pickupPack->getY()) + abs(p->getX() - pickupPack->getX());
		if (localDistance < distance) {
			closestPickupPack = pickupPack;
			distance = localDistance;
		}
	}
	return closestPickupPack;
}

// enter current state
void CollectAmmoState::OnEnter(NPC* p, Game* game)
{
	if (p->getIsDead()) {
		Transition(p, game);
		return;
	}
	Support* pAsSupport = (Support*)p;
	if (pAsSupport->hasAmmoPacks()) {
		Transition(p, game);
		return;
	}
	PickupPack* pickupPack = findClosestAmmoPack(p, game);
	if (!pickupPack) {
		return;
	}
	if (p->isCloseToMe(pickupPack->getX(), pickupPack->getY(), 1)) {
		pickupPack->take();
		pAsSupport->collectAmmo();
		Transition(p, game);
	}
	p->setIsMoving(true);
	p->setTarget(pickupPack->getX(), pickupPack->getY());
	p->moveTowardTarget(game);
}


void CollectAmmoState::Transition(NPC* p, Game* game)
{
	// exititng from current state
	OnExit(p);
	// create next state and pass it to NPC
	p->setCurrentState(new IdleState());
	// entering new state
	p->getCurrentState()->OnEnter(p, game);
}

// exiting current state
void CollectAmmoState::OnExit(NPC* p)
{
	p->setIsMoving(false);
}

