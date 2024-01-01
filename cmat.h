#ifndef EXPRC_H
#define EXPRC_H

#include <stdio.h>
#include <stdlib.h>

#include "generation.h"
#include "symbtab.h"

extern SymTable* SYMTAB;
extern Code* CODE;

extern int yyparse();

#endif