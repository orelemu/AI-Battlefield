#pragma once
#include "State.h"
#include "definitions.h"
#include "Room.h"

class DefenseState : public State
{
public:
	void OnEnter(NPC* p, Game* game);
	void Transition(NPC* p, Game* game);
	void OnExit(NPC* p);

};

