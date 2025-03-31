#include "DeadState.h"

// enter current state
void DeadState::OnEnter(NPC* p, Game* game)
{
	p->setIsMoving(false);
}


void DeadState::Transition(NPC* p, Game* game)
{
}

// exiting current state
void DeadState::OnExit(NPC* p)
{
	p->setIsMoving(false);

}

