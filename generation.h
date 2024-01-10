#ifndef GENERATION_H
#define GENERATION_H

#include "symbtab.h"

typedef struct _quad {
  enum quad_kind {
    BOP_PLUS,
    BOP_MINUS,
    BOP_MULT,
    BOP_DIV,
    UOP_MINUS,
    UOP_TILDE,
    COPY,
    EXTR_LIGNE,
    EXTR_COLONNE,
    CALL_PRINT,
    CALL_PRINTMAT,
    CALL_PRINTF,
    GET_ELEMENT,
    PUT_ELEMENT,
    B_EVAL,
    B_OU,
    B_ET,
    B_NOT,
    B_LT,
    B_LTOE,
    B_GT,
    B_GTOE,
    B_EQUAL,
    B_NOT_EQUAL,

    DEBUT_BLOC,
    FIN_BLOC,
    DEBUT_IF,
    FIN_IF,
    JUMP_FIN_IF,
    DEBUT_WHILE,
    FIN_WHILE,
    JUMP_DEBUT_WHILE,
    DEBUT_FOR,
    FIN_FOR,
    JUMP_DEBUT_FOR,
    JUMP_MAJ_FOR,

    LABEL_FC,
    JAL_FC,
    JR,
    RETOUR_FC,
    MOVE,
    EMPILER,
    DEPILER,
    DEPILER_ADRESSE,
    RE_EMPILER_ADRESSE,

  } kind;
  Symbol *sym1;
  Symbol *sym2;
  Symbol *sym3;
  unsigned int num;
} Quad;

typedef struct _code {
  unsigned int capacity;
  unsigned int nextquad;
  Quad *quads;
} Code;

/**
 * @brief Crée un nouveau bloc de code
 *
 * @return Code*
 */
Code *code_new();

/**
 * @brief Génère le code correspondant au quad pour les 3 Symbols
 *
 * @param c le code
 * @param k le quad
 * @param s1 premier Symbol
 * @param s2 deuxième Symbol
 * @param s3 troisième Symbol
 */
void gencode(Code *c, enum quad_kind k, Symbol *s1, Symbol *s2, Symbol *s3);

/**
 * @brief Crée une nouvelle variable temporaire dans une SymTable
 *
 * @param t la SymTable
 * @param type le type de la variable à ajouter
 * @param v la valeur associée à cette variable
 * @return Symbol*
 */
Symbol *newtemp(SymTable *t, unsigned type, valeur v);

/**
 * @brief Génère le code stocké dans Code
 *
 * @param c le code
 */
void code_dump(Code *c);

/**
 * @brief Supprime le code (nettoie la mémoire)
 *
 * @param c le code à supprimer
 */
void code_free(Code *c);

/**
 * @brief Ajoute une fonction à la liste des fonctions
 *
 * @param fc le nom de la fonction
 */
void add_fonction(nom_fonction fc);

#endif