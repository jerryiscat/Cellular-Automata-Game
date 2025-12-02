#include "savegame.h"
#include <string.h>

bool SaveGame(void) {
    SaveGameData saveData;
    
    saveData.magic = SAVE_MAGIC;
    saveData.version = SAVE_VERSION;
    
    memcpy(saveData.grid, grid, sizeof(int) * GRID_WIDTH * GRID_HEIGHT);
    
    saveData.currentRound = currentRound;
    saveData.currentState = currentState;
    saveData.currentMaterial = currentMaterial;
    saveData.isFlowing = isFlowing;
    
    saveData.level = level;
    
    FILE* file = fopen(SAVE_FILE, "wb");
    if (!file) {
        return false;
    }
    
    size_t written = fwrite(&saveData, sizeof(SaveGameData), 1, file);
    fclose(file);
    
    return (written == 1);
}

bool LoadGame(void) {
    SaveGameData saveData;
    
    FILE* file = fopen(SAVE_FILE, "rb");
    if (!file) {
        return false;
    }
    
    size_t read = fread(&saveData, sizeof(SaveGameData), 1, file);
    fclose(file);
    
    if (read != 1) {
        return false;
    }
    
    if (saveData.magic != SAVE_MAGIC) {
        return false; 
    }
    
    if (saveData.version != SAVE_VERSION) {
        return false;
    }
    
    memcpy(grid, saveData.grid, sizeof(int) * GRID_WIDTH * GRID_HEIGHT);
    
    memset(next_grid, 0, sizeof(int) * GRID_WIDTH * GRID_HEIGHT);
    
    currentRound = saveData.currentRound;
    currentState = saveData.currentState;
    currentMaterial = saveData.currentMaterial;
    isFlowing = saveData.isFlowing;
    
    level = saveData.level;
    
    return true;
}

