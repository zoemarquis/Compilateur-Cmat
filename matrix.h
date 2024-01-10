#ifndef MATRIX_H
#define MATRIX_H

#include "error.h"

typedef struct matrix_ {
  unsigned l;  // nombre de lignes : si une matrice a 1 dimension alors l = 1
               // (on a seulement des colonnes)
  unsigned c;  // nombre de colonnes
  float *data;
} Matrix;

/**
 * @brief Crée une matrice
 *
 * @param l nombre de ligne
 * @param c nombre de colonne
 * @return Matrix*
 */
Matrix *create_matrix(unsigned l, unsigned c);

/**
 * @brief Supprime la matrice (nettoie la mémoire)
 *
 * @param matrix la matrice à supprimer
 */
void delete_matrix(Matrix *matrix);

/**
 * @brief Place la valeur aux indices correspondant dans la matrice.
 *
 * @param m la matrice
 * @param ligne l'indice de ligne
 * @param colonne l'indice de colonne
 * @param valeur la valeur a y mettre
 */
void put_value_at(Matrix *m, unsigned ligne, unsigned colonne, float valeur);

/**
 * @brief Ajoute une colonne à la matrice.
 *
 * @param m la matrice
 */
void add_colonne(Matrix *m);

/**
 * @brief Ajoute une ligne à une matrice
 *
 * @param m la matrice où on ajoute une ligne
 * @param m2 la ligne à ajouter
 */
void add_ligne(Matrix *m, Matrix *m2);

#endif