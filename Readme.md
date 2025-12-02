# Elemental Puzzler (C + raylib)

A puzzle game where you guide water and other elements through obstacles to fill containers. Built with C and raylib.

## Demo

![Game Demo](elemental_puzzler.gif)

## Features

- **4 Levels** with increasing difficulty and unique mechanics
- **Multiple Elements**: Walls, Sand, Fire, Water (blue and red)
- **Plant System**: Blue water grows plants, red water destroys them (creates ash)
- **Physics Simulation**: Real-time particle interactions and gravity
- **Save/Load**: Binary serialization to save your progress
- **Web Build**: Compile to WebAssembly and play in browser (see [README_WEB.md](README_WEB.md))

## Controls

- **Mouse Left Click**: Draw/place elements
- **1 Key**: Wall
- **2 Key**: Sand (Level 2+)
- **3 Key**: Fire (Level 3+)
- **R Key**: Restart current level
- **S Key**: Skip to next level
- **F5**: Save game
- **F9**: Load game
- **Click ON/OFF Button**: Start/stop water flow

## Prerequisites

- A C compiler (e.g. clang or gcc).
- raylib (graphics/input library). On macOS Homebrew is the easiest way to install it.

Install raylib via Homebrew:

```bash
brew update
brew install raylib
```

If you prefer pkg-config integration (recommended when available):

```bash
brew install pkg-config
```

## Build

Build the project using the provided `Makefile` from the project root:

```bash
make
```

## Run

After building with `make`, run the program from the project root:

```bash
./sand_game
```

## Contributing

Feel free to open issues or submit pull requests. 

---
