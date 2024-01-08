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
  fprintf(OUTPUT, ".data\n");
  fprintf(OUTPUT, "newline: .asciiz \"\\n\"\n");
  fprintf(OUTPUT, "tab: .asciiz \"\\t\"\n");

  SymTable *current_element, *tmp;
  HASH_ITER(hh, th, current_element, tmp) {
    for (unsigned int i = 0; i < current_element->size; i++) {
      Symbol s = current_element->symbols[i];
      if (s.kind == NAME) {
        switch (s.var->type) {
          case INT:
            if (current_element == GLOBAL) {
              fprintf(OUTPUT, "%s: .word %d\n", s.nom_var_fc,
                      s.var->val.entier);
            } else {
              fprintf(OUTPUT, "%s: .word 0\n", s.nom_var_fc);
            }
            break;
            // y a un s.var->val.entier pas à zéro
            // d'où ça vient ? de toute façon on va tout init à zéro puis
            // affecter donc la fuite va disparaitre

          case FLOAT:
            if (current_element == GLOBAL) {
              fprintf(OUTPUT, "%s: .float %f\n", s.nom_var_fc,
                      s.var->val.flottant);
            } else {
              fprintf(OUTPUT, "%s: .float 0.0\n", s.nom_var_fc);
            }
            break;

          case MATRIX:
            fprintf(OUTPUT, "%s:\n", s.nom_var_fc);
            for (unsigned i = 0; i < s.var->val.matrix->l; i++) {
              fprintf(OUTPUT, "\t.float");
              for (unsigned j = 0; j < s.var->val.matrix->c; j++) {
                fprintf(
                    OUTPUT, "\t%f",
                    s.var->val.matrix->data[(i * s.var->val.matrix->c) + j]);

                if (j != s.var->val.matrix->c - 1) fprintf(OUTPUT, ",");
              }
              fprintf(OUTPUT, "\n");
            }
            fprintf(OUTPUT, "%s_rows: .word %d\n", s.nom_var_fc,
                    s.var->val.matrix->l);
            fprintf(OUTPUT, "%s_cols: .word %d\n", s.nom_var_fc,
                    s.var->val.matrix->c);
            break;

          default:
            break;
        }
      } else if (s.kind == STRING) {
        fprintf(OUTPUT, "%s: .asciiz %s\n", s.nom_var_fc, s.string);
      } else if (s.kind == CONST_INT) {
        fprintf(OUTPUT, "%s: .word %d\n", s.nom_var_fc, s.const_int);
      } else if (s.kind == CONST_FLOAT) {
        fprintf(OUTPUT, "%s: .float %f\n", s.nom_var_fc, s.const_float);
      } else if (s.kind == EXTRACT) {
        fprintf(OUTPUT, "%s: .word ", s.nom_var_fc);
        for (unsigned i = 0; i < s.extr.taille; i++) {
          fprintf(OUTPUT, "%d ", s.extr.liste[i]);
        }
        fprintf(OUTPUT, "\n");
      }
    }
  }
}

void print_table_hachage() {
  fprintf(OUTPUT, "Table des symboles\n");
  /*
  for (Token *s = th; s != NULL; s = s->hh.next) {
    fprintf(OUTPUT,"%s\n", s->name);
    (s->var->init) ? fprintf(OUTPUT,"INITIALISÉ\n") : fprintf(OUTPUT,"NON
  INIT\n");

    if (s->var->init) {
      switch (s->var->type) {
        case INT:
          fprintf(OUTPUT,"%d\n", s->var->valeur.entier);
          fprintf(OUTPUT,"ENTIER\n");
          break;

        case FLOAT:
          fprintf(OUTPUT,"%f\n", s->var->valeur.flottant);
          fprintf(OUTPUT,"FLOTTANT\n");
          break;

        case MATRIX:
          printmat(s->var->valeur.matrix);
          fprintf(OUTPUT,"MATRIX\n");
          break;

        default:
          fprintf(OUTPUT,"default\n");
          fprintf(OUTPUT,"%d\n", s->var->type);
          break;
      }
    }
    fprintf(OUTPUT,"--------------\n");
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
