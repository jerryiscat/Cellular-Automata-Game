#include "game.h"

void DrawBlock(int startX, int startY, int w, int h) {
    for (int x = startX; x < startX + w; x++) {
        for (int y = startY; y < startY + h; y++) {
            if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT) {
                grid[x][y] = OBSTACLE; 
            }
        }
    }
}

void CheckWin() {
    int countBlue = 0;
    int countRedInBlue = 0;
    int countOtherInBlue = 0;
    
    for(int x = level.goalX; x < level.goalX + level.goalW; x++) {
        for(int y = level.goalY; y < level.goalY + level.goalH; y++) {
            int cell = grid[x][y];
            if (cell == WATER) countBlue++;
            else if (cell == WATER_RED) countRedInBlue++;
            else if (cell != EMPTY && cell != SPOUT && cell != GOAL && cell != OBSTACLE && cell != WALL) {
                countOtherInBlue++;
            }
        }
    }
    level.currentWater = countBlue;

    bool redWin = true;
    int countBlueInRed = 0;
    int countOtherInRed = 0;

    if (level.hasRedObjective) {
        int countRed = 0;
        for(int x = level.goalRedX; x < level.goalRedX + level.goalRedW; x++) {
            for(int y = level.goalRedY; y < level.goalRedY + level.goalRedH; y++) {
                int cell = grid[x][y];
                if (cell == WATER_RED) countRed++;
                else if (cell == WATER) countBlueInRed++;
                else if (cell != EMPTY && cell != SPOUT_RED && cell != GOAL_RED && cell != OBSTACLE && cell != WALL) {
                    countOtherInRed++;
                }
            }
        }
        level.currentRed = countRed;
        if (level.currentRed < level.requiredRed) redWin = false;
    }

    if (countRedInBlue > 10 || countBlueInRed > 10) {
        currentState = STATE_GAME_OVER;
        return;
    }
    
    if (countOtherInBlue > 5 || countOtherInRed > 5) {
        currentState = STATE_GAME_OVER;
        return;
    }

    if (level.currentWater >= level.requiredWater && redWin) {
        if (currentState != STATE_WIN) currentState = STATE_WIN;
    }
}

