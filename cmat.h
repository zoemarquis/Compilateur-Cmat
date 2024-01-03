#ifndef EXPRC_H
#define EXPRC_H

#include <assert.h>

#include "generation.h"
#include "symbtab.h"

extern SymTable* SYMTAB;
extern Code* CODE;

extern int yyparse();

#endif