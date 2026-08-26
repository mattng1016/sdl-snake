#pragma once
#include <SDL3/SDL.h>

#define WIDTH 900
#define HEIGHT 600
#define CELL_SIZE 20
#define LINE_WIDTH 1

#define RED_COLOR 0xff0000

enum snakeDir {UP, DOWN, LEFT, RIGHT};

typedef struct RGB RGB;
struct RGB {
  int r;
  int g;
  int b;
};

RGB hexToRGB(int hex);
void drawGrid(SDL_Renderer *renderer);
void drawCell(SDL_Renderer *renderer, int x, int y, int hex);


