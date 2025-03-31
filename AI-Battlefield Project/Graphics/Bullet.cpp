#include "Bullet.h"
#include <math.h>
#include "glut.h"
#include "Game.h"

// angle is in radians
Bullet::Bullet(double xx, double yy, double angle, int shooterTeam)
{
	x = xx;
	y = yy;
	dir_angle = angle;
	dirX = cos(angle);
	dirY = sin(angle);
	speed = 0.3;
	isMoving = false;
	team = shooterTeam;
}

void Bullet::move(Game* game)
{
	if (isMoving)
	{
		x += speed * dirX;
		y += speed * dirY;
		for (int i = 0; i < NUM_NPCS; i++) {
			NPC* currNPC = game->NPCs[i];
			if (currNPC != nullptr && currNPC->getTeam() != team && currNPC->getX() == (int)x && currNPC->getY() == (int)y) {
				currNPC->setHp(currNPC->getHp() - 5);
			}
		}
		if (game->maze[(int)y][(int)x] == WALL)
			isMoving = false;
	}
}

void Bullet::show()
{
	glColor3d(1, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2d(x - 0.5, y);
	glVertex2d(x , y+ 0.5);
	glVertex2d(x + 0.5, y);
	glVertex2d(x , y- 0.5);
	glEnd();
}

void Bullet::SimulateExplosion(Game* game)
{
	isMoving = true;
	while (isMoving)
	{
		game->securityMap[(int)y][(int)x] += SECURITY_FACTOR;
		move(game);
	}
}
