#ifndef SYMBTAB_H
#define SYMBTAB_H

#include <stdbool.h>

#include "error.h"
#include "matrix.h"

typedef char name_t[8];

typedef union _valeur {
  int entier;
  float flottant;
  Matrix *matrix;
} valeur;

typedef struct _var {
  name_t name;
  unsigned type;  // flag
  bool init;      // si la variable a été initialisée
  valeur val;
} variable;

typedef struct _extract {
  int *liste;
  unsigned int taille;
} Extract;

typedef struct _tuple {
  int ligne;
  int colonne;
} Indices;

typedef struct _symbol {
  enum { NAME, CONST_INT, CONST_FLOAT, STRING, EXTRACT, INDICES } kind;
  name_t name;
  union {
    variable *var;
    char *string;
    int const_int;
    float const_float;
    Extract extr;
    Indices tuple;
  };
} Symbol;

typedef struct _symtable {
  unsigned int capacity;
  unsigned int temporary;
  unsigned int size;
  Symbol *symbols;
} SymTable;

typedef struct _tuple_declaration {
  variable *gauche;  // la variable entrain de se faire déclarer
  Symbol *droite;    // résultat de l'expression ou matrix_litt
} Tuple_Declaration;

typedef struct _liste_tuple_decla {
  Tuple_Declaration *liste;
  unsigned taille;
} Liste_Tuple_Declaration;

SymTable *symtable_new();

Symbol *symtable_const_int(SymTable *t, int v);

Symbol *symtable_const_float(SymTable *t, float v);

Symbol *symtable_string(SymTable *t, const char *string);

Symbol *symtable_extract(SymTable *t, Extract extract);

Symbol *symtable_indices(SymTable *t, Indices tuple);

Symbol *symtable_get(SymTable *t, const char *s);

Symbol *symtable_put(SymTable *t, const char *id, variable *var);

void symtable_dump(SymTable *t);

void symtable_free(SymTable *t);

// extraction

Extract creer_liste_extract(int valeur);

Extract creer_liste_extract_intervalle(int vmin, int vmax);

Extract concat_extract_liste(Extract e1, Extract e2);

// variable

variable *creer_variable(name_t name, unsigned type, bool init, valeur val);

Liste_Tuple_Declaration creer_liste_tuple_declaration(Tuple_Declaration t);

Liste_Tuple_Declaration ajouter_tuple(Liste_Tuple_Declaration liste,
                                      Tuple_Declaration t);

/*
variable **creer_var_liste(variable *v1);

variable **ajouter_var_liste(variable **liste, unsigned taille, variable
*var);
*/

void delete_var(variable *var);

#endif