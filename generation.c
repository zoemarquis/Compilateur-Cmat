#include "generation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "EXPR.tab.h"
#include "cmat.h"
#include "error.h"
#include "pile.h"

unsigned cpt_label = 0;
unsigned cpt_bloc = 0;
unsigned cpt_ctrl = 0;

unsigned cpt_fonction = 0;

typedef struct {
  nom_fonction liste[100];
  unsigned taille;
} Noms_Fonctions;

Noms_Fonctions noms_fc;

void add_fonction(nom_fonction fc) {
  strncpy(noms_fc.liste[noms_fc.taille], fc, 16);
  // noms_fc->liste[noms_fc.taille][15] = '\0`;
  noms_fc.taille++;
}

static char *get_fonction(unsigned indice) {
  assert(indice < noms_fc.taille);
  return noms_fc.liste[indice];
}

Code *code_new() {
  Code *r = malloc(sizeof(Code));
  if (r == NULL) {  // Échec de l'allocation, gérer l'erreur
    fprintf(stderr, "Allocation mémoire échouée\n");
    exit(MEMORY_FAILURE);
  }
  r->capacity = 1024;
  r->quads = malloc(r->capacity * sizeof(Quad));
  if (r->quads == NULL) {  // Échec de l'allocation, gérer l'erreur
    fprintf(stderr, "Allocation mémoire échouée\n");
    exit(MEMORY_FAILURE);
  }
  r->nextquad = 0;
  return r;
}

static void code_grow(Code *c) {
  c->capacity += 1024;
  c->quads = realloc(c->quads, c->capacity * sizeof(Quad));
  if (c->quads == NULL) {  // Échec de l'allocation, gérer l'erreur
    fprintf(stderr, "Allocation mémoire échouée\n");
    exit(MEMORY_FAILURE);
  }
  if (c->quads == NULL) {
    fprintf(stderr,
            "Erreur lors de la tentative d'expansion de la liste des "
            "quadruplets (taille actuelle = %d)\n",
            c->nextquad);
    exit(MEMORY_FAILURE);
  }
}

void gencode(Code *c, enum quad_kind k, Symbol *s1, Symbol *s2, Symbol *s3) {
  if (c->nextquad == c->capacity) code_grow(c);
  Quad *q = &(c->quads[c->nextquad]);
  q->kind = k;
  q->sym1 = s1;
  q->sym2 = s2;
  q->sym3 = s3;
  ++(c->nextquad);
}

Symbol *newtemp(SymTable *t, unsigned type, valeur val) {
  Symbol *s;
  nom_var name;
  sprintf(name, "t%d", t->temporary);
  variable *v = creer_variable(name, type, true, val);
  s = symtable_put(t, name, v);
  ++(t->temporary);
  return s;
}

