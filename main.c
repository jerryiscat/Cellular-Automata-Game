#include "game.h"
#include "savegame.h"
#include <math.h>


// --- Global Variable Definitions ---
int grid[GRID_WIDTH][GRID_HEIGHT];
int next_grid[GRID_WIDTH][GRID_HEIGHT];
int currentMaterial = WALL;
GameState currentState = STATE_PLAYING;
int currentRound = 1;
LevelConfig level;
// Number of grid rows to offset spout emission / UI area. Set after topBarH is defined in main.
int uiRowOffset = 0;

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Elemental Puzzler (Final Polish)");
    SetTargetFPS(60);

    LoadLevelByIndex(1);

    int topBarH = 70;
    uiRowOffset = topBarH / CELL_SIZE;
    Rectangle btnRect = { 350, topBarH + 5, 80, 30 };
    
    float time = 0.0f;

    while (!WindowShouldClose()) {
        time += 0.05f;
        
        Vector2 mouse = GetMousePosition();
        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, btnRect)) {
            if (currentState == STATE_PLAYING) {
                isFlowing = !isFlowing;
            }
        }
        if (currentState == STATE_WIN) {
            if (IsKeyPressed(KEY_SPACE)) {
                LoadLevelByIndex(currentRound + 1);
                if (currentRound > 4) currentState = STATE_ALL_FINISHED; 
            }
        }
        else if (currentState == STATE_ALL_FINISHED) {
             if (IsKeyPressed(KEY_R)) LoadLevelByIndex(1);
        }
        else if (currentState == STATE_GAME_OVER) {
             if (IsKeyPressed(KEY_R)) LoadLevelByIndex(currentRound);
        }
        else {
            if (IsKeyPressed(KEY_R)) LoadLevelByIndex(currentRound);
            if (IsKeyPressed(KEY_S)) LoadLevelByIndex(currentRound + 1);
            
            if (IsKeyPressed(KEY_F5)) {
                if (SaveGame()) {
                    TraceLog(LOG_INFO, "Game saved successfully!");
                } else {
                    TraceLog(LOG_WARNING, "Failed to save game!");
                }
            }
            if (IsKeyPressed(KEY_F9)) {
                if (LoadGame()) {
                    TraceLog(LOG_INFO, "Game loaded successfully!");
                } else {
                    TraceLog(LOG_WARNING, "Failed to load game or no save file found!");
                }
            }
        }
        if (IsKeyPressed(KEY_ONE))   currentMaterial = WALL;
        if (currentRound >= 2 && IsKeyPressed(KEY_TWO))   currentMaterial = SAND;
        if (currentRound >= 3 && IsKeyPressed(KEY_THREE)) currentMaterial = FIRE;

        // Painting (Left Click)
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && currentState == STATE_PLAYING && !CheckCollisionPointRec(mouse, btnRect)) {
            int gx = (int)mouse.x / CELL_SIZE;
            int gy = (int)mouse.y / CELL_SIZE;
            
            if (gy >= uiRowOffset) { 
                int brushSize = 2;
                for (int i = -brushSize; i <= brushSize; i++) {
                    for (int j = -brushSize; j <= brushSize; j++) {
                        int px = gx + i, py = gy + j;
                        if (px >= 0 && px < GRID_WIDTH && py >= 0 && py < GRID_HEIGHT) {
                            int target = grid[px][py];
                            if (target != SPOUT && target != SPOUT_RED && 
                                target != GOAL && target != GOAL_RED && target != OBSTACLE) {
                                if (currentMaterial == WALL) grid[px][py] = WALL;
                                else if (target != WALL) {
                                    if (GetRandomValue(0, 10) > 2) grid[px][py] = currentMaterial;
                                }
                            }
                        }
                    }
                }
            }
        }
        
        UpdatePhysics(); 
        if (currentState == STATE_PLAYING) CheckWin();

        BeginDrawing();
        ClearBackground(BLACK);
        for (int y = 0; y < GRID_HEIGHT; y++) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                int type = grid[x][y];
                if (type != EMPTY) {
                    Color c = MAGENTA;
                    switch (type) {
                        case WALL: 
                            if (y % 4 == 0 || ((x + ((y/4)%2==0?0:4)) % 8 == 0)) c = COL_MORTAR;
                            else if ((x*y)%7==0) c = (Color){150, 70, 40, 255};
                            else c = COL_BRICK;
                            break;
                        case SAND:  
                            c = ((x+y)%3==0) ? (Color){230, 210, 80, 255} : YELLOW; 
                            break;
                        case PLANT:
                            if ((x + y) % 2 == 0) c = COL_PLANT;
                            else c = (Color){ 0, 100, 0, 255 };
                            break;
                        case ASH:
                            if (GetRandomValue(0, 10) > 8) c = LIGHTGRAY;
                            else c = COL_ASH;
                            break;
                        case OBSTACLE: 
                            if (x % 4 == 0 || y % 4 == 0) c = LIGHTGRAY;
                            else c = COL_OBSTACLE;
                            break;

                        case FIRE:  c = COL_FIRE; break;
                        case WATER: c = COL_WATER; break;
                        case SPOUT: c = COL_SPOUT; break;
                        case WATER_RED: c = COL_WATER_RED; break;
                        case SPOUT_RED: c = COL_SPOUT_RED; break;
                    }
                    DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, c);
                }
            }
        }

        Color mainGoalCol = (level.requiredWater > 0) ? COL_WATER : GREEN;
        DrawRectangleLines(level.goalX * CELL_SIZE, level.goalY * CELL_SIZE, level.goalW * CELL_SIZE, level.goalH * CELL_SIZE, mainGoalCol);
        if (level.hasRedObjective) {
            DrawRectangleLines(level.goalRedX * CELL_SIZE, level.goalRedY * CELL_SIZE, level.goalRedW * CELL_SIZE, level.goalRedH * CELL_SIZE, COL_GOAL_RED);
        }

        if (!isFlowing && currentState == STATE_PLAYING) {
            float floatOffset = sinf(time) * 5.0f;
            
            Vector2 spoutPos = { (float)level.spoutX * CELL_SIZE + CELL_SIZE/2, (float)(uiRowOffset) * CELL_SIZE };
            DrawTriangle(
                (Vector2){ spoutPos.x - 10, spoutPos.y - 20 + floatOffset },
                (Vector2){ spoutPos.x + 10, spoutPos.y - 20 + floatOffset },
                (Vector2){ spoutPos.x,      spoutPos.y - 5 + floatOffset },
                WHITE
            );

            if (level.hasRedObjective) {
                Vector2 redSpoutPos = { (float)level.spoutRedX * CELL_SIZE + CELL_SIZE/2, (float)(uiRowOffset) * CELL_SIZE };
                DrawTriangle(
                    (Vector2){ redSpoutPos.x - 10, redSpoutPos.y - 20 + floatOffset },
                    (Vector2){ redSpoutPos.x + 10, redSpoutPos.y - 20 + floatOffset },
                    (Vector2){ redSpoutPos.x,      redSpoutPos.y - 5 + floatOffset },
                    PINK
                );
            }
        }

        DrawRectangle(0, 0, SCREEN_WIDTH, topBarH, (Color){220, 220, 220, 255});
        DrawText(GetCurrentLevelInfo(), 10, 10, 22, DARKGRAY);
        if (currentRound == 1) {
            DrawText("Round 1: Fill the container with water.  Wall: 1  |  R: Restart  |  S: Skip  |  F5: Save  |  F9: Load", 10, 38, 14, DARKGRAY);
        } else if (currentRound == 2) {
            DrawText("Round 2: Fill the container with the correct color.  Wall: 1  |  Sand: 2  |  R: Restart  |  S: Skip  |  F5: Save  |  F9: Load", 10, 38, 14, DARKGRAY);
        } else if (currentRound >= 3) {
            DrawText("Wall: 1  |  Sand: 2  |  Fire: 3  |  R: Restart  |  S: Skip  |  F5: Save  |  F9: Load", 10, 38, 14, DARKGRAY);
        }

        int spx = level.spoutX * CELL_SIZE + CELL_SIZE/2;
        int spy = (uiRowOffset) * CELL_SIZE + 2 * CELL_SIZE;
        int spSize = 12;
        DrawRectangle(spx - spSize/2, spy - spSize/2, spSize, spSize, COL_WATER);
        DrawRectangleLines(spx - spSize/2, spy - spSize/2, spSize, spSize, BLACK);

        if (level.hasRedObjective) {
            int spx2 = level.spoutRedX * CELL_SIZE + CELL_SIZE/2;
            int spy2 = (uiRowOffset) * CELL_SIZE + 2 * CELL_SIZE;
            DrawRectangle(spx2 - spSize/2, spy2 - spSize/2, spSize, spSize, COL_WATER_RED);
            DrawRectangleLines(spx2 - spSize/2, spy2 - spSize/2, spSize, spSize, BLACK);
        }

        Color btnColor = isFlowing ? (Color){120,200,120,255} : (Color){200,200,200,255};
        const char* btnText = isFlowing ? "ON" : "OFF";
        DrawRectangleRec(btnRect, btnColor);
        DrawRectangleLines(btnRect.x, btnRect.y, btnRect.width, btnRect.height, BLACK);
        DrawText(btnText, btnRect.x + 22, btnRect.y + 6, 18, BLACK);
        if (currentState == STATE_WIN) {
            DrawRectangle(0, SCREEN_HEIGHT/2 - 60, SCREEN_WIDTH, 120, Fade(BLACK, 0.8f));
            DrawText("LEVEL COMPLETE!", SCREEN_WIDTH/2 - 120, SCREEN_HEIGHT/2 - 30, 30, GREEN);
            DrawText("Press SPACE for Next Round", SCREEN_WIDTH/2 - 140, SCREEN_HEIGHT/2 + 20, 20, WHITE);
        } 
        else if (currentState == STATE_GAME_OVER) { 
            DrawRectangle(0, SCREEN_HEIGHT/2 - 60, SCREEN_WIDTH, 120, Fade(MAROON, 0.9f));
            DrawText("CONTAMINATION!", SCREEN_WIDTH/2 - 120, SCREEN_HEIGHT/2 - 30, 30, YELLOW);
            DrawText("Press R to Restart", SCREEN_WIDTH/2 - 120, SCREEN_HEIGHT/2 + 20, 20, WHITE);
        }
        else if (currentState == STATE_ALL_FINISHED) {
            DrawRectangle(0, SCREEN_HEIGHT/2 - 60, SCREEN_WIDTH, 120, Fade(GOLD, 0.9f));
            DrawText("ALL LEVELS CLEARED!", SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 - 30, 30, MAROON);
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}