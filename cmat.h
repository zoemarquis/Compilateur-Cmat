#ifndef EXPRC_H
#define EXPRC_H

#include <stdio.h>
#include <stdlib.h>

#include "generation.h"
#include "symbtab.h"

extern struct symtable* SYMTAB;
extern struct code* CODE;

extern int yyparse();

#endif