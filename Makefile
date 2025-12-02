CC = gcc
CFLAGS = -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lraylib

SOURCES = main.c logic.c levels.c level_1.c level_2.c level_3.c level_4.c savegame.c
TARGET = sand_game

all:
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGET)