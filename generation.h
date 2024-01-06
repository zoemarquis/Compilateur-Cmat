#ifndef GENERATION_H
#define GENERATION_H

#include "symbtab.h"

typedef struct _quad {
  enum quad_kind {
    BOP_PLUS,
    BOP_MINUS,
    BOP_MULT,
    UOP_MINUS,
    COPY,
    EXTR_LIGNE,
    EXTR_COLONNE,
    CALL_PRINT,
    CALL_PRINTMAT,
    CALL_PRINTF,
    GET_ELEMENT,
    PUT_ELEMENT,
    B_EVAL,
    B_OU,
    B_ET,
    B_NOT,
    B_LT,
    B_LTOE,
    B_GT,
    B_GTOE,
    B_EQUAL,
    B_NOT_EQUAL,
    DEBUT_BLOC,
    FIN_BLOC,
    DEBUT_IF,
    FIN_IF,
    JUMP_FIN_IF,
    DEBUT_WHILE,
    FIN_WHILE,
    JUMP_DEBUT_WHILE,
    DEBUT_FOR,
    FIN_FOR,
    JUMP_DEBUT_FOR,
    JUMP_MAJ_FOR,
  } kind;
  Symbol *sym1;
  Symbol *sym2;
  Symbol *sym3;
  unsigned int num;
} Quad;

typedef struct _code {
  unsigned int capacity;
  unsigned int nextquad;
  Quad *quads;
} Code;

Code *code_new();

void gencode(Code *c, enum quad_kind k, Symbol *s1, Symbol *s2, Symbol *s3);

Symbol *newtemp(SymTable *t, unsigned type, valeur v);

void code_dump(Code *c);

void code_free(Code *c);

#endif