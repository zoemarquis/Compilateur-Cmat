#ifndef HASHTAB_H
#define HASHTAB_H

#include <stdbool.h>

#include "symbtab.h"
#include "uthash.h"

typedef SymTable *Table_Hachage;

SymTable *get_symtable(char *name);

bool add_symtable(SymTable *s);

void print_table_hachage();

void delete_table_hachage();

void print_zone_data();

#endif