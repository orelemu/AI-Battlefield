#include "CoolGrenade.h"
#include <math.h>
#include "glut.h"
#include "Game.h"
#include "Grenade.h"

// angle is in radians
CoolGrenade::CoolGrenade(double xx, double yy, double targetX, double targetY, int shooterTeam)
{
	x = xx;
	y = yy;
	double dx = targetX - x;
	double dy = targetY - y;
	this->targetX = targetX;
	this->targetY = targetY;
	dir_angle = atan2(dy, dx); // Angle in radians
	dirX = cos(dir_angle);
	dirY = sin(dir_angle);
	speed = 0.2;
	isMoving = false;
	isExploding = false;
	team = shooterTeam;
}

void CoolGrenade::move(Game* game)
{
	if (isMoving && !isExploding)
	{
		x += speed * dirX;
		y += speed * dirY;
		bool isCloseToTarget = (abs(targetX - x) < 2 && abs(targetY - y) < 2);
		if (isCloseToTarget) {
			setIsExploding(true);
			double alpha = 2 * PI / GRENADE_NUM_BULLETS; // bullet separation angle
			for (int i = 0;i < GRENADE_NUM_BULLETS;i++)
			{
				bullets[i] = new Bullet(x, y, i * alpha, team);
				bullets[i]->setIsMoving(true);
			}
		}
	}
	if (isMoving && isExploding) {
		int isOneBulletMoving = false;

		for (int i = 0; i < GRENADE_NUM_BULLETS;i++)
		{
			bullets[i]->show();
			bullets[i]->move(game);
			if (!isOneBulletMoving) {
				isOneBulletMoving = bullets[i]->getIsMoving();
			}
		}
		if (!isOneBulletMoving) {
			setIsMoving(false);
		}
	}
}

void CoolGrenade::show()
{
	if (!isExploding) {
		glColor3d(1, 0.3, 0.3);
		glBegin(GL_POLYGON);
		glVertex2d(x - 1, y);
		glVertex2d(x, y + 1);
		glVertex2d(x + 1, y);
		glVertex2d(x, y - 1);
		glEnd();
	}
}