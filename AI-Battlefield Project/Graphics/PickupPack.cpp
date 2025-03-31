#include "PickupPack.h"
#include "glut.h"

PickupPack::PickupPack(int x, int y, int type) 
{
    this->x = x;
    this->y = y;
    this->type = type;
}

void PickupPack::show()
{
    if (isTaken) {
        return;
    }
    if (type == AMMO_PACK) glColor3d(1, 0, 1);
    else if (type == HEALTH_PACK) glColor3d(0, 1, 0);
    const double SIZE = 1;

    glBegin(GL_POLYGON);
    glVertex2d(x, y);
    glVertex2d(x, y + 1);
    glVertex2d(x + 1, y + 1);
    glVertex2d(x + 1, y);
    glEnd();

    glColor3d(1, 1, 1);
    glBegin(GL_LINE_LOOP);
    glVertex2d(x, y);
    glVertex2d(x, y + 1);
    glVertex2d(x + 1, y + 1);
    glVertex2d(x + 1, y);
    glEnd();
}
