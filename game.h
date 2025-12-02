#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define GRID_WIDTH 200
#define GRID_HEIGHT 150
#define CELL_SIZE 4

#define EMPTY 0
#define WALL  1
#define SAND  2
#define WATER 3
#define SPOUT 4
#define GOAL  5
#define OBSTACLE 6
#define FIRE  7
#define WATER_RED 8
#define SPOUT_RED 9
#define GOAL_RED  10

#define PLANT 11
#define ASH   12

#define COL_BRICK     (Color){ 160, 80, 50, 255 }
#define COL_MORTAR    (Color){ 100, 50, 30, 255 }
#define COL_SPOUT     (Color){ 80, 80, 255, 255 }
#define COL_OBSTACLE  (Color){ 80, 80, 80, 255 }
#define COL_WATER     (Color){ 30, 144, 255, 200 }
#define COL_FIRE      (Color){ 255, 69, 0, 255 }
#define COL_WATER_RED (Color){ 220, 20, 60, 200 }
#define COL_SPOUT_RED (Color){ 255, 100, 100, 255 }
#define COL_GOAL_RED  (Color){ 200, 50, 50, 100 }

#define COL_PLANT (Color){ 34, 139, 34, 255 } 
#define COL_ASH   (Color){ 169, 169, 169, 255 } 


typedef struct {
    void (*init)(void);
    const char* (*get_info)(void);
} LevelDef;


typedef enum { STATE_PLAYING, STATE_WIN, STATE_ALL_FINISHED, STATE_GAME_OVER } GameState;

typedef struct {
    int spoutX, spoutY; 
    int goalX, goalY, goalW, goalH;
    int requiredWater;
    int currentWater;
    
    int spoutRedX, spoutRedY;
    int goalRedX, goalRedY, goalRedW, goalRedH;
    int requiredRed;
    int currentRed;
    
    bool hasRedObjective;
} LevelConfig;


extern int grid[GRID_WIDTH][GRID_HEIGHT];
extern int next_grid[GRID_WIDTH][GRID_HEIGHT];
extern int currentMaterial;
extern int currentRound;
extern GameState currentState;
extern LevelConfig level;

extern bool isFlowing; 
extern int uiRowOffset;

void UpdatePhysics(void);
void CheckWin(void);
void DrawBlock(int startX, int startY, int w, int h);
void LoadLevelByIndex(int index);
const char* GetCurrentLevelInfo(void);

#endif