#pragma once
#include "definitions.h"
#include "NPC.h"
#include "Room.h"

class NPC;
class Game;

class State
{
public:
    virtual void OnEnter(NPC* p, Game* game) = 0;
    virtual void Transition(NPC* p, Game* game) = 0;
    virtual void OnExit(NPC* p) = 0;
};