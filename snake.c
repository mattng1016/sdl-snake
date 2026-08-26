#include "snake.h"
#include <stdlib.h>

snakeNode* nodeInit(int x, int y) {
  snakeNode* node = (snakeNode*)malloc(sizeof(snakeNode));
  node->x = x;
  node->y = y;
  node->next = NULL;
  node->prev = NULL;
  return node;
}

void insertHead(snakeNode** head, int x, int y) {
  snakeNode* newNode = nodeInit(x, y);
  newNode->prev = *head;
  (*head)->next = newNode;
  *head = newNode;
}

void insertTail(snakeNode** tail, int x, int y) {
  snakeNode* newNode = nodeInit(x, y);
  (*tail)->prev = newNode;
  newNode->next = *tail;
  *tail = newNode;
}

void deleteTail(snakeNode** tail) {
  snakeNode* temp = *tail;
  (*tail)->next->prev = NULL;
  *tail = (*tail)->next;
  free(temp);
}

int getCoordX(snakeNode* node) {
  return node->x;
}

int getCoordY(snakeNode* node) {
  return node->y;
}

void printDebug(snakeNode* head) {
  snakeNode* temp = head;
  while (temp != NULL) {
    printf("%d, %d\n", temp->x, temp->y);
    temp = temp->prev;
  }
}

snakeInfo* snakeInit(int x, int y) {
  snakeInfo* snake = (snakeInfo*)malloc(sizeof(snakeInfo)); 
  snakeNode* head = nodeInit(x, y);
  snake->size = 1;
  snake->dir = RIGHT; 
  snake->head = head; 
  snake->tail = snake->head;
  return snake;
}

void snakeIncrease(snakeInfo* snake) {
  snake->size++;
  insertTail(&(snake->tail), snake->tail->x, snake->tail->y); 
}

void deb(snakeInfo* snake, int x, int y) {
  snake->size++;
  insertTail(&(snake->tail), x, y); 
}
