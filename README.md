# SDL Snake

 classic Snake game built in C using [SDL3](https://www.libsdl.org/).

## Requirements

- CMake 3.20+
- SDL3
- C compiler (Clang/GCC)

### macOS setup

```bash
brew install sdl3
```

## Build

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Run

```bash
./snake*
```

## Controls

| Key | Action |
|-----|--------|
| Arrow keys | Move the snake |

## Project structure

```
sdl-snake/
├── CMakeLists.txt
├── main.c / main.h      # Game loop and SDL setup
├── snake.c / snake.h     # Snake logic 
```

## Implementation 

- **Doubly linked list for the snake** — each snake body (`snakeNode`) has `next` and `prev` pointers. Moving the snake is O(1): insert a new head, delete the old tail, rather than shifting an array.
- **SDL3's callback-based app model** — built on `SDL_AppInit` / `SDL_AppEvent` / `SDL_AppIterate` / `SDL_AppQuit` (with `SDL_MAIN_USE_CALLBACKS`) 
- **Time-based movement** — snake speed is determined from framerate using `SDL_GetTicks()` against a fixed move interval, rather than moving once per rendered frame, allowing flexibility.
- **Batched rendering** — the snake body is drawn in a single `SDL_RenderFillRects` call instead of one draw call per segment.
- **Shared collision logic** — one linked-list traversal (`collideWithSnake`) is reused both for self-collision detection and for keeping apple spawns off the snake's body.

