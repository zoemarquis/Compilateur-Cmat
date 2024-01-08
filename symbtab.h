#ifndef SYMBTAB_H
#define SYMBTAB_H

#include <stdbool.h>

#include "error.h"
#include "extract.h"
#include "matrix.h"
#include "parametre.h"
#include "uthash.h"
#include "variable.h"

typedef char nom_var[16];
typedef char nom_var_fonction[32];
typedef char nom_fonction[16];

typedef struct _tuple {
  int ligne;
  int colonne;
} Indices;

typedef struct _symbol {
  enum {
    NAME,
    CONST_INT,
    CONST_FLOAT,
    STRING,
    EXTRACT,
    INDICES,
    FONCTION,
    PARAMETRE
  } kind;
  nom_var name;
  nom_var_fonction nom_var_fc;
  union {
    struct _var *var;
    char *string;
    int const_int;
    float const_float;
    Extract extr;
    Indices tuple;
    struct _symtable *st;
    struct _param *param;
  };
} Symbol;

typedef struct _symtable {
  unsigned int capacity;
  unsigned int temporary;
  unsigned int size;
  Symbol *symbols;
  struct _param *param;
  nom_fonction nom;
  unsigned type_fonction;
  UT_hash_handle hh;
} SymTable;

SymTable *symtable_new(char *fonction);

Symbol *symtable_const_int(SymTable *t, int v);

Symbol *symtable_const_float(SymTable *t, float v);

Symbol *symtable_string(SymTable *t, char *string);

Symbol *symtable_extract(SymTable *t, Extract extract);

Symbol *symtable_indices(SymTable *t, Indices tuple);

Symbol *symtable_get(SymTable *t, char *s);

Symbol *symtable_put(SymTable *t, char *id, struct _var *var);

void symtable_dump(SymTable *t);

void symtable_free(SymTable *t);

#endif