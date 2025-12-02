#include "game.h"

static void DrawPlant(int startX, int startY, int w, int h) {
    for (int x = startX; x < startX + w; x++) {
        for (int y = startY; y < startY + h; y++) {
            if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT) {
                if (GetRandomValue(0, 100) > 10) {
                    grid[x][y] = PLANT;
                }
            }
        }
    }
}

static void Init() {
    level.hasRedObjective = false;

    level.spoutX = 20; level.spoutY = 10;
    level.goalX = 80; level.goalY = 130;
    level.goalW = 40; level.goalH = 15;
    level.requiredWater = 300;
    level.currentWater = 0;

    grid[level.spoutX][level.spoutY] = SPOUT;

    DrawBlock(level.goalX - 2, level.goalY, 2, level.goalH);
    DrawBlock(level.goalX + level.goalW, level.goalY, 2, level.goalH);
    DrawBlock(level.goalX - 2, level.goalY + level.goalH, level.goalW + 4, 2);

    DrawBlock(0, 40, 60, 4); 
    DrawPlant(60, 50, 80, 50);
    DrawBlock(160, 100, 4, 50); 
}

static const char* GetInfo() {
    return "Round 3: Try the fire! Notice: Ash will clog the goal. ";
}

LevelDef Level3 = { .init = Init, .get_info = GetInfo };