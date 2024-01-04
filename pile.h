#ifndef PILE_H
#define PILE_H

#define MAX_SIZE 100

typedef struct {
  int *array;
  int top;
  int capacity;
} Stack;

Stack *createStack(int capacity);
void push(Stack *stack, int item);
int pop(Stack *stack);
void freeStack(Stack *stack);

int top(Stack *stack);

#endif