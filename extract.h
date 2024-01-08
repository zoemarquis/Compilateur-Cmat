#ifndef EXTRACT_H
#define EXTRACT_H

typedef struct _extract {
  int *liste;
  unsigned int taille;
} Extract;

Extract creer_liste_extract(int valeur);

Extract creer_liste_extract_intervalle(int vmin, int vmax);

Extract concat_extract_liste(Extract e1, Extract e2);

#endif