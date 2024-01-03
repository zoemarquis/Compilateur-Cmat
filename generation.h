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
    PUT_ELEMENT
  } kind;
  Symbol *sym1;
  Symbol *sym2;
  Symbol *sym3;
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