#define SDL_MAIN_USE_CALLBACKS 1

#include "main.h"
#include "snake.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static snakeInfo *snake;

// Converts hex from macro to RGB values
RGB hexToRGB(int hex) {
  RGB rgbColor;
  rgbColor.r = ((hex >> 16) & 0xFF);
  rgbColor.g = ((hex >> 8) & 0xFF);
  rgbColor.b = ((hex) & 0xFF);
  return rgbColor;
} 

// Draws game grid
void drawGrid(SDL_Renderer *renderer) {
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
void drawCell(SDL_Renderer *renderer, int x, int y, int hex) {
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
void drawSnake(SDL_Renderer *renderer, snakeInfo* snake) {
  drawCell(renderer, snake->head->x, snake->head->y, RED_COLOR);
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
  SDL_Delay(100);
}

// Initializer
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  SDL_SetAppMetadata("snake", "0.0", "com.snake");
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer("snake", WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer);
  
  snake = snakeInit(10, 10);
  deb(snake, 10, 11);
  deb(snake, 10, 12);
  deb(snake, 10, 13);
  deb(snake, 10, 14);
  return SDL_APP_CONTINUE;
}
// Events
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }
  if (event->key.key == SDLK_RIGHT) {
    snake->dir = RIGHT;
  }
  if (event->key.key == SDLK_LEFT) {
    snake->dir = LEFT;
  }
  if (event->key.key == SDLK_DOWN) {
    snake->dir = DOWN;
  }
  if (event->key.key == SDLK_UP) {
    snake->dir = UP; 
  }
  return SDL_APP_CONTINUE;
}

// Iterates every frame
SDL_AppResult SDL_AppIterate(void *appstate) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer); 
  drawGrid(renderer);

  drawSnake(renderer, snake);
  moveSnake(snake);

  SDL_RenderPresent(renderer); 
  return SDL_APP_CONTINUE;
}

// Quit app
void SDL_AppQuit(void *appstate, SDL_AppResult result) {

}

/*cmake --build build
./build/snake*/
