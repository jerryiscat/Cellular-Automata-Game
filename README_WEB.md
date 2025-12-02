# Web Build Setup

This game can be compiled to run in web browsers using WebAssembly.


1. **Install Emscripten** (if not already installed):
   ```bash
   git clone https://github.com/emscripten-core/emsdk.git
   cd emsdk
   ./emsdk install latest
   ./emsdk activate latest
   source ./emsdk_env.sh
   ```

2. **Build the game**:
   ```bash
   make -f Makefile.web
   ```

3. **Run locally** (must use a web server):
   ```bash
   python3 -m http.server 8000
   # Then open: http://localhost:8000/game.html
   ```

## After Building

The build process creates:
- `game.html` - Main HTML file (open this in browser)
- `game.js` - JavaScript wrapper code
- `game.wasm` - WebAssembly binary (the compiled game)

All three files need to be uploaded together to a web server for deployment.

