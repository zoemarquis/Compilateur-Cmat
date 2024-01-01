#ifndef SYMBTAB_H
#define SYMBTAB_H

#include <stdbool.h>

typedef char name_t[8];

typedef struct matrix_ {
  unsigned l;  // nombre de lignes
               // si une matrice a 1 dimension alors l = 1 (on a seulement des
               // colonnes)
  unsigned c;  // nombre de colonnes
  float *data;
} Matrix;

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

typedef struct _liste_var {
  variable **liste;
  unsigned taille;
} Liste_Variable;

typedef struct _symbol {
  enum { NAME, CONST_INT, CONST_FLOAT, STRING } kind;
  name_t name;
  union {
    variable *var;
    char *string;
    int const_int;
    float const_float;
  };
} Symbol;

typedef struct _symtable {
  unsigned int capacity;
  unsigned int temporary;
  unsigned int size;
  Symbol *symbols;
} SymTable;

SymTable *symtable_new();

Symbol *symtable_const_int(SymTable *t, int v);

Symbol *symtable_const_float(SymTable *t, float v);

Symbol *symtable_get(SymTable *t, const char *s);

Symbol *symtable_put(SymTable *t, const char *id, variable *var);

void symtable_dump(SymTable *t);

Symbol *symtable_string(SymTable *t, const char *string);

void symtable_free(SymTable *t);

variable *creer_variable(name_t name, unsigned type, bool init, valeur val);

variable **creer_var_liste(variable *v1);

variable **ajouter_var_liste(variable **liste, unsigned taille, variable *var);

void delete_var(variable *var);

Matrix *create_matrix(unsigned l, unsigned c);

void printmat(Matrix *matrix);

void delete_matrix(Matrix *matrix);

void put_value_at(Matrix *m, unsigned ligne, unsigned colonne, float valeur);

void add_colonne(Matrix *m);

void add_ligne(Matrix *m, Matrix *m2);

#endif