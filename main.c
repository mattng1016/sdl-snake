#define SDL_MAIN_USE_CALLBACKS 1

#include "main.h"
#include "snake.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <time.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static bool gameOver = false;
static int appleLocation[2];
static snakeInfo *snake;

const Uint32 moveSpeed = 100;
static Uint32 lastMoveTime = 0;

// Converts hex from macro to RGB values
RGB hexToRGB(int hex) {
  RGB rgbColor;
  rgbColor.r = ((hex >> 16) & 0xFF);
  rgbColor.g = ((hex >> 8) & 0xFF);
  rgbColor.b = ((hex) & 0xFF);
  return rgbColor;
} 

// Draws game grid
void drawGrid() {
  SDL_SetRenderDrawColor(renderer, 105, 105, 105, SDL_ALPHA_OPAQUE);
  for (int x = 0; x <= WIDTH; x += CELL_SIZE) {
    const SDL_FRect rect = {x, 0, LINE_WIDTH, HEIGHT};
    SDL_RenderRect(renderer, &rect);
  }
  for (int y = 0; y <= HEIGHT; y += CELL_SIZE) {
    const SDL_FRect rect = {0, y, WIDTH, LINE_WIDTH};
    SDL_RenderRect(renderer, &rect);
  }
}

// Draws cell based on given coordinates and color
void drawCell(int x, int y, int hex) {
  RGB rgb;
  const SDL_FRect rect = {(x*CELL_SIZE)+1, (y*CELL_SIZE)+1, CELL_SIZE-1, CELL_SIZE-1}; 

  if (x > WIDTH/CELL_SIZE || y > HEIGHT/CELL_SIZE) {
    SDL_Log("Out of bounds drawCell"); 
  }
  rgb = hexToRGB(hex);
  SDL_SetRenderDrawColor(renderer, rgb.r, rgb.g, rgb.b, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(renderer, &rect);
}

// Draws a snake
void drawSnake(snakeInfo* snake) {
  drawCell(snake->head->x, snake->head->y, RED_COLOR);
  snakeNode *temp = snake->head->prev; 
  SDL_FRect *body = (SDL_FRect*)malloc((snake->size)*sizeof(SDL_FRect));
  for (int i = 0; i < snake->size - 1; i++) {
    int x = getCoordX(temp);
    int y = getCoordY(temp);
    body[i].x = (CELL_SIZE*x) + 1;
    body[i].y = (CELL_SIZE*y) + 1;
    body[i].w = body[i].h = CELL_SIZE - 1;
    temp = temp->prev;
  }
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRects(renderer, body, snake->size);
  free(body);
}

// Moves the given snake
void moveSnake(snakeInfo* snake) {
  switch (snake->dir) {
    case UP:
      insertHead(&(snake->head), snake->head->x, (snake->head->y)-1);
      deleteTail(&(snake->tail));
      break;
    case DOWN:
      insertHead(&(snake->head), snake->head->x, (snake->head->y)+1);
      deleteTail(&(snake->tail));
      break;
    case LEFT:
      insertHead(&(snake->head), (snake->head->x)-1, snake->head->y);
      deleteTail(&(snake->tail));
      break;
    case RIGHT:
      insertHead(&(snake->head), (snake->head->x)+1, snake->head->y);
      deleteTail(&(snake->tail));
      break;
  }
}

// Checks if given coordinate collides with snake
bool collideWithSnake(snakeInfo* snake, int x, int y, bool head) {
  snakeNode* temp = snake->head; 
  int s = snake->size;
  if (head == 1) {
    temp = temp->prev;
    s--;
  }
  for (int i = 0; i < s; i++) {
    if (x == getCoordX(temp) && y == getCoordY(temp)) {
      return true;
    } else temp = temp->prev;
  }
  return false;
}

// Checks if snake head collides with body
bool snakeCollideBody(snakeInfo* snake) {
  if (collideWithSnake(snake, getCoordX(snake->head), getCoordY(snake->head), 1)) {
    return true;
  }
  return false;
}

// Checks if snake is out of bounds
bool snakeOutOfBounds(snakeInfo* snake) {
  int x = getCoordX(snake->head);
  int y = getCoordY(snake->head);
  if (x < 0 || x > WIDTH/CELL_SIZE || y < 0 || y > HEIGHT/CELL_SIZE) {
    return true;
  }
    return false; 
}

// Generates a apple in a random location
void generateApple(snakeInfo* snake) {
  int x, y;
  do {
    x = rand() % WIDTH/CELL_SIZE;
    y = rand() % HEIGHT/CELL_SIZE;
   } while (collideWithSnake(snake, x, y, 0)); 
  appleLocation[0] = x;
  appleLocation[1] = y;
}

// Draws apple 
void drawApple() {
  drawCell(appleLocation[0], appleLocation[1], GREEN_COLOR);
}

// Increase snake size when snake eats apple
void snakeEatsApple(snakeInfo* snake) {
  if (appleLocation[0] == getCoordX(snake->head) && appleLocation[1] == getCoordY(snake->head)) {
    snakeIncrease(snake);
    generateApple(snake);
  }
}

// Changes snake direction based on key pressed
void updateSnakeDir (const bool *keystates) {
  if (keystates[SDL_SCANCODE_UP] && snake->dir != DOWN) {
    snake->dir = UP;
  } else if (keystates[SDL_SCANCODE_LEFT] && snake->dir != RIGHT) {
    snake->dir = LEFT;
  } else if (keystates[SDL_SCANCODE_DOWN] && snake->dir != UP){
    snake->dir = DOWN;
  } else if (keystates[SDL_SCANCODE_RIGHT] && snake->dir != LEFT) {
    snake->dir = RIGHT;
  }
}

// Initializer
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  SDL_SetAppMetadata("snake", "0.0", "com.snake");
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer("snake", WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer);
  srand(time(NULL));

  snake = snakeInit((WIDTH/CELL_SIZE)/2, (HEIGHT/CELL_SIZE)/2);
  generateApple(snake);

  return SDL_APP_CONTINUE;
}

// Events
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }
  return SDL_APP_CONTINUE;
}

// Iterates every frame
SDL_AppResult SDL_AppIterate(void *appstate) {
  Uint32 currentTime = SDL_GetTicks();
  const bool* keystates = SDL_GetKeyboardState(NULL);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer); 
  drawGrid();

  updateSnakeDir(keystates);

  drawApple();
  drawSnake(snake);

  if (currentTime - lastMoveTime >= moveSpeed) {
    moveSnake(snake);  
    printHeadCoord(snake->head);
    if (snakeCollideBody(snake) || snakeOutOfBounds(snake)) {
      return SDL_APP_FAILURE;
    }
    lastMoveTime = currentTime;
  }
  
  snakeEatsApple(snake);

  SDL_RenderPresent(renderer); 
  return SDL_APP_CONTINUE;
}

// Quit app
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  free(snake);
}

/*cmake --build build
./build/snake*/
