#ifndef MATRIX_H
#define MATRIX_H

#include "error.h"

typedef struct matrix_ {
  unsigned l;  // nombre de lignes : si une matrice a 1 dimension alors l = 1
               // (on a seulement des colonnes)
  unsigned c;  // nombre de colonnes
  float *data;
} Matrix;

Matrix *create_matrix(unsigned l, unsigned c);

// inutile ?
// void printmat(Matrix *matrix);

void delete_matrix(Matrix *matrix);

void put_value_at(Matrix *m, unsigned ligne, unsigned colonne, float valeur);

void add_colonne(Matrix *m);

void add_ligne(Matrix *m, Matrix *m2);

#endif