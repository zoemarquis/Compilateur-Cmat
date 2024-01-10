#include "hashtab.h"

#include <assert.h>
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
    fprintf(stderr, "La table des symboles de la fonction %s existe déjà.\n",
            s->nom);
    exit(SEMANTIC_FAILURE);
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

static void print_parametres(Parametres *p) {
  for (unsigned i = 0; i < p->nb; i++) {
    assert(p->liste[i]->kind == NAME);
    assert(p->liste[i]->var->type == INT || p->liste[i]->var->type == FLOAT);
    if (p->liste[i]->var->type == INT) {
      printf("\tINT %s", p->liste[i]->name);
    }
    printf("\n");
  }
  printf("[%d paramètre(s)]\n", p->nb);
}

static void print_tos_parametres(SymTable *st) {
  printf("Paramètre(s) : \n");
  print_parametres(st->param);
}

static void print_ligne() {
  for (int i = 0; i < 50; i++) {
    printf("-");
  }
  printf("\n");
}

static void print_int(Symbol *s) {
  assert(s->kind == CONST_INT || (s->kind == NAME && s->var->type == INT));
  printf("\tINT %s\n", s->name);
}

static void print_float(Symbol *s) {
  assert(s->kind == CONST_FLOAT || (s->kind == NAME && s->var->type == FLOAT));
  printf("\tFLOAT %s\n", s->name);
}

static void print_constantes(SymTable *st) {
  printf("CONSTANTES GLOBALES\n");
  for (unsigned int i = 0; i < st->size; i++) {
    Symbol *s = &(st->symbols[i]);
    if (s->var->type == INT)
      print_int(s);
    else if (s->var->type == FLOAT)
      print_float(s);
  }
}

static bool in_param(Parametres *p, Symbol *s) {
  for (unsigned i = 0; i < p->nb; i++) {
    if (p->liste[i] == s) return true;
  }
  return false;
}

static void print_extract(Symbol *s) {
  Extract e = s->extr;
  printf("\tEXTRACT %s :\n\t\t[indices pour extraction] ", s->name);
  for (unsigned i = 0; i < e.taille; i++) {
    printf("%d,", e.liste[i]);
  }
  printf("\n");
}

static void print_indices(Symbol *s) {
  printf("\tINDICES %s :\n\t\t[indices ligne et colonne] ", s->name);
  printf("%d - %d", s->tuple.ligne, s->tuple.colonne);
  printf("\n");
}

static void print_matrix(Symbol *s) { printf("\tMATRIX %s\n", s->name); }

static void print_name(Symbol *s) {
  switch (s->var->type) {
    case INT:
      print_int(s);
      break;
    case FLOAT:
      print_float(s);
      break;
    case MATRIX:
      print_matrix(s);
      break;
    default:
      // printf("DEFAULT NAME\n");
      break;
  }
}

static void print_symbol(Symbol *s) {
  switch (s->kind) {
    case NAME:
      print_name(s);
      break;

    case CONST_INT:
      print_int(s);
      break;

    case CONST_FLOAT:
      print_float(s);
      break;

    case STRING:
      printf("\tSTRING %s = '%s'\n", s->name, s->string);
      break;

    case EXTRACT:
      print_extract(s);
      break;

    case INDICES:
      print_indices(s);
      break;

    case PARAMETRE:
      // printf("PARAMETRE\n");
      break;

    case FONCTION:
      break;

    default:
      printf("default");
      break;
  }
}

void print_table_hachage() {
  printf("Table des symboles : \n");
  print_constantes(GLOBAL);
  SymTable *current_st, *tmp;
  HASH_ITER(hh, th, current_st, tmp) {
    if (current_st != GLOBAL) {
      printf("Fonction %s :\n", current_st->nom);
      print_tos_parametres(current_st);
      for (unsigned int i = 0; i < current_st->size; i++) {
        Symbol *s = &(current_st->symbols[i]);
        if (!in_param(current_st->param, s)) print_symbol(s);
      }
    }
    print_ligne();
  }
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
