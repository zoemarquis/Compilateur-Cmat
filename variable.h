#ifndef VARIABLE_H
#define VARIABLE_H

#include <stdbool.h>

#include "matrix.h"
#include "symbtab.h"

typedef char nom_var[16];
typedef char nom_var_fonction[32];

typedef union _valeur {
  int entier;
  float flottant;
  Matrix *matrix;
} valeur;

typedef struct _var {
  nom_var name;
  nom_var_fonction nom_var_fc;
  unsigned type;  // flag
  bool init;      // si la variable a été initialisée
  valeur val;
} variable;

typedef struct _tuple_declaration {
  variable *gauche;        // la variable entrain de se faire déclarer
  struct _symbol *droite;  // résultat de l'expression ou matrix_litt
} Tuple_Declaration;

typedef struct _liste_tuple_decla {
  Tuple_Declaration *liste;
  unsigned taille;
} Liste_Tuple_Declaration;

/**
 * @brief Crée une nouvelle variable.
 *
 * @param name nom de la nouvelle variable
 * @param type type de cette variable
 * @param init marqueur pour savoir si cette variable a été initialisée
 * @param val valeur de la variable
 * @return variable*
 */
variable *creer_variable(nom_var name, unsigned type, bool init, valeur val);

/**
 * @brief Crée une liste de tuple pour la déclaration des variables et place le
 * tuple en paramètre comme le premier tuple de la liste.
 *
 * @param t premier tuple de la liste
 * @return Liste_Tuple_Declaration
 */
Liste_Tuple_Declaration creer_liste_tuple_declaration(Tuple_Declaration t);

/**
 * @brief Ajoute un tuple à une liste de déclaration.
 *
 * @param liste la liste existante
 * @param t le tuple à ajouter
 * @return Liste_Tuple_Declaration
 */
Liste_Tuple_Declaration ajouter_tuple(Liste_Tuple_Declaration liste,
                                      Tuple_Declaration t);

/**
 * @brief Supprime la variable (nettoie la mémoire).
 *
 * @param var pointeur sur la variable à supprimer
 */
void delete_var(variable *var);

#endif