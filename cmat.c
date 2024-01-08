#include "cmat.h"

#include <stdio.h>
#include <stdlib.h>

#include "EXPR.tab.h"
#include "hashtab.h"

extern int yylex_destroy();

SymTable* SYMTAB;
Code* CODE;
SymTable* GLOBAL;
FILE* OUTPUT;

int main(int argc, char* argv[]) {
  char* version =
      "Zoé Marquis\nCharlotte Kruzic\nSabina Askerova\nKatia Zaitceva\n";
  bool tos = false;
  bool o = false;
  OUTPUT = stdout;
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-version") == 0) {
      printf("%s", version);  // toujours dans le terminal
    } else if (strcmp(argv[i], "-tos") == 0) {
      tos = true;
    } else if (strcmp(argv[i], "-o") == 0) {
      if (i + 1 < argc) {
        o = true;
        OUTPUT = fopen(argv[i + 1], "w");
        if (OUTPUT == NULL) {
          fprintf(stderr, "Impossible d'ouvrir le fichier.\n");
          exit(MEMORY_FAILURE);
        }
        i = i + 1;
      } else {
        fprintf(stderr, "Il manque le nom du fichier output.\n");
        exit(EXIT_FAILURE);
      }
    } /*else if (strcmp(argv[i], "-i") == 0) {
  tos = true;
}*/ else {
      fprintf(stderr, "%s", argv[i]);
      exit(MEMORY_FAILURE);
    }
  }

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

  // if tos -> imprimer maintenant
  // TODO

  // clean
  delete_table_hachage();
  code_free(CODE);

  if (o) {
    fclose(OUTPUT);
  }

  yylex_destroy();
  return r;
}
