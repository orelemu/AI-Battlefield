#pragma once
#include "State.h"
#include "Cell.h"
#include "CompareCells.h"
#include <vector>
#include <queue>
#include "definitions.h"
#include "Room.h"

const double SPEED = 0.1;

class State;
class Game;

class NPC
{
protected:
    double x, y;
    bool isMoving;
    double targetX, targetY;
    State* pCurrentState;
    double red, green, blue;
    int team;
    int hp = INITIAL_HP;
    bool isDead;
    int randFactor;

public:
    NPC(double xx, double yy, int team);
    virtual void show(Game* game);
    virtual void moveTowardTarget(Game* game);
    bool isTargetInTheSameRoom(Game* game);
    bool isCloseToMe(int x, int y, int distance);
    virtual Cell* calcPath(int maze[MSZ][MSZ], double securityMap[MSZ][MSZ]);
    virtual void AddNeighbor(int r, int c, Cell* pCurrent,
        std::priority_queue < Cell*, std::vector<Cell*>, CompareCells >& pq,
        std::vector <Cell>& grays, std::vector <Cell>& black, double securityMap[MSZ][MSZ]);

    void setIsMoving(bool value) { isMoving = value; }
    void setTarget(double x, double y) { targetX = x; targetY = y; }
    double getX();
    double getY();
    int getTargetX() { return targetX; }
    int getTargetY() { return targetY; }
    void setCurrentState(State* ps) { pCurrentState = ps; }
    State* getCurrentState() { return pCurrentState; }
    void setColor(double r, double g, double b) { red = r; green = g; blue = b; }
    int getTeam();
    void setHp(int newHp);
    int getHp() { return hp; }
    bool getIsDead() { return isDead; }
    int getRandFactor() { return randFactor; }

    virtual ~NPC() = default;
};
