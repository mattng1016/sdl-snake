#pragma once
#include "main.h"

typedef struct snakeNode {
  int x, y;
  struct snakeNode* next;
  struct snakeNode* prev;
} snakeNode;

typedef struct snakeInfo {
  int size;
  enum snakeDir dir;
  snakeNode* head;
  snakeNode* tail;
} snakeInfo;

snakeNode* nodeInit(int x, int y); 
void insertHead(snakeNode** head, int x, int y);
void insertTail(snakeNode** tail, int x, int y);
void deleteTail(snakeNode** tail);
int getCoordX(snakeNode* node);
int getCoordY(snakeNode* node);
void printDebug(snakeNode* head);
snakeInfo* snakeInit(int x, int y); 
void snakeIncrease(snakeInfo* snake);
void deb(snakeInfo* snake, int x, int y);
