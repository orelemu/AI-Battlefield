#include "Game.h"

Game::Game() : isGameStarted(false) {
    // Initialize maze and securityMap to zero
    for (int i = 0; i < MSZ; i++) {
        for (int j = 0; j < MSZ; j++) {
            maze[i][j] = 0;
            securityMap[i][j] = 0.0;
        }
    }

    // Initialize rooms (assuming they are dynamically allocated)
    for (int i = 0; i < NUM_ROOMS; i++) {
        rooms[i] = nullptr;  // Or allocate if necessary
    }

    // Initialize NPCs
    for (int i = 0; i < 6; i++) {
        NPCs[i] = nullptr;  // Or allocate if necessary
    }
}