static void quad_dump(Stack *pile_bloc, Stack *pile_if, Stack *pile_while,
                      Stack *pile_for, Quad *q) {
  unsigned type1, type2;
  switch (q->kind) {
    case LABEL_FC:
      // besoin de connaitre le nom actuel de la fonction
      fprintf(OUTPUT, "%s: \n", get_fonction(cpt_fonction));
      cpt_fonction++;
      break;

    case DEPILER_ADRESSE:
      fprintf(OUTPUT, "\t# lw $ra, 0($sp)\n");
      fprintf(OUTPUT, "\tadd $sp, $sp, 4\n");
      break;

    case RE_EMPILER_ADRESSE:
      fprintf(OUTPUT, "\taddi $sp, $sp, -4\n");
      fprintf(OUTPUT, "\tsw $ra, 0($sp)\n");
      break;

    case DEPILER:
      fprintf(OUTPUT, "\tlw $t0, 0($sp)\n");
      fprintf(OUTPUT, "\tsw $t0, %s\n", q->sym1->nom_var_fc);
      fprintf(OUTPUT, "\taddi $sp, $sp, 4\n");
      break;

    case EMPILER:
      fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym1->nom_var_fc);
      fprintf(OUTPUT, "\taddi $sp, $sp, -4\n");
      fprintf(OUTPUT, "\tsw $t0, 0($sp)\n");
      break;

    case JAL_FC:
      assert(q->sym1->kind == FONCTION);
      fprintf(OUTPUT, "\taddi $sp, $sp, -4\n");
      fprintf(OUTPUT, "\tsw $ra, 0($sp)\n");

      fprintf(OUTPUT, "\tjal %s\n", q->sym1->st->nom);
      break;

    case JR:
      fprintf(OUTPUT, "\tlw $ra, 0($sp)\n");
      fprintf(OUTPUT, "\tadd $sp, $sp, 4\n");
      fprintf(OUTPUT, "\tjr $ra\n");
      break;

    case RETOUR_FC:
      fprintf(OUTPUT, "\tlw $v0, %s\n", q->sym1->nom_var_fc);
      break;

    case MOVE:
      fprintf(OUTPUT, "\tmove $t0, $v0\n");
      fprintf(OUTPUT, "\tsw $t0, %s\n", q->sym1->nom_var_fc);
      break;

    // structures de controle
    case DEBUT_BLOC:
      fprintf(OUTPUT, "debut_bloc_%d:\n", cpt_bloc);
      push(pile_bloc, cpt_bloc);
      cpt_bloc++;
      break;
    case FIN_BLOC:
      fprintf(OUTPUT, "fin_bloc_%d:\n", pop(pile_bloc));
      break;
    case DEBUT_IF:
      fprintf(OUTPUT, "debut_if_%d:\n", cpt_ctrl);
      fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym1->nom_var_fc);
      fprintf(OUTPUT, "\tbeqz $t0, fin_bloc_%d\n", cpt_bloc);
      push(pile_if, cpt_ctrl);
      cpt_ctrl++;
      break;

    case JUMP_FIN_IF:
      fprintf(OUTPUT, "\tj fin_if_%d\n", top(pile_if));
      break;

    case FIN_IF:
      fprintf(OUTPUT, "fin_if_%d:\n", pop(pile_if));
      break;

    case DEBUT_WHILE:
      fprintf(OUTPUT, "while_%d:\n", cpt_ctrl);
      push(pile_while, cpt_ctrl);
      cpt_ctrl++;
      break;

    case FIN_WHILE:
      fprintf(OUTPUT, "\tbeqz $t0, fin_bloc_%d\n", cpt_bloc);
      break;

    case JUMP_DEBUT_WHILE:
      fprintf(OUTPUT, "j while_%d\n", pop(pile_while));
      break;

    case DEBUT_FOR:
      fprintf(OUTPUT, "for_%d:\n", cpt_ctrl);
      push(pile_for, cpt_ctrl);
      cpt_ctrl++;
      break;

    case FIN_FOR:
      fprintf(OUTPUT, "\tbeqz $t0, fin_bloc_%d\n", cpt_bloc);  // si faux : fin
      fprintf(OUTPUT, "\tj debut_bloc_%d\n", cpt_bloc);  // si vrai : dans bloc
      fprintf(OUTPUT, "for_maj_%d:\n", cpt_bloc);
      break;

    case JUMP_DEBUT_FOR:
      fprintf(OUTPUT, "j for_%d\n", pop(pile_for));
      break;

    case JUMP_MAJ_FOR:
      fprintf(OUTPUT, "j for_maj_%d\n", top(pile_bloc));
      break;

    case COPY:

      // int = int
      if (q->sym1->var->type == INT) {
        if (q->sym2->kind == CONST_INT ||
            (q->sym2->kind == NAME && q->sym2->var->type == INT))
          fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym2->nom_var_fc);
        else {
          fprintf(stderr, "Incompatibilité de types à l'affectation de %s.\n",
                  q->sym1->nom_var_fc);
          exit(SEMANTIC_FAILURE);
        }
        fprintf(OUTPUT, "\tsw $t0,%s\n", q->sym1->nom_var_fc);
      }

      if (q->sym1->var->type == FLOAT) {
        // float = float
        if ((q->sym2->kind == CONST_FLOAT) ||
            (q->sym2->kind == NAME && q->sym2->var->type == FLOAT))
          fprintf(OUTPUT, "\tl.s $f0, %s\n", q->sym2->nom_var_fc);
        // float = int
        else if (q->sym2->kind == CONST_INT ||
                 (q->sym2->kind == NAME && q->sym2->var->type == INT)) {
          fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym2->nom_var_fc);
          fprintf(OUTPUT, "\tmtc1 $t0, $f0\n");
          fprintf(OUTPUT, "\tcvt.s.w $f0, $f0\n");
        } else {
          fprintf(stderr, "Incompatibilité de types à l'affectation de %s.\n",
                  q->sym1->nom_var_fc);
          exit(SEMANTIC_FAILURE);
        }
        fprintf(OUTPUT, "\ts.s $f0, %s\n", q->sym1->nom_var_fc);
      }

      // matrix = matrix
      if (q->sym1->var->type == MATRIX) {
        if (q->sym2->kind == NAME && q->sym2->var->type == MATRIX) {
          fprintf(OUTPUT, "\tla $a0, %s\n", q->sym2->nom_var_fc);
          fprintf(OUTPUT, "\tla $a1, %s\n", q->sym1->nom_var_fc);
          fprintf(OUTPUT, "\tlw $t2, %s_rows\n", q->sym1->nom_var_fc);
          fprintf(OUTPUT, "\tlw $t3, %s_cols\n", q->sym1->nom_var_fc);

          fprintf(OUTPUT, "\tli $t0, 0\n");
          fprintf(OUTPUT, "boucle_copy_matrices_ligne_%d:\n", cpt_label);
          fprintf(OUTPUT, "\tbge $t0, $t2, fin_copy_%d\n", cpt_label);

          fprintf(OUTPUT, "\tli $t1, 0\n");
          fprintf(OUTPUT, "\tboucle_copy_matrices_colonne_%d:\n", cpt_label);
          fprintf(OUTPUT, "\t\tbge $t1, $t3, ligne_suivante_%d\n", cpt_label);

          fprintf(OUTPUT, "\t\tl.s $f0, 0($a0)\n");
          fprintf(OUTPUT, "\t\ts.s $f0, 0($a1)\n");

          fprintf(OUTPUT, "\t\taddi $a0, $a0, 4\n");
          fprintf(OUTPUT, "\t\taddi $a1, $a1, 4\n");
          fprintf(OUTPUT, "\t\taddi $t1, $t1, 1\n");

          fprintf(OUTPUT, "\t\tj boucle_copy_matrices_colonne_%d\n", cpt_label);
          fprintf(OUTPUT, "\tligne_suivante_%d:\n", cpt_label);
          fprintf(OUTPUT, "\t\taddi $t0, $t0, 1\n");
          fprintf(OUTPUT, "\t\tj boucle_copy_matrices_ligne_%d\n", cpt_label);

          fprintf(OUTPUT, "fin_copy_%d:\n", cpt_label);
          cpt_label++;
        } else {
          fprintf(stderr, "Incompatibilité de types à l'affectation de %s.\n",
                  q->sym1->nom_var_fc);
          exit(SEMANTIC_FAILURE);
        }
      }

      break;

    // OPÉRATIONS
    case BOP_PLUS:
      if ((q->sym2->kind == NAME && q->sym2->var->type == MATRIX) &&
          (q->sym3->kind == NAME &&
           q->sym3->var->type == MATRIX)) {  // add 2 matrix
        fprintf(OUTPUT, "\tla $a0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tla $a1, %s\n", q->sym3->nom_var_fc);
        fprintf(OUTPUT, "\tla $a2, %s\n", q->sym1->nom_var_fc);

        fprintf(OUTPUT, "\tlw $t2, %s_rows\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tlw $t3, %s_cols\n", q->sym2->nom_var_fc);

        fprintf(OUTPUT, "\tli $t0, 0\n");
        fprintf(OUTPUT, "boucle_add_matrices_ligne_%d:\n", cpt_label);
        fprintf(OUTPUT, "\tbge $t0, $t2, fin_add_%d\n", cpt_label);

        fprintf(OUTPUT, "\tli $t1, 0\n");
        fprintf(OUTPUT, "\tboucle_add_matrices_colonne_%d:\n", cpt_label);
        fprintf(OUTPUT, "\t\tbge $t1, $t3, ligne_suivante_add_%d\n", cpt_label);

        fprintf(OUTPUT, "\t\tl.s $f0, 0($a0)\n");
        fprintf(OUTPUT, "\t\tl.s $f1, 0($a1)\n");
        fprintf(OUTPUT, "\t\tadd.s $f2, $f0, $f1\n");
        fprintf(OUTPUT, "\t\ts.s $f2, 0($a2)\n");

        fprintf(OUTPUT, "\t\taddi $a0, $a0, 4\n");
        fprintf(OUTPUT, "\t\taddi $a1, $a1, 4\n");
        fprintf(OUTPUT, "\t\taddi $a2, $a2, 4\n");
        fprintf(OUTPUT, "\t\taddi $t1, $t1, 1\n");

        fprintf(OUTPUT, "\t\tj boucle_add_matrices_colonne_%d\n", cpt_label);
        fprintf(OUTPUT, "\tligne_suivante_add_%d:\n", cpt_label);
        fprintf(OUTPUT, "\t\taddi $t0, $t0, 1\n");
        fprintf(OUTPUT, "\t\tj boucle_add_matrices_ligne_%d\n", cpt_label);

        fprintf(OUTPUT, "fin_add_%d:\n", cpt_label);

        cpt_label++;
      } else if ((q->sym2->kind == NAME && q->sym2->var->type == MATRIX) ||
                 (q->sym3->kind == NAME &&
                  q->sym3->var->type == MATRIX)) {  // mat + cst ou cst + mat
        nom_var_fonction mat;
        nom_var_fonction cst;
        unsigned type = 0;

        if (q->sym2->kind == NAME && q->sym2->var->type == MATRIX) {
          strcpy(mat, q->sym2->nom_var_fc);
          strcpy(cst, q->sym3->nom_var_fc);
          if (q->sym3->kind == NAME) {
            type = q->sym3->var->type;
          } else {
            if (q->sym3->kind == CONST_INT)
              type = INT;
            else if (q->sym3->kind == CONST_FLOAT)
              type = FLOAT;
            else {  // cas impossible
              fprintf(stderr, "Incompatibilité de types dans une addition.\n");
              exit(SEMANTIC_FAILURE);
            }
          }
        } else {
          strcpy(mat, q->sym3->nom_var_fc);
          strcpy(cst, q->sym2->nom_var_fc);
          if (q->sym2->kind == NAME) {
            type = q->sym2->var->type;
          } else {
            if (q->sym2->kind == CONST_INT)
              type = INT;
            else if (q->sym2->kind == CONST_FLOAT)
              type = FLOAT;
            else {  // cas impossible
              fprintf(stderr, "Incompatibilité de types dans une addition.\n");
              exit(SEMANTIC_FAILURE);
            }
          }
        }

        fprintf(OUTPUT, "\tla $a0, %s\n", mat);
        fprintf(OUTPUT, "\tlwc1 $f1, %s\n", cst);
        if (type == INT) {  // conversion entier vers flottant
          fprintf(OUTPUT, "\tcvt.s.w $f1, $f1\n");
        }
        fprintf(OUTPUT, "\tla $a1, %s\n", q->sym1->var->nom_var_fc);
        fprintf(OUTPUT, "\tlw $t2, %s_rows\n", mat);
        fprintf(OUTPUT, "\tlw $t3, %s_cols\n", mat);

        fprintf(OUTPUT, "\tli $t4, 0\n");
        fprintf(OUTPUT, "boucle_add_matrice_cst_ligne_%d:\n", cpt_label);
        fprintf(OUTPUT, "\tbge $t4, $t2, fin_add_%d\n", cpt_label);

        fprintf(OUTPUT, "\tli $t5, 0\n");
        fprintf(OUTPUT, "\tboucle_add_matrice_cst_colonne_%d:\n", cpt_label);
        fprintf(OUTPUT, "\t\tbge $t5, $t3, ligne_suivante_add_%d\n", cpt_label);

        fprintf(OUTPUT, "\t\tlwc1 $f2, 0($a0)\n");
        fprintf(OUTPUT, "\t\tadd.s $f3, $f2, $f1\n");

        fprintf(OUTPUT, "\t\tswc1 $f3, 0($a1)\n");

        fprintf(OUTPUT, "\t\taddi $a0, $a0, 4\n");
        fprintf(OUTPUT, "\t\taddi $a1, $a1, 4\n");
        fprintf(OUTPUT, "\t\taddi $t5, $t5, 1\n");

        fprintf(OUTPUT, "\t\tj boucle_add_matrice_cst_colonne_%d\n", cpt_label);
        fprintf(OUTPUT, "\tligne_suivante_add_%d:\n", cpt_label);
        fprintf(OUTPUT, "\t\taddi $t4, $t4, 1\n");
        fprintf(OUTPUT, "\t\tj boucle_add_matrice_cst_ligne_%d\n", cpt_label);

        fprintf(OUTPUT, "fin_add_%d:\n", cpt_label);
        cpt_label++;
      } else {  // add de int et de float
        // sym2
        if (q->sym2->kind == CONST_INT ||
            (q->sym2->kind == NAME && q->sym2->var->type == INT)) {
          fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym2->nom_var_fc);
          type1 = INT;
        }  // int
        else if ((q->sym2->kind == CONST_FLOAT) ||
                 (q->sym2->kind == NAME && q->sym2->var->type == FLOAT)) {
          fprintf(OUTPUT, "\tl.s $f0, %s\n", q->sym2->nom_var_fc);
          type1 = FLOAT;
        }  // float

        // sym3
        if (q->sym3->kind == CONST_INT ||
            (q->sym3->kind == NAME && q->sym3->var->type == INT)) {
          fprintf(OUTPUT, "\tlw $t1, %s\n", q->sym3->nom_var_fc);
          type2 = INT;
        }  // int
        else if ((q->sym3->kind == CONST_FLOAT) ||
                 (q->sym3->kind == NAME && q->sym3->var->type == FLOAT)) {
          fprintf(OUTPUT, "\tl.s $f1, %s\n", q->sym3->nom_var_fc);
          type2 = FLOAT;
        }  // float

        if (type1 == INT && type2 == INT) {  // add int
          fprintf(OUTPUT, "\tadd $t0,$t0,$t1\n");
          fprintf(OUTPUT, "\tsw $t0, %s\n", q->sym1->nom_var_fc);
        } else {
          if (type1 == INT && type2 == FLOAT) {
            fprintf(OUTPUT, "mtc1 $t0, $f0\n");
            fprintf(OUTPUT, "cvt.s.w $f0, $f0\n");
          } else if (type1 == FLOAT && type2 == INT) {
            fprintf(OUTPUT, "mtc1 $t1, $f1\n");
            fprintf(OUTPUT, "cvt.s.w $f1, $f1\n");
          }  // cast int -> float
          fprintf(OUTPUT, "\tadd.s $f2,$f0,$f1\n");
          fprintf(OUTPUT, "\ts.s $f2, %s\n", q->sym1->nom_var_fc);  // add float
        }
      }
      break;

    case BOP_MINUS:
      if ((q->sym2->kind == NAME && q->sym2->var->type == MATRIX) &&
          (q->sym3->kind == NAME && q->sym3->var->type == MATRIX)) {
        fprintf(OUTPUT, "\tla $a0, %s\n", q->sym2->nom_var_fc);  // Matrix 1
        fprintf(OUTPUT, "\tla $a1, %s\n", q->sym3->nom_var_fc);  // Matrix 2
        fprintf(OUTPUT, "\tla $a2, %s\n",
                q->sym1->nom_var_fc);  // Matrix résultat

        fprintf(OUTPUT, "\tlw $t2, %s_rows\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tlw $t3, %s_cols\n", q->sym2->nom_var_fc);

        fprintf(OUTPUT, "\tli $t0, 0\n");
        fprintf(OUTPUT, "boucle_sub_matrices_ligne_%d:\n", cpt_label);
        fprintf(OUTPUT, "\tbge $t0, $t2, fin_sub_%d\n", cpt_label);

        fprintf(OUTPUT, "\tli $t1, 0\n");
        fprintf(OUTPUT, "\tboucle_sub_matrices_colonne_%d:\n", cpt_label);
        fprintf(OUTPUT, "\t\tbge $t1, $t3, ligne_suivante_sub_%d\n", cpt_label);

        fprintf(OUTPUT, "\t\tl.s $f0, 0($a0)\n");  // Chargement de la valeur de
                                                   // la matrice 1 en [i][j]
        fprintf(OUTPUT, "\t\tl.s $f1, 0($a1)\n");  // Chargement de la valeur de
                                                   // la matrice 2 en [i][j]
        fprintf(OUTPUT, "\t\tsub.s $f2, $f0, $f1\n");  // Soustraction des 2
                                                       // valeurs chargées
        fprintf(OUTPUT, "\t\ts.s $f2, 0($a2)\n");  // Stockage du résultat dans
                                                   // la matrice 3 en [i][j]

        fprintf(OUTPUT, "\t\taddi $a0, $a0, 4\n");
        fprintf(OUTPUT, "\t\taddi $a1, $a1, 4\n");
        fprintf(OUTPUT, "\t\taddi $a2, $a2, 4\n");
        fprintf(OUTPUT, "\t\taddi $t1, $t1, 1\n");

        fprintf(OUTPUT, "\t\tj boucle_sub_matrices_colonne_%d\n", cpt_label);
        fprintf(OUTPUT, "\tligne_suivante_sub_%d:\n", cpt_label);
        fprintf(OUTPUT, "\t\taddi $t0, $t0, 1\n");
        fprintf(OUTPUT, "\t\tj boucle_sub_matrices_ligne_%d\n", cpt_label);

        fprintf(OUTPUT, "fin_sub_%d:\n", cpt_label);

        cpt_label++;
      }
      // Soustraction d'une matrice et une constante(float ou integer)
      else if ((q->sym2->kind == NAME && q->sym2->var->type == MATRIX) ||
               (q->sym3->kind == NAME && q->sym3->var->type == MATRIX)) {
        nom_var_fonction mat;
        nom_var_fonction cst;
        unsigned type = 0;

        // Cas : matrice - constante
        if (q->sym2->kind == NAME && q->sym2->var->type == MATRIX) {
          strcpy(mat, q->sym2->nom_var_fc);
          strcpy(cst, q->sym3->nom_var_fc);
          if (q->sym3->kind == NAME) {
            type = q->sym3->var->type;
          } else {
            if (q->sym3->kind == CONST_INT)
              type = INT;
            else if (q->sym3->kind == CONST_FLOAT)
              type = FLOAT;
            else {
              fprintf(stderr,
                      "Incompatibilité de types dans une soustraction.\n");
              exit(SEMANTIC_FAILURE);
            }
          }

        } else {
          // Cas : constante - matrice
          strcpy(mat, q->sym3->nom_var_fc);
          strcpy(cst, q->sym2->nom_var_fc);
          if (q->sym2->kind == NAME) {
            type = q->sym2->var->type;
          } else {
            if (q->sym2->kind == CONST_INT)
              type = INT;
            else if (q->sym2->kind == CONST_FLOAT)
              type = FLOAT;
            else {
              fprintf(stderr,
                      "Incompatibilité de types dans une soustraction.\n");
              exit(SEMANTIC_FAILURE);
            }
          }
        }

        fprintf(OUTPUT, "\tla $a0, %s\n", mat);    // Chargement matrice
        fprintf(OUTPUT, "\tlwc1 $f1, %s\n", cst);  // Chargement constante

        // Conversion entier vers flottant
        if (type == INT) {
          fprintf(OUTPUT, "\tcvt.s.w $f1, $f1\n");
        }

        fprintf(OUTPUT, "\tla $a1, %s\n",
                q->sym1->var->nom_var_fc);  // Matrice résultat
        fprintf(OUTPUT, "\tlw $t2, %s_rows\n", mat);
        fprintf(OUTPUT, "\tlw $t3, %s_cols\n", mat);

        fprintf(OUTPUT, "\tli $t4, 0\n");
        fprintf(OUTPUT, "boucle_sub_matrice_cst_ligne_%d:\n", cpt_label);
        fprintf(OUTPUT, "\tbge $t4, $t2, fin_sub_%d\n", cpt_label);

        fprintf(OUTPUT, "\tli $t5, 0\n");
        fprintf(OUTPUT, "\tboucle_sub_matrice_cst_colonne_%d:\n", cpt_label);
        fprintf(OUTPUT, "\t\tbge $t5, $t3, ligne_suivante_sub_%d\n", cpt_label);

        // Soustraction
        fprintf(OUTPUT, "\t\tlwc1 $f2, 0($a0)\n");
        if (q->sym2->kind == NAME && q->sym2->var->type == MATRIX) {
          // Matrice - constante
          fprintf(OUTPUT, "\t\tsub.s $f3, $f2, $f1\n");
        } else {
          // Constante - matrice
          fprintf(OUTPUT, "\t\tsub.s $f3, $f1, $f2\n");
        }

        fprintf(OUTPUT, "\t\tswc1 $f3, 0($a1)\n");  // Stockage du résultat

        fprintf(OUTPUT, "\t\taddi $a0, $a0, 4\n");
        fprintf(OUTPUT, "\t\taddi $a1, $a1, 4\n");
        fprintf(OUTPUT, "\t\taddi $t5, $t5, 1\n");

        // Prochain élément ligne
        fprintf(OUTPUT, "\t\tj boucle_sub_matrice_cst_colonne_%d\n", cpt_label);
        fprintf(OUTPUT, "\tligne_suivante_sub_%d:\n", cpt_label);

        // Prochaine ligne
        fprintf(OUTPUT, "\t\taddi $t4, $t4, 1\n");
        fprintf(OUTPUT, "\t\tj boucle_sub_matrice_cst_ligne_%d\n", cpt_label);

        // Fin
        fprintf(OUTPUT, "fin_sub_%d:\n", cpt_label);
        cpt_label++;
      }
      // Cas : constante - constante
      else {
        // Operateur 1
        // Si l'operateur 1 est un entier
        if (q->sym2->kind == CONST_INT ||
            (q->sym2->kind == NAME && q->sym2->var->type == INT)) {
          fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym2->nom_var_fc);
          type1 = INT;
        }
        // Si l'operateur 1 est un flottant
        else if ((q->sym2->kind == CONST_FLOAT) ||
                 (q->sym2->kind == NAME && q->sym2->var->type == FLOAT)) {
          fprintf(OUTPUT, "\tl.s $f0, %s\n", q->sym2->nom_var_fc);
          type1 = FLOAT;
        }

        // Operateur 2
        // Si l'operateur 2 est un entier
        if (q->sym3->kind == CONST_INT ||
            (q->sym3->kind == NAME && q->sym3->var->type == INT)) {
          fprintf(OUTPUT, "\tlw $t1, %s\n", q->sym3->nom_var_fc);
          type2 = INT;
        }
        // Si l'operateur 2 est un flottant
        else if ((q->sym3->kind == CONST_FLOAT) ||
                 (q->sym3->kind == NAME && q->sym3->var->type == FLOAT)) {
          fprintf(OUTPUT, "\tl.s $f1, %s\n", q->sym3->nom_var_fc);
          type2 = FLOAT;
        }

        // Soustraction de 2 entiers
        if (type1 == INT && type2 == INT) {
          fprintf(OUTPUT, "\tsub $t0,$t0,$t1\n");
          fprintf(OUTPUT, "\tsw $t0, %s\n", q->sym1->nom_var_fc);
        } else {
          // Conversion d'un entier en flottant si necessaire
          if (type1 == INT && type2 == FLOAT) {
            fprintf(OUTPUT, "mtc1 $t0, $f0\n");
            fprintf(OUTPUT, "cvt.s.w $f0, $f0\n");
          } else if (type1 == FLOAT && type2 == INT) {
            fprintf(OUTPUT, "mtc1 $t1, $f1\n");
            fprintf(OUTPUT, "cvt.s.w $f1, $f1\n");
          }

          // Soustraction
          fprintf(OUTPUT, "\tsub.s $f2,$f0,$f1\n");
          fprintf(OUTPUT, "\ts.s $f2, %s\n", q->sym1->nom_var_fc);
        }
      }
      break;

    case BOP_MULT:
      if ((q->sym2->kind == NAME && q->sym2->var->type == MATRIX) &&
          (q->sym3->kind == NAME && q->sym3->var->type == MATRIX)) {
        fprintf(OUTPUT, "\tla $a0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tla $a1, %s\n", q->sym3->nom_var_fc);
        fprintf(OUTPUT, "\tla $a2, %s\n", q->sym1->nom_var_fc);
        fprintf(OUTPUT, "\tlw $t0, %s_rows\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tlw $t1, %s_cols\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tlw $t2, %s_cols\n", q->sym3->nom_var_fc);

        fprintf(OUTPUT, "\tli $t3, 0\n");
        fprintf(OUTPUT, "boucle_i_%d:\n", cpt_label);
        fprintf(OUTPUT, "\tli $t4, 0\n");

        fprintf(OUTPUT, "\tboucle_j_%d:\n", cpt_label);
        fprintf(OUTPUT, "\t\tli $t5, 0\n");
        fprintf(OUTPUT, "\t\tmtc1 $zero, $f10\n");

        fprintf(OUTPUT, "\t\tboucle_k_%d:\n", cpt_label);
        fprintf(OUTPUT, "\t\t\tmul $t6, $t3, $t1\n");
        fprintf(OUTPUT, "\t\t\tadd $t6, $t6, $t5\n");

        fprintf(OUTPUT, "\t\t\tmul $t7, $t5, $t2\n");
        fprintf(OUTPUT, "\t\t\tadd $t7, $t7, $t4\n");

        fprintf(OUTPUT, "\t\t\tmul $t6, $t6, 4\n");
        fprintf(OUTPUT, "\t\t\tadd $t6, $t6, $a0\n");
        fprintf(OUTPUT, "\t\t\tmul $t7, $t7, 4\n");
        fprintf(OUTPUT, "\t\t\tadd $t7, $t7, $a1\n");

        fprintf(OUTPUT, "\t\t\tl.s $f0, 0($t6)\n");
        fprintf(OUTPUT, "\t\t\tl.s $f1, 0($t7)\n");
        fprintf(OUTPUT, "\t\t\tmul.s $f11, $f0, $f1\n");
        fprintf(OUTPUT, "\t\t\tadd.s $f10, $f10, $f11\n");

        fprintf(OUTPUT, "\t\t\taddi $t5, $t5, 1\n");
        fprintf(OUTPUT, "\t\t\tbne $t5, $t1, boucle_k_%d\n", cpt_label);

        fprintf(OUTPUT, "\t\tmul $t8, $t3, $t2\n");
        fprintf(OUTPUT, "\t\tadd $t8, $t8, $t4\n");

        fprintf(OUTPUT, "\t\tmul $t8, $t8, 4\n");
        fprintf(OUTPUT, "\t\tadd $t8, $t8, $a2\n");

        fprintf(OUTPUT, "\t\ts.s $f10, 0($t8)\n");

        fprintf(OUTPUT, "\t\taddi $t4, $t4, 1\n");
        fprintf(OUTPUT, "\t\tbne $t4, $t2, boucle_j_%d\n", cpt_label);
        fprintf(OUTPUT, "\t\taddi $t3, $t3, 1\n");
        fprintf(OUTPUT, "\t\tbne $t3, $t0, boucle_i_%d\n", cpt_label);

        cpt_label++;
      }
      // Multiplication d'une matrice et une constante(float ou integer)
      else if ((q->sym2->kind == NAME && q->sym2->var->type == MATRIX) ||
               (q->sym3->kind == NAME && q->sym3->var->type == MATRIX)) {
        nom_var_fonction mat;
        nom_var_fonction cst;
        unsigned type = 0;

        // Cas : matrice * constante
        if (q->sym2->kind == NAME && q->sym2->var->type == MATRIX) {
          strcpy(mat, q->sym2->nom_var_fc);
          strcpy(cst, q->sym3->nom_var_fc);
          if (q->sym3->kind == NAME) {
            type = q->sym3->var->type;
          } else {
            if (q->sym3->kind == CONST_INT)
              type = INT;
            else if (q->sym3->kind == CONST_FLOAT)
              type = FLOAT;
            else {
              fprintf(stderr,
                      "Incompatibilité de types dans une multiplication.\n");
              exit(SEMANTIC_FAILURE);
            }
          }
        } else {
          strcpy(mat, q->sym3->nom_var_fc);
          strcpy(cst, q->sym2->nom_var_fc);
          if (q->sym2->kind == NAME) {
            type = q->sym2->var->type;
          } else {
            if (q->sym2->kind == CONST_INT)
              type = INT;
            else if (q->sym2->kind == CONST_FLOAT)
              type = FLOAT;
            else {
              fprintf(stderr,
                      "Incompatibilité de types dans une multiplication.\n");
              exit(SEMANTIC_FAILURE);
            }
          }
        }
        fprintf(OUTPUT, "\tla $a0, %s\n", mat);    // Chargement matrice
        fprintf(OUTPUT, "\tlwc1 $f1, %s\n", cst);  // Chargement constante

        // Conversion entier vers flottant
        if (type == INT) {
          fprintf(OUTPUT, "\tcvt.s.w $f1, $f1\n");
        }

        fprintf(OUTPUT, "\tla $a1, %s\n",
                q->sym1->var->nom_var_fc);  // Matrice résultat
        fprintf(OUTPUT, "\tlw $t2, %s_rows\n", mat);
        fprintf(OUTPUT, "\tlw $t3, %s_cols\n", mat);

        fprintf(OUTPUT, "\tli $t4, 0\n");
        fprintf(OUTPUT, "boucle_mult_matrice_cst_ligne_%d:\n", cpt_label);
        fprintf(OUTPUT, "\tbge $t4, $t2, fin_mult_%d\n", cpt_label);

        fprintf(OUTPUT, "\tli $t5, 0\n");
        fprintf(OUTPUT, "\tboucle_mult_matrice_cst_colonne_%d:\n", cpt_label);
        fprintf(OUTPUT, "\t\tbge $t5, $t3, ligne_suivante_mult_%d\n",
                cpt_label);

        // Multiplication
        fprintf(OUTPUT, "\t\tlwc1 $f2, 0($a0)\n");
        fprintf(OUTPUT, "\t\tmul.s $f3, $f2, $f1\n");
        fprintf(OUTPUT, "\t\tswc1 $f3, 0($a1)\n");  // Stockage du résultat

        fprintf(OUTPUT, "\t\taddi $a0, $a0, 4\n");
        fprintf(OUTPUT, "\t\taddi $a1, $a1, 4\n");
        fprintf(OUTPUT, "\t\taddi $t5, $t5, 1\n");

        // Prochain élément ligne
        fprintf(OUTPUT, "\t\tj boucle_mult_matrice_cst_colonne_%d\n",
                cpt_label);
        fprintf(OUTPUT, "\tligne_suivante_mult_%d:\n", cpt_label);

        // Prochaine ligne
        fprintf(OUTPUT, "\t\taddi $t4, $t4, 1\n");
        fprintf(OUTPUT, "\t\tj boucle_mult_matrice_cst_ligne_%d\n", cpt_label);

        // Fin
        fprintf(OUTPUT, "fin_mult_%d:\n", cpt_label);
        cpt_label++;
      }

      // Cas : constante * constante
      else {
        // Opérateur 1
        // Si l'opérateur 1 est un entier
        if (q->sym2->kind == CONST_INT ||
            (q->sym2->kind == NAME && q->sym2->var->type == INT)) {
          fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym2->nom_var_fc);
          type1 = INT;
        }
        // Si l'opérateur 1 est un flottant
        else if ((q->sym2->kind == CONST_FLOAT) ||
                 (q->sym2->kind == NAME && q->sym2->var->type == FLOAT)) {
          fprintf(OUTPUT, "\tl.s $f0, %s\n", q->sym2->nom_var_fc);
          type1 = FLOAT;
        }

        // Opérateur 2
        // Si l'opérateur 2 est un entier
        if (q->sym3->kind == CONST_INT ||
            (q->sym3->kind == NAME && q->sym3->var->type == INT)) {
          fprintf(OUTPUT, "\tlw $t1, %s\n", q->sym3->nom_var_fc);
          type2 = INT;
        }
        // Si l'opérateur 2 est un flottant
        else if ((q->sym3->kind == CONST_FLOAT) ||
                 (q->sym3->kind == NAME && q->sym3->var->type == FLOAT)) {
          fprintf(OUTPUT, "\tl.s $f1, %s\n", q->sym3->nom_var_fc);
          type2 = FLOAT;
        }

        // Multiplication de 2 entiers
        if (type1 == INT && type2 == INT) {
          fprintf(OUTPUT, "\tmul $t0,$t0,$t1\n");
          fprintf(OUTPUT, "\tsw $t0, %s\n", q->sym1->nom_var_fc);
        } else {
          // Conversion d'un entier en flottant si nécessaire
          if (type1 == INT && type2 == FLOAT) {
            fprintf(OUTPUT, "mtc1 $t0, $f0\n");
            fprintf(OUTPUT, "cvt.s.w $f0, $f0\n");
          } else if (type1 == FLOAT && type2 == INT) {
            fprintf(OUTPUT, "mtc1 $t1, $f1\n");
            fprintf(OUTPUT, "cvt.s.w $f1, $f1\n");
          }

          // Multiplication
          fprintf(OUTPUT, "\tmul.s $f2,$f0,$f1\n");
          fprintf(OUTPUT, "\ts.s $f2, %s\n", q->sym1->nom_var_fc);
        }
      }
      break;

    case BOP_DIV:
      if ((q->sym2->kind == NAME && q->sym2->var->type == MATRIX) &&
          (q->sym3->kind == NAME && q->sym3->var->type == MATRIX)) {
        fprintf(OUTPUT, "\tla $a0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tla $a1, %s\n", q->sym3->nom_var_fc);
        fprintf(OUTPUT, "\tla $a2, %s\n", q->sym1->nom_var_fc);
        fprintf(OUTPUT, "\tlw $t0, %s_rows\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tlw $t1, %s_cols\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tlw $t2, %s_cols\n", q->sym3->nom_var_fc);

        fprintf(OUTPUT, "\tli $t3, 0\n");
        fprintf(OUTPUT, "boucle_i_%d:\n", cpt_label);
        fprintf(OUTPUT, "\tli $t4, 0\n");

        fprintf(OUTPUT, "\tboucle_j_%d:\n", cpt_label);
        fprintf(OUTPUT, "\t\tli $t5, 0\n");
        fprintf(OUTPUT, "\t\tmtc1 $zero, $f10\n");

        fprintf(OUTPUT, "\t\tboucle_k_%d:\n", cpt_label);
        fprintf(OUTPUT, "\t\t\tmul $t6, $t3, $t1\n");
        fprintf(OUTPUT, "\t\t\tadd $t6, $t6, $t5\n");

        fprintf(OUTPUT, "\t\t\tmul $t7, $t5, $t2\n");
        fprintf(OUTPUT, "\t\t\tadd $t7, $t7, $t4\n");

        fprintf(OUTPUT, "\t\t\tmul $t6, $t6, 4\n");
        fprintf(OUTPUT, "\t\t\tadd $t6, $t6, $a0\n");
        fprintf(OUTPUT, "\t\t\tmul $t7, $t7, 4\n");
        fprintf(OUTPUT, "\t\t\tadd $t7, $t7, $a1\n");

        fprintf(OUTPUT, "\t\t\tl.s $f0, 0($t6)\n");
        fprintf(OUTPUT, "\t\t\tl.s $f1, 0($t7)\n");
        fprintf(OUTPUT, "\t\t\tdiv.s $f11, $f0, $f1\n");
        fprintf(OUTPUT, "\t\t\tadd.s $f10, $f10, $f11\n");

        fprintf(OUTPUT, "\t\t\taddi $t5, $t5, 1\n");
        fprintf(OUTPUT, "\t\t\tbne $t5, $t1, boucle_k_%d\n", cpt_label);

        fprintf(OUTPUT, "\t\tmul $t8, $t3, $t2\n");
        fprintf(OUTPUT, "\t\tadd $t8, $t8, $t4\n");

        fprintf(OUTPUT, "\t\tmul $t8, $t8, 4\n");
        fprintf(OUTPUT, "\t\tadd $t8, $t8, $a2\n");

        fprintf(OUTPUT, "\t\ts.s $f10, 0($t8)\n");

        fprintf(OUTPUT, "\t\taddi $t4, $t4, 1\n");
        fprintf(OUTPUT, "\t\tbne $t4, $t2, boucle_j_%d\n", cpt_label);
        fprintf(OUTPUT, "\t\taddi $t3, $t3, 1\n");
        fprintf(OUTPUT, "\t\tbne $t3, $t0, boucle_i_%d\n", cpt_label);

        cpt_label++;
      }
      // Division d'une matrice et une constante(float ou integer)
      else if ((q->sym2->kind == NAME && q->sym2->var->type == MATRIX) ||
               (q->sym3->kind == NAME && q->sym3->var->type == MATRIX)) {
        nom_var_fonction mat;
        nom_var_fonction cst;
        unsigned type = 0;

        // Cas : matrice / constante
        if (q->sym2->kind == NAME && q->sym2->var->type == MATRIX) {
          strcpy(mat, q->sym2->nom_var_fc);
          strcpy(cst, q->sym3->nom_var_fc);
          if (q->sym3->kind == NAME) {
            type = q->sym3->var->type;
          } else {
            if (q->sym3->kind == CONST_INT)
              type = INT;
            else if (q->sym3->kind == CONST_FLOAT)
              type = FLOAT;
            else {
              fprintf(stderr, "Incompatibilité de types dans une division.\n");
              exit(SEMANTIC_FAILURE);
            }
          }
        } else {
          // Cas : constante / matrice
          strcpy(mat, q->sym3->nom_var_fc);
          strcpy(cst, q->sym2->nom_var_fc);
          if (q->sym2->kind == NAME) {
            type = q->sym2->var->type;
          } else {
            if (q->sym2->kind == CONST_INT)
              type = INT;
            else if (q->sym2->kind == CONST_FLOAT)
              type = FLOAT;
            else {
              fprintf(stderr, "Incompatibilité de types dans une division.\n");
              exit(SEMANTIC_FAILURE);
            }
          }
        }
        fprintf(OUTPUT, "\tla $a0, %s\n", mat);    // Chargement matrice
        fprintf(OUTPUT, "\tlwc1 $f1, %s\n", cst);  // Chargement constante

        // Conversion entier vers flottant
        if (type == INT) {
          fprintf(OUTPUT, "\tcvt.s.w $f1, $f1\n");
        }

        fprintf(OUTPUT, "\tla $a1, %s\n",
                q->sym1->var->nom_var_fc);  // Matrice résultat
        fprintf(OUTPUT, "\tlw $t2, %s_rows\n", mat);
        fprintf(OUTPUT, "\tlw $t3, %s_cols\n", mat);

        fprintf(OUTPUT, "\tli $t4, 0\n");
        fprintf(OUTPUT, "boucle_div_matrice_cst_ligne_%d:\n", cpt_label);
        fprintf(OUTPUT, "\tbge $t4, $t2, fin_div_%d\n", cpt_label);

        fprintf(OUTPUT, "\tli $t5, 0\n");
        fprintf(OUTPUT, "\tboucle_div_matrice_cst_colonne_%d:\n", cpt_label);
        fprintf(OUTPUT, "\t\tbge $t5, $t3, ligne_suivante_div_%d\n", cpt_label);

        fprintf(OUTPUT, "\t\tlwc1 $f2, 0($a0)\n");
        // fprintf(OUTPUT, "\t\tdiv.s $f3, $f2, $f1\n");
        if (q->sym2->kind == NAME && q->sym2->var->type == MATRIX) {
          // Matrice / constante
          fprintf(OUTPUT, "\t\tdiv.s $f3, $f2, $f1\n");
        } else {
          // Constante / matrice
          fprintf(OUTPUT, "\t\tdiv.s $f3, $f1, $f2\n");
        }

        fprintf(OUTPUT, "\t\tswc1 $f3, 0($a1)\n");  // Stockage du résultat

        fprintf(OUTPUT, "\t\taddi $a0, $a0, 4\n");
        fprintf(OUTPUT, "\t\taddi $a1, $a1, 4\n");
        fprintf(OUTPUT, "\t\taddi $t5, $t5, 1\n");

        // Prochain élément ligne
        fprintf(OUTPUT, "\t\tj boucle_div_matrice_cst_colonne_%d\n", cpt_label);
        fprintf(OUTPUT, "\tligne_suivante_div_%d:\n", cpt_label);

        // Prochaine ligne
        fprintf(OUTPUT, "\t\taddi $t4, $t4, 1\n");
        fprintf(OUTPUT, "\t\tj boucle_div_matrice_cst_ligne_%d\n", cpt_label);

        // Fin
        fprintf(OUTPUT, "fin_div_%d:\n", cpt_label);
        cpt_label++;
      }

      // Cas : constante / constante
      else {
        // Opérateur 1
        // Si l'opérateur 1 est un entier
        if (q->sym2->kind == CONST_INT ||
            (q->sym2->kind == NAME && q->sym2->var->type == INT)) {
          fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym2->nom_var_fc);
          type1 = INT;
        }
        // Si l'opérateur 1 est un flottant
        else if ((q->sym2->kind == CONST_FLOAT) ||
                 (q->sym2->kind == NAME && q->sym2->var->type == FLOAT)) {
          fprintf(OUTPUT, "\tl.s $f0, %s\n", q->sym2->nom_var_fc);
          type1 = FLOAT;
        }

        // Opérateur 2
        // Si l'opérateur 2 est un entier
        if (q->sym3->kind == CONST_INT ||
            (q->sym3->kind == NAME && q->sym3->var->type == INT)) {
          fprintf(OUTPUT, "\tlw $t1, %s\n", q->sym3->nom_var_fc);
          type2 = INT;
        }
        // Si l'opérateur 2 est un flottant
        else if ((q->sym3->kind == CONST_FLOAT) ||
                 (q->sym3->kind == NAME && q->sym3->var->type == FLOAT)) {
          fprintf(OUTPUT, "\tl.s $f1, %s\n", q->sym3->nom_var_fc);
          type2 = FLOAT;
        }

        /*
        // Div de 2 entiers
        if (type1 == INT && type2 == INT) {
          fprintf(OUTPUT,"\tdiv $t0,$t0,$t1\n");
          fprintf(OUTPUT,"\tsw $t0, %s\n", q->sym1->nom_var_fc);
        } else {
          // Conversion d'un entier en flottant si nécessaire
          if (type1 == INT && type2 == FLOAT) {
            fprintf(OUTPUT,"mtc1 $t0, $f0\n");
            fprintf(OUTPUT,"cvt.s.w $f0, $f0\n");
          } else if (type1 == FLOAT && type2 == INT) {
            fprintf(OUTPUT,"mtc1 $t1, $f1\n");
            fprintf(OUTPUT,"cvt.s.w $f1, $f1\n");
          }
        */

        // Conversion d'un entier en flottant
        if (type1 == INT) {
          fprintf(OUTPUT, "mtc1 $t0, $f0\n");
          fprintf(OUTPUT, "cvt.s.w $f0, $f0\n");
        }
        if (type2 == INT) {
          fprintf(OUTPUT, "mtc1 $t1, $f1\n");
          fprintf(OUTPUT, "cvt.s.w $f1, $f1\n");
        }

        // Division
        fprintf(OUTPUT, "\tdiv.s $f2,$f0,$f1\n");
        fprintf(OUTPUT, "\ts.s $f2, %s\n", q->sym1->nom_var_fc);
      }
      break;

    case UOP_TILDE:
      // Transposition d'une matrice
      if (q->sym2->kind == NAME &&
          q->sym2->var->type == MATRIX) {  // Toujours le cas normalement
        fprintf(OUTPUT, "\tla $a1, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tla $a2, %s\n", q->sym1->nom_var_fc);
        fprintf(OUTPUT, "\tlw $t0, %s_rows\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tlw $t1, %s_cols\n", q->sym2->nom_var_fc);

        fprintf(OUTPUT, "\tli $t3, 0\n");
        fprintf(OUTPUT, "transpose_i_loop_%d:\n", cpt_label);
        fprintf(OUTPUT, "\tli $t4, 0\n");

        fprintf(OUTPUT, "transpose_j_loop_%d:\n", cpt_label);
        fprintf(OUTPUT, "\tmul $t5, $t3, $t1\n");
        fprintf(OUTPUT, "\tadd $t5, $t5, $t4\n");

        fprintf(OUTPUT, "\tmul $t6, $t4, $t0\n");
        fprintf(OUTPUT, "\tadd $t6, $t6, $t3\n");

        fprintf(OUTPUT, "\tmul $t7, $t5, 4\n");
        fprintf(OUTPUT, "\tadd $t7, $t7, $a1\n");
        fprintf(OUTPUT, "\tl.s $f10, 0($t7)\n");

        fprintf(OUTPUT, "\tmul $t8, $t6, 4\n");
        fprintf(OUTPUT, "\tadd $t8, $t8, $a2\n");
        fprintf(OUTPUT, "\ts.s $f10, 0($t8)\n");

        fprintf(OUTPUT, "\taddi $t4, $t4, 1\n");
        fprintf(OUTPUT, "\tbne $t4, $t1, transpose_j_loop_%d\n", cpt_label);

        fprintf(OUTPUT, "\taddi $t3, $t3, 1\n");
        fprintf(OUTPUT, "\tbne $t3, $t0, transpose_i_loop_%d\n", cpt_label);

        cpt_label++;
      }
      break;

    // extraction
    case EXTR_LIGNE:

      fprintf(OUTPUT, "\tlw $t3, %s_rows\n", q->sym1->nom_var_fc);  // sym1_cols
      fprintf(OUTPUT, "\tlw $t4, %s_cols\n", q->sym2->nom_var_fc);  // sym2_rows

      fprintf(OUTPUT, "\tli $t0, 0\n");  // $t0 = i
      fprintf(OUTPUT, "boucle_extrligne_i_%d:\n", cpt_label);
      fprintf(OUTPUT, "bge $t0, $t3, i_suivant_%d\n", cpt_label);

      fprintf(OUTPUT, "\tli $t1, 0\n");  // $t1 = j
      fprintf(OUTPUT, "boucle_extrligne_j_%d:\n", cpt_label);
      fprintf(OUTPUT, "\t\tbge $t1, $t4, j_suivant_%d\n", cpt_label);

      fprintf(OUTPUT, "\t\tla $a0, %s\n",
              q->sym2->nom_var_fc);  // matrice originale
      fprintf(OUTPUT, "\t\tla $a1, %s\n",
              q->sym1->nom_var_fc);  // matrice cible
      fprintf(OUTPUT, "\t\tla $a2, %s\n", q->sym3->nom_var_fc);
      // liste des indices des colonnes à extraire

      fprintf(OUTPUT, "\t\tmove $t2, $t0\n");
      fprintf(OUTPUT, "\t\tsll $t2, $t2, 2\n");
      fprintf(OUTPUT, "\t\tadd $a2, $a2, $t2\n");
      fprintf(OUTPUT, "\t\tlw $t5, 0($a2)\n");  // $t5 = indice[i]

      fprintf(OUTPUT, "\t\tlw $t6, %s_cols\n", q->sym2->nom_var_fc);
      fprintf(OUTPUT, "\t\tmul $t6, $t5, $t6\n");
      fprintf(OUTPUT, "\t\tadd $t6, $t6, $t1 \n");  //
      fprintf(OUTPUT, "\t\tsll $t6, $t6, 2\n");
      fprintf(OUTPUT, "\t\tadd $a0, $a0, $t6\n");
      fprintf(OUTPUT, "\t\tl.s $f0, 0($a0)\n");

      fprintf(OUTPUT, "\t\tlw $t7, %s_cols\n", q->sym1->nom_var_fc);
      fprintf(OUTPUT, "\t\tmul $t7, $t0, $t7\n");
      fprintf(OUTPUT, "\t\tadd $t7, $t7, $t1\n");  //
      fprintf(OUTPUT, "\t\tsll $t7, $t7, 2\n");
      fprintf(OUTPUT, "\t\tadd $a1, $a1, $t7\n");

      fprintf(OUTPUT, "\t\ts.s $f0, 0($a1)\n");

      fprintf(OUTPUT, "\t\taddi $t1, $t1, 1\n");  // j++
      fprintf(OUTPUT, "\t\tj boucle_extrligne_j_%d\n", cpt_label);

      fprintf(OUTPUT, "\tj_suivant_%d:\n", cpt_label);
      fprintf(OUTPUT, "\taddi $t0, $t0, 1\n");  // i++
      fprintf(OUTPUT, "\tj boucle_extrligne_i_%d\n", cpt_label);
      fprintf(OUTPUT, "i_suivant_%d:\n", cpt_label);

      cpt_label++;

      break;

    case EXTR_COLONNE:

      fprintf(OUTPUT, "\tlw $t3, %s_cols\n", q->sym1->nom_var_fc);  // sym1_cols
      fprintf(OUTPUT, "\tlw $t4, %s_rows\n", q->sym2->nom_var_fc);  // sym2_rows

      fprintf(OUTPUT, "\tli $t0, 0\n");  // $t0 = i
      fprintf(OUTPUT, "boucle_extrcolonne_i_%d:\n", cpt_label);
      fprintf(OUTPUT, "bge $t0, $t3, i_suivant_%d\n", cpt_label);

      fprintf(OUTPUT, "\tli $t1, 0\n");  // $t1 = j
      fprintf(OUTPUT, "boucle_extrcolonne_j_%d:\n", cpt_label);
      fprintf(OUTPUT, "\t\tbge $t1, $t4, j_suivant_%d\n", cpt_label);

      fprintf(OUTPUT, "\t\tla $a0, %s\n",
              q->sym2->nom_var_fc);  // matrice originale
      fprintf(OUTPUT, "\t\tla $a1, %s\n",
              q->sym1->nom_var_fc);  // matrice cible
      fprintf(OUTPUT, "\t\tla $a2, %s\n", q->sym3->nom_var_fc);
      // liste des indices des colonnes à extraire

      fprintf(OUTPUT, "\t\tmove $t2, $t0\n");
      fprintf(OUTPUT, "\t\tsll $t2, $t2, 2\n");
      fprintf(OUTPUT, "\t\tadd $a2, $a2, $t2\n");
      fprintf(OUTPUT, "\t\tlw $t5, 0($a2)\n");  // $t5 = indice[i]

      fprintf(OUTPUT, "\t\tlw $t6, %s_cols\n", q->sym2->nom_var_fc);
      fprintf(OUTPUT, "\t\tmul $t6, $t1, $t6\n");
      fprintf(OUTPUT, "\t\tadd $t6, $t6, $t5 \n");  // A_cols * j + indice[i]
      fprintf(OUTPUT, "\t\tsll $t6, $t6, 2\n");
      fprintf(OUTPUT, "\t\tadd $a0, $a0, $t6\n");
      fprintf(OUTPUT, "\t\tl.s $f0, 0($a0)\n");

      fprintf(OUTPUT, "\t\tlw $t7, %s_cols\n", q->sym1->nom_var_fc);
      fprintf(OUTPUT, "\t\tmul $t7, $t1, $t7\n");
      fprintf(OUTPUT, "\t\tadd $t7, $t7, $t0\n");  // B_rows * j + i
      fprintf(OUTPUT, "\t\tsll $t7, $t7, 2\n");
      fprintf(OUTPUT, "\t\tadd $a1, $a1, $t7\n");

      fprintf(OUTPUT, "\t\ts.s $f0, 0($a1)\n");

      fprintf(OUTPUT, "\t\taddi $t1, $t1, 1\n");  // j++
      fprintf(OUTPUT, "\t\tj boucle_extrcolonne_j_%d\n", cpt_label);

      fprintf(OUTPUT, "\tj_suivant_%d:\n", cpt_label);
      fprintf(OUTPUT, "\taddi $t0, $t0, 1\n");  // i++
      fprintf(OUTPUT, "\tj boucle_extrcolonne_i_%d\n", cpt_label);
      fprintf(OUTPUT, "i_suivant_%d:\n", cpt_label);

      cpt_label++;

      break;

    case GET_ELEMENT:
      fprintf(OUTPUT, "\tli $t0, %d\n", q->sym3->tuple.ligne);
      fprintf(OUTPUT, "\tli $t1, %d\n", q->sym3->tuple.colonne);

      fprintf(OUTPUT, "\tlw $t2, %s_cols\n", q->sym2->nom_var_fc);

      fprintf(OUTPUT, "\tmul $t3, $t0, $t2\n");
      fprintf(OUTPUT, "\tadd $t3, $t3, $t1\n");
      fprintf(OUTPUT, "\tsll $t3, $t3, 2\n");

      fprintf(OUTPUT, "\tla $a0, %s\n", q->sym2->nom_var_fc);
      fprintf(OUTPUT, "\tadd $a0, $a0, $t3\n");

      fprintf(OUTPUT, "\tl.s $f0, 0($a0)\n");
      fprintf(OUTPUT, "\ts.s $f0, %s\n", q->sym1->nom_var_fc);
      break;

    case PUT_ELEMENT:
      fprintf(OUTPUT, "\tli $t0, %d\n", q->sym3->tuple.ligne);
      fprintf(OUTPUT, "\tli $t1, %d\n", q->sym3->tuple.colonne);

      fprintf(OUTPUT, "\tlw $t2, %s_cols\n", q->sym1->nom_var_fc);

      fprintf(OUTPUT, "\tmul $t3, $t0, $t2\n");
      fprintf(OUTPUT, "\tadd $t3, $t3, $t1\n");
      fprintf(OUTPUT, "\tsll $t3, $t3, 2\n");

      fprintf(OUTPUT, "\tla $a0, %s\n", q->sym1->nom_var_fc);
      fprintf(OUTPUT, "\tadd $a0, $a0, $t3\n");

      if (q->sym2->kind == CONST_INT ||
          (q->sym2->kind == NAME && q->sym2->var->type == INT)) {
        fprintf(OUTPUT, "\tlw $t4, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tmtc1 $t4, $f0\n");
        fprintf(OUTPUT, "\tcvt.s.w $f0, $f0\n");
      }  // int
      else if ((q->sym2->kind == CONST_FLOAT) ||
               (q->sym2->kind == NAME && q->sym2->var->type == FLOAT)) {
        fprintf(OUTPUT, "\tl.s $f0, %s\n", q->sym2->nom_var_fc);
      }  // float
      else {
        fprintf(stderr, "something went wrong\n");
        exit(SEMANTIC_FAILURE);
      }
      fprintf(OUTPUT, "\ts.s $f0, 0($a0)\n");

      break;

    // affichage :
    case CALL_PRINT:
      if (q->sym1->var->type == INT) {
        fprintf(OUTPUT, "\tli $v0,1\n");
        fprintf(OUTPUT, "\tlw $a0, %s\n", q->sym1->nom_var_fc);
      } else if (q->sym1->var->type == FLOAT) {
        fprintf(OUTPUT, "\tli $v0,2\n");
        fprintf(OUTPUT, "\tlwc1 $f12, %s\n", q->sym1->nom_var_fc);
      }
      fprintf(OUTPUT, "\tsyscall\n");
      break;

    case CALL_PRINTMAT:
      // charger les dimensions de la matrice
      fprintf(OUTPUT, "\tlw $t0, %s_rows\n", q->sym1->nom_var_fc);
      fprintf(OUTPUT, "\tlw $t1, %s_cols\n", q->sym1->nom_var_fc);

      fprintf(OUTPUT, "\tli $t2, 0\n");
      fprintf(OUTPUT, "boucle_afficher_matrix_%s_ligne_%d:\n",
              q->sym1->nom_var_fc, cpt_label);

      fprintf(OUTPUT, "\tli $t3, 0\n");
      fprintf(OUTPUT, "boucle_afficher_matrix_%s_colonne_%d:\n",
              q->sym1->nom_var_fc, cpt_label);

      // indice dans la matrice
      fprintf(OUTPUT, "\tmul $t4, $t2, $t1\n");
      fprintf(OUTPUT, "\tadd $t5, $t4, $t3\n");

      // afficher l'élément
      fprintf(OUTPUT, "\tla $t6, %s\n", q->sym1->nom_var_fc);
      fprintf(OUTPUT, "\tmul $t5, $t5, 4\n");
      fprintf(OUTPUT, "\tadd $t6, $t6, $t5\n");
      fprintf(OUTPUT, "\tlw $t7, 0($t6)\n");
      fprintf(OUTPUT, "\tmtc1 $t7, $f12\n");

      fprintf(OUTPUT, "\tli $v0, 2\n");
      fprintf(OUTPUT, "\tsyscall\n");

      // \t
      fprintf(OUTPUT, "\tli $v0, 4\n");
      fprintf(OUTPUT, "\tla $a0, tab\n");
      fprintf(OUTPUT, "\tsyscall\n");

      // indice colonne ++
      fprintf(OUTPUT, "\taddi $t3, $t3, 1\n");

      // indice colonne < matrix_cols
      fprintf(OUTPUT, "\tbne $t3, $t1, boucle_afficher_matrix_%s_colonne_%d\n",
              q->sym1->nom_var_fc, cpt_label);

      // \n
      fprintf(OUTPUT, "\tli $v0, 4\n");
      fprintf(OUTPUT, "\tla $a0, newline\n");
      fprintf(OUTPUT, "\tsyscall\n");

      // indice ligne ++
      fprintf(OUTPUT, "\taddi $t2, $t2, 1\n");

      // indice ligne < matrix_rows
      fprintf(OUTPUT, "\tbne $t2, $t0, boucle_afficher_matrix_%s_ligne_%d\n",
              q->sym1->nom_var_fc, cpt_label);
      cpt_label++;
      break;

    case CALL_PRINTF:
      fprintf(OUTPUT, "\tli $v0, 4\n");
      fprintf(OUTPUT, "\tla $a0, %s\n", q->sym1->nom_var_fc);
      fprintf(OUTPUT, "\tsyscall\n");

      break;

    // expression booleenne
    case B_EVAL:
      // assert type
      if (q->sym2->kind == CONST_INT ||
          (q->sym2->kind == NAME && q->sym2->var->type == INT)) {
        fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tbeqz $t0, false_%d\n", cpt_label);
      } else if ((q->sym2->kind == CONST_FLOAT) ||
                 (q->sym2->kind == NAME && q->sym2->var->type == FLOAT)) {
        fprintf(OUTPUT, "\tl.s $f0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tli.s $f1, 0.0\n");
        fprintf(OUTPUT, "\tc.eq.s $f0, $f1\n");
        fprintf(OUTPUT, "\tbc1t false_%d\n", cpt_label);
      } else {
        fprintf(stderr, "something went wrong\n");
        exit(SEMANTIC_FAILURE);
      }

      fprintf(OUTPUT, "\tli $t0, 1\n");
      fprintf(OUTPUT, "\tsw $t0, %s\n", q->sym1->nom_var_fc);
      fprintf(OUTPUT, "\tj fin_condi_%d\n", cpt_label);

      fprintf(OUTPUT, "false_%d:\n", cpt_label);
      fprintf(OUTPUT, "\tli $t0, 0\n");
      fprintf(OUTPUT, "\tsw $t0, %s\n", q->sym1->nom_var_fc);

      fprintf(OUTPUT, "fin_condi_%d:\n", cpt_label);

      cpt_label++;
      break;

    case B_OU:
      fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym2->nom_var_fc);
      fprintf(OUTPUT, "\tlw $t1, %s\n", q->sym3->nom_var_fc);
      fprintf(OUTPUT, "\tor $t0, $t0, $t1\n");
      fprintf(OUTPUT, "\tsw $t0, %s\n", q->sym1->nom_var_fc);
      break;

    case B_ET:
      fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym2->nom_var_fc);
      fprintf(OUTPUT, "\tlw $t1, %s\n", q->sym3->nom_var_fc);
      fprintf(OUTPUT, "\tand $t0, $t0, $t1\n");
      fprintf(OUTPUT, "\tsw $t0, %s\n", q->sym1->nom_var_fc);
      break;

    case B_NOT:
      fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym2->nom_var_fc);
      // fprintf(OUTPUT,"\tlw $t1, 1\n");
      fprintf(OUTPUT, "\txori $t0, $t0, 1\n");  // not = xor 1
      fprintf(OUTPUT, "\tsw $t0, %s\n", q->sym1->nom_var_fc);
      break;

    case B_LT:
      switch (q->sym2->kind) {
        case (NAME):
          assert(q->sym2->var->init);
          type1 = q->sym2->var->type;
          break;
        case (CONST_INT):
          type1 = INT;
          break;
        case (CONST_FLOAT):
          type1 = FLOAT;
          break;
        default:
          fprintf(stderr,
                  "Une expression booléenne ne manipule que des expressions "
                  "correspondant à des int ou des float.\n");
          exit(SEMANTIC_FAILURE);
          break;
      }
      switch (q->sym3->kind) {
        case (NAME):
          assert(q->sym3->var->init);
          type2 = q->sym3->var->type;
          break;
        case (CONST_INT):
          type2 = INT;
          break;
        case (CONST_FLOAT):
          type2 = FLOAT;
          break;
        default:
          fprintf(stderr,
                  "Une expression booléenne ne manipule que des expressions "
                  "correspondant à des int ou des float.\n");
          exit(SEMANTIC_FAILURE);
          break;
      }
      if (type1 == INT && type2 == INT) {  // si les 2 sont des int
        fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tlw $t1, %s\n", q->sym3->nom_var_fc);
        fprintf(OUTPUT, "\tbge $t0, $t1, false_%d\n", cpt_label);
      } else if (type1 == FLOAT && type2 == FLOAT) {  // si les 2 sont des float
        fprintf(OUTPUT, "\tl.s $f0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tl.s $f1, %s\n", q->sym3->nom_var_fc);
        fprintf(OUTPUT, "\tc.lt.s $f0, $f1\n");
        fprintf(OUTPUT, "\tbc1f, false_%d\n", cpt_label);
      } else if (type1 == INT && type2 == FLOAT) {  // int et float
        fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tmtc1 $t0, $f0\n");
        fprintf(OUTPUT, "\tcvt.s.w $f0, $f0\n");
        fprintf(OUTPUT, "\tl.s $f1, %s\n", q->sym3->nom_var_fc);
        fprintf(OUTPUT, "\tc.lt.s $f0, $f1\n");
        fprintf(OUTPUT, "\tbc1f, false_%d\n", cpt_label);
      } else if (type1 == FLOAT && type2 == INT) {
        fprintf(OUTPUT, "\tl.s $f0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym3->nom_var_fc);
        fprintf(OUTPUT, "\tmtc1 $t0, $f1\n");
        fprintf(OUTPUT, "\tcvt.s.w $f1, $f1\n");
        fprintf(OUTPUT, "\tc.lt.s $f0, $f1\n");
        fprintf(OUTPUT, "\tbc1f, false_%d\n", cpt_label);
      } else {
        fprintf(stderr, "something went wrong\n");
        exit(SEMANTIC_FAILURE);
      }

      // cas vrai
      fprintf(OUTPUT, "\tli $t0, 1\n");
      fprintf(OUTPUT, "\tsw $t0, %s\n", q->sym1->nom_var_fc);
      fprintf(OUTPUT, "\tj fin_condi_%d\n", cpt_label);

      // cas faux
      fprintf(OUTPUT, "false_%d:\n", cpt_label);
      fprintf(OUTPUT, "\tli $t0, 0\n");
      fprintf(OUTPUT, "\tsw $t0, %s\n", q->sym1->nom_var_fc);

      fprintf(OUTPUT, "fin_condi_%d:\n", cpt_label);

      cpt_label++;

      break;

    case B_LTOE:
      switch (q->sym2->kind) {
        case (NAME):
          assert(q->sym2->var->init);
          type1 = q->sym2->var->type;
          break;
        case (CONST_INT):
          type1 = INT;
          break;
        case (CONST_FLOAT):
          type1 = FLOAT;
          break;
        default:
          fprintf(stderr,
                  "Une expression booléenne ne manipule que des expressions "
                  "correspondant à des int ou des float.\n");
          exit(SEMANTIC_FAILURE);
          break;
      }
      switch (q->sym3->kind) {
        case (NAME):
          assert(q->sym3->var->init);
          type2 = q->sym3->var->type;
          break;
        case (CONST_INT):
          type2 = INT;
          break;
        case (CONST_FLOAT):
          type2 = FLOAT;
          break;
        default:
          fprintf(stderr,
                  "Une expression booléenne ne manipule que des expressions "
                  "correspondant à des int ou des float.\n");
          exit(SEMANTIC_FAILURE);
          break;
      }
      if (type1 == INT && type2 == INT) {  // si les 2 sont des int
        fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tlw $t1, %s\n", q->sym3->nom_var_fc);
        fprintf(OUTPUT, "\tbgt $t0, $t1, false_%d\n", cpt_label);
      } else if (type1 == FLOAT && type2 == FLOAT) {  // si les 2 sont des float
        fprintf(OUTPUT, "\tl.s $f0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tl.s $f1, %s\n", q->sym3->nom_var_fc);
        fprintf(OUTPUT, "\tc.le.s $f0, $f1\n");
        fprintf(OUTPUT, "\tbc1f, false_%d\n", cpt_label);
      } else if (type1 == INT && type2 == FLOAT) {  // int et float
        fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tmtc1 $t0, $f0\n");
        fprintf(OUTPUT, "\tcvt.s.w $f0, $f0\n");
        fprintf(OUTPUT, "\tl.s $f1, %s\n", q->sym3->nom_var_fc);
        fprintf(OUTPUT, "\tc.le.s $f0, $f1\n");
        fprintf(OUTPUT, "\tbc1f, false_%d\n", cpt_label);
      } else if (type1 == FLOAT && type2 == INT) {  // float et int
        fprintf(OUTPUT, "\tl.s $f0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym3->nom_var_fc);
        fprintf(OUTPUT, "\tmtc1 $t0, $f1\n");
        fprintf(OUTPUT, "\tcvt.s.w $f1, $f1\n");
        fprintf(OUTPUT, "\tc.le.s $f0, $f1\n");
        fprintf(OUTPUT, "\tbc1f, false_%d\n", cpt_label);
      } else {
        fprintf(stderr, "something went wrong\n");
        exit(SEMANTIC_FAILURE);
      }

      // cas vrai
      fprintf(OUTPUT, "\tli $t0, 1\n");
      fprintf(OUTPUT, "\tsw $t0, %s\n", q->sym1->nom_var_fc);
      fprintf(OUTPUT, "\tj fin_condi_%d\n", cpt_label);

      // cas faux
      fprintf(OUTPUT, "false_%d:\n", cpt_label);
      fprintf(OUTPUT, "\tli $t0, 0\n");
      fprintf(OUTPUT, "\tsw $t0, %s\n", q->sym1->nom_var_fc);

      fprintf(OUTPUT, "fin_condi_%d:\n", cpt_label);

      cpt_label++;

      break;

    case B_GT:
      switch (q->sym2->kind) {
        case (NAME):
          assert(q->sym2->var->init);
          type1 = q->sym2->var->type;
          break;
        case (CONST_INT):
          type1 = INT;
          break;
        case (CONST_FLOAT):
          type1 = FLOAT;
          break;
        default:
          fprintf(stderr,
                  "Une expression booléenne ne manipule que des expressions "
                  "correspondant à des int ou des float.\n");
          exit(SEMANTIC_FAILURE);
          break;
      }
      switch (q->sym3->kind) {
        case (NAME):
          assert(q->sym3->var->init);
          type2 = q->sym3->var->type;
          break;
        case (CONST_INT):
          type2 = INT;
          break;
        case (CONST_FLOAT):
          type2 = FLOAT;
          break;
        default:
          fprintf(stderr,
                  "Une expression booléenne ne manipule que des expressions "
                  "correspondant à des int ou des float.\n");
          exit(SEMANTIC_FAILURE);
          break;
      }
      if (type1 == INT && type2 == INT) {  // si les 2 sont des int
        fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tlw $t1, %s\n", q->sym3->nom_var_fc);
        fprintf(OUTPUT, "\tble $t0, $t1, false_%d\n", cpt_label);
      } else if (type1 == FLOAT && type2 == FLOAT) {  // si les 2 sont des float
        fprintf(OUTPUT, "\tl.s $f0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tl.s $f1, %s\n", q->sym3->nom_var_fc);
        fprintf(OUTPUT, "\tc.le.s $f0, $f1\n");
        fprintf(OUTPUT, "\tbc1t, false_%d\n", cpt_label);
      } else if (type1 == INT && type2 == FLOAT) {  // int et float
        fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tmtc1 $t0, $f0\n");
        fprintf(OUTPUT, "\tcvt.s.w $f0, $f0\n");
        fprintf(OUTPUT, "\tl.s $f1, %s\n", q->sym3->nom_var_fc);
        fprintf(OUTPUT, "\tc.le.s $f0, $f1\n");
        fprintf(OUTPUT, "\tbc1t, false_%d\n", cpt_label);
      } else if (type1 == FLOAT && type2 == INT) {
        fprintf(OUTPUT, "\tl.s $f0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym3->nom_var_fc);
        fprintf(OUTPUT, "\tmtc1 $t0, $f1\n");
        fprintf(OUTPUT, "\tcvt.s.w $f1, $f1\n");
        fprintf(OUTPUT, "\tc.le.s $f0, $f1\n");
        fprintf(OUTPUT, "\tbc1t, false_%d\n", cpt_label);
      } else {
        fprintf(stderr, "something went wrong\n");
        exit(SEMANTIC_FAILURE);
      }

      // cas vrai
      fprintf(OUTPUT, "\tli $t0, 1\n");
      fprintf(OUTPUT, "\tsw $t0, %s\n", q->sym1->nom_var_fc);
      fprintf(OUTPUT, "\tj fin_condi_%d\n", cpt_label);

      // cas faux
      fprintf(OUTPUT, "false_%d:\n", cpt_label);
      fprintf(OUTPUT, "\tli $t0, 0\n");
      fprintf(OUTPUT, "\tsw $t0, %s\n", q->sym1->nom_var_fc);

      fprintf(OUTPUT, "fin_condi_%d:\n", cpt_label);

      cpt_label++;

      break;  // B_GT

    case B_GTOE:
      switch (q->sym2->kind) {
        case (NAME):
          assert(q->sym2->var->init);
          type1 = q->sym2->var->type;
          break;
        case (CONST_INT):
          type1 = INT;
          break;
        case (CONST_FLOAT):
          type1 = FLOAT;
          break;
        default:
          fprintf(stderr,
                  "Une expression booléenne ne manipule que des expressions "
                  "correspondant à des int ou des float.\n");
          exit(SEMANTIC_FAILURE);
          break;
      }
      switch (q->sym3->kind) {
        case (NAME):
          assert(q->sym3->var->init);
          type2 = q->sym3->var->type;
          break;
        case (CONST_INT):
          type2 = INT;
          break;
        case (CONST_FLOAT):
          type2 = FLOAT;
          break;
        default:
          fprintf(stderr,
                  "Une expression booléenne ne manipule que des expressions "
                  "correspondant à des int ou des float.\n");
          exit(SEMANTIC_FAILURE);
          break;
      }
      if (type1 == INT && type2 == INT) {  // si les 2 sont des int
        fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tlw $t1, %s\n", q->sym3->nom_var_fc);
        fprintf(OUTPUT, "\tblt $t0, $t1, false_%d\n", cpt_label);
      } else if (type1 == FLOAT && type2 == FLOAT) {  // si les 2 sont des float
        fprintf(OUTPUT, "\tl.s $f0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tl.s $f1, %s\n", q->sym3->nom_var_fc);
        fprintf(OUTPUT, "\tc.lt.s $f0, $f1\n");
        fprintf(OUTPUT, "\tbc1t, false_%d\n", cpt_label);
      } else if (type1 == INT && type2 == FLOAT) {  // int et float
        fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tmtc1 $t0, $f0\n");
        fprintf(OUTPUT, "\tcvt.s.w $f0, $f0\n");
        fprintf(OUTPUT, "\tl.s $f1, %s\n", q->sym3->nom_var_fc);
        fprintf(OUTPUT, "\tc.lt.s $f0, $f1\n");
        fprintf(OUTPUT, "\tbc1t, false_%d\n", cpt_label);
      } else if (type1 == FLOAT && type2 == INT) {
        fprintf(OUTPUT, "\tl.s $f0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym3->nom_var_fc);
        fprintf(OUTPUT, "\tmtc1 $t0, $f1\n");
        fprintf(OUTPUT, "\tcvt.s.w $f1, $f1\n");
        fprintf(OUTPUT, "\tc.lt.s $f0, $f1\n");
        fprintf(OUTPUT, "\tbc1t, false_%d\n", cpt_label);
      } else {
        fprintf(stderr, "something went wrong\n");
        exit(SEMANTIC_FAILURE);
      }

      // cas vrai
      fprintf(OUTPUT, "\tli $t0, 1\n");
      fprintf(OUTPUT, "\tsw $t0, %s\n", q->sym1->nom_var_fc);
      fprintf(OUTPUT, "\tj fin_condi_%d\n", cpt_label);

      // cas faux
      fprintf(OUTPUT, "false_%d:\n", cpt_label);
      fprintf(OUTPUT, "\tli $t0, 0\n");
      fprintf(OUTPUT, "\tsw $t0, %s\n", q->sym1->nom_var_fc);

      fprintf(OUTPUT, "fin_condi_%d:\n", cpt_label);

      cpt_label++;

      break;

    case B_EQUAL:
      switch (q->sym2->kind) {
        case (NAME):
          assert(q->sym2->var->init);
          type1 = q->sym2->var->type;
          break;
        case (CONST_INT):
          type1 = INT;
          break;
        case (CONST_FLOAT):
          type1 = FLOAT;
          break;
        default:
          fprintf(stderr,
                  "Une expression booléenne ne manipule que des expressions "
                  "correspondant à des int ou des float.\n");
          exit(SEMANTIC_FAILURE);
          break;
      }
      switch (q->sym3->kind) {
        case (NAME):
          assert(q->sym3->var->init);
          type2 = q->sym3->var->type;
          break;
        case (CONST_INT):
          type2 = INT;
          break;
        case (CONST_FLOAT):
          type2 = FLOAT;
          break;
        default:
          fprintf(stderr,
                  "Une expression booléenne ne manipule que des expressions "
                  "correspondant à des int ou des float.\n");
          exit(SEMANTIC_FAILURE);
          break;
      }
      if (type1 == INT && type2 == INT) {  // si les 2 sont des int
        fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tlw $t1, %s\n", q->sym3->nom_var_fc);
        fprintf(OUTPUT, "\tbne $t0, $t1, false_%d\n", cpt_label);
      } else if (type1 == FLOAT && type2 == FLOAT) {  // si les 2 sont des float
        fprintf(OUTPUT, "\tl.s $f0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tl.s $f1, %s\n", q->sym3->nom_var_fc);
        fprintf(OUTPUT, "\tc.eq.s $f0, $f1\n");
        fprintf(OUTPUT, "\tbc1f, false_%d\n", cpt_label);
      } else if (type1 == INT && type2 == FLOAT) {  // int et float
        fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tmtc1 $t0, $f0\n");
        fprintf(OUTPUT, "\tcvt.s.w $f0, $f0\n");
        fprintf(OUTPUT, "\tl.s $f1, %s\n", q->sym3->nom_var_fc);
        fprintf(OUTPUT, "\tc.eq.s $f0, $f1\n");
        fprintf(OUTPUT, "\tbc1f, false_%d\n", cpt_label);
      } else if (type1 == FLOAT && type2 == INT) {
        fprintf(OUTPUT, "\tl.s $f0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym3->nom_var_fc);
        fprintf(OUTPUT, "\tmtc1 $t0, $f1\n");
        fprintf(OUTPUT, "\tcvt.s.w $f1, $f1\n");
        fprintf(OUTPUT, "\tc.eq.s $f0, $f1\n");
        fprintf(OUTPUT, "\tbc1f, false_%d\n", cpt_label);
      } else {
        fprintf(stderr, "something went wrong\n");
        exit(SEMANTIC_FAILURE);
      }

      // cas vrai
      fprintf(OUTPUT, "\tli $t0, 1\n");
      fprintf(OUTPUT, "\tsw $t0, %s\n", q->sym1->nom_var_fc);
      fprintf(OUTPUT, "\tj fin_condi_%d\n", cpt_label);

      // cas faux
      fprintf(OUTPUT, "false_%d:\n", cpt_label);
      fprintf(OUTPUT, "\tli $t0, 0\n");
      fprintf(OUTPUT, "\tsw $t0, %s\n", q->sym1->nom_var_fc);

      fprintf(OUTPUT, "fin_condi_%d:\n", cpt_label);

      cpt_label++;

      break;

    case B_NOT_EQUAL:
      switch (q->sym2->kind) {
        case (NAME):
          assert(q->sym2->var->init);
          type1 = q->sym2->var->type;
          break;
        case (CONST_INT):
          type1 = INT;
          break;
        case (CONST_FLOAT):
          type1 = FLOAT;
          break;
        default:
          fprintf(stderr,
                  "Une expression booléenne ne manipule que des expressions "
                  "correspondant à des int ou des float.\n");
          exit(SEMANTIC_FAILURE);
          break;
      }
      switch (q->sym3->kind) {
        case (NAME):
          assert(q->sym3->var->init);
          type2 = q->sym3->var->type;
          break;
        case (CONST_INT):
          type2 = INT;
          break;
        case (CONST_FLOAT):
          type2 = FLOAT;
          break;
        default:
          fprintf(stderr,
                  "Une expression booléenne ne manipule que des expressions "
                  "correspondant à des int ou des float.\n");
          exit(SEMANTIC_FAILURE);
          break;
      }
      if (type1 == INT && type2 == INT) {  // si les 2 sont des int
        fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tlw $t1, %s\n", q->sym3->nom_var_fc);
        fprintf(OUTPUT, "\tbeq $t0, $t1, false_%d\n", cpt_label);
      } else if (type1 == FLOAT && type2 == FLOAT) {  // si les 2 sont des float
        fprintf(OUTPUT, "\tl.s $f0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tl.s $f1, %s\n", q->sym3->nom_var_fc);
        fprintf(OUTPUT, "\tc.eq.s $f0, $f1\n");
        fprintf(OUTPUT, "\tbc1t, false_%d\n", cpt_label);
      } else if (type1 == INT && type2 == FLOAT) {  // int et float
        fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tmtc1 $t0, $f0\n");
        fprintf(OUTPUT, "\tcvt.s.w $f0, $f0\n");
        fprintf(OUTPUT, "\tl.s $f1, %s\n", q->sym3->nom_var_fc);
        fprintf(OUTPUT, "\tc.eq.s $f0, $f1\n");
        fprintf(OUTPUT, "\tbc1t, false_%d\n", cpt_label);
      } else if (type1 == FLOAT && type2 == INT) {
        fprintf(OUTPUT, "\tl.s $f0, %s\n", q->sym2->nom_var_fc);
        fprintf(OUTPUT, "\tlw $t0, %s\n", q->sym3->nom_var_fc);
        fprintf(OUTPUT, "\tmtc1 $t0, $f1\n");
        fprintf(OUTPUT, "\tcvt.s.w $f1, $f1\n");
        fprintf(OUTPUT, "\tc.eq.s $f0, $f1\n");
        fprintf(OUTPUT, "\tbc1t, false_%d\n", cpt_label);
      } else {
        fprintf(stderr, "something went wrong\n");
        exit(SEMANTIC_FAILURE);
      }

      // cas vrai
      fprintf(OUTPUT, "\tli $t0, 1\n");
      fprintf(OUTPUT, "\tsw $t0, %s\n", q->sym1->nom_var_fc);
      fprintf(OUTPUT, "\tj fin_condi_%d\n", cpt_label);

      // cas faux
      fprintf(OUTPUT, "false_%d:\n", cpt_label);
      fprintf(OUTPUT, "\tli $t0, 0\n");
      fprintf(OUTPUT, "\tsw $t0, %s\n", q->sym1->nom_var_fc);

      fprintf(OUTPUT, "fin_condi_%d:\n", cpt_label);

      cpt_label++;

      break;

    default:
      fprintf(OUTPUT, "BUG\n");
      break;
  }
}

void code_dump(Code *c) {
  unsigned int i;

  fprintf(OUTPUT, "\t.text\n");
  // fprintf(OUTPUT,"main:\n");

  Stack *pile_bloc = createStack(MAX_SIZE);
  Stack *pile_if = createStack(MAX_SIZE);
  Stack *pile_while = createStack(MAX_SIZE);
  Stack *pile_for = createStack(MAX_SIZE);

  for (i = 0; i < c->nextquad; i++) {
    quad_dump(pile_bloc, pile_if, pile_while, pile_for, &(c->quads[i]));
  }
  freeStack(pile_bloc);
  freeStack(pile_if);
  freeStack(pile_while);
  freeStack(pile_for);

  fprintf(OUTPUT, "exit: \n");
  fprintf(OUTPUT, "# exit\n");
  fprintf(OUTPUT, "\tli $v0,10\n");
  fprintf(OUTPUT, "\tsyscall\n");
}

void code_free(Code *c) {
  free(c->quads);
  free(c);
}