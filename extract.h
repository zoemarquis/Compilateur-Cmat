#ifndef EXTRACT_H
#define EXTRACT_H

typedef struct _extract {
  int *liste;
  unsigned int taille;
} Extract;

/**
 * @brief Crée une liste d'indices (extract) en initialisant le premier
 * paramètre avec valeur
 *
 * @param valeur le premier paramètre de la liste d'indice
 * @return Extract
 */
Extract creer_liste_extract(int valeur);

/**
 * @brief Crée une liste d'indices entre deux valeurs
 *
 * @param vmin valeur minimale
 * @param vmax valeur maximale
 * @return Extract
 */
Extract creer_liste_extract_intervalle(int vmin, int vmax);

/**
 * @brief Concatène deux extracts
 *
 * @param e1 le premier extract
 * @param e2 le deuxième extract
 * @return Extract
 */
Extract concat_extract_liste(Extract e1, Extract e2);

#endif