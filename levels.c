#include "game.h"

extern LevelDef Level1;
extern LevelDef Level2;
extern LevelDef Level3;
extern LevelDef Level4;

static LevelDef* allLevels[] = { &Level1, &Level2, &Level3, &Level4 };
static int totalLevels = 4;

bool isFlowing = false; 

void LoadLevelByIndex(int index) {
    int safeIndex = index - 1; 
    if (safeIndex < 0) safeIndex = 0;
    if (safeIndex >= totalLevels) safeIndex = 0;

    currentRound = index;
    currentState = STATE_PLAYING;
    
    isFlowing = false; 
    
    memset(grid, 0, sizeof(int) * GRID_WIDTH * GRID_HEIGHT);
    allLevels[safeIndex]->init();
}

const char* GetCurrentLevelInfo(void) {
    int safeIndex = currentRound - 1;
    if (safeIndex < 0 || safeIndex >= totalLevels) return "";
    return allLevels[safeIndex]->get_info();
}