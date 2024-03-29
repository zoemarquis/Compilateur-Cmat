#include "symbtab.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "EXPR.tab.h"
#include "cmat.h"
#include "hashtab.h"
#include "parametre.h"

unsigned string_num = 1;
unsigned const_int_num = 1;
unsigned const_float_num = 1;
unsigned extract_num = 1;
unsigned indices_num = 1;

SymTable *symtable_new(char *fonction) {
  SymTable *t = malloc(sizeof(SymTable));
  if (t == NULL) {  // Échec de l'allocation, gérer l'erreur
    fprintf(stderr, "Allocation mémoire échouée\n");
    exit(MEMORY_FAILURE);
  }
  strcpy(t->nom, fonction);
  // sprintf(t->nom, "%s", fonction);
  t->capacity = 1024;
  t->symbols = malloc(t->capacity * sizeof(Symbol));
  if (t->symbols == NULL) {  // Échec de l'allocation, gérer l'erreur
    fprintf(stderr, "Allocation mémoire échouée\n");
    exit(MEMORY_FAILURE);
  }
  t->temporary = 0;
  t->size = 0;

  // créer une zone pour param
  t->param = init_param();
  return t;
}

static void symtable_grow(SymTable *t) {
  t->capacity += 1024;
  t->symbols = realloc(t->symbols, t->capacity * sizeof(Symbol));
  if (t->symbols == NULL) {  // Échec de l'allocation, gérer l'erreur
    fprintf(stderr, "Allocation mémoire échouée\n");
    exit(MEMORY_FAILURE);
  }
}

Symbol *symtable_const_int(SymTable *t, int v) {
  unsigned int i;
  for (i = 0; i < t->size /*&& t->symbols[i].const_int != v*/; i++) {
    if (t->symbols[i].const_int == v && CONST_INT == t->symbols[i].kind) break;
    // CONFLITS AVEC INDICES QUI SONT DES INT AUSSI
    /*
    if (t->symbols[i].const_int == v) {
      fprintf(stderr, "type : %d\n", t->symbols[i].kind);
      fprintf(stderr, "EXTRACT : %d\n", EXTRACT);
      fprintf(stderr, "INDICES : %d\n", INDICES);
    }
    */
  }
  if (i == t->size) {
    if (t->size == t->capacity) symtable_grow(t);
    Symbol *s = &(t->symbols[t->size]);
    s->kind = CONST_INT;
    sprintf(s->name, "%s%d", "cint", const_int_num);
    const_int_num++;
    s->const_int = v;
    sprintf(s->nom_var_fc, "%s%s", t->nom, s->name);  // nom pour zone data
    ++(t->size);
    return s;
  } else {
    return &(t->symbols[i]);
  }
}

Symbol *symtable_const_float(SymTable *t, float v) {
  unsigned int i;
  for (i = 0; i < t->size && t->symbols[i].const_float != v; i++)
    ;
  if (i == t->size) {
    if (t->size == t->capacity) symtable_grow(t);
    Symbol *s = &(t->symbols[t->size]);
    s->kind = CONST_FLOAT;
    sprintf(s->name, "%s%d", "cflt", const_float_num);
    const_float_num++;
    s->const_float = v;
    sprintf(s->nom_var_fc, "%s%s", t->nom, s->name);  // nom pour zone data
    ++(t->size);
    return s;
  } else {
    return &(t->symbols[i]);
  }
}

Symbol *symtable_string(SymTable *t, char *string) {
  if (t->size == t->capacity) symtable_grow(t);
  Symbol *s = &(t->symbols[t->size]);
  s->kind = STRING;
  sprintf(s->name, "%s%d", "str", string_num);
  string_num++;
  s->string = string;
  // strcpy(s->string, string);
  sprintf(s->nom_var_fc, "%s%s", t->nom, s->name);  // nom pour zone data
  ++(t->size);
  return s;
}

Symbol *symtable_extract(SymTable *t, Extract extract) {
  if (t->size == t->capacity) symtable_grow(t);
  Symbol *s = &(t->symbols[t->size]);
  s->kind = EXTRACT;
  sprintf(s->name, "%s%d", "extr", extract_num);
  extract_num++;
  s->extr = extract;
  sprintf(s->nom_var_fc, "%s%s", t->nom, s->name);  // nom pour zone data
  ++(t->size);
  return s;
}

Symbol *symtable_indices(SymTable *t, Indices tuple) {
  if (t->size == t->capacity) symtable_grow(t);
  Symbol *s = &(t->symbols[t->size]);
  s->kind = INDICES;
  sprintf(s->name, "%s%d", "ind", indices_num);
  indices_num++;
  s->tuple = tuple;
  sprintf(s->nom_var_fc, "%s%s", t->nom, s->name);  // nom pour zone data
  ++(t->size);
  return s;
}

Symbol *symtable_get(SymTable *t, char *id) {
  unsigned int i;
  for (i = 0; i < GLOBAL->size && strcmp(GLOBAL->symbols[i].name, id) != 0; i++)
    ;
  if (i < GLOBAL->size) return &(GLOBAL->symbols[i]);
  for (i = 0; i < t->size && strcmp(t->symbols[i].name, id) != 0; i++)
    ;
  if (i < t->size) return &(t->symbols[i]);
  return NULL;
}

Symbol *symtable_put(SymTable *t, char *id, variable *var) {
  // il faudrait assert que symtable_get retourne null
  if (t->size == t->capacity) symtable_grow(t);
  Symbol *s = &(t->symbols[t->size]);
  s->kind = NAME;
  strcpy(s->name, id);
  s->var = var;
  sprintf(var->nom_var_fc, "%s%s", t->nom, s->name);
  sprintf(s->nom_var_fc, "%s%s", t->nom, s->name);  // nom pour zone data
  ++(t->size);
  return s;
}

void symtable_free(SymTable *t) {
  unsigned int i;
  free(t->param->liste);
  free(t->param);
  for (i = 0; i < t->size; i++) {
    if (t->symbols[i].kind == NAME) {
      delete_var(t->symbols[i].var);
    } else if (t->symbols[i].kind == STRING) {
      free(t->symbols[i].string);
    } else if (t->symbols[i].kind == EXTRACT) {
      free(t->symbols[i].extr.liste);
    }
  }
  free(t->symbols);
  free(t);
}
