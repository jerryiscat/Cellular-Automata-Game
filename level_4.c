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

static void DrawGreenWall(int startX, int startY, int w, int h) {
    for (int x = startX; x < startX + w; x++) {
        for (int y = startY; y < startY + h; y++) {
            if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT) {
                grid[x][y] = PLANT;
            }
        }
    }
}

static void Init() {
    level.hasRedObjective = true;

    level.spoutX = 15; level.spoutY = 10;
    level.spoutRedX = 100; level.spoutRedY = 10;

    int wallX = 30;
    int wallW = 5;
    int wallTopY = 60;
    int wallHeight = 90;
    int wallBottomY = wallTopY + wallHeight;
    DrawBlock(wallX, wallTopY, wallW, wallHeight);

    int greenX = wallX + wallW;
    int greenY = wallTopY;
    int greenH = wallHeight / 2;
    int greenW = 80;
    int greenBottomY = greenY + greenH;
    int wallThickness = 6;
    
    level.goalW = 20;
    level.goalH = 20;
    level.goalX = greenX + (greenW - level.goalW) / 2;
    level.goalY = greenY + (greenH - level.goalH) / 2;
    
    DrawBlock(level.goalX - 2, level.goalY, 2, level.goalH);
    DrawBlock(level.goalX + level.goalW, level.goalY, 2, level.goalH);
    DrawBlock(level.goalX - 2, level.goalY + level.goalH, level.goalW + 4, 2);
    
    DrawGreenWall(greenX, greenY - wallThickness, greenW, wallThickness);
    DrawGreenWall(greenX, greenY, wallThickness, greenH);
    DrawGreenWall(greenX + greenW - wallThickness, greenY, wallThickness, greenH);
    DrawGreenWall(greenX, greenBottomY, greenW, wallThickness);
    level.requiredWater = 200;
    level.currentWater = 0;

    level.goalRedX = wallX + wallW + 10;
    level.goalRedY = wallBottomY - 25;
    level.goalRedW = 20;
    level.goalRedH = 20;
    level.requiredRed = 200;
    level.currentRed = 0;
    
    DrawBlock(level.goalRedX - 2, level.goalRedY, 2, level.goalRedH);
    DrawBlock(level.goalRedX + level.goalRedW, level.goalRedY, 2, level.goalRedH);
    DrawBlock(level.goalRedX - 2, level.goalRedY + level.goalRedH, level.goalRedW + 4, 2);

    DrawPlant(20, 100, 18, 15);
    DrawPlant(120, 90, 30, 25);
    DrawPlant(115, 50, 20, 20);

    grid[level.spoutX][level.spoutY] = SPOUT;
    grid[level.spoutRedX][level.spoutRedY] = SPOUT_RED;
}

static const char* GetInfo() {
    return "Round 4: Red damages plants, Blue grows them. Fill both goals!";
}

LevelDef Level4 = { .init = Init, .get_info = GetInfo };

