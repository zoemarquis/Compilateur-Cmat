#include "cmat.h"

#include <stdio.h>
#include <stdlib.h>

#include "EXPR.tab.h"

extern int yylex_destroy();

SymTable* SYMTAB;
Code* CODE;

int main() {
  SYMTAB = symtable_new();
  CODE = code_new();
  int r = yyparse();
  // symtable_dump(SYMTAB);
  // .data : créer une fonction pour ça, pour que ce soit + propre
  printf(".data\n");
  printf("newline: .asciiz \"\\n\"\n");
  printf("tab: .asciiz \"\\t\"\n");
  for (int i = 0; i < SYMTAB->size; i++) {
    Symbol s = SYMTAB->symbols[i];
    if (s.kind == NAME) {
      switch (s.var->type) {
        case INT:
          printf("%s: .word %d\n", s.name,
                 (s.var->init ? s.var->val.entier : 0));
          break;
        case FLOAT:
          printf("%s: .float %f\n", s.name,
                 (s.var->init ? s.var->val.flottant : 0));
          break;
        case MATRIX:
          printf("%s:\n", s.name);
          for (unsigned i = 0; i < s.var->val.matrix->l; i++) {
            printf("\t.float");
            for (unsigned j = 0; j < s.var->val.matrix->c; j++) {
              printf("\t%f",
                     s.var->val.matrix->data[(i * s.var->val.matrix->c) + j]);

              if (j != s.var->val.matrix->c - 1) printf(",");
            }
            printf("\n");
          }
          printf("%s_rows: .word %d\n", s.name, s.var->val.matrix->l);
          printf("%s_cols: .word %d\n", s.name, s.var->val.matrix->c);
          break;
        default:
          break;
      }
    } else if (s.kind == STRING) {
      printf("%s: .asciiz %s\n", s.name, s.string);
    } else if (s.kind == CONST_INT) {
      printf("%s: .word %d\n", s.name, s.const_int);
    } else if (s.kind == CONST_FLOAT) {
      printf("%s: .float %f\n", s.name, s.const_float);
    } else if (s.kind == EXTRACT) {
      printf("%s: .word ", s.name);
      for (unsigned i = 0; i < s.extr.taille; i++) {
        printf("%d ", s.extr.liste[i]);
      }
      printf("\n");
    }
  }
  code_dump(CODE);
  symtable_free(SYMTAB);
  code_free(CODE);

  yylex_destroy();
  return r;
}
