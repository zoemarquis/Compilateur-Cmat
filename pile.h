#ifndef PILE_H
#define PILE_H

#define MAX_SIZE 100

typedef struct {
  int *array;
  int top;
  int capacity;
} Stack;

/**
 * @brief Crée une pile
 *
 * @param capacity capacité de la pile
 * @return Stack*
 */
Stack *createStack(int capacity);

/**
 * @brief Empile un entier
 *
 * @param stack la pile
 * @param item l'entier à mettre au sommet
 */
void push(Stack *stack, int item);

/**
 * @brief Dépile un entier
 *
 * @param stack la pile
 * @return int
 */
int pop(Stack *stack);

/**
 * @brief Supprime la pile (nettoie la mémoire)
 *
 * @param stack
 */
void freeStack(Stack *stack);

/**
 * @brief Retourne l'entier au sommet de la pile
 *
 * @param stack la pile
 * @return int
 */
int top(Stack *stack);

#endif