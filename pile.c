#include "pile.h"

#include <stdio.h>
#include <stdlib.h>

Stack *createStack(int capacity) {
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  if (!stack) {
    fprintf(stderr, "Erreur lors de la création de la pile.\n");
    exit(EXIT_FAILURE);
  }

  stack->array = (int *)malloc(capacity * sizeof(int));
  if (!stack->array) {
    fprintf(stderr, "Erreur lors de l'allocation mémoire pour la pile.\n");
    free(stack);
    exit(EXIT_FAILURE);
  }

  stack->top = -1;
  stack->capacity = capacity;
  return stack;
}

int isEmpty(Stack *stack) { return stack->top == -1; }

int isFull(Stack *stack) { return stack->top == stack->capacity - 1; }

void push(Stack *stack, int item) {
  if (isFull(stack)) {
    fprintf(stderr, "La pile est pleine. Impossible de pousser %d.\n", item);
    return;
  }
  stack->array[++stack->top] = item;
}

int pop(Stack *stack) {
  if (isEmpty(stack)) {
    fprintf(stderr, "La pile est vide. Impossible de faire une dépose.\n");
    exit(EXIT_FAILURE);
  }

  return stack->array[stack->top--];
}

int top(Stack *stack) {
  if (isEmpty(stack)) {
    fprintf(stderr, "La pile est vide. Aucun élément au sommet.\n");
    exit(EXIT_FAILURE);
  }

  return stack->array[stack->top];
}

void freeStack(Stack *stack) {
  free(stack->array);
  free(stack);
}