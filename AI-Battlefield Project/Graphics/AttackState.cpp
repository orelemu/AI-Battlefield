#include "AttackState.h"
#include "DefenseState.h"
#include "DeadState.h"
#include "Fighter.h"
#include "Game.h"


NPC* findNearbyAliveEnemy(NPC* p, Game* game) {
	int closestDistance = INT_MAX;
	NPC* enemy = nullptr;
	for (int i = 0; i < NUM_NPCS; i++) {
		NPC* otherNPC = game->NPCs[i];
		bool isEnemyTeam = otherNPC->getTeam() != p->getTeam();
		bool isAlive = !otherNPC->getIsDead();
		if (isEnemyTeam && isAlive) {
			int distance = abs(otherNPC->getX() - p->getX()) + abs(otherNPC->getY() - p->getY());
			if (distance < closestDistance) {
				closestDistance = distance;
				enemy = otherNPC;
			}
		}
	}
	return enemy;
}

int findClosestRoom(NPC* p, Game* game) {
	int closestDistance = INT_MAX;
	int closestRoomIndex = -1;
	for (int i = 0; i < NUM_ROOMS; i++) {
		Room* room = game->rooms[i];
		if (room->isInRoom(p->getX(), p->getY())) {
			return i;
		}
		
		int localDistance = abs(p->getX() - room->getCenterX()) + abs(p->getY() - room->getCenterY());
		if (localDistance < closestDistance) {
			closestDistance = localDistance;
			closestRoomIndex = i;
		}
	}
	return closestRoomIndex;
}

// enter current state
void AttackState::OnEnter(NPC* p, Game* game)
{
	if (p->getIsDead()) {
		Transition(p, game);
		return;
	}
	p->setIsMoving(true);
	Fighter* fighter = dynamic_cast<Fighter*>(p);
	if (p->getHp() < INITIAL_HP / 2 || (fighter && fighter->isOutOfAmmo())) {
		Transition(p, game);
		return;
	}
	NPC* enemy = findNearbyAliveEnemy(p, game);
	if (enemy) {
		p->setTarget(enemy->getX(), enemy->getY());
	}

	if (p->isCloseToMe(p->getTargetX(), p->getTargetY(), p->getRandFactor())) {
		if (p->isTargetInTheSameRoom(game)) {
			if (fighter) {
				fighter->shoot(game);
			}
		}
		int roomIndex = findClosestRoom(p, game);
		if (p->getTeam() == 1) {
			p->setTarget(game->rooms[roomIndex]->getSecuredX(), game->rooms[roomIndex]->getSecuredY());
		}
		else if (p->getTeam() == 2) {
			p->setTarget(game->rooms[roomIndex]->getOtherSecuredX(), game->rooms[roomIndex]->getOtherSecuredY());
		}
		p->moveTowardTarget(game);
		return;
	}

	if (p->isTargetInTheSameRoom(game)) {
		if (fighter) {
			fighter->shoot(game);
		}
	}
	else p->moveTowardTarget(game);
	
}

void AttackState::Transition(NPC* p, Game* game)
{
	// exititng from current state
	OnExit(p);
	// create next state and pass it to NPC
	if (p->getIsDead()) p->setCurrentState(new DeadState());
	else p->setCurrentState(new DefenseState());
	// entering new state
	p->getCurrentState()->OnEnter(p, game);
}

// exiting current state
void AttackState::OnExit(NPC* p)
{
	p->setIsMoving(false);

}
