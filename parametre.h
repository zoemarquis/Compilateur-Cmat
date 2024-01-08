#ifndef PARAMETRE_H
#define PARAMETRE_H

#include "symbtab.h"

typedef struct _param {
  struct _symbol **liste;
  unsigned nb;
} Parametres;

Parametres *add_parametre(Parametres *p, struct _symbol *s);

struct _symbol *get_parametre(Parametres p, unsigned indice);

Parametres *init_param();

#endif