void UpdatePhysics() {
    for(int x=0; x<GRID_WIDTH; x++){
        for(int y=0; y<GRID_HEIGHT; y++){
            next_grid[x][y] = grid[x][y];
        }
    }

    if (currentState == STATE_PLAYING && isFlowing) {
        if (grid[level.spoutX][level.spoutY] == SPOUT) {
            for (int i = -1; i <= 1; i++) { 
                if (level.spoutX+i >=0 && level.spoutX+i < GRID_WIDTH && grid[level.spoutX+i][level.spoutY+1] == EMPTY)
                    next_grid[level.spoutX+i][level.spoutY+1] = WATER;
            }
        }
        if (level.hasRedObjective && grid[level.spoutRedX][level.spoutRedY] == SPOUT_RED) {
             for (int i = -1; i <= 1; i++) {
                if (level.spoutRedX+i >=0 && level.spoutRedX+i < GRID_WIDTH && grid[level.spoutRedX+i][level.spoutRedY+1] == EMPTY)
                    next_grid[level.spoutRedX+i][level.spoutRedY+1] = WATER_RED;
             }
        }
    }

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            int type = grid[x][y];
            if (type == EMPTY || type == WALL || type == OBSTACLE || type == SPOUT || type == GOAL || 
                type == SPOUT_RED || type == GOAL_RED) continue;

            if (type == FIRE) {
                int dx[] = {0, 0, 1, -1};
                int dy[] = {1, -1, 0, 0};
                bool burnedSomething = false;

                for(int i=0; i<4; i++) {
                    int nx = x + dx[i];
                    int ny = y + dy[i];
                    if (nx>=0 && nx<GRID_WIDTH && ny>=0 && ny<GRID_HEIGHT) {
                        if (grid[nx][ny] == PLANT) {
                            next_grid[nx][ny] = FIRE;
                            burnedSomething = true;
                        }
                    }
                }

                int fate = GetRandomValue(0, 100);
                if (fate < 15) { 
                    next_grid[x][y] = ASH;
                } 
                else if (fate < 30) {
                    next_grid[x][y] = EMPTY;
                } 
                else {
                    int newY = y - 1;
                    int newX = x + GetRandomValue(-1, 1);
                    if (newY >= 0 && newX >=0 && newX < GRID_WIDTH && grid[newX][newY] == EMPTY) {
                        next_grid[x][y] = EMPTY; 
                        next_grid[newX][newY] = FIRE;
                    }
                }
                continue;
            }

            if (type == ASH) {
                if (y+1 < GRID_HEIGHT) {
                    int belowCell = grid[x][y+1];
                    if ((belowCell == EMPTY || belowCell == PLANT || 
                         belowCell == WATER || belowCell == WATER_RED) && 
                        next_grid[x][y+1] != ASH) {
                        next_grid[x][y] = EMPTY; 
                        next_grid[x][y+1] = ASH;
                    } else {
                        int dir = GetRandomValue(0,1)==0?-1:1;
                        if (y+1 < GRID_HEIGHT && x+dir >=0 && x+dir < GRID_WIDTH) {
                            int diagCell = grid[x+dir][y+1];
                            if ((diagCell == EMPTY || diagCell == PLANT || 
                                 diagCell == WATER || diagCell == WATER_RED) && 
                                next_grid[x+dir][y+1] != ASH) {
                                next_grid[x][y] = EMPTY; 
                                next_grid[x+dir][y+1] = ASH;
                            }
                        }
                    }
                }
                continue;
            }

            if (type == SAND) {
                if (y+1 < GRID_HEIGHT && grid[x][y+1] == EMPTY && next_grid[x][y+1] == EMPTY) {
                    next_grid[x][y] = EMPTY; next_grid[x][y+1] = SAND;
                } else {
                    int dir = GetRandomValue(0,1)==0?-1:1;
                    if (y+1 < GRID_HEIGHT && x+dir >=0 && x+dir < GRID_WIDTH && grid[x+dir][y+1] == EMPTY && next_grid[x+dir][y+1] == EMPTY) {
                        next_grid[x][y] = EMPTY; next_grid[x+dir][y+1] = SAND;
                    }
                }
                continue;
            }

            if (type == WATER || type == WATER_RED) {
                int dx[] = {0, 0, 1, -1};
                int dy[] = {1, -1, 0, 0};
                
                for(int i = 0; i < 4; i++) {
                    int nx = x + dx[i];
                    int ny = y + dy[i];
                    if (nx >= 0 && nx < GRID_WIDTH && ny >= 0 && ny < GRID_HEIGHT) {
                        if (grid[nx][ny] == PLANT) {
                            if (type == WATER_RED) {
                                next_grid[nx][ny] = ASH;
                            } else if (type == WATER) {
                                if (ny - 1 >= 0 && grid[nx][ny - 1] == EMPTY && next_grid[nx][ny - 1] == EMPTY) {
                                    next_grid[nx][ny - 1] = PLANT;
                                }
                                if (ny + 1 < GRID_HEIGHT && grid[nx][ny + 1] == EMPTY && next_grid[nx][ny + 1] == EMPTY) {
                                    next_grid[nx][ny + 1] = PLANT;
                                }
                                if (nx - 1 >= 0 && grid[nx - 1][ny] == EMPTY && next_grid[nx - 1][ny] == EMPTY) {
                                    next_grid[nx - 1][ny] = PLANT;
                                }
                                if (nx + 1 < GRID_WIDTH && grid[nx + 1][ny] == EMPTY && next_grid[nx + 1][ny] == EMPTY) {
                                    next_grid[nx + 1][ny] = PLANT;
                                }
                                
                                int plantDx[] = {0, 0, 1, -1};
                                int plantDy[] = {1, -1, 0, 0};
                                for(int j = 0; j < 4; j++) {
                                    int px = nx + plantDx[j];
                                    int py = ny + plantDy[j];
                                    if (px >= 0 && px < GRID_WIDTH && py >= 0 && py < GRID_HEIGHT && grid[px][py] == PLANT) {
                                        if (py - 1 >= 0 && grid[px][py - 1] == EMPTY && next_grid[px][py - 1] == EMPTY) {
                                            next_grid[px][py - 1] = PLANT;
                                        }
                                        if (py + 1 < GRID_HEIGHT && grid[px][py + 1] == EMPTY && next_grid[px][py + 1] == EMPTY) {
                                            next_grid[px][py + 1] = PLANT;
                                        }
                                        if (px - 1 >= 0 && grid[px - 1][py] == EMPTY && next_grid[px - 1][py] == EMPTY) {
                                            next_grid[px - 1][py] = PLANT;
                                        }
                                        if (px + 1 < GRID_WIDTH && grid[px + 1][py] == EMPTY && next_grid[px + 1][py] == EMPTY) {
                                            next_grid[px + 1][py] = PLANT;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                
                if (y + 1 < GRID_HEIGHT && grid[x][y+1] == PLANT) {
                    if (type == WATER_RED) {
                        next_grid[x][y+1] = ASH;
                    } else if (type == WATER) {
                        if (y >= 0 && grid[x][y] == EMPTY && next_grid[x][y] == EMPTY) {
                            next_grid[x][y] = PLANT;
                        } else if (y - 1 >= 0 && grid[x][y-1] == EMPTY && next_grid[x][y-1] == EMPTY) {
                            next_grid[x][y-1] = PLANT;
                        }
                    }
                } else if (y + 1 < GRID_HEIGHT && grid[x][y+1] == EMPTY && next_grid[x][y+1] == EMPTY) {
                    next_grid[x][y] = EMPTY; next_grid[x][y+1] = type;
                }
                else {
                    int dir = GetRandomValue(0, 1) == 0 ? -1 : 1;
                    if (x + dir >= 0 && x + dir < GRID_WIDTH && grid[x+dir][y] == PLANT) {
                        if (type == WATER_RED) {
                            next_grid[x+dir][y] = ASH;
                        } else if (type == WATER) {
                            if (y - 1 >= 0 && grid[x+dir][y-1] == EMPTY && next_grid[x+dir][y-1] == EMPTY) {
                                next_grid[x+dir][y-1] = PLANT;
                            } else if (x + dir - 1 >= 0 && grid[x+dir-1][y] == EMPTY && next_grid[x+dir-1][y] == EMPTY) {
                                next_grid[x+dir-1][y] = PLANT;
                            } else if (x + dir + 1 < GRID_WIDTH && grid[x+dir+1][y] == EMPTY && next_grid[x+dir+1][y] == EMPTY) {
                                next_grid[x+dir+1][y] = PLANT;
                            }
                        }
                    } else if (x + dir >= 0 && x + dir < GRID_WIDTH && 
                        grid[x+dir][y] == EMPTY && next_grid[x+dir][y] == EMPTY) {
                        next_grid[x][y] = EMPTY; next_grid[x+dir][y] = type;
                    } 
                    else {
                        int otherDir = -dir;
                        if (x + otherDir >= 0 && x + otherDir < GRID_WIDTH) {
                            if (grid[x+otherDir][y] == PLANT) {
                                if (type == WATER_RED) {
                                    next_grid[x+otherDir][y] = ASH;
                                } else if (type == WATER) {
                                    if (y - 1 >= 0 && grid[x+otherDir][y-1] == EMPTY && next_grid[x+otherDir][y-1] == EMPTY) {
                                        next_grid[x+otherDir][y-1] = PLANT;
                                    } else if (x + otherDir - 1 >= 0 && grid[x+otherDir-1][y] == EMPTY && next_grid[x+otherDir-1][y] == EMPTY) {
                                        next_grid[x+otherDir-1][y] = PLANT;
                                    } else if (x + otherDir + 1 < GRID_WIDTH && grid[x+otherDir+1][y] == EMPTY && next_grid[x+otherDir+1][y] == EMPTY) {
                                        next_grid[x+otherDir+1][y] = PLANT;
                                    }
                                }
                            } else if (grid[x+otherDir][y] == EMPTY && next_grid[x+otherDir][y] == EMPTY) {
                                next_grid[x][y] = EMPTY; next_grid[x+otherDir][y] = type;
                            }
                        }
                    }
                }
            }
        }
    }
    memcpy(grid, next_grid, sizeof(int) * GRID_WIDTH * GRID_HEIGHT);
}
