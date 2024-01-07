#include "hashtab.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "EXPR.tab.h"
#include "cmat.h"

Table_Hachage th = NULL;

SymTable *get_symtable(char *name) {
  SymTable *s;
  HASH_FIND_STR(th, name, s);
  return s;
}

bool add_symtable(SymTable *s) {
  if (!get_symtable(s->nom)) {  // NULL si pas trouvé
    HASH_ADD_STR(th, nom, s);
    return true;
  } else {
    // message d'erreur
  }
  return false;
}

void print_zone_data() {
  printf(".data\n");
  printf("newline: .asciiz \"\\n\"\n");
  printf("tab: .asciiz \"\\t\"\n");

  SymTable *current_element, *tmp;
  HASH_ITER(hh, th, current_element, tmp) {
    for (int i = 0; i < current_element->size; i++) {
      Symbol s = current_element->symbols[i];
      if (s.kind == NAME) {
        switch (s.var->type) {
          case INT:

            /* printf("%s: .word %d\n", s.nom_var_fc,
                   (s.var->init ? s.var->val.entier : 0));
                   */
            if (current_element == GLOBAL) {
              printf("%s: .word %d\n", s.nom_var_fc, s.var->val.entier);
            } else {
              printf("%s: .word 0\n", s.nom_var_fc);
            }
            break;
            // y a un s.var->val.entier pas à zéro
            // d'où ça vient ? de toute façon on va tout init à zéro puis
            // affecter donc la fuite va disparaitre

          case FLOAT:
            /*
              printf("%s: .float %f\n", s.nom_var_fc,
                     (s.var->init ? s.var->val.flottant : 0));
              */
            if (current_element == GLOBAL) {
              printf("%s: .float %f\n", s.nom_var_fc, s.var->val.flottant);
            } else {
              printf("%s: .float 0.0\n", s.nom_var_fc);
            }
            break;

          case MATRIX:
            printf("%s:\n", s.nom_var_fc);
            for (unsigned i = 0; i < s.var->val.matrix->l; i++) {
              printf("\t.float");
              for (unsigned j = 0; j < s.var->val.matrix->c; j++) {
                printf("\t%f",
                       s.var->val.matrix->data[(i * s.var->val.matrix->c) + j]);

                if (j != s.var->val.matrix->c - 1) printf(",");
              }
              printf("\n");
            }
            printf("%s_rows: .word %d\n", s.nom_var_fc, s.var->val.matrix->l);
            printf("%s_cols: .word %d\n", s.nom_var_fc, s.var->val.matrix->c);
            break;

          default:
            break;
        }
      } else if (s.kind == STRING) {
        printf("%s: .asciiz %s\n", s.nom_var_fc, s.string);
      } else if (s.kind == CONST_INT) {
        printf("%s: .word %d\n", s.nom_var_fc, s.const_int);
      } else if (s.kind == CONST_FLOAT) {
        printf("%s: .float %f\n", s.nom_var_fc, s.const_float);
      } else if (s.kind == EXTRACT) {
        printf("%s: .word ", s.nom_var_fc);
        for (unsigned i = 0; i < s.extr.taille; i++) {
          printf("%d ", s.extr.liste[i]);
        }
        printf("\n");
      }
    }
  }
}

void print_table_hachage() {
  printf("Table des symboles\n");
  /*
  for (Token *s = th; s != NULL; s = s->hh.next) {
    printf("%s\n", s->name);
    (s->var->init) ? printf("INITIALISÉ\n") : printf("NON INIT\n");

    if (s->var->init) {
      switch (s->var->type) {
        case INT:
          printf("%d\n", s->var->valeur.entier);
          printf("ENTIER\n");
          break;

        case FLOAT:
          printf("%f\n", s->var->valeur.flottant);
          printf("FLOTTANT\n");
          break;

        case MATRIX:
          printmat(s->var->valeur.matrix);
          printf("MATRIX\n");
          break;

        default:
          printf("default\n");
          printf("%d\n", s->var->type);
          break;
      }
    }
    printf("--------------\n");
  }
  */
}

void delete_table_hachage() {
  SymTable *t, *tmp;
  if (th) {  // si NULL
    HASH_ITER(hh, th, t, tmp) {
      HASH_DEL(th, t);
      symtable_free(t);
    }
  }
  free(th);
}
