#ifndef CMAT_H
#define CMAT_H

#include <assert.h>

#include "generation.h"
#include "parametre.h"
#include "symbtab.h"

extern SymTable* SYMTAB;
extern Code* CODE;
extern SymTable* GLOBAL;

extern int yyparse();

#endif