#pragma once
#include <SDL3/SDL.h>

#define WIDTH 900
#define HEIGHT 600
#define CELL_SIZE 20
#define LINE_WIDTH 1

#define RED_COLOR 0xff0000
#define GREEN_COLOR 0x00ff00

typedef enum {UP, DOWN, LEFT, RIGHT} snakeDir ;

typedef struct RGB RGB;
struct RGB {
  int r;
  int g;
  int b;
};

RGB hexToRGB(int hex);
void drawGrid();
void drawCell(int x, int y, int hex);
//void drawSnake(SDL_Renderer *renderer, snakeInfo* snake);
//void moveSnake(snakeInfo* snake);
