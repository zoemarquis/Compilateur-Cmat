#include "variable.h"

#include <stdio.h>
#include <stdlib.h>

#include "EXPR.tab.h"

variable *creer_variable(nom_var name, unsigned type, bool init, valeur val) {
  variable *var = (variable *)malloc(sizeof(variable));
  if (var == NULL) {  // Échec de l'allocation, gérer l'erreur
    fprintf(stderr, "Allocation mémoire échouée\n");
    exit(MEMORY_FAILURE);
  }
  strcpy(var->name, name);
  var->type = type;
  var->init = init;
  var->val = val;
  if (!init) {
    if (type == INT) {
      var->val.entier = 0;
    } else if (type == FLOAT) {
      var->val.flottant = 0;
    }
  }
  return var;
}

void delete_var(variable *var) {
  if (var->type == MATRIX) {
    delete_matrix(var->val.matrix);
  }
  if (var) free(var);
}

Liste_Tuple_Declaration creer_liste_tuple_declaration(Tuple_Declaration t) {
  Tuple_Declaration *td =
      (Tuple_Declaration *)malloc(sizeof(Tuple_Declaration));
  if (td == NULL) {  // Échec de l'allocation, gérer l'erreur
    fprintf(stderr, "Allocation mémoire échouée\n");
    exit(MEMORY_FAILURE);
  }
  *td = t;
  return (Liste_Tuple_Declaration){td, 1};
}

Liste_Tuple_Declaration ajouter_tuple(Liste_Tuple_Declaration liste,
                                      Tuple_Declaration t) {
  Tuple_Declaration *td = (Tuple_Declaration *)malloc(
      sizeof(Tuple_Declaration) * (liste.taille + 1));
  if (td == NULL) {  // Échec de l'allocation, gérer l'erreur
    fprintf(stderr, "Allocation mémoire échouée\n");
    exit(MEMORY_FAILURE);
  }
  for (unsigned i = 0; i < liste.taille; i++) {
    td[i] = liste.liste[i];
  }
  td[liste.taille] = t;
  free(liste.liste);
  return (Liste_Tuple_Declaration){td, liste.taille + 1};
}
