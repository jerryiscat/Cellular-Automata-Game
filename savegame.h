#ifndef SAVEGAME_H
#define SAVEGAME_H

#include "game.h"

typedef struct {
    int magic;
    int version;
    
    int grid[GRID_WIDTH][GRID_HEIGHT];
    
    int currentRound;
    GameState currentState;
    int currentMaterial;
    bool isFlowing;
    
    LevelConfig level;
} SaveGameData;

#define SAVE_MAGIC 0x53415645
#define SAVE_VERSION 1
#define SAVE_FILE "game_save.dat"

// Functions
bool SaveGame(void);
bool LoadGame(void);

#endif

