#ifndef GENERATION_H
#define GENERATION_H

#include "symbtab.h"

struct quad {
  enum quad_kind {
    BOP_PLUS,
    BOP_MINUS,
    BOP_MULT,
    UOP_MINUS,
    COPY,
    CALL_PRINT,
    CALL_PRINTMAT,
    CALL_PRINTF
  } kind;
  struct symbol *sym1;
  struct symbol *sym2;
  struct symbol *sym3;
};

struct code {
  unsigned int capacity;
  unsigned int nextquad;
  struct quad *quads;
};

struct code *code_new();

void gencode(struct code *c, enum quad_kind k, struct symbol *s1,
             struct symbol *s2, struct symbol *s3);

struct symbol *newtemp(struct symtable *t, unsigned type, valeur v);

void code_dump(struct code *c);

void code_free(struct code *c);

#endif