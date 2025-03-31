#include "Fighter.h"
#include "AttackState.h"
#include "Bullet.h"
#include "Game.h"
#include "CoolGrenade.h"

Fighter::Fighter(double x, double y, int team) : NPC(x, y, team), ammo(INITIAL_AMMO), grenadeAmmo(INITIAL_GRENADE_AMMO), bullet(nullptr) {
	this->setCurrentState(new AttackState());
}

void Fighter::shoot(Game* game)
{
	double dx = targetX - x;
	double dy = targetY - y;
	double direction = atan2(dy, dx); // Angle in radians
	if ((bullet == nullptr || !bullet->getIsMoving()) && ammo > 0) {
		ammo--;
		bullet = new Bullet(x, y, direction, team);
		bullet->setIsMoving(true);
	}
	if (ammo <= 0) {
		throwGrenade(game);
	}
}

void findRandomTargetNearTarget(Game* game, Fighter* fighter, int* targetX, int* targetY) {
	int radius = 2;
	int randomNearTargetX = -1;
	int randomNearTargetY = -1;
	while (randomNearTargetX == -1 || randomNearTargetY == -1) {
		int tempRandX = (fighter->getTargetX() - radius) + rand() % radius * 2;
		int tempRandY = (fighter->getTargetY() - radius) + rand() % radius * 2;
		if (tempRandY < MSZ && tempRandY > 0 && tempRandX < MSZ && tempRandX > 0 && game->maze[tempRandY][tempRandX] != WALL) {
			randomNearTargetX = tempRandX;
			randomNearTargetY = tempRandY;
		}
	}
	*targetX = randomNearTargetX;
	*targetY = randomNearTargetY;


}

void Fighter::throwGrenade(Game* game)
{
	int nearTargetX, nearTargetY;
	findRandomTargetNearTarget(game, this, &nearTargetX, &nearTargetY);
	if ((grenade == nullptr || !grenade->getIsMoving()) && grenadeAmmo > 0 && !isCloseToMe(nearTargetX, nearTargetY, 2)) {
		grenadeAmmo--;
		grenade = new CoolGrenade(x, y, nearTargetX, nearTargetY, team);
		grenade->setIsMoving(true);
	}
}

void Fighter::show(Game* game)
{
	NPC::show(game); // Call base class NPC's show()
	if (bullet != nullptr) {
		bullet->show(); // Call bullet's show()
		bullet->move(game);
	}
	if (grenade != nullptr) {
		grenade->show();
		grenade->move(game);
	}
}