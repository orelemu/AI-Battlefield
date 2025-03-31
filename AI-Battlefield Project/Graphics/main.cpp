
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "glut.h"
#include <queue>
#include "Cell.h"
#include <iostream>
#include "Room.h"
#include "CompareCells.h"
#include "Bullet.h"
#include "Grenade.h"
#include "Fighter.h"
#include "Support.h"
#include "Game.h"

using namespace std;

const int WIDTH = 700;
const int HEIGHT = 700;

const double WALL_COST = 5;
const double SPACE_COST = 1;

bool bulletFired = false;
bool grenadeThrown = false;

Game* game = new Game();


void GenerateSecurityMap();

void RestorePath(Cell* pc)
{
	int r, c;
	while (pc != nullptr)
	{
		r = pc->getRow();
		c = pc->getCol();
		if (game->maze[r][c] == WALL)
			game->maze[r][c] = SPACE;
		pc = pc->getParent();
	}
}

// row, col are the indices of neighbor cell
void AddNeighbor(int r, int c, Cell* pCurrent,
	priority_queue<Cell*, vector<Cell*>, CompareCells>& pq,
	vector <Cell>& grays,  vector <Cell> &black) // blacks shouldn't be changed
{
	double newg,cost;
	vector<Cell>::iterator itGray;
	vector<Cell>::iterator itBlack;

	if (game->maze[r][c] == WALL) cost = WALL_COST;
	else cost = SPACE_COST;
	newg = pCurrent->getG() + cost;
	Cell* pNeighbor = new Cell(r, c, pCurrent->getTargetRow(), pCurrent->getTargetCol(),
		newg, pCurrent);
	// check what to do with the neighbor Cell
	// 1. if the neighbor is black: do nothing
	// 2. if the neighbor is white: add it to PQ and to grays
	// 3. if it is gray:
	// 3.1 if F of neighbor is not below the neighbor copy that is stored in PQ then do nothing
	// 3.2 otherwise then we must update the PQ and grays
	itGray = find(grays.begin(), grays.end(), *pNeighbor);
	itBlack = find(black.begin(), black.end(), *pNeighbor);

	if (itBlack == black.end()) // then it is not black
	{
		if (itGray == grays.end()) // then it is not gray => it is white
		{
			// paint it gray
			pq.push(pNeighbor);
			grays.push_back(*pNeighbor);
		}
		else // it is gray
		{
			if (pNeighbor->getF() < itGray->getF()) // then we found a better path and we have to exchange it
			{
				grays.erase(itGray);
				grays.push_back(*pNeighbor);

				// and do the same with PQ
				vector<Cell*> tmp;
				while (!pq.empty() &&  !((*pq.top()) == (*pNeighbor)))
				{
					tmp.push_back(pq.top()); // save the top element in tmp
					pq.pop(); // remove top element from pq
				}
				if (pq.empty()) // ERROR!
				{
					cout << "PQ is empty\n";
					exit(2);
				}
				else // we have found the neighbor cell in PQ
				{
					pq.pop(); // remove old neighbor from pq
					pq.push(pNeighbor);
					// now restore pq
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



// run A* from room at index1 to room at index2
void BuildPath(int index1, int index2)
{
	int r, c, tr, tc;

	r = game->rooms[index1]->getCenterY();
	c = game->rooms[index1]->getCenterX();
	tr = game->rooms[index2]->getCenterY();
	tc = game->rooms[index2]->getCenterX();
	Cell* pCurrent;
	Cell* start = new Cell(r,c ,tr ,tc , 0, nullptr);
	priority_queue<Cell*, vector<Cell*>, CompareCells> pq;
	vector <Cell> grays;
	vector <Cell> black;
	vector<Cell>::iterator itGray;

	pq.push(start);
	grays.push_back(*start);
	// pq shouldn't be empty because we are going to reach the target beforehand
	while (!pq.empty()) 	
	{
		pCurrent = pq.top();
		if (pCurrent->getH() < 0.001) // this is a targt cell
		{
			RestorePath(pCurrent);
			return;
		}
		else // target hasn't been reached
		{
			// 1. remove pCurrent from pq 
			pq.pop();
			// 2. find and remove pCurrent from grays
			itGray = find(grays.begin(), grays.end(), *pCurrent);
			if (itGray == grays.end()) // pCurrent wasn't found
			{
				cout << "Error: pCurrent wasn't found in grays\n";
				exit(1);
			}
			grays.erase(itGray);
			// 3. paint pCurrent black
			black.push_back(*pCurrent);
			// 4. take care of neighbors
			r = pCurrent->getRow();
			c = pCurrent->getCol();
			// up
			if (r + 1 < MSZ)
				AddNeighbor(r + 1, c, pCurrent, pq, grays, black);
			// down
			if (r - 1 >=0)
				AddNeighbor(r - 1, c, pCurrent, pq, grays, black);
			// left
			if (c - 1 >= 0)
				AddNeighbor(r , c-1, pCurrent, pq, grays, black);
			// right
			if (c + 1 < MSZ)
				AddNeighbor(r, c + 1, pCurrent, pq, grays, black);
		}

	}
	cout << "Error pq is empty, target hasn't been found\n";
}

void BuildPathBetweenTheRooms()
{
	int i, j;

	for (i = 0;i < NUM_ROOMS;i++)
		for (j = i + 1;j < NUM_ROOMS;j++)
		{
			BuildPath(i, j); // A*
			cout << "The path from " << i << " to " << j << " has been paved\n";
		}
}

void findEmptySpotForPickup(int* row, int* col) {
	while (true) {
		int randRow = rand() % MSZ;
		int randCol = rand() % MSZ;
		if (game->maze[randRow][randCol] != WALL) {
			*row = randRow;
			*col = randCol;
			return;
		}
	}
}

void SetupAmmoPickupPacks() {
	int ammoPacksSet = 0;
	int row, col;
	while (ammoPacksSet < NUM_AMMO_PACKS) {
		findEmptySpotForPickup(&row, &col);
		PickupPack* ammoPickup = new PickupPack(col, row, AMMO_PACK);
		game->ammoPickups[ammoPacksSet] = ammoPickup;
		ammoPacksSet++;
	}
}

void SetupHealthPickupPacks() {
	int healthPacksSet = 0;
	int row, col;
	while (healthPacksSet < NUM_HEALTH_PACKS) {
		findEmptySpotForPickup(&row, &col);
		PickupPack* healthPickup = new PickupPack(col, row, HEALTH_PACK);
		game->healthPickups[healthPacksSet] = healthPickup;
		healthPacksSet++;
	}
}

void SetupDungeon()
{
	int i,j;
	int cx, cy, w, h;
	bool hasOverlap;

	for (i = 0;i < NUM_ROOMS;i++)
	{
		do
		{
			hasOverlap = false;
			w = 6 + rand() % (MSZ / 5);
			h = 6 + rand() % (MSZ / 5);
			cx = 2 + w / 2 + rand() % (MSZ - w - 4);
			cy = 2 + h / 2 + rand() % (MSZ - h - 4);
			for(j=0;j<i && !hasOverlap;j++)
				hasOverlap = game->rooms[j]->Overlap(cx, cy, w, h);
		} while (hasOverlap); // check the validity of the room
			
		game->rooms[i] = new Room(cx, cy, w, h, game->maze);
	}

	for (i = 0;i < 100;i++)
		game->maze[rand() % MSZ][rand() % MSZ] = WALL;
	SetupAmmoPickupPacks();
	SetupHealthPickupPacks();

	BuildPathBetweenTheRooms();
}

void SpawnTeam(int startIndex, int team, int roomIndex) {
	int spawnX = game->rooms[roomIndex]->getCenterX();
	int spawnY = game->rooms[roomIndex]->getCenterY();

	game->NPCs[startIndex] = new Fighter(spawnX - 2, spawnY, team);
	game->NPCs[startIndex + 1] = new Fighter(spawnX, spawnY, team);
	game->NPCs[startIndex + 2] = new Support(spawnX + 2, spawnY, team);
}

void SetupTeams() {
	int firstRoom = rand() % NUM_ROOMS;
	SpawnTeam(0, 1, firstRoom);

	int secondRoom;
	do {
		secondRoom = rand() % NUM_ROOMS;
	} while (secondRoom == firstRoom);

	SpawnTeam(3, 2, secondRoom);
}

void init()
{
	glClearColor(0.5, 0.5, 0.5, 0);// color of window background
	glOrtho(0, MSZ, 0, MSZ, -1, 1); // set the coordinates system

	srand(time(0));

	SetupDungeon();
	GenerateSecurityMap();
	SetupTeams();
	//game->isGameStarted = true;
}

void ShowDungeon()
{
	int i, j;
	double s;

	for(i=0;i<MSZ;i++)
		for (j = 0;j < MSZ;j++)
		{
			s = game->securityMap[i][j];
			//1. set color of cell
			switch (game->maze[i][j])
			{
			case SPACE:
				glColor3d(1 - s, 1 - s, 1 - s); // gray //TODO: remove security map color on dungeon.
				break;
			case WALL:
				glColor3d(0.3, 0.3, 0.4); // dark gray
				break;
			}
			// show cell
			glBegin(GL_POLYGON);
			glVertex2d(j, i);
			glVertex2d(j, i + 1);
			glVertex2d(j+1, i + 1);
			glVertex2d(j + 1, i );
			glEnd();
		}
}

void ShowPickups() 
{
	for (int i = 0; i < NUM_AMMO_PACKS; i++) game->ammoPickups[i]->show();
	for (int i = 0; i < NUM_HEALTH_PACKS; i++) game->healthPickups[i]->show();
}

void ShowTeams()
{
	for (int i = 0; i < NUM_NPCS; i++) game->NPCs[i]->show(game);
}

void GenerateSecurityMap()
{
	int numSimulations = 5000;
	int i;

	for (i = 0;i < numSimulations;i++)
	{
		Grenade* g = new Grenade(rand() % MSZ, rand() % MSZ, -1); // Team is irrelevant here.

		g->SimulateExplosion(game);
	}
	for (int i = 0; i < NUM_ROOMS; i++) {
		Room* room = game->rooms[i];
		int startX = room->getCenterX() - room->getWidth() / 2;
		int endX = startX + room->getWidth();
		int startY = room->getCenterY() - room->getHeight() / 2;
		int endY = startY + room->getHeight();
		//int mostSecuredValue = INT_MAX;
		int securedX = -1;
		int securedY = -1;
		int otherSecuredX = -1;
		int otherSecuredY = -1;

		while (securedX == -1 || securedY == -1) {
			int randX = startX + rand() % room->getWidth();
			int randY = startY + rand() % room->getHeight();
			if (game->maze[randY][randX] != WALL && game->securityMap[randY][randX] < 0.3) {
				securedX = randX;
				securedY = randY;
			}
		}

		while (otherSecuredX == -1 || otherSecuredY == -1) {
			int randX = startX + rand() % room->getWidth();
			int randY = startY + rand() % room->getHeight();
			int isFarEnough = (abs(randX - securedX) > 2 && abs(randY - securedY) > 2);
			if (isFarEnough && game->maze[randY][randX] != WALL && game->securityMap[randY][randX] < 0.3) {
				otherSecuredX = randX;
				otherSecuredY = randY;
			}
		}
		//for (int i = startX; i < endX; i++) {
		//	for (int j = startY; j < endY; j++) {
		//		if (game->maze[j][i] == WALL) continue;
		//		int localMin = game->securityMap[j][i];
		//		if (localMin < mostSecuredValue) {
		//			int isFarEnough = (abs(otherSecuredX - securedX) > 5 && abs(otherSecuredY - securedY) > 5);
		//			if (isFarEnough) {
		//				otherSecuredX = securedX;
		//				otherSecuredY = securedY;
		//			}
		//			mostSecuredValue = localMin;
		//			securedX = i;
		//			securedY = j;
		//		}
		//	}
		//}
		room->setSecuredPositions(securedX, securedY, otherSecuredX, otherSecuredY);
	}

}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	ShowDungeon();
	ShowPickups();
	ShowTeams();

	glutSwapBuffers(); // show all
}

static void setTargetPos(NPC* npc, NPC* targetNPC) {
	npc->setTarget(targetNPC->getX(), targetNPC->getY());
}

bool isTeamFightersDead(int team) {
	bool isAllTeamDead = true;
	for (int i = 0; i < NUM_NPCS && isAllTeamDead; i++) {
		if (i == SUPPORT_TEAM_1 || i == SUPPORT_TEAM_2) continue;
		NPC* teammate = game->NPCs[i];
		if (teammate->getTeam() == team) {
			isAllTeamDead = teammate->getIsDead();
		}
	}
	return isAllTeamDead;
}

bool isAllAmmoPickupsTaken() {
	for (int i = 0; i < NUM_AMMO_PACKS; i++) {
		if (!game->ammoPickups[i]->getIsTaken()) {
			return false; // If any ammo pack is not taken, return false
		}
	}
	return true;
}

bool isAllHealthPickupsTaken() {
	for (int i = 0; i < NUM_HEALTH_PACKS; i++) {
		if (!game->healthPickups[i]->getIsTaken()) {
			return false; // If any health pack is not taken, return false
		}
	}
	return true; // All pickups are taken
}

void idle()
{
	if (game->isGameStarted) {
		if (isTeamFightersDead(1)) {
			cout << "Team 2 won!!\n";
			game->isGameStarted = false;
			//cout << "Press Enter to exit...\n";
			//cin.get(); // Waits for user input

			//exit(0); // Terminates GLUT
		}
		if (isTeamFightersDead(2)) {
			cout << "Team 1 won!!\n";
			cout << "Press Enter to exit...\n";
			//cin.get(); // Waits for user input
			game->isGameStarted = false;
			//exit(0); // Terminates GLUT
		}
		if (isAllAmmoPickupsTaken()) {
			SetupAmmoPickupPacks();
		}
		if (isAllHealthPickupsTaken()) {
			SetupHealthPickupPacks();
		}
		for (int i = 0; i < NUM_NPCS; i++) game->NPCs[i]->getCurrentState()->OnEnter(game->NPCs[i], game);
	}

	glutPostRedisplay(); // indirect call to display
}

void menu(int choice)
{
	switch (choice)
	{
	case 1: // Start game
		game->isGameStarted = true;
		break;
	}
}

void main(int argc, char* argv[]) 
{
	glutInit(&argc, argv);
	// definitions for visual memory (Frame buffer) and double buffer
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(600, 20);
	glutCreateWindow("BFS");

	// display is a refresh function
	glutDisplayFunc(display);
	// idle is a update function
	glutIdleFunc(idle);

	// menu
	glutCreateMenu(menu);
	glutAddMenuEntry("Start game", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	init();

	glutMainLoop();
}