#include "game.h"

static void Init() {
    level.spoutX = 40;  level.spoutY = 10;
    level.goalX = 140;  level.goalY = 120;
    level.goalW = 20;   level.goalH = 20;
    level.requiredWater = 200;
    level.currentWater = 0;

    level.hasRedObjective = true;
    level.spoutRedX = 160; level.spoutRedY = 10;
    level.goalRedX = 40;   level.goalRedY = 120;
    level.goalRedW = 20;   level.goalRedH = 20;
    level.requiredRed = 200;
    level.currentRed = 0;

    grid[level.spoutX][level.spoutY] = SPOUT;
    grid[level.spoutRedX][level.spoutRedY] = SPOUT_RED;

    DrawBlock(90, 50, 20, 60); 

    DrawBlock(level.goalX - 2, level.goalY, 2, level.goalH);
    DrawBlock(level.goalX + level.goalW, level.goalY, 2, level.goalH);
    DrawBlock(level.goalX - 2, level.goalY + level.goalH, level.goalW + 4, 2);

    DrawBlock(level.goalRedX - 2, level.goalRedY, 2, level.goalRedH);
    DrawBlock(level.goalRedX + level.goalRedW, level.goalRedY, 2, level.goalRedH);
    DrawBlock(level.goalRedX - 2, level.goalRedY + level.goalRedH, level.goalRedW + 4, 2);
}

static const char* GetInfo() {
    return "Round 2: CROSSOVER! Guide Blue to Right, Red to Left.";
}

LevelDef Level2 = { .init = Init, .get_info = GetInfo };