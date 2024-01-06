#include "cmat.h"

#include <stdio.h>
#include <stdlib.h>

#include "EXPR.tab.h"
#include "hashtab.h"

extern int yylex_destroy();

SymTable* SYMTAB;
Code* CODE;

SymTable* GLOBAL;

int main() {
  /* 1. créer symtab pour les constantes */
  SymTable* global = symtable_new("global");
  /* 2. ajouter cette table des symboles à la table de hachage */
  add_symtable(global);
  GLOBAL = global;

  CODE = code_new();
  int r = yyparse();

  // print
  print_zone_data();
  code_dump(CODE);

  // clean
  delete_table_hachage();
  code_free(CODE);

  yylex_destroy();
  return r;
}
