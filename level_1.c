#include "game.h"

static void Init() {
    level.spoutX = 20; level.spoutY = 10;
    level.goalX = 160; level.goalY = 120;
    level.goalW = 15;  level.goalH = 15;
    level.requiredWater = 150;
    level.currentWater = 0;
    level.hasRedObjective = false;

    grid[level.spoutX][level.spoutY] = SPOUT;
    
    DrawBlock(80, 50, 10, 50);
    DrawBlock(level.goalX - 2, level.goalY, 2, level.goalH);
    DrawBlock(level.goalX + level.goalW, level.goalY, 2, level.goalH);
    DrawBlock(level.goalX - 2, level.goalY + level.goalH, level.goalW + 4, 2);
}

static const char* GetInfo() {
    return "Round 1: Draw WALLS (1) to guide water.";
}

LevelDef Level1 = { .init = Init, .get_info = GetInfo };