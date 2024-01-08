#ifndef CMAT_H
#define CMAT_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "generation.h"
#include "symbtab.h"

extern SymTable* SYMTAB;
extern Code* CODE;
extern SymTable* GLOBAL;
extern FILE* OUTPUT;

extern int yyparse();

#endif