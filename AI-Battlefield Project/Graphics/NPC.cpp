#include "NPC.h"
#include "glut.h"
#include <cmath>
#include "CompareCells.h"
#include <queue>
#include <iostream>
#include "definitions.h"
#include "Game.h"

using namespace std;

NPC::NPC(double xx, double yy, int NPCTeam)
{
    randFactor = rand() % 5;
    isDead = false;
    x = xx;
    y = yy;
    isMoving = true;
    targetX = 0;
    targetY = 0;
    pCurrentState = nullptr;
    team = NPCTeam;
    if (team == 1) {
        red = 1;
        green = 1;
        blue = 0;
    }
    else {
        red = 0;
        green = 1;
        blue = 1;
    }
}

void NPC::show(Game* game)
{
    const double SIZE = 1;
    glColor3d(red, green, blue);

    glBegin(GL_POLYGON);
    glVertex2d(x, y);
    glVertex2d(x, y + 1);
    glVertex2d(x + 1, y + 1);
    glVertex2d(x + 1, y);
    glEnd();

    glColor3d(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2d(x, y);
    glVertex2d(x, y + 1);
    glVertex2d(x + 1, y + 1);
    glVertex2d(x + 1, y);
    glEnd();
}

void NPC::moveTowardTarget(Game* game)
{
    if (isMoving)
    {
        Cell* nextCell = calcPath(game->maze, game->securityMap);
        if (nextCell != nullptr) {
            x = nextCell->getCol();
            y = nextCell->getRow();
        }
    }
}

bool NPC::isTargetInTheSameRoom(Game* game) {
    bool isInTheSameRoom = false;
    for (int i = 0; i < NUM_ROOMS && !isInTheSameRoom; i++) {
        isInTheSameRoom = game->rooms[i]->isInRoom(x, y) && game->rooms[i]->isInRoom(targetX, targetY);
    }
    return isInTheSameRoom;
}

bool NPC::isCloseToMe(int otherX, int otherY, int distance)
{
    return (abs(otherX - x) < distance && abs(otherY - y) < distance);
}

void NPC::AddNeighbor(int r, int c, Cell* pCurrent,
    std::priority_queue < Cell*, std::vector<Cell*>, CompareCells >& pq,
    std::vector <Cell>& grays, std::vector <Cell>& black, double securityMap[MSZ][MSZ])
{

    double newg, cost;
    std::vector<Cell>::iterator itGray;
    std::vector<Cell>::iterator itBlack;

    cost = securityMap[r][c] * 10;

    newg = pCurrent->getG() + cost;
    Cell* pNeighbor = new Cell(r, c, pCurrent->getTargetRow(), pCurrent->getTargetCol(),
        newg, pCurrent);

    itGray = find(grays.begin(), grays.end(), *pNeighbor);
    itBlack = find(black.begin(), black.end(), *pNeighbor);

    if (itBlack == black.end()) // then it is not black
    {
        if (itGray == grays.end()) // then it is not gray => it is white
        {
            pq.push(pNeighbor);
            grays.push_back(*pNeighbor);
        }
        else // it is gray
        {
            if (pNeighbor->getF() < itGray->getF()) // found a better path
            {
                grays.erase(itGray);
                grays.push_back(*pNeighbor);

                std::vector<Cell*> tmp;
                while (!pq.empty() && !((*pq.top()) == (*pNeighbor)))
                {
                    tmp.push_back(pq.top());
                    pq.pop();
                }

                if (pq.empty())
                {
                    std::cout << "PQ is empty\n";
                    exit(2);
                }
                else
                {
                    pq.pop();
                    pq.push(pNeighbor);

                    while (!tmp.empty())
                    {
                        pq.push(tmp.back());
                        tmp.pop_back();
                    }
                }
            }
        }
    }
}

Cell* NPC::calcPath(int maze[MSZ][MSZ], double securityMap[MSZ][MSZ])
{
    int r, c, tr, tc;

    r = y;
    c = x;
    tr = targetY;
    tc = targetX;
    Cell* pCurrent;
    Cell* start = new Cell(r, c, tr, tc, 0, nullptr);
    std::priority_queue<Cell*, std::vector<Cell*>, CompareCells> pq;
    std::vector <Cell> grays; // 
    std::vector <Cell> black; // already visited
    std::vector<Cell>::iterator itGray;

    pq.push(start);
    grays.push_back(*start);

    while (!pq.empty())
    {
        pCurrent = pq.top();

        if (pCurrent->getH() < 0.001) // target reached
        {
            if (pCurrent->getParent() == nullptr)
            {
                return pCurrent;
            }
            while (pCurrent->getParent() != start)
            {
                pCurrent = pCurrent->getParent();
            }
            //delete start; // Prevent memory leak
            return pCurrent;
        }
        else
        {
            pq.pop();
            itGray = find(grays.begin(), grays.end(), *pCurrent);

            if (itGray == grays.end())
            {
                std::cout << "Error: pCurrent wasn't found in grays\n";
                exit(1);
            }
            grays.erase(itGray);
            black.push_back(*pCurrent);

            r = pCurrent->getRow();
            c = pCurrent->getCol();

            // Check neighbors
            if (r + 1 < MSZ && maze[r+1][c] != WALL)
                AddNeighbor(r + 1, c, pCurrent, pq, grays, black, securityMap);
            if (r - 1 >= 0 && maze[r-1][c] != WALL)
                AddNeighbor(r - 1, c, pCurrent, pq, grays, black, securityMap);
            if (c - 1 >= 0 && maze[r][c-1] != WALL)
                AddNeighbor(r, c - 1, pCurrent, pq, grays, black, securityMap);
            if (c + 1 < MSZ && maze[r][c+1] != WALL)
                AddNeighbor(r, c + 1, pCurrent, pq, grays, black, securityMap);
        }
    }

    delete start; // Prevent memory leak
    return nullptr;
}

double NPC::getX()
{
    return x;
}

double NPC::getY()
{
    return y;
}

int NPC::getTeam()
{
    return team;
}

void NPC::setHp(int newHp)
{
    if (!isDead) {
        hp = newHp;
        if (hp <= 0) {
            setColor(red - 0.5, green - 0.5, blue - 0.5);
            isDead = true;
        }
    }
}
