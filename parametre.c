#include "parametre.h"

#include <stdio.h>
#include <stdlib.h>

Parametres *init_param() {
  Parametres *param = (Parametres *)malloc(sizeof(Parametres));
  param->liste = NULL;
  param->nb = 0;
  return param;
}

Parametres *add_parametre(Parametres *p, Symbol *s) {
  Parametres *param = (Parametres *)malloc(sizeof(Parametres));
  param->liste = (Symbol **)realloc(p->liste, (p->nb + 1) * sizeof(Symbol *));
  param->liste[p->nb] = s;
  param->nb = p->nb + 1;
  free(p);
  return param;
}

Symbol *get_parametre(Parametres p, unsigned indice) {
  if (indice >= p.nb) {
    fprintf(stderr, "error\n");
    exit(0);
  }
  return p.liste[indice];
}