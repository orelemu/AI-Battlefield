#include "DefenseState.h"
#include "Support.h"
#include "Game.h"
#include "AttackState.h"
#include "DeadState.h"

int findClosestRoom(NPC* p, Game* game);

// enter current state
void DefenseState::OnEnter(NPC* p, Game* game)
{
	if (p->getIsDead()) { 
		Transition(p, game);
		return;
	}
	p->setIsMoving(true);
	Support* support;
	support = p->getTeam() == 1 ? (Support*)game->NPCs[SUPPORT_TEAM_1] : (Support*)game->NPCs[SUPPORT_TEAM_2];
	if (support->getIsDead()) {
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
	if (p->isCloseToMe(support->getX(), support->getY(), 1)) {
		bool isHealthGiven = support->giveHealth(p);
		Fighter* fighter = dynamic_cast<Fighter*>(p);
		bool isAmmoGiven = fighter && support->giveAmmo(fighter);;
		if (isHealthGiven || isAmmoGiven) {
			p->getCurrentState()->Transition(p, game);
			return;
		}
	}
	p->setTarget(support->getX(), support->getY());
	p->moveTowardTarget(game);
}


void DefenseState::Transition(NPC* p, Game* game)
{
	// exititng from current state
	OnExit(p);
	// create next state and pass it to NPC
	if (p->getIsDead()) p->setCurrentState(new DeadState());
	else p->setCurrentState(new AttackState());
	// entering new state
	p->getCurrentState()->OnEnter(p, game);
}

// exiting current state
void DefenseState::OnExit(NPC* p)
{
	p->setIsMoving(false);

}
