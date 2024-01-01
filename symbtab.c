#include "symbtab.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned string_num = 1;
unsigned const_int_num = 1;
unsigned const_float_num = 1;

struct symtable *symtable_new() {
  struct symtable *t = malloc(sizeof(struct symtable));
  t->capacity = 1024;
  t->symbols = malloc(t->capacity * sizeof(struct symbol));
  t->temporary = 0;
  t->size = 0;
  return t;
}

static void symtable_grow(struct symtable *t) {
  t->capacity += 1024;
  t->symbols = realloc(t->symbols, t->capacity * sizeof(struct symbol));
  if (t->symbols == NULL) {
    fprintf(stderr,
            "Erreur lors de la tentative d'aggreandissement de la table des "
            "symboles (taille actuelle = %d)\n",
            t->size);
    exit(1);
  }
}

struct symbol *symtable_const_int(struct symtable *t, int v) {
  unsigned int i;
  for (i = 0; i < t->size && t->symbols[i].const_int != v; i++)
    ;
  if (i == t->size) {
    if (t->size == t->capacity) symtable_grow(t);
    struct symbol *s = &(t->symbols[t->size]);
    s->kind = CONST_INT;
    sprintf(s->name, "%s%d", "cint", const_int_num);
    const_int_num++;
    s->const_int = v;
    ++(t->size);
    return s;
  } else {
    return &(t->symbols[i]);
  }
}

struct symbol *symtable_const_float(struct symtable *t, float v) {
  unsigned int i;
  for (i = 0; i < t->size && t->symbols[i].const_float != v; i++)
    ;
  if (i == t->size) {
    if (t->size == t->capacity) symtable_grow(t);
    struct symbol *s = &(t->symbols[t->size]);
    s->kind = CONST_FLOAT;
    sprintf(s->name, "%s%d", "cflt", const_float_num);
    const_float_num++;
    s->const_float = v;
    ++(t->size);
    return s;
  } else {
    return &(t->symbols[i]);
  }
}

struct symbol *symtable_string(struct symtable *t, const char *string) {
  if (t->size == t->capacity) symtable_grow(t);
  struct symbol *s = &(t->symbols[t->size]);
  s->kind = STRING;
  sprintf(s->name, "%s%d", "str", string_num);
  string_num++;
  s->string = string;
  ++(t->size);
  return s;
}

struct symbol *symtable_get(struct symtable *t, const char *id) {
  unsigned int i;
  for (i = 0; i < t->size && strcmp(t->symbols[i].name, id) != 0; i++)
    ;
  if (i < t->size) return &(t->symbols[i]);
  return NULL;
}

struct symbol *symtable_put(struct symtable *t, const char *id, variable *var) {
  if (t->size == t->capacity) symtable_grow(t);
  struct symbol *s = &(t->symbols[t->size]);
  s->kind = NAME;
  strcpy(s->name, id);
  s->var = var;
  ++(t->size);
  return s;
}

void symtable_dump(struct symtable *t) {
  unsigned int i;
  for (i = 0; i < t->size; i++) {
    if (t->symbols[i].kind == CONST_INT)
      printf("       %p = %d\n", &(t->symbols[i]),
             t->symbols[i].var->val.entier);
    if (t->symbols[i].kind == NAME)
      printf("       %p = %s\n", &(t->symbols[i]), t->symbols[i].name);
  }
  printf("       --------\n");
}

void symtable_free(struct symtable *t) {
  free(t->symbols);
  free(t);
}

// VARIABLE

variable *creer_variable(name_t name, unsigned type, bool init, valeur val) {
  variable *var = (variable *)malloc(sizeof(variable));
  strcpy(var->name, name);
  var->type = type;
  var->init = init;
  var->val = val;
  return var;
}

variable **creer_var_liste(variable *v1) {
  variable **liste = (variable **)malloc(sizeof(variable *));
  liste[0] = v1;
  return liste;
}

variable **ajouter_var_liste(variable **liste, unsigned taille, variable *var) {
  liste = (variable **)realloc(liste, taille * sizeof(variable *));
  liste[taille - 1] = var;
  return liste;
}

void delete_var(variable *var) {
  free(var->name);
  /*if (var->type == MATRIX) {
    delete_matrix(var->valeur.matrix);
  }*/
  free(var);
}

// MATRIX

Matrix *create_matrix(unsigned l, unsigned c) {
  Matrix *matrix = (Matrix *)malloc(sizeof(Matrix));
  matrix->l = l;
  matrix->c = c;
  matrix->data = (float *)calloc(l * c, sizeof(float));
  return matrix;
}

void printmat(Matrix *matrix) {
  for (unsigned i = 0; i < matrix->l; i++) {
    for (unsigned j = 0; j < matrix->c; j++) {
      printf("%f\t", matrix->data[(i * matrix->c) + j]);
    }
    printf("\n");
  }
}

#if 0
Matrix* extraction(Matrix* a, int* lignes, int* colonnes, int n, int m) {
  Matrix* mnew = create_matrix(n, m);
  int k = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      int l = lignes[i];
      int c = colonnes[j];
      mnew->data[k++] = a->data[l * a->c + c];
    }
  }
  return mnew;
}
#endif

void delete_matrix(Matrix *matrix) {
  free(matrix->data);
  free(matrix);
}

void put_value_at(Matrix *m, unsigned ligne, unsigned colonne, float valeur) {
  assert(ligne < m->l && colonne < m->c);
  m->data[ligne * m->c + colonne] = valeur;
}

void add_colonne(Matrix *m) {
  assert(m->l == 1);
  m->c++;
  m->data = (float *)realloc(m->data, m->c * sizeof(float));
}

void add_ligne(Matrix *m, Matrix *m2) {
  assert(m->c == m2->c);
  assert(m2->l == 1);
  m->l++;
  m->data = (float *)realloc(m->data, m->l * m->c * sizeof(float));
  for (unsigned i = 0; i < (m->c); i++) {
    m->data[(m->l - 1) * (m->c) + i] = m2->data[i];
  }
  delete_matrix(m2);
}

/*
int *creer_liste_extract(int valeur) {
  int *v = (int *)malloc(sizeof(int));
  *v = valeur;
  return v;
}

int *creer_liste_extract_intervalle(int vmin, int vmax) {
  int *l = (int *)malloc(sizeof(int) * (vmax - vmin + 1));
  for (int i = 0; i < (vmax - vmin + 1); i++) {
    l[i] = vmin + i;
  }
  return l;
}

int *concat_extract_liste(int *liste1, int *liste2) {
  liste1 = (int *)realloc(liste1, sizeof(liste1) + sizeof(liste2));
  for (int i = 0; i < sizeof(liste2) / sizeof(int); i++) {
    liste1[sizeof(liste1) / sizeof(int) + i] = liste2[i];
  }
  free(liste2);
  return liste1;
}
*/