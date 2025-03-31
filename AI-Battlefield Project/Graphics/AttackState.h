#pragma once
#include "State.h"
#include "definitions.h"

class AttackState : public State
{
public:
	void OnEnter(NPC* p, Game* game);
	void Transition(NPC* p, Game* game);
	void OnExit(NPC* p);
};

