#ifndef PARAMETRE_H
#define PARAMETRE_H

#include "symbtab.h"

typedef struct _param {
  struct _symbol **liste;
  unsigned nb;
} Parametres;

/**
 * @brief Ajoute un Symbol à la liste de paramètres
 *
 * @param p la liste de paramètres
 * @param s le symbol à ajouter
 * @return Parametres*
 */
Parametres *add_parametre(Parametres *p, struct _symbol *s);

/**
 * @brief Récupérer le Symbol à la indice ième place dans la liste de
 * paramètres
 *
 * @param p la liste de paramètre
 * @param indice indice dans la liste du paramètre cherché
 * @return struct _symbol*
 */
struct _symbol *get_parametre(Parametres p, unsigned indice);

/**
 * @brief Crée une liste de paramètres.
 *
 * @return Parametres*
 */
Parametres *init_param();

#endif