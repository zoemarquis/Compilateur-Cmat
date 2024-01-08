#ifndef VARIABLE_H
#define VARIABLE_H

#include <stdbool.h>

#include "matrix.h"
#include "symbtab.h"

typedef char nom_var[16];
typedef char nom_var_fonction[32];

typedef union _valeur {
  int entier;
  float flottant;
  Matrix *matrix;
} valeur;

typedef struct _var {
  nom_var name;
  nom_var_fonction nom_var_fc;
  unsigned type;  // flag
  bool init;      // si la variable a été initialisée
  valeur val;
} variable;

typedef struct _tuple_declaration {
  variable *gauche;        // la variable entrain de se faire déclarer
  struct _symbol *droite;  // résultat de l'expression ou matrix_litt
} Tuple_Declaration;

typedef struct _liste_tuple_decla {
  Tuple_Declaration *liste;
  unsigned taille;
} Liste_Tuple_Declaration;

variable *creer_variable(nom_var name, unsigned type, bool init, valeur val);

Liste_Tuple_Declaration creer_liste_tuple_declaration(Tuple_Declaration t);

Liste_Tuple_Declaration ajouter_tuple(Liste_Tuple_Declaration liste,
                                      Tuple_Declaration t);

void delete_var(variable *var);

#endif