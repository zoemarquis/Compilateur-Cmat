#include "extract.h"

#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

// extraction
Matrix *extraction(Matrix *a, int *lignes, int *colonnes, int n, int m) {
  Matrix *mnew = create_matrix(n, m);
  int k = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      int l = lignes[i];
      int c = colonnes[j];
      mnew->data[k++] = a->data[l * a->c + c];
    }
  }
  return mnew;
}

Extract creer_liste_extract(int valeur) {
  int *v = (int *)malloc(sizeof(int));
  if (v == NULL) {  // Échec de l'allocation, gérer l'erreur
    fprintf(stderr, "Allocation mémoire échouée\n");
    exit(MEMORY_FAILURE);
  }
  *v = valeur;
  return (Extract){v, 1};
}

Extract creer_liste_extract_intervalle(int vmin, int vmax) {
  int *l = (int *)malloc(sizeof(int) * (vmax - vmin + 1));
  if (l == NULL) {  // Échec de l'allocation, gérer l'erreur
    fprintf(stderr, "Allocation mémoire échouée\n");
    exit(MEMORY_FAILURE);
  }
  for (int i = 0; i < (vmax - vmin + 1); i++) {
    l[i] = vmin + i;
  }
  return (Extract){l, vmax - vmin + 1};
}

Extract concat_extract_liste(Extract e1, Extract e2) {
  int *l = (int *)malloc(sizeof(int) * (e1.taille + e2.taille));
  if (l == NULL) {  // Échec de l'allocation, gérer l'erreur
    fprintf(stderr, "Allocation mémoire échouée\n");
    exit(MEMORY_FAILURE);
  }
  for (unsigned i = 0; i < e1.taille; i++) {
    l[i] = e1.liste[i];
  }
  for (unsigned i = 0; i < e2.taille; i++) {
    l[i + e1.taille] = e2.liste[i];
  }
  free(e1.liste);
  free(e2.liste);
  return (Extract){l, e1.taille + e2.taille};
}
