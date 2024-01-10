#ifndef SYMBTAB_H
#define SYMBTAB_H

#include <stdbool.h>

#include "error.h"
#include "extract.h"
#include "matrix.h"
#include "parametre.h"
#include "uthash.h"
#include "variable.h"

typedef char nom_var[16];
typedef char nom_var_fonction[32];
typedef char nom_fonction[16];

typedef struct _tuple {
  int ligne;
  int colonne;
} Indices;

typedef struct _symbol {
  enum {
    NAME,
    CONST_INT,
    CONST_FLOAT,
    STRING,
    EXTRACT,
    INDICES,
    FONCTION,
    PARAMETRE
  } kind;
  nom_var name;
  nom_var_fonction nom_var_fc;
  union {
    struct _var *var;
    char *string;
    int const_int;
    float const_float;
    Extract extr;
    Indices tuple;
    struct _symtable *st;
    struct _param *param;
  };
} Symbol;

typedef struct _symtable {
  unsigned int capacity;
  unsigned int temporary;
  unsigned int size;
  Symbol *symbols;
  struct _param *param;
  nom_fonction nom;
  unsigned type_fonction;
  UT_hash_handle hh;
} SymTable;

/**
 * @brief Crée une nouvelle table des symboles.
 *
 * @param fonction Nom associé à la fonction associée à cette table des symboles
 * @return SymTable*
 */
SymTable *symtable_new(char *fonction);

/**
 * @brief Crée une nouvelle constante entière dans la table des symboles.
 *
 * @param t la table des symboles
 * @param v la valeur de l'entier
 * @return Symbol*
 */
Symbol *symtable_const_int(SymTable *t, int v);

/**
 * @brief Crée une nouvelle constante flottante dans la table des symboles.
 *
 * @param t la table des symboles
 * @param v la valeur du flottant
 * @return Symbol*
 */
Symbol *symtable_const_float(SymTable *t, float v);

/**
 * @brief Crée une nouvelle entrée dans la table des symboles pour une chaine de
 * caractères.
 *
 * @param t la table des symboles
 * @param string la chaine de caractères
 * @return Symbol*
 */
Symbol *symtable_string(SymTable *t, char *string);

/**
 * @brief Crée une nouvelle entrée dans la table des symboles pour un extract.
 * (liste d'entiers correspondant aux indices des lignes à extraire)
 *
 * @param t
 * @param extract
 * @return Symbol*
 */
Symbol *symtable_extract(SymTable *t, Extract extract);

/**
 * @brief Crée une nouvelle entrée dans la table des symboles pour des indices
 * (ligne et colonne).
 *
 * @param t la table des symboles
 * @param tuple la tuple d'indices ligne colonne
 * @return Symbol*
 */
Symbol *symtable_indices(SymTable *t, Indices tuple);

/**
 * @brief Récupère le symbole associé à un nom dans la table des symboles.
 *
 * @param t la table des symboles
 * @param s le nom
 * @return Symbol*
 */
Symbol *symtable_get(SymTable *t, char *s);

/**
 * @brief Ajoute une variable dans la table des symboles.
 *
 * @param t la table des symboles
 * @param id le nom associé
 * @param var la variable
 * @return Symbol*
 */
Symbol *symtable_put(SymTable *t, char *id, struct _var *var);

/**
 * @brief Supprime toutes les entrées de la table des symboles (nettoie la
 * mémoire).
 *
 * @param t la table des symboles
 */
void symtable_free(SymTable *t);

#endif