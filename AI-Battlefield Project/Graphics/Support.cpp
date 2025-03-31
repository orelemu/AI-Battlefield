#include "Support.h"
#include <iostream>
#include "IdleState.h"
#include "Fighter.h"

Support::Support(double x, double y, int team) : NPC(x, y, team), ammoPacks(3), healthPacks(3) {
    setColor(red - 0.2, green - 0.2, blue - 0.2);
    // If you have a state system, you can uncomment these
    pCurrentState = new IdleState();
     //pCurrentState->OnEnter(this);
}

bool Support::giveAmmo(Fighter* p)
{

    if (ammoPacks < 1 || p->getAmmo() > 0) { // If no ammo at all, than allow reload.
        return false;
    }
    ammoPacks--;
    p->setAmmo(INITIAL_AMMO);
    p->setGrenadeAmmo(INITIAL_GRENADE_AMMO);
    return true;
}

bool Support::giveHealth(NPC* p)
{
    if (healthPacks < 1 || p->getHp() >= INITIAL_HP) {
        return false;
    }
    healthPacks--;
    p->setHp(INITIAL_HP);
    return true;
}

void Support::collectAmmo()
{
    ammoPacks++;
}

void Support::collectHealth()
{
    healthPacks++;
}
