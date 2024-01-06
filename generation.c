#include "generation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "EXPR.tab.h"
#include "error.h"
#include "pile.h"

unsigned cpt_label = 0;
unsigned cpt_bloc = 0;
unsigned cpt_ctrl = 0;

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
    exit(1);
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
  name_t name;
  sprintf(name, "t%d", t->temporary);
  variable *v = creer_variable(name, type, true, val);
  s = symtable_put(t, name, v);
  ++(t->temporary);
  return s;
}

static void symbol_dump(Symbol *s) {
  switch (s->kind) {
    case NAME:
      printf("%s", s->name);
      break;
    case CONST_INT:
      printf("%d", s->const_int);
      break;
    case CONST_FLOAT:
      printf("%f", s->const_float);
      break;
    default:
      break;
  }
}

static void quad_dump(Stack *pile_bloc, Stack *pile_if, Stack *pile_while,
                      Stack *pile_for, Quad *q) {
  unsigned type1, type2;
  switch (q->kind) {
    case DEBUT_BLOC:
      printf("debut_bloc_%d:\n", cpt_bloc);
      push(pile_bloc, cpt_bloc);
      cpt_bloc++;
      break;

    case FIN_BLOC:
      printf("fin_bloc_%d:\n", pop(pile_bloc));
      break;
    case DEBUT_IF:
      printf("debut_if_%d:\n", cpt_ctrl);
      printf("\tlw $t0, %s\n", q->sym1->name);
      printf("\tbeqz $t0, fin_bloc_%d\n", cpt_bloc);
      push(pile_if, cpt_ctrl);
      cpt_ctrl++;
      break;

    case JUMP_FIN_IF:
      printf("j fin_if_%d\n", top(pile_if));
      break;

    case FIN_IF:
      printf("fin_if_%d:\n", pop(pile_if));
      break;

    case DEBUT_WHILE:
      printf("while_%d:\n", cpt_ctrl);
      push(pile_while, cpt_ctrl);
      cpt_ctrl++;
      break;

    case FIN_WHILE:
      printf("\tbeqz $t0, fin_bloc_%d\n", cpt_bloc);
      break;

    case JUMP_DEBUT_WHILE:
      printf("j while_%d\n", pop(pile_while));
      break;

    case DEBUT_FOR:
      printf("for_%d:\n", cpt_ctrl);
      push(pile_for, cpt_ctrl);
      cpt_ctrl++;
      break;

    case FIN_FOR:
      printf("\tbeqz $t0, fin_bloc_%d\n", cpt_bloc);  // si faux : fin
      printf("\tj debut_bloc_%d\n", cpt_bloc);        // si vrai : dans bloc
      printf("\tfor_maj_%d:\n", cpt_bloc);
      break;

    case JUMP_DEBUT_FOR:
      printf("j for_%d\n", pop(pile_for));
      break;

    case JUMP_MAJ_FOR:
      printf("j for_maj_%d\n", top(pile_bloc));
      break;

    case COPY:

      // int = int
      if (q->sym1->var->type == INT) {
        if (q->sym2->kind == CONST_INT ||
            (q->sym2->kind == NAME && q->sym2->var->type == INT))
          printf("\tlw $t0, %s\n", q->sym2->name);
        else {
          fprintf(stderr, "Incompatibilité de types à l'affectation de %s.\n",
                  q->sym1->name);
          exit(1);
        }  // plutot un assert ?
        printf("\tsw $t0,%s\n", q->sym1->name);
      }

      if (q->sym1->var->type == FLOAT) {
        // float = float
        if ((q->sym2->kind == CONST_FLOAT) ||
            (q->sym2->kind == NAME && q->sym2->var->type == FLOAT))
          printf("\tl.s $f0, %s\n", q->sym2->name);
        // float = int
        else if (q->sym2->kind == CONST_INT ||
                 (q->sym2->kind == NAME && q->sym2->var->type == INT)) {
          printf("\tlw $t0, %s\n", q->sym2->name);
          printf("\tmtc1 $t0, $f0\n");
          printf("\tcvt.s.w $f0, $f0\n");
        } else {
          fprintf(stderr, "Incompatibilité de types à l'affectation de %s.\n",
                  q->sym1->name);
          exit(1);
        }
        printf("\ts.s $f0, %s\n", q->sym1->name);
      }

      // matrix = matrix
      if (q->sym1->var->type == MATRIX) {
        if (q->sym2->kind == NAME && q->sym2->var->type == MATRIX) {
          printf("\tla $a0, %s\n", q->sym2->name);
          printf("\tla $a1, %s\n", q->sym1->name);
          printf("\tlw $t2, %s_rows\n", q->sym1->name);
          printf("\tlw $t3, %s_cols\n", q->sym1->name);

          printf("\tli $t0, 0\n");
          printf("boucle_copy_matrices_ligne_%d:\n", cpt_label);
          printf("\tbge $t0, $t2, fin_copy_%d\n", cpt_label);

          printf("\tli $t1, 0\n");
          printf("\tboucle_copy_matrices_colonne_%d:\n", cpt_label);
          printf("\t\tbge $t1, $t3, ligne_suivante_%d\n", cpt_label);

          printf("\t\tl.s $f0, 0($a0)\n");
          printf("\t\ts.s $f0, 0($a1)\n");

          printf("\t\taddi $a0, $a0, 4\n");
          printf("\t\taddi $a1, $a1, 4\n");
          printf("\t\taddi $t1, $t1, 1\n");

          printf("\t\tj boucle_copy_matrices_colonne_%d\n", cpt_label);
          printf("\tligne_suivante_%d:\n", cpt_label);
          printf("\t\taddi $t0, $t0, 1\n");
          printf("\t\tj boucle_copy_matrices_ligne_%d\n", cpt_label);

          printf("fin_copy_%d:\n", cpt_label);
          cpt_label++;
        } else {
          fprintf(stderr, "Incompatibilité de types à l'affectation de %s.\n",
                  q->sym1->name);
          exit(1);
        }
      }

      break;

    case BOP_PLUS:
      if ((q->sym2->kind == NAME && q->sym2->var->type == MATRIX) &&
          (q->sym3->kind == NAME &&
           q->sym3->var->type == MATRIX)) {  // add 2 matrix
        printf("\tla $a0, %s\n", q->sym2->name);
        printf("\tla $a1, %s\n", q->sym3->name);
        printf("\tla $a2, %s\n", q->sym1->name);
        printf("\tlw $t2, %s_rows\n", q->sym2->name);
        printf("\tlw $t3, %s_cols\n", q->sym2->name);

        printf("\tli $t0, 0\n");
        printf("boucle_add_matrices_ligne_%d:\n", cpt_label);
        printf("\tbge $t0, $t2, fin_add_%d\n", cpt_label);

        printf("\tli $t1, 0\n");
        printf("\tboucle_add_matrices_colonne_%d:\n", cpt_label);
        printf("\t\tbge $t1, $t3, ligne_suivante_%d\n", cpt_label);

        printf("\t\tl.s $f0, 0($a0)\n");
        printf("\t\tl.s $f1, 0($a1)\n");
        printf("\t\tadd.s $f2, $f0, $f1\n");
        printf("\t\ts.s $f2, 0($a2)\n");

        printf("\t\taddi $a0, $a0, 4\n");
        printf("\t\taddi $a1, $a1, 4\n");
        printf("\t\taddi $a2, $a2, 4\n");
        printf("\t\taddi $t1, $t1, 1\n");

        printf("\t\tj boucle_add_matrices_colonne_%d\n", cpt_label);
        printf("\tligne_suivante_%d:\n", cpt_label);
        printf("\t\taddi $t0, $t0, 1\n");
        printf("\t\tj boucle_add_matrices_ligne_%d\n", cpt_label);

        printf("fin_add_%d:\n", cpt_label);

        cpt_label++;
      } else if ((q->sym2->kind == NAME && q->sym2->var->type == MATRIX) ||
                 (q->sym3->kind == NAME &&
                  q->sym3->var->type == MATRIX)) {  // mat + cst ou cst + mat
        name_t mat;
        name_t cst;
        unsigned type = 0;

        if (q->sym2->kind == NAME && q->sym2->var->type == MATRIX) {
          strcpy(mat, q->sym2->name);
          strcpy(cst, q->sym3->name);
          if (q->sym3->kind == NAME) {
            type = q->sym3->var->type;
          } else {
            if (q->sym3->kind == CONST_INT)
              type = INT;
            else if (q->sym3->kind == CONST_FLOAT)
              type = FLOAT;
            else {  // cas impossible ?
              fprintf(stderr, "Incompatibilité de types dans une addition.\n");
              exit(1);
            }
          }
        } else {
          strcpy(mat, q->sym3->name);
          strcpy(cst, q->sym2->name);
          if (q->sym2->kind == NAME) {
            type = q->sym2->var->type;
          } else {
            if (q->sym2->kind == CONST_INT)
              type = INT;
            else if (q->sym2->kind == CONST_FLOAT)
              type = FLOAT;
            else {  // cas impossible ?
              fprintf(stderr, "Incompatibilité de types dans une addition.\n");
              exit(1);
            }
          }
        }

        printf("\tla $a0, %s\n", mat);
        printf("\tlwc1 $f1, %s\n", cst);
        if (type == INT) {
          printf("\tcvt.s.w $f1, $f1\n");
        }
        printf("\tla $a1, %s\n", q->sym1->var->name);
        printf("\tlw $t2, %s_rows\n", mat);
        printf("\tlw $t3, %s_cols\n", mat);

        printf("\tli $t4, 0\n");
        printf("boucle_add_matrice_cst_ligne_%d:\n", cpt_label);
        printf("\tbge $t4, $t2, fin_add_%d\n", cpt_label);

        printf("\tli $t5, 0\n");
        printf("\tboucle_add_matrice_cst_colonne_%d:\n", cpt_label);
        printf("\t\tbge $t5, $t3, ligne_suivante_%d\n", cpt_label);

        printf("\t\tlwc1 $f2, 0($a0)\n");
        printf("\t\tadd.s $f3, $f2, $f1\n");

        printf("\t\tswc1 $f3, 0($a1)\n");

        printf("\t\taddi $a0, $a0, 4\n");
        printf("\t\taddi $a1, $a1, 4\n");
        printf("\t\taddi $t5, $t5, 1\n");

        printf("\t\tj boucle_add_matrice_cst_colonne_%d\n", cpt_label);
        printf("\tligne_suivante_%d:\n", cpt_label);
        printf("\t\taddi $t4, $t4, 1\n");
        printf("\t\tj boucle_add_matrice_cst_ligne_%d\n", cpt_label);

        printf("fin_add_%d:\n", cpt_label);
        cpt_label++;
      } else {  // add de int et de float
        // sym2
        if (q->sym2->kind == CONST_INT ||
            (q->sym2->kind == NAME && q->sym2->var->type == INT)) {
          printf("\tlw $t0, %s\n", q->sym2->name);
          type1 = INT;
        }  // int
        else if ((q->sym2->kind == CONST_FLOAT) ||
                 (q->sym2->kind == NAME && q->sym2->var->type == FLOAT)) {
          printf("\tl.s $f0, %s\n", q->sym2->name);
          type1 = FLOAT;
        }  // float

        // sym3
        if (q->sym3->kind == CONST_INT ||
            (q->sym3->kind == NAME && q->sym3->var->type == INT)) {
          printf("\tlw $t1, %s\n", q->sym3->name);
          type2 = INT;
        }  // int
        else if ((q->sym3->kind == CONST_FLOAT) ||
                 (q->sym3->kind == NAME && q->sym3->var->type == FLOAT)) {
          printf("\tl.s $f1, %s\n", q->sym3->name);
          type2 = FLOAT;
        }  // float

        if (type1 == INT && type2 == INT) {  // add int
          printf("\tadd $t0,$t0,$t1\n");
          printf("\tsw $t0, %s\n", q->sym1->name);
        } else {
          if (type1 == INT && type2 == FLOAT) {
            printf("mtc1 $t0, $f0\n");
            printf("cvt.s.w $f0, $f0\n");
          } else if (type1 == FLOAT && type2 == INT) {
            printf("mtc1 $t1, $f1\n");
            printf("cvt.s.w $f1, $f1\n");
          }  // cast int -> float
          printf("\tadd.s $f2,$f0,$f1\n");
          printf("\ts.s $f2, %s\n", q->sym1->name);  // add float
        }
      }
      break;

    // TODO
    case BOP_MINUS:
      if (q->sym2->kind == CONST_INT)
        printf("\tli $t0, %d\n", q->sym2->var->val.entier);
      else
        printf("\tlw $t0, %s\n", q->sym2->name);
      if (q->sym3->kind == CONST_INT)
        printf("\tli $t1, %d\n", q->sym3->var->val.entier);
      else
        printf("\tlw $t1, %s\n", q->sym3->name);
      printf("\tsub $t0,$t0,$t1\n");
      printf("\tsw $t0, %s\n", q->sym1->name);
      break;

    // TODO
    case BOP_MULT:
      if (q->sym2->kind == CONST_INT)
        printf("\tli $t0, %d\n", q->sym2->var->val.entier);
      else
        printf("\tlw $t0, %s\n", q->sym2->name);
      if (q->sym3->kind == CONST_INT)
        printf("\tli $t1, %d\n", q->sym3->var->val.entier);
      else
        printf("\tlw $t1, %s\n", q->sym3->name);
      printf("\tmul $t0,$t0,$t1\n");
      printf("\tsw $t0, %s\n", q->sym1->name);
      break;

    // TODO
    case UOP_MINUS:
      if (q->sym2->kind == CONST_INT)
        printf("\tli $t0, %d\n", q->sym2->var->val.entier);
      else
        printf("\tlw $t0, %s\n", q->sym2->name);
      printf("\tnegu $t0,$t0\n");
      printf("\tsw $t0, %s\n", q->sym1->name);
      break;

    // extraction
    case EXTR_LIGNE:

      printf("\tlw $t3, %s_rows\n", q->sym1->name);  // sym1_cols
      printf("\tlw $t4, %s_cols\n", q->sym2->name);  // sym2_rows

      printf("\tli $t0, 0\n");  // $t0 = i
      printf("boucle_extrligne_i_%d:\n", cpt_label);
      printf("bge $t0, $t3, i_suivant_%d\n", cpt_label);

      printf("\tli $t1, 0\n");  // $t1 = j
      printf("boucle_extrligne_j_%d:\n", cpt_label);
      printf("\t\tbge $t1, $t4, j_suivant_%d\n", cpt_label);

      printf("\t\tla $a0, %s\n", q->sym2->name);  // matrice originale
      printf("\t\tla $a1, %s\n", q->sym1->name);  // matrice cible
      printf("\t\tla $a2, %s\n", q->sym3->name);
      // liste des indices des colonnes à extraire

      printf("\t\tmove $t2, $t0\n");
      printf("\t\tsll $t2, $t2, 2\n");
      printf("\t\tadd $a2, $a2, $t2\n");
      printf("\t\tlw $t5, 0($a2)\n");  // $t5 = indice[i]

      printf("\t\tlw $t6, %s_cols\n", q->sym2->name);
      printf("\t\tmul $t6, $t5, $t6\n");
      printf("\t\tadd $t6, $t6, $t1 \n");  //
      printf("\t\tsll $t6, $t6, 2\n");
      printf("\t\tadd $a0, $a0, $t6\n");
      printf("\t\tl.s $f0, 0($a0)\n");

      printf("\t\tlw $t7, %s_cols\n", q->sym1->name);
      printf("\t\tmul $t7, $t0, $t7\n");
      printf("\t\tadd $t7, $t7, $t1\n");  //
      printf("\t\tsll $t7, $t7, 2\n");
      printf("\t\tadd $a1, $a1, $t7\n");

      printf("\t\ts.s $f0, 0($a1)\n");

      printf("\t\taddi $t1, $t1, 1\n");  // j++
      printf("\t\tj boucle_extrligne_j_%d\n", cpt_label);

      printf("\tj_suivant_%d:\n", cpt_label);
      printf("\taddi $t0, $t0, 1\n");  // i++
      printf("\tj boucle_extrligne_i_%d\n", cpt_label);
      printf("i_suivant_%d:\n", cpt_label);

      cpt_label++;

      break;

    case EXTR_COLONNE:

      printf("\tlw $t3, %s_cols\n", q->sym1->name);  // sym1_cols
      printf("\tlw $t4, %s_rows\n", q->sym2->name);  // sym2_rows

      printf("\tli $t0, 0\n");  // $t0 = i
      printf("boucle_extrcolonne_i_%d:\n", cpt_label);
      printf("bge $t0, $t3, i_suivant_%d\n", cpt_label);

      printf("\tli $t1, 0\n");  // $t1 = j
      printf("boucle_extrcolonne_j_%d:\n", cpt_label);
      printf("\t\tbge $t1, $t4, j_suivant_%d\n", cpt_label);

      printf("\t\tla $a0, %s\n", q->sym2->name);  // matrice originale
      printf("\t\tla $a1, %s\n", q->sym1->name);  // matrice cible
      printf("\t\tla $a2, %s\n", q->sym3->name);
      // liste des indices des colonnes à extraire

      printf("\t\tmove $t2, $t0\n");
      printf("\t\tsll $t2, $t2, 2\n");
      printf("\t\tadd $a2, $a2, $t2\n");
      printf("\t\tlw $t5, 0($a2)\n");  // $t5 = indice[i]

      printf("\t\tlw $t6, %s_cols\n", q->sym2->name);
      printf("\t\tmul $t6, $t1, $t6\n");
      printf("\t\tadd $t6, $t6, $t5 \n");  // A_cols * j + indice[i]
      printf("\t\tsll $t6, $t6, 2\n");
      printf("\t\tadd $a0, $a0, $t6\n");
      printf("\t\tl.s $f0, 0($a0)\n");

      printf("\t\tlw $t7, %s_cols\n", q->sym1->name);
      printf("\t\tmul $t7, $t1, $t7\n");
      printf("\t\tadd $t7, $t7, $t0\n");  // B_rows * j + i
      printf("\t\tsll $t7, $t7, 2\n");
      printf("\t\tadd $a1, $a1, $t7\n");

      printf("\t\ts.s $f0, 0($a1)\n");

      printf("\t\taddi $t1, $t1, 1\n");  // j++
      printf("\t\tj boucle_extrcolonne_j_%d\n", cpt_label);

      printf("\tj_suivant_%d:\n", cpt_label);
      printf("\taddi $t0, $t0, 1\n");  // i++
      printf("\tj boucle_extrcolonne_i_%d\n", cpt_label);
      printf("i_suivant_%d:\n", cpt_label);

      cpt_label++;

      break;

    case GET_ELEMENT:
      printf("\tli $t0, %d\n", q->sym3->tuple.ligne);
      printf("\tli $t1, %d\n", q->sym3->tuple.colonne);

      printf("\tlw $t2, %s_cols\n", q->sym2->name);

      printf("\tmul $t3, $t0, $t2\n");
      printf("\tadd $t3, $t3, $t1\n");
      printf("\tsll $t3, $t3, 2\n");

      printf("\tla $a0, %s\n", q->sym2->name);
      printf("\tadd $a0, $a0, $t3\n");

      printf("\tl.s $f0, 0($a0)\n");
      printf("\ts.s $f0, %s\n", q->sym1->name);
      break;

    case PUT_ELEMENT:
      printf("\tli $t0, %d\n", q->sym3->tuple.ligne);
      printf("\tli $t1, %d\n", q->sym3->tuple.colonne);

      printf("\tlw $t2, %s_cols\n", q->sym1->name);

      printf("\tmul $t3, $t0, $t2\n");
      printf("\tadd $t3, $t3, $t1\n");
      printf("\tsll $t3, $t3, 2\n");

      printf("\tla $a0, %s\n", q->sym1->name);
      printf("\tadd $a0, $a0, $t3\n");

      if (q->sym2->kind == CONST_INT ||
          (q->sym2->kind == NAME && q->sym2->var->type == INT)) {
        printf("\tlw $t4, %s\n", q->sym2->name);
        printf("\tmtc1 $t4, $f0\n");
        printf("\tcvt.s.w $f0, $f0\n");
      }  // int
      else if ((q->sym2->kind == CONST_FLOAT) ||
               (q->sym2->kind == NAME && q->sym2->var->type == FLOAT)) {
        printf("\tl.s $f0, %s\n", q->sym2->name);
      }  // float
      else {
        fprintf(stderr, "something went wrong\n");
        exit(1);
      }
      printf("\ts.s $f0, 0($a0)\n");

      break;

    // affichage :
    case CALL_PRINT:
      if (q->sym1->var->type == INT) {
        printf("\tli $v0,1\n");
        printf("\tlw $a0, %s\n", q->sym1->name);
      } else if (q->sym1->var->type == FLOAT) {
        printf("\tli $v0,2\n");
        printf("\tlwc1 $f12, %s\n", q->sym1->name);
      }
      printf("\tsyscall\n");
      break;

    case CALL_PRINTMAT:
      // charger les dimensions de la matrice
      printf("\tlw $t0, %s_rows\n", q->sym1->name);
      printf("\tlw $t1, %s_cols\n", q->sym1->name);

      printf("\tli $t2, 0\n");
      printf("boucle_afficher_matrix_%s_ligne_%d:\n", q->sym1->name, cpt_label);

      printf("\tli $t3, 0\n");
      printf("boucle_afficher_matrix_%s_colonne_%d:\n", q->sym1->name,
             cpt_label);

      // indice dans la matrice
      printf("\tmul $t4, $t2, $t1\n");
      printf("\tadd $t5, $t4, $t3\n");

      // afficher l'élément
      printf("\tla $t6, %s\n", q->sym1->name);
      printf("\tmul $t5, $t5, 4\n");
      printf("\tadd $t6, $t6, $t5\n");
      printf("\tlw $t7, 0($t6)\n");
      printf("\tmtc1 $t7, $f12\n");
      // cvt ?

      printf("\tli $v0, 2\n");
      printf("\tsyscall\n");

      // \t
      printf("\tli $v0, 4\n");
      printf("\tla $a0, tab\n");
      printf("\tsyscall\n");

      // indice colonne ++
      printf("\taddi $t3, $t3, 1\n");

      // indice colonne < matrix_cols
      printf("\tbne $t3, $t1, boucle_afficher_matrix_%s_colonne_%d\n",
             q->sym1->name, cpt_label);

      // \n
      printf("\tli $v0, 4\n");
      printf("\tla $a0, newline\n");
      printf("\tsyscall\n");

      // indice ligne ++
      printf("\taddi $t2, $t2, 1\n");

      // indice ligne < matrix_rows
      printf("\tbne $t2, $t0, boucle_afficher_matrix_%s_ligne_%d\n",
             q->sym1->name, cpt_label);
      cpt_label++;
      break;

    case CALL_PRINTF:
      printf("\tli $v0, 4\n");
      printf("\tla $a0, %s\n", q->sym1->name);
      printf("\tsyscall\n");

      break;

    // expression booleenne
    case B_EVAL:
      // assert type
      if (q->sym2->kind == CONST_INT ||
          (q->sym2->kind == NAME && q->sym2->var->type == INT)) {
        printf("\tlw $t0, %s\n", q->sym2->name);
        printf("\tbeqz $t0, false_%d\n", cpt_label);
      } else if ((q->sym2->kind == CONST_FLOAT) ||
                 (q->sym2->kind == NAME && q->sym2->var->type == FLOAT)) {
        printf("\tl.s $f0, %s\n", q->sym2->name);
        printf("\tli.s $f1, 0.0\n");
        printf("\tc.eq.s $f0, $f1\n");
        printf("\tbc1t false_%d\n", cpt_label);
      } else {
        fprintf(stderr, "something went wrong\n");
        exit(1);
      }

      printf("\tli $t0, 1\n");
      printf("\tsw $t0, %s\n", q->sym1->name);
      printf("\tj fin_condi_%d\n", cpt_label);

      printf("false_%d:\n", cpt_label);
      printf("\tli $t0, 0\n");
      printf("\tsw $t0, %s\n", q->sym1->name);

      printf("\tfin_condi_%d:\n", cpt_label);

      cpt_label++;
      break;

    case B_OU:
      printf("\tlw $t0, %s\n", q->sym2->name);
      printf("\tlw $t1, %s\n", q->sym3->name);
      printf("\tor $t0, $t0, $t1\n");
      printf("\tsw $t0, %s\n", q->sym1->name);
      break;

    case B_ET:
      printf("\tlw $t0, %s\n", q->sym2->name);
      printf("\tlw $t1, %s\n", q->sym3->name);
      printf("\tand $t0, $t0, $t1\n");
      printf("\tsw $t0, %s\n", q->sym1->name);
      break;

    case B_NOT:
      printf("\tlw $t0, %s\n", q->sym2->name);
      // printf("\tlw $t1, 1\n");
      printf("\txori $t0, $t0, 1\n");  // not = xor 1
      printf("\tsw $t0, %s\n", q->sym1->name);
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
          exit(1);
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
          exit(1);
          break;
      }
      if (type1 == INT && type2 == INT) {  // si les 2 sont des int
        printf("\tlw $t0, %s\n", q->sym2->name);
        printf("\tlw $t1, %s\n", q->sym3->name);
        printf("\tbge $t0, $t1, false_%d\n", cpt_label);
      } else if (type1 == FLOAT && type2 == FLOAT) {  // si les 2 sont des float
        printf("\tl.s $f0, %s\n", q->sym2->name);
        printf("\tl.s $f1, %s\n", q->sym3->name);
        printf("\tc.lt.s $f0, $f1\n");
        printf("\tbc1f, false_%d\n", cpt_label);
      } else if (type1 == INT && type2 == FLOAT) {  // int et float
        printf("\tlw $t0, %s\n", q->sym2->name);
        printf("\tmtc1 $t0, $f0\n");
        printf("\tcvt.s.w $f0, $f0\n");
        printf("\tl.s $f1, %s\n", q->sym3->name);
        printf("\tc.lt.s $f0, $f1\n");
        printf("\tbc1f, false_%d\n", cpt_label);
      } else if (type1 == FLOAT && type2 == INT) {
        printf("\tl.s $f0, %s\n", q->sym2->name);
        printf("\tlw $t0, %s\n", q->sym3->name);
        printf("\tmtc1 $t0, $f1\n");
        printf("\tcvt.s.w $f1, $f1\n");
        printf("\tc.lt.s $f0, $f1\n");
        printf("\tbc1f, false_%d\n", cpt_label);
      } else {
        fprintf(stderr, "something went wrong\n");
        exit(1);
      }

      // cas vrai
      printf("\tli $t0, 1\n");
      printf("\tsw $t0, %s\n", q->sym1->name);
      printf("\tj fin_condi_%d\n", cpt_label);

      // cas faux
      printf("false_%d:\n", cpt_label);
      printf("\tli $t0, 0\n");
      printf("\tsw $t0, %s\n", q->sym1->name);

      printf("\tfin_condi_%d:\n", cpt_label);

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
          exit(1);
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
          exit(1);
          break;
      }
      if (type1 == INT && type2 == INT) {  // si les 2 sont des int
        printf("\tlw $t0, %s\n", q->sym2->name);
        printf("\tlw $t1, %s\n", q->sym3->name);
        printf("\tbgt $t0, $t1, false_%d\n", cpt_label);
      } else if (type1 == FLOAT && type2 == FLOAT) {  // si les 2 sont des float
        printf("\tl.s $f0, %s\n", q->sym2->name);
        printf("\tl.s $f1, %s\n", q->sym3->name);
        printf("\tc.le.s $f0, $f1\n");
        printf("\tbc1f, false_%d\n", cpt_label);
      } else if (type1 == INT && type2 == FLOAT) {  // int et float
        printf("\tlw $t0, %s\n", q->sym2->name);
        printf("\tmtc1 $t0, $f0\n");
        printf("\tcvt.s.w $f0, $f0\n");
        printf("\tl.s $f1, %s\n", q->sym3->name);
        printf("\tc.le.s $f0, $f1\n");
        printf("\tbc1f, false_%d\n", cpt_label);
      } else if (type1 == FLOAT && type2 == INT) {  // float et int
        printf("\tl.s $f0, %s\n", q->sym2->name);
        printf("\tlw $t0, %s\n", q->sym3->name);
        printf("\tmtc1 $t0, $f1\n");
        printf("\tcvt.s.w $f1, $f1\n");
        printf("\tc.le.s $f0, $f1\n");
        printf("\tbc1f, false_%d\n", cpt_label);
      } else {
        fprintf(stderr, "something went wrong\n");
        exit(1);
      }

      // cas vrai
      printf("\tli $t0, 1\n");
      printf("\tsw $t0, %s\n", q->sym1->name);
      printf("\tj fin_condi_%d\n", cpt_label);

      // cas faux
      printf("false_%d:\n", cpt_label);
      printf("\tli $t0, 0\n");
      printf("\tsw $t0, %s\n", q->sym1->name);

      printf("\tfin_condi_%d:\n", cpt_label);

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
          exit(1);
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
          exit(1);
          break;
      }
      if (type1 == INT && type2 == INT) {  // si les 2 sont des int
        printf("\tlw $t0, %s\n", q->sym2->name);
        printf("\tlw $t1, %s\n", q->sym3->name);
        printf("\tble $t0, $t1, false_%d\n", cpt_label);
      } else if (type1 == FLOAT && type2 == FLOAT) {  // si les 2 sont des float
        printf("\tl.s $f0, %s\n", q->sym2->name);
        printf("\tl.s $f1, %s\n", q->sym3->name);
        printf("\tc.le.s $f0, $f1\n");
        printf("\tbc1t, false_%d\n", cpt_label);
      } else if (type1 == INT && type2 == FLOAT) {  // int et float
        printf("\tlw $t0, %s\n", q->sym2->name);
        printf("\tmtc1 $t0, $f0\n");
        printf("\tcvt.s.w $f0, $f0\n");
        printf("\tl.s $f1, %s\n", q->sym3->name);
        printf("\tc.le.s $f0, $f1\n");
        printf("\tbc1t, false_%d\n", cpt_label);
      } else if (type1 == FLOAT && type2 == INT) {
        printf("\tl.s $f0, %s\n", q->sym2->name);
        printf("\tlw $t0, %s\n", q->sym3->name);
        printf("\tmtc1 $t0, $f1\n");
        printf("\tcvt.s.w $f1, $f1\n");
        printf("\tc.le.s $f0, $f1\n");
        printf("\tbc1t, false_%d\n", cpt_label);
      } else {
        fprintf(stderr, "something went wrong\n");
        exit(1);
      }

      // cas vrai
      printf("\tli $t0, 1\n");
      printf("\tsw $t0, %s\n", q->sym1->name);
      printf("\tj fin_condi_%d\n", cpt_label);

      // cas faux
      printf("false_%d:\n", cpt_label);
      printf("\tli $t0, 0\n");
      printf("\tsw $t0, %s\n", q->sym1->name);

      printf("\tfin_condi_%d:\n", cpt_label);

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
          exit(1);
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
          exit(1);
          break;
      }
      if (type1 == INT && type2 == INT) {  // si les 2 sont des int
        printf("\tlw $t0, %s\n", q->sym2->name);
        printf("\tlw $t1, %s\n", q->sym3->name);
        printf("\tblt $t0, $t1, false_%d\n", cpt_label);
      } else if (type1 == FLOAT && type2 == FLOAT) {  // si les 2 sont des float
        printf("\tl.s $f0, %s\n", q->sym2->name);
        printf("\tl.s $f1, %s\n", q->sym3->name);
        printf("\tc.lt.s $f0, $f1\n");
        printf("\tbc1t, false_%d\n", cpt_label);
      } else if (type1 == INT && type2 == FLOAT) {  // int et float
        printf("\tlw $t0, %s\n", q->sym2->name);
        printf("\tmtc1 $t0, $f0\n");
        printf("\tcvt.s.w $f0, $f0\n");
        printf("\tl.s $f1, %s\n", q->sym3->name);
        printf("\tc.lt.s $f0, $f1\n");
        printf("\tbc1t, false_%d\n", cpt_label);
      } else if (type1 == FLOAT && type2 == INT) {
        printf("\tl.s $f0, %s\n", q->sym2->name);
        printf("\tlw $t0, %s\n", q->sym3->name);
        printf("\tmtc1 $t0, $f1\n");
        printf("\tcvt.s.w $f1, $f1\n");
        printf("\tc.lt.s $f0, $f1\n");
        printf("\tbc1t, false_%d\n", cpt_label);
      } else {
        fprintf(stderr, "something went wrong\n");
        exit(1);
      }

      // cas vrai
      printf("\tli $t0, 1\n");
      printf("\tsw $t0, %s\n", q->sym1->name);
      printf("\tj fin_condi_%d\n", cpt_label);

      // cas faux
      printf("false_%d:\n", cpt_label);
      printf("\tli $t0, 0\n");
      printf("\tsw $t0, %s\n", q->sym1->name);

      printf("\tfin_condi_%d:\n", cpt_label);

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
          exit(1);
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
          exit(1);
          break;
      }
      if (type1 == INT && type2 == INT) {  // si les 2 sont des int
        printf("\tlw $t0, %s\n", q->sym2->name);
        printf("\tlw $t1, %s\n", q->sym3->name);
        printf("\tbne $t0, $t1, false_%d\n", cpt_label);
      } else if (type1 == FLOAT && type2 == FLOAT) {  // si les 2 sont des float
        printf("\tl.s $f0, %s\n", q->sym2->name);
        printf("\tl.s $f1, %s\n", q->sym3->name);
        printf("\tc.eq.s $f0, $f1\n");
        printf("\tbc1f, false_%d\n", cpt_label);
      } else if (type1 == INT && type2 == FLOAT) {  // int et float
        printf("\tlw $t0, %s\n", q->sym2->name);
        printf("\tmtc1 $t0, $f0\n");
        printf("\tcvt.s.w $f0, $f0\n");
        printf("\tl.s $f1, %s\n", q->sym3->name);
        printf("\tc.eq.s $f0, $f1\n");
        printf("\tbc1f, false_%d\n", cpt_label);
      } else if (type1 == FLOAT && type2 == INT) {
        printf("\tl.s $f0, %s\n", q->sym2->name);
        printf("\tlw $t0, %s\n", q->sym3->name);
        printf("\tmtc1 $t0, $f1\n");
        printf("\tcvt.s.w $f1, $f1\n");
        printf("\tc.eq.s $f0, $f1\n");
        printf("\tbc1f, false_%d\n", cpt_label);
      } else {
        fprintf(stderr, "something went wrong\n");
        exit(1);
      }

      // cas vrai
      printf("\tli $t0, 1\n");
      printf("\tsw $t0, %s\n", q->sym1->name);
      printf("\tj fin_condi_%d\n", cpt_label);

      // cas faux
      printf("false_%d:\n", cpt_label);
      printf("\tli $t0, 0\n");
      printf("\tsw $t0, %s\n", q->sym1->name);

      printf("\tfin_condi_%d:\n", cpt_label);

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
          exit(1);
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
          exit(1);
          break;
      }
      if (type1 == INT && type2 == INT) {  // si les 2 sont des int
        printf("\tlw $t0, %s\n", q->sym2->name);
        printf("\tlw $t1, %s\n", q->sym3->name);
        printf("\tbeq $t0, $t1, false_%d\n", cpt_label);
      } else if (type1 == FLOAT && type2 == FLOAT) {  // si les 2 sont des float
        printf("\tl.s $f0, %s\n", q->sym2->name);
        printf("\tl.s $f1, %s\n", q->sym3->name);
        printf("\tc.eq.s $f0, $f1\n");
        printf("\tbc1t, false_%d\n", cpt_label);
      } else if (type1 == INT && type2 == FLOAT) {  // int et float
        printf("\tlw $t0, %s\n", q->sym2->name);
        printf("\tmtc1 $t0, $f0\n");
        printf("\tcvt.s.w $f0, $f0\n");
        printf("\tl.s $f1, %s\n", q->sym3->name);
        printf("\tc.eq.s $f0, $f1\n");
        printf("\tbc1t, false_%d\n", cpt_label);
      } else if (type1 == FLOAT && type2 == INT) {
        printf("\tl.s $f0, %s\n", q->sym2->name);
        printf("\tlw $t0, %s\n", q->sym3->name);
        printf("\tmtc1 $t0, $f1\n");
        printf("\tcvt.s.w $f1, $f1\n");
        printf("\tc.eq.s $f0, $f1\n");
        printf("\tbc1t, false_%d\n", cpt_label);
      } else {
        fprintf(stderr, "something went wrong\n");
        exit(1);
      }

      // cas vrai
      printf("\tli $t0, 1\n");
      printf("\tsw $t0, %s\n", q->sym1->name);
      printf("\tj fin_condi_%d\n", cpt_label);

      // cas faux
      printf("false_%d:\n", cpt_label);
      printf("\tli $t0, 0\n");
      printf("\tsw $t0, %s\n", q->sym1->name);

      printf("\tfin_condi_%d:\n", cpt_label);

      cpt_label++;

      break;

    default:
      printf("BUG\n");
      break;
  }
}

void code_dump(Code *c) {
  unsigned int i;

  printf("\t.text\n");
  printf("main:\n");

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

  printf("# exit\n");
  printf("\tli $v0,10\n");
  printf("\tsyscall\n");
}

void code_free(Code *c) {
  free(c->quads);
  free(c);
}