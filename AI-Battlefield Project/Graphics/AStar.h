#pragma once
#include <stdlib.h>
#include <queue>
#include "Cell.h"
#include "CompareCells.h"

static class AStar
{
public:
    void BuildPath(int r, int c, int tr, int tc);
    void AddNeighbor(int r, int c, Cell* pCurrent,
        std::priority_queue<Cell*, std::vector<Cell*>, CompareCells>& pq,
        std::vector<Cell>& grays, std::vector<Cell>& black);
};
