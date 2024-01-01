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

struct symbol {
  enum { NAME, CONST_INT, CONST_FLOAT, STRING } kind;
  name_t name;
  union {
    variable *var;
    char *string;
    int const_int;
    float const_float;
  };
};

struct symtable {
  unsigned int capacity;
  unsigned int temporary;
  unsigned int size;
  struct symbol *symbols;
};

struct symtable *symtable_new();

struct symbol *symtable_const_int(struct symtable *t, int v);

struct symbol *symtable_const_float(struct symtable *t, float v);

struct symbol *symtable_get(struct symtable *t, const char *s);

struct symbol *symtable_put(struct symtable *t, const char *id, variable *var);

void symtable_dump(struct symtable *t);

struct symbol *symtable_string(struct symtable *t, const char *string);

void symtable_free(struct symtable *t);

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