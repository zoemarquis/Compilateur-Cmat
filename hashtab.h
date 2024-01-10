#ifndef HASHTAB_H
#define HASHTAB_H

#include <stdbool.h>

#include "parametre.h"
#include "symbtab.h"
#include "uthash.h"

typedef SymTable *Table_Hachage;

/**
 * @brief Retourne la SymTable correspondant au nom associé.
 *
 * @param name le nom de la symtable recherchée
 * @return SymTable*
 */
SymTable *get_symtable(char *name);

/**
 * @brief Ajoute une SymTable à la table de hachage
 *
 * @param s la SymTable à ajouter
 * @return true si l'ajout est effectif
 * @return false si une symtable à ce nom existe déjà
 */
bool add_symtable(SymTable *s);

/**
 * @brief Affiche le contenu de la table de hachage
 *
 */
void print_table_hachage();

/**
 * @brief Supprime le contenu de la table de hachage (nettoie la mémoire)
 *
 */
void delete_table_hachage();

/**
 * @brief Affiche la zone du ".data" de MIPS
 *
 */
void print_zone_data();

#endif