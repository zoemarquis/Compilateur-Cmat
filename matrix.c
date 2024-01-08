#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>

Matrix *create_matrix(unsigned l, unsigned c) {
  Matrix *matrix = (Matrix *)malloc(sizeof(Matrix));
  if (matrix == NULL) {  // Échec de l'allocation, gérer l'erreur
    fprintf(stderr, "Allocation mémoire échouée\n");
    exit(MEMORY_FAILURE);
  }
  matrix->l = l;
  matrix->c = c;
  matrix->data = (float *)calloc(l * c, sizeof(float));
  if (matrix->data == NULL) {  // Échec de l'allocation, gérer l'erreur
    fprintf(stderr, "Allocation mémoire échouée\n");
    exit(MEMORY_FAILURE);
  }
  return matrix;
}

void delete_matrix(Matrix *matrix) {
  free(matrix->data);
  free(matrix);
}

void put_value_at(Matrix *m, unsigned ligne, unsigned colonne, float valeur) {
  assert(ligne < m->l && colonne < m->c);
  m->data[ligne * m->c + colonne] = valeur;
}

void add_colonne(Matrix *m) {
  assert(m->l == 1);
  m->c++;
  m->data = (float *)realloc(m->data, m->c * sizeof(float));
  if (m->data == NULL) {  // Échec de l'allocation, gérer l'erreur
    fprintf(stderr, "Allocation mémoire échouée\n");
    exit(MEMORY_FAILURE);
  }
}

void add_ligne(Matrix *m, Matrix *m2) {
  assert(m->c == m2->c);
  assert(m2->l == 1);
  m->l++;
  m->data = (float *)realloc(m->data, m->l * m->c * sizeof(float));
  if (m->data == NULL) {  // Échec de l'allocation, gérer l'erreur
    fprintf(stderr, "Allocation mémoire échouée\n");
    exit(MEMORY_FAILURE);
  }
  for (unsigned i = 0; i < (m->c); i++) {
    m->data[(m->l - 1) * (m->c) + i] = m2->data[i];
  }
  delete_matrix(m2);
}