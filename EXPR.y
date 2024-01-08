
%{
  #include "cmat.h" // pas besoin ?
  #include "error.h"
  #include "symbtab.h"
  #include "generation.h"
  #include "hashtab.h"
  #include <stdio.h>// ?
  #include <stdlib.h>// ?

  extern void yyerror(const char * s);
  extern int yylex();
  extern unsigned nb_ligne;
%}

%union {
  int intval;
  float floatval;
  nom_var strval;
  struct {
    Symbol * ptr;
  } exprval;
  Symbol * variable;
  unsigned type;
  Tuple_Declaration var;
  Liste_Tuple_Declaration l_var;
  Matrix* matrix;
  char * string;
  Extract liste_extract;
  struct _param * liste_parametres;
}

%token DOTDOT CONST NOT AND OR DOT MAIN PRINTF PRINTMAT MATRIX INT FLOAT IF ELSE WHILE FOR TILDE PLUSPLUS MINUSMINUS DIV LCURLY RCURLY LBRACKET RBRACKET COMMA EQUAL QUOTE APOSTROPHE BACKSLASH RETURN EXIT  NOT_EQUAL LESS_THAN GREATER_THAN LTOE GTOE

%token PRINT SEMICOLON PLUS MINUS MULT ASSIGN LPAR RPAR
%token <strval> ID
%token <intval> V_INT 
%token <floatval> V_FLOAT
%token <string> V_STRING

%left MULT DIV EQUAL NOT_EQUAL OR AND
%left PLUS MINUS GTOE LTOE GREATER_THAN LESS_THAN
%nonassoc UMINUS NOT //LPAR RPAR

%type <type> type_var type_fonction
%type<l_var> liste_var
%type<var> une_var
%type<intval> taille cste_int 
%type<floatval> valeur_matrix cste_float
%type<matrix> init_matrix liste_matrix_ligne matrix_une_ligne matrix_remplir_colonne 
%type <exprval> expr expr_bool matrix_litt retour 
%type <variable> affectation init_for
%type<liste_extract> extraction intervalle
%type<liste_parametres> liste_param

%start Start

%%

Start 
  : %empty
  | def_constantes fonction_principale
  | def_constantes fonction fonction_principale
  | def_constantes fonction fonction fonction_principale
  ;

/*
suite_fonctions
  : fonction
  | fonction suite_fonctions
  ;
*/

depiler_adresse
  : LPAR
    { 
      gencode(CODE, DEPILER_ADRESSE, NULL, NULL, NULL);
    }
  ;

re_empiler_adresse
  : RPAR
    {
      gencode(CODE, RE_EMPILER_ADRESSE, NULL, NULL, NULL);
    }
  ;

type_fonction
  : INT       { $$ = INT; }
  | FLOAT     
    { 
      fprintf(stderr,"Ligne %d : Déclaration de fonction de type de retour INT uniquement.\n",nb_ligne);
      exit(SEMANTIC_FAILURE);
    }
  ;

fonction 
  : type_fonction nom_fonction depiler_adresse def_parametre re_empiler_adresse LCURLY suite_instructions retour RCURLY 
    {
      assert($1 == INT);
      unsigned type1;
      switch($8.ptr->kind){
        case(NAME):
          assert($8.ptr->var->init);
          type1 = $8.ptr->var->type;
          break;
        case(CONST_INT):
          type1 = INT;
          break;
        default:
          fprintf(stderr,"Ligne %d : Possibilité de renvoyer des int seulement.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
          break;
      }
      if (type1 != $1){
        fprintf(stderr,"Ligne %d : Incompatibilité de type entre la valeur renvoyée et le type de la fonction.\n",nb_ligne);
        exit(SEMANTIC_FAILURE);
        break;
      }

      gencode(CODE, RETOUR_FC, $8.ptr, NULL, NULL);
      gencode(CODE, JR, NULL, NULL, NULL);
    }
  ;

nom_fonction 
  : ID 
    {
      // vérifier qu'une fonction portant ce nom n'existe pas déjà
      SymTable * s = get_symtable($1);
      if (s != NULL){
        fprintf(stderr,"Ligne %d : Une fonction portant le nom '%s' est déjà déclarée.\n",nb_ligne,$1);
        exit(SEMANTIC_FAILURE);
      }
      // création de la table des symboles pour cette fonction
      SYMTAB = symtable_new($1);
      add_symtable(SYMTAB);
      add_fonction($1);
      gencode(CODE, LABEL_FC, NULL, NULL, NULL);
    }
  ;

fonction_principale 
  : type_fonction fc_main LPAR RPAR LCURLY suite_instructions retour RCURLY 
    {
      // vérifier que type de retour est int
      unsigned type1;

      switch($7.ptr->kind){
        case(NAME):
          assert($7.ptr->var->init);
          type1 = $7.ptr->var->type;
          break;
        case(CONST_INT):
          type1 = INT;
          break;
        case(CONST_FLOAT):
          type1 = FLOAT;
          break;
        default:
          fprintf(stderr,"Ligne %d : Possibilité de renvoyer des int seulement.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
          break;
      }

      if (type1 != INT){
        fprintf(stderr,"Ligne %d : Incompatibilité de type de la fonction return.\n",nb_ligne);
        exit(SEMANTIC_FAILURE);
        break;
      }

    }
  ; 

fc_main
  : MAIN 
    {
      nom_fonction nf;
      strncpy(nf,"main",15);
      SYMTAB = symtable_new("main");
      add_symtable(SYMTAB);
      add_fonction(nf);
      gencode(CODE, LABEL_FC, NULL, NULL, NULL);
    }
  ;


/*
struct_controle 
  : struct_if 
  | struct_for
  | struct_while
  ;

instr_avec_declaration
  : declaration_variable SEMICOLON instr_avec_declaration
  | afficher SEMICOLON instr_avec_declaration
  | affectation SEMICOLON instr_avec_declaration
  ;

instr_sans_declaration
  : struct_controle instr_sans_declaration
  // | appel_fonction
  | afficher SEMICOLON instr_avec_declaration
  | affectation SEMICOLON instr_avec_declaration
  ;
*/

suite_instructions 
  : %empty
  | instr SEMICOLON suite_instructions
  | struct_if suite_instructions
  | struct_while suite_instructions
  | struct_for suite_instructions

  | affectation_fonction suite_instructions
  ;

instr 
  : declaration_variable  
  | afficher 
  | affectation
  ;

def_parametre 
  : %empty
  | parametre
  | parametre COMMA def_parametre
  ;

parametre
  : INT ID 
    {
      // 1. vérifier que ça exite pas déjà
      Symbol * id = symtable_get(SYMTAB,$2);
      if (id) {
        fprintf(stderr,"Ligne %d : La variable '%s' a déjà été déclarée.\n",nb_ligne, $2);
        exit(SEMANTIC_FAILURE);
      }

      // 2. créer une entrée dans la table des symboles
      valeur v;
      variable * var = creer_variable($2, INT, true, v);
      Symbol * res = symtable_put(SYMTAB, $2, var);
      SYMTAB->param = add_parametre(SYMTAB->param, res);

      gencode(CODE, DEPILER, res, NULL, NULL);
    }
  | FLOAT ID
    {
      // 1. vérifier que ça exite pas déjà
      Symbol * id = symtable_get(SYMTAB,$2);
      if (id) {
        fprintf(stderr,"Ligne %d : La variable '%s' a déjà été déclarée.\n",nb_ligne, $2);
        exit(SEMANTIC_FAILURE);
      }

      // 2. créer une entrée dans la table des symboles
      valeur v;
      variable * var = creer_variable($2, FLOAT, true, v);
      Symbol * res = symtable_put(SYMTAB, $2, var);
      SYMTAB->param = add_parametre(SYMTAB->param, res);

      gencode(CODE, DEPILER, res, NULL, NULL);
    }
  ;

affectation_fonction
  : ID ASSIGN ID LPAR liste_param RPAR SEMICOLON
    {
      // sémantique : vérifier que l'id existe
      Symbol * id = symtable_get(SYMTAB,$1);
      if ( id == NULL ){
        fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été déclarée.\n",nb_ligne,$1);
        exit(SEMANTIC_FAILURE);
      }
      // vérifier qu'on essaye pas de modifier variable globale
      Symbol * glob = symtable_get(GLOBAL,$1);
      if ( glob != NULL ){
        fprintf(stderr,"Ligne %d : La constante '%s' ne peut pas être modifiée.\n",nb_ligne,$1);
        exit(SEMANTIC_FAILURE);
      }
      unsigned type1, type2;

      type1 = id->var->type;
      if (type1 != INT){
        fprintf(stderr,"Ligne %d : Incompatibilité de types.\n",nb_ligne);
        exit(SEMANTIC_FAILURE);
      }

      // 1. chercher fonction ID dans table de hachage
      SymTable * st = get_symtable($3);
      if (st == NULL){
        fprintf(stderr,"Ligne %d : fonction '%s' non déclarée.\n",nb_ligne,$3);
        exit(SEMANTIC_FAILURE);
      }

      // 3. vérifier que il y a le meme nombre de parametres et que ces paramtres sont de meme type
      Parametres *p_fc = st->param;
      Parametres *p = $5;

      // même nombre de parametres
      if (p->nb != p_fc->nb){
        fprintf(stderr,"Ligne %d : Le nombre de paramètres à passer à la fonction %s n'est pas le bon.\n",nb_ligne, st->nom);
        exit(SEMANTIC_FAILURE);
      }

      // même type pour chaque parametre
      for(int i = (p->nb) -1; i>=0; i--){
        switch(p_fc->liste[i]->kind){
          case(NAME):
            type1 = p_fc->liste[i]->var->type;
            break;
          default:
            fprintf(stderr,"Ligne %d : Les paramètres des fonctions ne peuvent être que des INT ou des FLOAT.\n",nb_ligne);
            exit(SEMANTIC_FAILURE);
            break;
        }
        switch(p->liste[i]->kind){
          case(NAME):
            type2 = p_fc->liste[i]->var->type;
            break;
          case(CONST_INT):
            type2 = INT;
            break;
          case(CONST_FLOAT):
            type2 = FLOAT;
            break;
          default:
            fprintf(stderr,"Ligne %d : Les paramètres des fonctions ne peuvent être que des INT ou des FLOAT.\n",nb_ligne);
            exit(SEMANTIC_FAILURE);
            break;
        }

        assert(type1 == INT || type1 == FLOAT);
        if (type1 == INT && type1 != type2){
          fprintf(stderr,"Ligne %d : Erreur de type d'un paramètre lors de l'appel à la fonction %s\n",nb_ligne, st->nom);
          exit(SEMANTIC_FAILURE);
          break;
        }
        assert(type2 == INT || type2 == FLOAT);

        gencode(CODE, EMPILER, p->liste[i], NULL, NULL);
      }

      id->var->init = true;

      // créer un symbol juste pour avoir le nom d'où on veut jump
      valeur val;
      Symbol * s1 = newtemp(SYMTAB, 0, val);
      free(s1->var); // pas besoin de ce qu'il y a dedans
      s1->kind = FONCTION;
      s1->st = st;

      gencode(CODE, JAL_FC, s1, NULL, NULL);

      valeur val2;
      Symbol * tmp = newtemp(SYMTAB, INT, val2);
      gencode(CODE, MOVE, tmp, NULL, NULL); 

      gencode(CODE,COPY,id,tmp,NULL);
      free(p->liste);
      free(p);
    }

liste_param 
  : %empty
    {
      $$ = init_param();
    }
  | expr 
    {
      Parametres *p = init_param();
      $$ = add_parametre(p, $1.ptr);
    }
  | liste_param COMMA expr
    {
      $$ = add_parametre($1, $3.ptr);
    }
  ;

// CONSTANTES  
def_constantes 
  : %empty
  | def_constante SEMICOLON def_constantes
  ;

cste_int
  : V_INT 
    {
      $$ = $1;
    }
  | MINUS V_INT
    {
      $$ = -$2;
    }
cste_float
  : V_FLOAT
    {
      $$ = $1;
    }
  | MINUS V_FLOAT
    {
      $$ = -$2;
    }
  ;

def_constante
  : CONST INT ID ASSIGN cste_int
    {
      Symbol * id = symtable_get(GLOBAL,$3);
      if (id) {
        fprintf(stderr,"Ligne %d : La constante '%s' a déjà été déclarée.\n",nb_ligne, $3);
        exit(SEMANTIC_FAILURE);
      }
      valeur v;
      v.entier = $5;
      variable * var = creer_variable($3, INT, true, v);
      symtable_put(GLOBAL, $3, var);
    }
  | CONST FLOAT ID ASSIGN cste_float 
    {
      Symbol * id = symtable_get(GLOBAL,$3);
      if (id) {
        fprintf(stderr,"Ligne %d : La constante '%s' a déjà été déclarée.\n",nb_ligne, $3);
        exit(SEMANTIC_FAILURE);
      }
      valeur v;
      v.flottant = $5;
      variable * var = creer_variable($3, FLOAT, true, v);
      symtable_put(GLOBAL, $3, var);
    }
  | CONST FLOAT ID ASSIGN cste_int
    {
      Symbol * id = symtable_get(GLOBAL,$3);
      if (id) {
        fprintf(stderr,"Ligne %d : La constante '%s' a déjà été déclarée.\n",nb_ligne, $3);
        exit(SEMANTIC_FAILURE);
      }
      valeur v;
      v.flottant = $5;
      variable * var = creer_variable($3, FLOAT, true, v);
      symtable_put(GLOBAL, $3, var);
    }
  | CONST INT ID ASSIGN cste_float
    {
      fprintf(stderr,"Ligne %d : Incompatibilité de type à la déclaration de %s.\n",nb_ligne, $3);
      exit(SEMANTIC_FAILURE);
    }

  ;
  
// AFFECTATION
affectation
  : ID ASSIGN expr
    {
      // sémantique : vérifier que l'id existe
      Symbol * id = symtable_get(SYMTAB,$1);
      if ( id == NULL ){
        fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été déclarée.\n",nb_ligne,$1);
        exit(SEMANTIC_FAILURE);
      }
      // vérifier qu'on essaye pas de modifier variable globale
      Symbol * glob = symtable_get(GLOBAL,$1);
      if ( glob != NULL ){
        fprintf(stderr,"Ligne %d : La constante '%s' ne peut pas être modifiée.\n",nb_ligne,$1);
        exit(SEMANTIC_FAILURE);
      }

      // sémantique vérifier que le type de l'expr est compatible avec le type de ID

      unsigned type1, type2;

      type1 = id->var->type;

      switch($3.ptr->kind){
        case(NAME):
          type2 = $3.ptr->var->type;
          break;
        case(CONST_INT):
          type2 = INT;
          break;
        case(CONST_FLOAT):
          type2 = FLOAT;
          break;
        default:
          fprintf(stderr,"Ligne %d : Erreur de type.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
          break;
      }

      if (type1 == MATRIX && type2 != MATRIX){ 
        fprintf(stderr,"Ligne %d : Incompatibilité de types.\n",nb_ligne);
        exit(SEMANTIC_FAILURE);
      }
      else if (type1 == INT && type2 != INT) {
        fprintf(stderr,"Ligne %d : Incompatibilité de types.\n",nb_ligne);
        exit(SEMANTIC_FAILURE);
      }
      else if (type1 == FLOAT && (type2 != FLOAT && type2 != INT)){
        fprintf(stderr,"Ligne %d : Incompatibilité de types.\n",nb_ligne);
        exit(SEMANTIC_FAILURE);
      }

      if (type1 == MATRIX && type2 == MATRIX){ 
        if ((id->var->val.matrix->l != $3.ptr->var->val.matrix->l) || (id->var->val.matrix->c != $3.ptr->var->val.matrix->c)){
          fprintf(stderr,"Ligne %d : Incompatibilité entre les tailles de matrice lors de l'affectation à %s.\n",nb_ligne, $1);
          exit(SEMANTIC_FAILURE);
        }
      }

      // marquer que la variable est initialisée
      id->var->init = true;
      
      gencode(CODE,COPY,id,$3.ptr,NULL);
      $$ = id; // utile pour for
    }
  
  | ID LBRACKET V_INT RBRACKET ASSIGN expr
    {
      // sémantique : vérifier que l'id existe
      Symbol * id = symtable_get(SYMTAB,$1);
      if ( id == NULL ){
        fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été déclarée.\n",nb_ligne,$1);
        exit(SEMANTIC_FAILURE);
      }

      // vérifier qu'on essaye pas de modifier variable globale
      Symbol * glob = symtable_get(GLOBAL,$1);
      if ( glob != NULL ){
        fprintf(stderr,"Ligne %d : La constante '%s' ne peut pas être modifiée.\n",nb_ligne,$1);
        exit(SEMANTIC_FAILURE);
      }

      unsigned type1, type2;

      assert(id->kind == NAME);
      type1 = id->var->type;

      switch($6.ptr->kind){
        case(NAME):
          type2 = $6.ptr->var->type;
          break;
        case(CONST_INT):
          type2 = INT;
          break;
        case(CONST_FLOAT):
          type2 = FLOAT;
          break;
        default:
          fprintf(stderr,"Ligne %d : Erreur de type.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
          break;
      }

      // vérifier qu'il s'agit d'une matrice
      if (type1 != MATRIX){
        fprintf(stderr,"Ligne %d : %s n'est pas une matrice, on ne peut donc pas accéder à %s[%d].\n",nb_ligne,id->name,id->name,$3);
        exit(SEMANTIC_FAILURE);
      }

      // vérifier que c'est une matrice à une dimension
      if (id->var->val.matrix->l != 1){
        fprintf(stderr,"Ligne %d : La matrice %s a deux dimensions : impossible d'affecter quelque chose à %s[%d].\n",nb_ligne,id->name,id->name,$3);
        exit(SEMANTIC_FAILURE);
      }

      // vérifier que 0 <= $3 < taille colonne
      if (id->var->val.matrix->c <= $3){
        fprintf(stderr,"Ligne %d : La matrice %s a %d colonnes : %s[%d] out of range.\n",nb_ligne,id->name,id->var->val.matrix->c,id->name,$3);
        exit(SEMANTIC_FAILURE);
      }

      // vérifier que le type de l'expr est compatible avec le type de ID
      if (type2 == MATRIX) {
        fprintf(stderr,"Ligne %d : Incompatibilité de types.\n",nb_ligne);
        exit(SEMANTIC_FAILURE);
      }
      assert(type2 == INT || type2 == FLOAT);

      // marquer que la variable est initialisée
      id->var->init = true;
      
      // aller modifier une case d'une matrice id
      // lui mettre la valeur de $6
      // aux coordonnées $3
      Symbol * t = symtable_indices(SYMTAB,(Indices){0,$3});
      gencode(CODE,PUT_ELEMENT,id,$6.ptr,t);

      $$ = id; // utile pour for
    }
  
  | ID LBRACKET V_INT RBRACKET LBRACKET V_INT RBRACKET ASSIGN expr
    {
      // sémantique : vérifier que l'id existe
      Symbol * id = symtable_get(SYMTAB,$1);
      if ( id == NULL ){
        fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été déclarée.\n",nb_ligne,$1);
        exit(SEMANTIC_FAILURE);
      }

      // vérifier qu'on essaye pas de modifier variable globale
      Symbol * glob = symtable_get(GLOBAL,$1);
      if ( glob != NULL ){
        fprintf(stderr,"Ligne %d : La constante '%s' ne peut pas être modifiée.\n",nb_ligne,$1);
        exit(SEMANTIC_FAILURE);
      }

      unsigned type1, type2;

      assert(id->kind == NAME);
      type1 = id->var->type;

      switch($9.ptr->kind){
        case(NAME):
          type2 = $9.ptr->var->type;
          break;
        case(CONST_INT):
          type2 = INT;
          break;
        case(CONST_FLOAT):
          type2 = FLOAT;
          break;
        default:
          fprintf(stderr,"Ligne %d : Erreur de type.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
          break;
      }

      // vérifier qu'il s'agit d'une matrice
      if (type1 != MATRIX){
        fprintf(stderr,"Ligne %d : %s n'est pas une matrice, on ne peut donc pas accéder à %s[%d].\n",nb_ligne,id->name,id->name,$3);
        exit(SEMANTIC_FAILURE);
      }

      // vérifier que 0 <= $3 < taille ligne
      if (id->var->val.matrix->l <= $3){
        fprintf(stderr,"Ligne %d : La matrice %s a %d lignes : %s[%d][%d] out of range.\n",nb_ligne,id->name,id->var->val.matrix->l,id->name,$3,$6);
        exit(SEMANTIC_FAILURE);
      }

      // vérifier que 0 <= $6 < taille colonne
      if (id->var->val.matrix->c <= $6){
        fprintf(stderr,"Ligne %d : La matrice %s a %d colonnes : %s[%d][%d] out of range.\n",nb_ligne,id->name,id->var->val.matrix->c,id->name,$3,$6);
        exit(SEMANTIC_FAILURE);
      }

      // vérifier que le type de l'expr est compatible avec le type de ID
      if (type2 == MATRIX) {
        fprintf(stderr,"Ligne %d : Incompatibilité de types.\n",nb_ligne);
        exit(SEMANTIC_FAILURE);
      }
      assert(type2 == INT || type2 == FLOAT);

      // marquer que la variable est initialisée
      id->var->init = true;
      
      // aller modifier une case d'une matrice id
      // lui mettre la valeur de $9
      // aux coordonnées $3 $6
      Symbol * t = symtable_indices(SYMTAB,(Indices){$3,$6});
      gencode(CODE,PUT_ELEMENT,id,$9.ptr,t);

      $$ = id; // utile pour for 
    }
  | ID ASSIGN matrix_litt
    {
      // sémantique : vérifier que l'id existe
      Symbol * id = symtable_get(SYMTAB,$1);
      if ( id == NULL ){
        fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été déclarée.\n",nb_ligne,$1);
        exit(SEMANTIC_FAILURE);
      }

      // vérifier qu'on essaye pas de modifier variable globale
      Symbol * glob = symtable_get(GLOBAL,$1);
      if ( glob != NULL ){
        fprintf(stderr,"Ligne %d : La constante '%s' ne peut pas être modifiée.\n",nb_ligne,$1);
        exit(SEMANTIC_FAILURE);
      }

      // sémantique vérifier que le type de l'expr est compatible avec le type de ID

      unsigned type1;

      type1 = id->var->type;

      if (type1 != MATRIX){ 
        fprintf(stderr,"Ligne %d : Incompatibilité de types.\n",nb_ligne);
        exit(SEMANTIC_FAILURE);
      }

      // marquer que la variable est initialisée
      id->var->init = true;
      
      gencode(CODE,COPY,id,$3.ptr,NULL);
      $$ = id; // utile pour for
    }
  
  | ID PLUSPLUS 
    {
      // sémantique : vérifier que l'id existe
      Symbol * id = symtable_get(SYMTAB,$1);
      if ( id == NULL ){
        fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été déclarée.\n",nb_ligne,$1);
        exit(SEMANTIC_FAILURE);
      }
      // vérifier qu'on essaye pas de modifier variable globale
      Symbol * glob = symtable_get(GLOBAL,$1);
      if ( glob != NULL ){
        fprintf(stderr,"Ligne %d : La constante '%s' ne peut pas être modifiée.\n",nb_ligne,$1);
        exit(SEMANTIC_FAILURE);
      }

      if(!id->var->init){
        fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été initialisée.\n",nb_ligne,$1);
        exit(SEMANTIC_FAILURE);
      }

      Symbol * ptr = symtable_const_int(SYMTAB, 1);
      gencode(CODE,BOP_PLUS,id,id,ptr);
      $$ = id; // utile pour for
    }
  | PLUSPLUS ID
    {
      // sémantique : vérifier que l'id existe
      Symbol * id = symtable_get(SYMTAB,$2);
      if ( id == NULL ){
        fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été déclarée.\n",nb_ligne,$2);
        exit(SEMANTIC_FAILURE);
      }
      // vérifier qu'on essaye pas de modifier variable globale
      Symbol * glob = symtable_get(GLOBAL,$2);
      if ( glob != NULL ){
        fprintf(stderr,"Ligne %d : La constante '%s' ne peut pas être modifiée.\n",nb_ligne,$2);
        exit(SEMANTIC_FAILURE);
      }

      if(!id->var->init){
        fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été initialisée.\n",nb_ligne,$2);
        exit(SEMANTIC_FAILURE);
      }

      Symbol * ptr = symtable_const_int(SYMTAB, 1);
      gencode(CODE,BOP_PLUS,id,id,ptr);
      $$ = id; // utile pour for
    }
  | ID MINUSMINUS
    {
      // sémantique : vérifier que l'id existe
      Symbol * id = symtable_get(SYMTAB,$1);
      if ( id == NULL ){
        fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été déclarée.\n",nb_ligne,$1);
        exit(SEMANTIC_FAILURE);
      }
      // vérifier qu'on essaye pas de modifier variable globale
      Symbol * glob = symtable_get(GLOBAL,$1);
      if ( glob != NULL ){
        fprintf(stderr,"Ligne %d : La constante '%s' ne peut pas être modifiée.\n",nb_ligne,$1);
        exit(SEMANTIC_FAILURE);
      }

      if(!id->var->init){
        fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été initialisée.\n",nb_ligne,$1);
        exit(SEMANTIC_FAILURE);
      }

      Symbol * ptr = symtable_const_int(SYMTAB, 1);
      gencode(CODE,BOP_MINUS,id,id,ptr);
      $$ = id; // utile pour for
    }
  | MINUSMINUS ID
    {
      // sémantique : vérifier que l'id existe
      Symbol * id = symtable_get(SYMTAB,$2);
      if ( id == NULL ){
        fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été déclarée.\n",nb_ligne,$2);
        exit(SEMANTIC_FAILURE);
      }
      // vérifier qu'on essaye pas de modifier variable globale
      Symbol * glob = symtable_get(GLOBAL,$2);
      if ( glob != NULL ){
        fprintf(stderr,"Ligne %d : La constante '%s' ne peut pas être modifiée.\n",nb_ligne,$2);
        exit(SEMANTIC_FAILURE);
      }

      if(!id->var->init){
        fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été initialisée.\n",nb_ligne,$2);
        exit(SEMANTIC_FAILURE);
      }

      Symbol * ptr = symtable_const_int(SYMTAB, 1);
      gencode(CODE,BOP_MINUS,id,id,ptr);
      $$ = id; // utile pour for
    }
  ;

// DÉCLARATION
declaration_variable
  : type_var liste_var {

      unsigned type1, type2, type3;
      type1 = $1;
      
      for (int i = 0; i < $2.taille; i++) {
        // vérifier la compatibilité entre $1 et les types gauche (peut-etre indéfini) et les types droits
        // et faire les COPY si c'est correct

        variable * v = $2.liste[i].gauche;
        Symbol * s = $2.liste[i].droite;
    
        // 1. vérifier que l'entrée n'existe pas
        Symbol * id = symtable_get(SYMTAB,v->name);
        if (id) {
          fprintf(stderr,"Ligne %d : La variable '%s' a déjà été déclarée.\n",nb_ligne, v->name);
          exit(SEMANTIC_FAILURE);
        }

        type2 = v->type; // 0 ou MATRIX
        assert((type2 == 0)||(type2 == MATRIX));

        if (s != NULL){ // cas où affectation

          // chercher type de ce qu'on essaye de mettre dans la variable
          switch(s->kind){
            case NAME:
              type3 = s->var->type;
              break;
            case(CONST_INT):
              type3 = INT;
              break;
            case(CONST_FLOAT):
              type3 = FLOAT;
              break;
            default:
              fprintf(stderr,"Ligne %d : Erreur de type.\n",nb_ligne);
              exit(SEMANTIC_FAILURE);
              break;
          }
          assert(type3 == INT || type3 == FLOAT || type3 == MATRIX);
          // fprintf + exit

          // vérifier la compatibilité des types
          if(type1 == MATRIX && (type2 != MATRIX || type3 != MATRIX)){
            if (type2 != MATRIX){
              fprintf(stderr,"Ligne %d : Il est nécessaire de déclarer les dimensions de %s.\n",nb_ligne,v->name);
              exit(SEMANTIC_FAILURE);
            }  // si type 2 != MATRIX : manque les dimensions à la déclaration
            fprintf(stderr,"Ligne %d : Incompatibilité de types.\n",nb_ligne);
            exit(SEMANTIC_FAILURE);
          }

          if(type1 == INT && (type2 != 0 || type3 != INT)){
            fprintf(stderr,"Ligne %d : Incompatibilité de types.\n",nb_ligne);
            exit(SEMANTIC_FAILURE);
          }

          if(type1 == FLOAT && (type2 != 0 || (type3 != INT && type3 != FLOAT))){
            fprintf(stderr,"Ligne %d : Incompatibilité de types.\n",nb_ligne);
            exit(SEMANTIC_FAILURE);
          }

          // ajout table des symboles

          Symbol * res;

          if (type1 == MATRIX){ 
            assert(type2 == MATRIX);
            assert(type3 == MATRIX);
            assert(s->kind == NAME);
            Matrix * m1 = v->val.matrix;
            Matrix * m2 = s->var->val.matrix;
            if ((m1->l != m2->l) || (m1->c != m2->c)){
              fprintf(stderr,"Ligne %d : Incompatibilité entre les tailles de matrice lors de l'affectation à %s.\n",nb_ligne, v->name);
              exit(SEMANTIC_FAILURE);
            }
            res = symtable_put(SYMTAB,v->name,v);
          } else if (type1 == INT){
            assert(type2 == 0);
            assert(type3 == INT);
            v->type = INT;
            res = symtable_put(SYMTAB,v->name,v);
          } else if (type1 == FLOAT){
            assert(type2 == 0);
            assert(type3 == INT || type3 == FLOAT);
            v->type = FLOAT;
            res = symtable_put(SYMTAB,v->name,v);
          } else {
            fprintf(stderr,"Ligne %d : Something went wrong\n",nb_ligne);
            exit(SEMANTIC_FAILURE);
          }

          v->init = true;
          gencode(CODE,COPY,res,s,NULL);

        } else { // pas d'affectation

          // vérifier la compatibilité des types
          if(type1 == MATRIX && type2 != MATRIX){
            fprintf(stderr,"Ligne %d : Il est nécessaire de déclarer les dimensions de %s.\n",nb_ligne,v->name);
            exit(SEMANTIC_FAILURE);
          }

          if(type1 == INT && type2 != 0){
            fprintf(stderr,"Ligne %d : Incompatibilité de types.\n",nb_ligne);
            exit(SEMANTIC_FAILURE);
          }

          if(type1 == FLOAT && type2 != 0){
            fprintf(stderr,"Ligne %d : Incompatibilité de types.\n",nb_ligne);
            exit(SEMANTIC_FAILURE);
          }

          if (type1 == MATRIX){ 
            assert(type2 == MATRIX);
            symtable_put(SYMTAB,v->name,v);
          } else if (type1 == INT){
            assert(type2 == 0);
            v->type = INT;
            symtable_put(SYMTAB,v->name,v);
          } else if (type1 == FLOAT){
            assert(type2 == 0);
            v->type = FLOAT;
            symtable_put(SYMTAB,v->name,v);
          } else {
            fprintf(stderr,"Ligne %d : Something went wrong\n",nb_ligne);
            exit(SEMANTIC_FAILURE);
          }

        }
      } // fin for 
      free($2.liste);
  }
  ;

type_var 
  : INT       { $$ = INT; }
  | FLOAT     { $$ = FLOAT; }
  | MATRIX    { $$ = MATRIX; }
  ;

liste_var
  : une_var
    {
      $$ = creer_liste_tuple_declaration($1);
    }
  | liste_var COMMA une_var 
    {
      $$ = ajouter_tuple($1, $3);
    }
  ;

une_var 
  : ID 
    { 
      valeur v;
      $$.gauche = creer_variable($1, 0, false, v);
      $$.droite = NULL;
    } // int ou float    
  | ID ASSIGN expr
    {
      valeur v;
      $$.gauche = creer_variable($1, 0, false, v);
      $$.droite = $3.ptr;
    } // int ou float

  | ID taille 
    {
      Matrix * m = create_matrix(1,$2); // créer la matrice meme si pas initialisée
      valeur v;
      v.matrix = m;
      $$.gauche = creer_variable($1, MATRIX, false, v);
      $$.droite = NULL;
    }
  | ID taille taille
    {
      Matrix *m = create_matrix($2,$3);
      valeur v;
      v.matrix = m;
      $$.gauche = creer_variable($1, MATRIX, false, v);
      $$.droite = NULL;
    }
  | ID taille ASSIGN matrix_litt 
    { 
      Matrix * m = create_matrix(1,$2); 
      valeur v;
      v.matrix = m;
      $$.gauche = creer_variable($1, MATRIX, true, v);
      $$.droite = $4.ptr;
    }
  | ID taille taille ASSIGN matrix_litt   
    { 
      Matrix *m = create_matrix($2,$3);
      valeur v;
      v.matrix = m;
      $$.gauche = creer_variable($1, MATRIX, true, v);
      $$.droite = $5.ptr;
    }
  | ID taille ASSIGN expr 
    { 
      // vérif que expr est une matrix
      // et que expr est de bonnes dimensions ?

      Matrix * m = create_matrix(1,$2); 
      valeur v;
      v.matrix = m;
      $$.gauche = creer_variable($1, MATRIX, true, v);
      $$.droite = $4.ptr;
      
    }
  | ID taille taille ASSIGN expr   
    { 
      // vérif que expr est une matrix
      // et que expr est de bonnes dimensions ?
      
      Matrix *m = create_matrix($2,$3);
      valeur v;
      v.matrix = m;
      $$.gauche = creer_variable($1, MATRIX, true, v);
      $$.droite = $5.ptr;
      
    }

  ;

taille
  : LBRACKET V_INT RBRACKET 
  { 
    if ($2==0){
      fprintf(stderr, "Ligne %d : Une dimension de matrice ne peut pas être nulle.\n",nb_ligne);
      exit(SEMANTIC_FAILURE);
    }
    $$ = $2; 
  }
  ;

// MATRIX
matrix_litt 
  : init_matrix
    {
      // généré temporaire
      valeur val;
      val.matrix = $1;
      $$.ptr = newtemp(SYMTAB, MATRIX, val);
    }
  ; // correspond à { {1,2}, {2,3} }

init_matrix 
    : matrix_une_ligne    { $$ = $1; }
    | LCURLY liste_matrix_ligne RCURLY  { $$ = $2; }
    ;

liste_matrix_ligne 
    : matrix_une_ligne  { $$ = $1; } // condition d'arret
    | liste_matrix_ligne COMMA matrix_une_ligne {
        if ($1->c != $3->c){
            fprintf(stderr, "Ligne %d : Chaque ligne d'une matrice doit avoir le même nombre de colonne.\n",nb_ligne);
            exit(SEMANTIC_FAILURE);
        } 
        add_ligne($1, $3); // ajouter $3 en derniere ligne de $1
        $$ = $1;
    }
    ;

matrix_une_ligne 
    : LCURLY matrix_remplir_colonne RCURLY { $$ = $2; }
    ;

matrix_remplir_colonne 
    : valeur_matrix   {
        Matrix *m = create_matrix(1,1); // créer une ligne 
        put_value_at(m, 0, 0, $1); 
        $$ = m;
    }
    | matrix_remplir_colonne COMMA valeur_matrix{
        add_colonne($1); // ajoute une colonne
        put_value_at($1, 0, ($1->c)-1, $3); // mets la valeur dans cette colonne
        $$ = $1;
    }
    ;

valeur_matrix
    : V_INT     { $$ = (float) $1; }
    | V_FLOAT   { $$ = $1; }
    ;

// AFFICHER
afficher 
  : PRINT LPAR expr RPAR 
    {
      unsigned type1;

      switch($3.ptr->kind){
        case(NAME):
          assert($3.ptr->var->init);
          type1 = $3.ptr->var->type;
          break;
        case(CONST_INT):
          type1 = INT;
          break;
        case(CONST_FLOAT):
          type1 = FLOAT;
          break;
        default:
          fprintf(stderr,"Ligne %d : La fonction print ne peut qu'afficher des expressions correspondantes à des INT ou des FLOAT.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
          break;
      }

      if (type1 != INT && type1 != FLOAT){
        fprintf(stderr,"Ligne %d : La fonction print ne peut qu'afficher des expressions correspondantes à des INT ou des FLOAT.\n",nb_ligne);
        exit(SEMANTIC_FAILURE);
      }
      gencode(CODE,CALL_PRINT,$3.ptr,NULL,NULL);
  }

  | PRINTMAT LPAR expr RPAR 
    {
      unsigned type1;

      switch($3.ptr->kind){
        case(NAME):
          assert($3.ptr->var->init);
          type1 = $3.ptr->var->type;
          break;
        default:
          fprintf(stderr,"Ligne %d : La fonction printmat ne peut qu'afficher des expressions correspondantes à des MATRIX.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
          break;
      }

      if (type1 != MATRIX){
        fprintf(stderr,"Ligne %d : La fonction printmat ne peut qu'afficher des expressions correspondantes à des MATRIX.\n",nb_ligne);
        exit(SEMANTIC_FAILURE);
      }

      gencode(CODE,CALL_PRINTMAT,$3.ptr,NULL,NULL);
  }

  | PRINTF LPAR V_STRING RPAR {
      Symbol * id = symtable_string(SYMTAB, $3);
      gencode(CODE,CALL_PRINTF,id,NULL,NULL);
    }
  ;

// OPÉRATIONS
expr
  : expr PLUS expr
    { 
      // vérifier compatibilité de type

      unsigned type1, type2, type;
      switch($1.ptr->kind){
        case(NAME):
          type1 = $1.ptr->var->type;
          break;
        case(CONST_INT):
          type1 = INT;
          break;
        case(CONST_FLOAT):
          type1 = FLOAT;
          break;
        default:
          fprintf(stderr,"Ligne %d : Erreur de type dans une addition.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
          break;
      }
      switch($3.ptr->kind){
        case(NAME):
          type2 = $3.ptr->var->type;
          break;
        case(CONST_INT):
          type2 = INT;
          break;
        case(CONST_FLOAT):
          type2 = FLOAT;
          break;
        default:
          fprintf(stderr,"Ligne %d : Erreur de type dans une addition.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
          break;
      }

      valeur val;
      if (type1 == MATRIX || type2 == MATRIX) {
        type = MATRIX;

        // vérifier même tailles si les 2 sont matrices
        if ((type1 == MATRIX && type2 == MATRIX)&&(($1.ptr->var->val.matrix->l != $3.ptr->var->val.matrix->l) || ($1.ptr->var->val.matrix->c != $3.ptr->var->val.matrix->c))){
          fprintf(stderr,"Ligne %d : Incompatibilité entre les tailles de matrice lors d'une addition.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
        }

        // créer une matrix pour la var temporaire
        if (type1 == MATRIX){
          Matrix *m = create_matrix($1.ptr->var->val.matrix->l, $1.ptr->var->val.matrix->c);
          val.matrix = m;
        } else {
          Matrix *m = create_matrix($3.ptr->var->val.matrix->l, $3.ptr->var->val.matrix->c);
          val.matrix = m;
        }
      }
      else if (type1 == FLOAT || type2 == FLOAT) 
        type = FLOAT;
      else 
        type = INT;

      $$.ptr = newtemp(SYMTAB, type, val);
      gencode(CODE,BOP_PLUS,$$.ptr,$1.ptr,$3.ptr); 
    }
  | LPAR expr RPAR
    { 
      $$.ptr = $2.ptr;
    }
  | ID
      { 
        Symbol * id = symtable_get(SYMTAB,$1);
        if (id==NULL){
            fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été déclarée.\n",nb_ligne,$1);
            exit(SEMANTIC_FAILURE);
        }
        if (!id->var->init){
          fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été initialisée, elle ne peut donc pas être utilisée dans une expression.\n",nb_ligne,$1);
          exit(SEMANTIC_FAILURE);
        }
        $$.ptr = id;
      }
  | V_INT
      { 
        $$.ptr = symtable_const_int(SYMTAB,$1); 
      }
  | V_FLOAT
      { 
        $$.ptr = symtable_const_float(SYMTAB,$1); 
      }

  | ID LBRACKET extraction RBRACKET // matrice une dimension
    {
      // TODO : si taille de extraction = 1  (et que c'est pas -1)-> float 

      // 1. vérifier que ID est déclarée
      Symbol * id = symtable_get(SYMTAB,$1);
      if (id == NULL){
        fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été déclarée.\n",nb_ligne,$1);
        exit(SEMANTIC_FAILURE);
      }

      if (!id->var->init){
        fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été initialisée, elle ne peut donc pas être utilisée dans une expression.\n",nb_ligne,$1);
        exit(SEMANTIC_FAILURE);
      }

      // 2. vérifier que c'est une matrice
      if (id->var->type != MATRIX){
        fprintf(stderr,"Ligne %d : La variable '%s' n'est pas de type matrix.\n",nb_ligne,$1);
        exit(SEMANTIC_FAILURE);
      }

      // 3. vérifier que c'est bien une matrice une dimension
      if (id->var->val.matrix->l != 1){
        fprintf(stderr,"Ligne %d : La matrix '%s' n'est pas de dimension 1.\n",nb_ligne,$1);
        exit(SEMANTIC_FAILURE);
      }

      // 4. vérifier correspondance dimensions matrice / extract

        Matrix *m = id->var->val.matrix;
        for (int i = 0; i < $3.taille; i++) {
          if ($3.liste[i] != -1 && $3.liste[i] >= m->c) {
            fprintf(stderr, "Ligne %d : Indice de colonne out of range.\n",nb_ligne);
            exit(SEMANTIC_FAILURE);
          }
        }

        // pour tous les -1, remplacer par la dimension de ID
        int *c = (int *) malloc(sizeof(int)* $3.taille);
        if (c == NULL) { // Échec de l'allocation, gérer l'erreur
          fprintf(stderr,"Allocation mémoire échouée\n");
          exit(MEMORY_FAILURE);
        }
        int c_taille = 0;
        int tmp = $3.taille; // pour réallouer la bonne taille
        for (int i = 0; i < $3.taille; i++){
          if ($3.liste[i]==-1){
            tmp += id->var->val.matrix->c;
            c = (int *)realloc(c, tmp * sizeof(int));
            if (c == NULL) { // Échec de l'allocation, gérer l'erreur
              fprintf(stderr,"Allocation mémoire échouée\n");
              exit(MEMORY_FAILURE);
            }
            for (int k = 0; k < id->var->val.matrix->c; k++){
              c[c_taille] = k;
              c_taille++;
            }
          } else {
            c[c_taille] = $3.liste[i];
            c_taille++;
          }
        }

      valeur val;
      if (c_taille == 1){
        // on va juste chercher le float correspondant (ici le type c'est float et pas matrix)
        $$.ptr = newtemp(SYMTAB, FLOAT, val);
        Symbol * t = symtable_indices(SYMTAB,(Indices){0,c[0]});
        gencode(CODE,GET_ELEMENT,$$.ptr,id,t);
        free(c);
      } else {
        Symbol * colonne = symtable_extract(SYMTAB,(Extract){c,c_taille});

        // créer la matrice temporaire de bonnes dimensions
        Matrix *mat = create_matrix(1, c_taille);
        val.matrix = mat;
        $$.ptr = newtemp(SYMTAB, MATRIX, val);
    
        gencode(CODE,EXTR_COLONNE,$$.ptr,id,colonne); 
      }
      free($3.liste);
    }
  | ID LBRACKET extraction RBRACKET LBRACKET extraction RBRACKET // matrice deux dimensions
    {
      // 1. vérifier que ID est déclarée
      Symbol * id = symtable_get(SYMTAB,$1);
      if (id == NULL){
        fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été déclarée.\n",nb_ligne,$1);
        exit(SEMANTIC_FAILURE);
      }

      if (!id->var->init){
        fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été initialisée, elle ne peut donc pas être utilisée dans une expression.\n",nb_ligne,$1);
        exit(SEMANTIC_FAILURE);
      }

      // 2. vérifier que c'est une matrice
      if (id->var->type != MATRIX){
        fprintf(stderr,"Ligne %d : La variable '%s' n'est pas de type matrix.\n",nb_ligne,$1);
        exit(SEMANTIC_FAILURE);
      }

      // vérifier correspondance dimensions matrice / extract
      Matrix *m = id->var->val.matrix;
      for (int i = 0; i < $3.taille; i++) {
        if ($3.liste[i] != -1 && $3.liste[i] >= m->l) {
          fprintf(stderr, "Ligne %d : Indice de ligne out of range.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
        }
      }
      for (int i = 0; i < $6.taille; i++) {
        if ($6.liste[i] != -1 && $6.liste[i] >= m->c) {
          fprintf(stderr, "Ligne %d : Indice de colonne out of range.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
        }
      }

      // pour tous les -1, remplacer par la dimension de ID : ligne
      int *l = (int *) malloc(sizeof(int)* $3.taille);
      if (l == NULL) { // Échec de l'allocation, gérer l'erreur
        fprintf(stderr,"Allocation mémoire échouée\n");
        exit(MEMORY_FAILURE);
      }
      int l_taille = 0;
      int tmp = $3.taille; // pour réallouer la bonne taille
      for (int i = 0; i < $3.taille; i++){
        if ($3.liste[i]==-1){
          tmp += id->var->val.matrix->l;
          l = (int *)realloc(l, tmp * sizeof(int));
          if (l == NULL) { // Échec de l'allocation, gérer l'erreur
            fprintf(stderr,"Allocation mémoire échouée\n");
            exit(MEMORY_FAILURE);
          }
          for (int k = 0; k < id->var->val.matrix->l; k++){
            l[l_taille] = k;
            l_taille++;
          }
        } else {
          l[l_taille] = $3.liste[i];
          l_taille++;
        }
      }

      // pour tous les -1, remplacer par la dimension de ID : colonne
      int *c = (int *) malloc(sizeof(int)* $6.taille);
      if (c == NULL) { // Échec de l'allocation, gérer l'erreur
        fprintf(stderr,"Allocation mémoire échouée\n");
        exit(MEMORY_FAILURE);
      }
      int c_taille = 0;
      tmp = $6.taille;
      for (int i = 0; i < $6.taille; i++){
        if ($6.liste[i]==-1){
          tmp += id->var->val.matrix->c;
          c = (int *)realloc(c, tmp * sizeof(int));
          if (c == NULL) { // Échec de l'allocation, gérer l'erreur
            fprintf(stderr,"Allocation mémoire échouée\n");
            exit(MEMORY_FAILURE);
          }
          for (int k = 0; k < id->var->val.matrix->c; k++){
            c[c_taille] = k;
            c_taille++;
          }
        } else {
          c[c_taille] = $6.liste[i];
          c_taille++;
        }
      }

      if (l_taille == 1 && c_taille == 1){
        // on va juste chercher le float correspondant (ici le type c'est float et pas matrix)
        valeur val;
        $$.ptr = newtemp(SYMTAB, FLOAT, val);
        Symbol * t = symtable_indices(SYMTAB,(Indices){l[0],c[0]});
        gencode(CODE,GET_ELEMENT,$$.ptr,id,t);
        free(l);
        free(c);
      } else {
        Symbol * ligne = symtable_extract(SYMTAB,(Extract){l,l_taille});
        // créer la matrice temporaire de bonnes dimensions
        Matrix *mat1 = create_matrix(l_taille, m->c);
        valeur val1;
        val1.matrix = mat1;
        Symbol * intermediaire = newtemp(SYMTAB, MATRIX, val1);
        gencode(CODE,EXTR_LIGNE,intermediaire,id,ligne); 

        Symbol * colonne = symtable_extract(SYMTAB,(Extract){c,c_taille});
        // créer la matrice temporaire de bonnes dimensions
        Matrix *mat2 = create_matrix(l_taille, c_taille);
        valeur val2;
        val2.matrix = mat2;
        $$.ptr = newtemp(SYMTAB, MATRIX, val2);
        gencode(CODE,EXTR_COLONNE,$$.ptr,intermediaire,colonne); 
      }
      free($3.liste);
      free($6.liste);
    }

  | expr MINUS expr
    { 
      //Vérification des types des opérateurs
      unsigned type1, type2, type;
      switch($1.ptr->kind){
        case(NAME):
          type1 = $1.ptr->var->type;
          break;
        case(CONST_INT):
          type1 = INT;
          break;
        case(CONST_FLOAT):
          type1 = FLOAT;
          break;
        default:
          fprintf(stderr,"Ligne %d : Erreur de type dans une soustraction.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
          break;
      }
      switch($3.ptr->kind){
        case(NAME):
          type2 = $3.ptr->var->type;
          break;
        case(CONST_INT):
          type2 = INT;
          break;
        case(CONST_FLOAT):
          type2 = FLOAT;
          break;
        default:
          fprintf(stderr,"Ligne %d : Erreur de type dans une soustraction.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
          break;
      }
      //Si on a une matrice dans le calcul
      valeur val;
      if (type1 == MATRIX || type2 == MATRIX) {
        type = MATRIX;

        // Si 2 matrices : vérifier si elles font la même taille
        if ((type1 == MATRIX && type2 == MATRIX)&&(($1.ptr->var->val.matrix->l != $3.ptr->var->val.matrix->l) || ($1.ptr->var->val.matrix->c != $3.ptr->var->val.matrix->c))){
          fprintf(stderr,"Ligne %d : Incompatibilité entre les tailles de matrice lors d'une soustraction.\n", nb_ligne);
          exit(SEMANTIC_FAILURE);
        }

          // créer une matrix pour la var temporaire
        if (type1 == MATRIX){
          Matrix *m = create_matrix($1.ptr->var->val.matrix->l, $1.ptr->var->val.matrix->c);
          val.matrix = m;
        } else {
          Matrix *m = create_matrix($3.ptr->var->val.matrix->l, $3.ptr->var->val.matrix->c);
          val.matrix = m;
        }
      }
      else if (type1 == FLOAT || type2 == FLOAT) 
        type = FLOAT;
      else 
        type = INT;

      $$.ptr = newtemp(SYMTAB, type, val);
      gencode(CODE,BOP_MINUS,$$.ptr,$1.ptr,$3.ptr); 
    }

  | expr MULT expr
    {
      //Vérification des types des opérateurs
      unsigned type1, type2, type;
      switch($1.ptr->kind){
        case(NAME):
          type1 = $1.ptr->var->type;
          break;
        case(CONST_INT):
          type1 = INT;
          break;
        case(CONST_FLOAT):
          type1 = FLOAT;
          break;
        default:
          fprintf(stderr,"Ligne %d : Erreur de type dans une multiplication.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
          break;
      }
      switch($3.ptr->kind){
        case(NAME):
          type2 = $3.ptr->var->type;
          break;
        case(CONST_INT):
          type2 = INT;
          break;
        case(CONST_FLOAT):
          type2 = FLOAT;
          break;
        default:
          fprintf(stderr,"Ligne %d : Erreur de type dans une multiplication.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
          break;
      }

      //Vérification sur les matrices, s'il y en a
      valeur val;
      if (type1 == MATRIX || type2 == MATRIX) {
        type = MATRIX;

      //Si 2 matrices : vérifier que les tailles sont compatibles
      if ((type1 == MATRIX && type2 == MATRIX) && ($1.ptr->var->val.matrix->c != $3.ptr->var->val.matrix->l)) {
        fprintf(stderr,"Ligne %d : Incompatibilité entre les dimensions de matrices lors d'une multiplication.\n", nb_ligne);
        exit(SEMANTIC_FAILURE);
      }

      // Créer une matrice pour la variable temporaire
      if (type1 == MATRIX && type2 == MATRIX){
        Matrix *m = create_matrix($1.ptr->var->val.matrix->l, $3.ptr->var->val.matrix->c);
        val.matrix = m;
      }
      else if (type1 == MATRIX){
        Matrix *m = create_matrix($1.ptr->var->val.matrix->l, $1.ptr->var->val.matrix->c);
        val.matrix = m;
      } else {
        Matrix *m = create_matrix($3.ptr->var->val.matrix->l, $3.ptr->var->val.matrix->c);
        val.matrix = m;
      }

      // TODO : vérif

      } else if (type1 == FLOAT || type2 == FLOAT){
          type = FLOAT;
      } else { 
          type = INT;
      }
      $$.ptr = newtemp(SYMTAB, type, val);
      gencode(CODE,BOP_MULT,$$.ptr,$1.ptr,$3.ptr); 
    }

  | expr DIV expr
    {
      //Vérification des types des opérateurs
      unsigned type1, type2, type;
      switch($1.ptr->kind){
        case(NAME):
          type1 = $1.ptr->var->type;
          break;
        case(CONST_INT):
          type1 = INT;
          break;
        case(CONST_FLOAT):
          type1 = FLOAT;
          break;
        default:
          fprintf(stderr,"Ligne %d : Erreur de type dans une division.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
          break;
      }
      switch($3.ptr->kind){
        case(NAME):
          type2 = $3.ptr->var->type;
          break;
        case(CONST_INT):
          type2 = INT;
          break;
        case(CONST_FLOAT):
          type2 = FLOAT;
          break;
        default:
          fprintf(stderr,"Ligne %d : Erreur de type dans une division.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
          break;
      }

      //Vérification sur les matrices, s'il y en a
      valeur val;
      if (type1 == MATRIX || type2 == MATRIX) {
        type = MATRIX;

      //Si 2 matrices : vérifier que les tailles sont compatibles
      if ((type1 == MATRIX && type2 == MATRIX) && ($1.ptr->var->val.matrix->c != $3.ptr->var->val.matrix->l)) {
        fprintf(stderr,"Ligne %d : Incompatibilité entre les dimensions de matrices lors d'une division.\n", nb_ligne);
        exit(SEMANTIC_FAILURE);
      }

      // Créer une matrice pour la variable temporaire
      if (type1 == MATRIX && type2 == MATRIX){
        Matrix *m = create_matrix($1.ptr->var->val.matrix->l, $3.ptr->var->val.matrix->c);
        val.matrix = m;
      }
      else if (type1 == MATRIX){
        Matrix *m = create_matrix($1.ptr->var->val.matrix->l, $1.ptr->var->val.matrix->c);
        val.matrix = m;
      } else {
        Matrix *m = create_matrix($3.ptr->var->val.matrix->l, $3.ptr->var->val.matrix->c);
        val.matrix = m;
      }

      // TODO : vérif

      } else {
        type = FLOAT; // un entier / par un entier -> un float
      }
      $$.ptr = newtemp(SYMTAB, type, val);
      gencode(CODE,BOP_DIV,$$.ptr,$1.ptr,$3.ptr); 
    }





  
  //| MINUS expr %prec UMINUS
    /*{ 
      $$.ptr = newtemp(SYMTAB);
      gencode(CODE,UOP_MINUS,$$.ptr,$2.ptr,NULL); 
    }*/
  | TILDE expr 
  ; 

extraction 
  : intervalle { $$ = $1; }
  | intervalle SEMICOLON extraction { $$ = concat_extract_liste($1,$3); }
  ;

intervalle 
  : MULT  { $$ = creer_liste_extract(-1); }
  | V_INT DOTDOT V_INT 
    { 
      if ($1 > $3) {
        fprintf(stderr, "Ligne %d : Dans une extraction, l'indice de gauche doit être inférieur ou égal à l'indice de droite.\n",nb_ligne);
        exit(SEMANTIC_FAILURE);
      }
      $$ = creer_liste_extract_intervalle($1,$3);
    }
  | V_INT { $$ = creer_liste_extract($1) ; }
  
  ;

// STRUCTURES DE CONTROLE
expr_bool
  /* règle supprimée à cause de conflits
  : expr 
    {
      unsigned type;
      switch($1.ptr->kind){
      case(NAME):
        assert($1.ptr->var->init);
        type = $1.ptr->var->type;
        break;
      case(CONST_INT):
        type = INT;
        break;
      case(CONST_FLOAT):
        type = FLOAT;
        break;
      default:
        fprintf(stderr,"Ligne %d : Une expression booléenne ne manipule que des expressions correspondant à des int ou des float.\n",nb_ligne);
        exit(1);
        break;
      }
      if (type==INT || type==FLOAT){
        valeur val;
        $$.ptr = newtemp(SYMTAB, INT, val);
        gencode(CODE,B_EVAL,$$.ptr,$1.ptr, NULL);

        
        //$$.true = crelist(CODE->nextquad);
        //gencode(CODE,IF_ID_PTR_GOTO,$1.ptr,NULL,NULL);
        //$$.false = crelist(CODE->nextquad);
        //gencode(CODE,GOTO,NULL,NULL,NULL);
        
      } else {
        fprintf(stderr, "Ligne %d : Une expression booléenne ne manipule que des expressions correspondant à des int ou des float.\n",nb_ligne);
        exit(1);
      }
    }
    */
  : expr_bool OR expr_bool
    {
      valeur val;
      $$.ptr = newtemp(SYMTAB, INT, val);
      gencode(CODE,B_OU,$$.ptr,$1.ptr,$3.ptr);
      // complete($1.false, $4);
    }
  | expr_bool AND expr_bool
    {
      valeur val;
      $$.ptr = newtemp(SYMTAB, INT, val);
      gencode(CODE,B_ET,$$.ptr,$1.ptr,$3.ptr);
    }
  | NOT expr_bool
    {
      valeur val;
      $$.ptr = newtemp(SYMTAB, INT, val);
      gencode(CODE,B_NOT,$$.ptr,$2.ptr,NULL);
    }
  | LPAR expr_bool RPAR
    {
      $$.ptr = $2.ptr;
    }
  | expr LESS_THAN expr 
    {
    unsigned type1, type2;

    switch($1.ptr->kind){
      case(NAME):
        assert($1.ptr->var->init);
        type1 = $1.ptr->var->type;
        break;
      case(CONST_INT):
        type1 = INT;
        break;
      case(CONST_FLOAT):
        type1 = FLOAT;
        break;
      default:
        fprintf(stderr,"Ligne %d : Une expression booléenne ne manipule que des expressions correspondant à des int ou des float.\n",nb_ligne);
        exit(SEMANTIC_FAILURE);
        break;
    }

    switch($3.ptr->kind){
      case(NAME):
        assert($3.ptr->var->init);
        type2 = $3.ptr->var->type;
        break;
      case(CONST_INT):
        type2 = INT;
        break;
      case(CONST_FLOAT):
        type2 = FLOAT;
        break;
      default:
        fprintf(stderr,"Ligne %d : Une expression booléenne ne manipule que des expressions correspondant à des int ou des float.\n",nb_ligne);
        exit(SEMANTIC_FAILURE);
        break;
      }

    if ((type1==INT || type1==FLOAT) && (type1==INT || type1==FLOAT)) {
      valeur val;
      $$.ptr = newtemp(SYMTAB, INT, val);
      gencode(CODE,B_LT,$$.ptr,$1.ptr,$3.ptr);
      /*
      $$.true = crelist(CODE->nextquad);
      gencode(CODE,IF_ID_PTR_GOTO,$1.ptr,NULL,NULL);
      $$.false = crelist(CODE->nextquad);
      gencode(CODE,GOTO,NULL,NULL,NULL);
      */
    } else {
      fprintf(stderr, "Ligne %d : Une expression booléenne ne manipule que des expressions correspondant à des int ou des float.\n",nb_ligne);
      exit(SEMANTIC_FAILURE);
    }

    
    }
  | expr LTOE expr 
    {
    unsigned type1, type2;

    switch($1.ptr->kind){
      case(NAME):
        assert($1.ptr->var->init);
        type1 = $1.ptr->var->type;
        break;
      case(CONST_INT):
        type1 = INT;
        break;
      case(CONST_FLOAT):
        type1 = FLOAT;
        break;
      default:
        fprintf(stderr,"Ligne %d : Une expression booléenne ne manipule que des expressions correspondant à des int ou des float.\n",nb_ligne);
        exit(SEMANTIC_FAILURE);
        break;
    }

    switch($3.ptr->kind){
      case(NAME):
        assert($3.ptr->var->init);
        type2 = $3.ptr->var->type;
        break;
      case(CONST_INT):
        type2 = INT;
        break;
      case(CONST_FLOAT):
        type2 = FLOAT;
        break;
      default:
        fprintf(stderr,"Ligne %d : Une expression booléenne ne manipule que des expressions correspondant à des int ou des float.\n",nb_ligne);
        exit(SEMANTIC_FAILURE);
        break;
      }

    if ((type1==INT || type1==FLOAT) && (type1==INT || type1==FLOAT)) {
      valeur val;
      $$.ptr = newtemp(SYMTAB, INT, val);
      gencode(CODE,B_LTOE,$$.ptr,$1.ptr,$3.ptr);
      /*
      $$.true = crelist(CODE->nextquad);
      gencode(CODE,IF_ID_PTR_GOTO,$1.ptr,NULL,NULL);
      $$.false = crelist(CODE->nextquad);
      gencode(CODE,GOTO,NULL,NULL,NULL);
      */
    } else {
      fprintf(stderr, "Ligne %d : Une expression booléenne ne manipule que des expressions correspondant à des int ou des float.\n",nb_ligne);
      exit(SEMANTIC_FAILURE);
    }    
    }

  | expr GREATER_THAN expr 
    {
      unsigned type1, type2;

      switch($1.ptr->kind){
        case(NAME):
          assert($1.ptr->var->init);
          type1 = $1.ptr->var->type;
          break;
        case(CONST_INT):
          type1 = INT;
          break;
        case(CONST_FLOAT):
          type1 = FLOAT;
          break;
        default:
          fprintf(stderr,"Ligne %d : Une expression booléenne ne manipule que des expressions correspondant à des int ou des float.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
          break;
      }

      switch($3.ptr->kind){
        case(NAME):
          assert($3.ptr->var->init);
          type2 = $3.ptr->var->type;
          break;
        case(CONST_INT):
          type2 = INT;
          break;
        case(CONST_FLOAT):
          type2 = FLOAT;
          break;
        default:
          fprintf(stderr,"Ligne %d : Une expression booléenne ne manipule que des expressions correspondant à des int ou des float.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
          break;
        }

      if ((type1==INT || type1==FLOAT) && (type1==INT || type1==FLOAT)) {
        valeur val;
        $$.ptr = newtemp(SYMTAB, INT, val);
        gencode(CODE,B_GT,$$.ptr,$1.ptr,$3.ptr);
        /*
        $$.true = crelist(CODE->nextquad);
        gencode(CODE,IF_ID_PTR_GOTO,$1.ptr,NULL,NULL);
        $$.false = crelist(CODE->nextquad);
        gencode(CODE,GOTO,NULL,NULL,NULL);
        */
      } else {
        fprintf(stderr, "Ligne %d : Une expression booléenne ne manipule que des expressions correspondant à des int ou des float.\n",nb_ligne);
        exit(SEMANTIC_FAILURE);
      }
    }
  | expr GTOE expr 
    {
      unsigned type1, type2;

      switch($1.ptr->kind){
        case(NAME):
          assert($1.ptr->var->init);
          type1 = $1.ptr->var->type;
          break;
        case(CONST_INT):
          type1 = INT;
          break;
        case(CONST_FLOAT):
          type1 = FLOAT;
          break;
        default:
          fprintf(stderr,"Ligne %d : Une expression booléenne ne manipule que des expressions correspondant à des int ou des float.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
          break;
      }

      switch($3.ptr->kind){
        case(NAME):
          assert($3.ptr->var->init);
          type2 = $3.ptr->var->type;
          break;
        case(CONST_INT):
          type2 = INT;
          break;
        case(CONST_FLOAT):
          type2 = FLOAT;
          break;
        default:
          fprintf(stderr,"Ligne %d : Une expression booléenne ne manipule que des expressions correspondant à des int ou des float.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
          break;
        }

      if ((type1==INT || type1==FLOAT) && (type1==INT || type1==FLOAT)) {
        valeur val;
        $$.ptr = newtemp(SYMTAB, INT, val);
        gencode(CODE,B_GTOE,$$.ptr,$1.ptr,$3.ptr);
        /*
        $$.true = crelist(CODE->nextquad);
        gencode(CODE,IF_ID_PTR_GOTO,$1.ptr,NULL,NULL);
        $$.false = crelist(CODE->nextquad);
        gencode(CODE,GOTO,NULL,NULL,NULL);
        */
      } else {
        fprintf(stderr, "Ligne %d : Une expression booléenne ne manipule que des expressions correspondant à des int ou des float.\n",nb_ligne);
        exit(SEMANTIC_FAILURE);
      }
    }
  | expr EQUAL expr 
    {
      unsigned type1, type2;

      switch($1.ptr->kind){
        case(NAME):
          //assert($1.ptr->var->init);
          type1 = $1.ptr->var->type;
          break;
        case(CONST_INT):
          type1 = INT;
          break;
        case(CONST_FLOAT):
          type1 = FLOAT;
          break;
        default:
          fprintf(stderr,"Ligne %d : Une expression booléenne ne manipule que des expressions correspondant à des int ou des float.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
          break;
      }

      switch($3.ptr->kind){
        case(NAME):
          assert($3.ptr->var->init);
          type2 = $3.ptr->var->type;
          break;
        case(CONST_INT):
          type2 = INT;
          break;
        case(CONST_FLOAT):
          type2 = FLOAT;
          break;
        default:
          fprintf(stderr,"Ligne %d : Une expression booléenne ne manipule que des expressions correspondant à des int ou des float.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
          break;
        }

      if ((type1==INT || type1==FLOAT) && (type1==INT || type1==FLOAT)) {
        valeur val;
        $$.ptr = newtemp(SYMTAB, INT, val);
        gencode(CODE,B_EQUAL,$$.ptr,$1.ptr,$3.ptr);
        /*
        $$.true = crelist(CODE->nextquad);
        gencode(CODE,IF_ID_PTR_GOTO,$1.ptr,NULL,NULL);
        $$.false = crelist(CODE->nextquad);
        gencode(CODE,GOTO,NULL,NULL,NULL);
        */
      } else {
        fprintf(stderr, "Ligne %d : Une expression booléenne ne manipule que des expressions correspondant à des int ou des float.\n",nb_ligne);
        exit(SEMANTIC_FAILURE);
      }
    }
  | expr NOT_EQUAL expr 
    {
      unsigned type1, type2;

      switch($1.ptr->kind){
        case(NAME):
          assert($1.ptr->var->init);
          type1 = $1.ptr->var->type;
          break;
        case(CONST_INT):
          type1 = INT;
          break;
        case(CONST_FLOAT):
          type1 = FLOAT;
          break;
        default:
          fprintf(stderr,"Ligne %d : Une expression booléenne ne manipule que des expressions correspondant à des int ou des float.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
          break;
      }

      switch($3.ptr->kind){
        case(NAME):
          assert($3.ptr->var->init);
          type2 = $3.ptr->var->type;
          break;
        case(CONST_INT):
          type2 = INT;
          break;
        case(CONST_FLOAT):
          type2 = FLOAT;
          break;
        default:
          fprintf(stderr,"Ligne %d : Une expression booléenne ne manipule que des expressions correspondant à des int ou des float.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
          break;
        }

      if ((type1==INT || type1==FLOAT) && (type1==INT || type1==FLOAT)) {
        valeur val;
        $$.ptr = newtemp(SYMTAB, INT, val);
        gencode(CODE,B_NOT_EQUAL,$$.ptr,$1.ptr,$3.ptr);
        /*
        $$.true = crelist(CODE->nextquad);
        gencode(CODE,IF_ID_PTR_GOTO,$1.ptr,NULL,NULL);
        $$.false = crelist(CODE->nextquad);
        gencode(CODE,GOTO,NULL,NULL,NULL);
        */
      } else {
        fprintf(stderr, "Ligne %d : Une expression booléenne ne manipule que des expressions correspondant à des int ou des float.\n",nb_ligne);
        exit(SEMANTIC_FAILURE);
      }
    }
  ;

debut_bloc
  : LCURLY 
  {
    gencode(CODE, DEBUT_BLOC, NULL, NULL, NULL);
  }
  ;

/* PAS UTILISÉ
fin_bloc 
  : RCURLY
  {
    gencode(CODE, FIN_BLOC, NULL, NULL, NULL);
  }
  ;
*/

evaluation_if
  : LPAR expr_bool RPAR 
  {
    gencode(CODE,DEBUT_IF,$2.ptr,NULL,NULL);
  }
  ;

fin_bloc_if
  : RCURLY
  {
    gencode(CODE, FIN_BLOC, NULL, NULL, NULL);
    gencode(CODE, FIN_IF, NULL, NULL, NULL);
  }
  ;

saut_fin_if 
  : RCURLY
  {
    gencode(CODE, JUMP_FIN_IF, NULL, NULL, NULL);
    gencode(CODE, FIN_BLOC, NULL, NULL, NULL);
  }

struct_if
  : IF evaluation_if debut_bloc bloc fin_bloc_if
  | IF evaluation_if debut_bloc bloc saut_fin_if ELSE debut_bloc bloc fin_bloc_if
  ;

debut_while
  : LPAR
  {
    gencode(CODE,DEBUT_WHILE,NULL,NULL,NULL);
  }
  ;

fin_while
  :  RPAR 
  {
    gencode(CODE,FIN_WHILE,NULL,NULL,NULL);
  }

saut_fin_while
  : RCURLY
  {
    gencode(CODE, JUMP_DEBUT_WHILE, NULL, NULL, NULL);
    gencode(CODE, FIN_BLOC, NULL, NULL, NULL);
  }

struct_while 
  : WHILE debut_while expr_bool fin_while debut_bloc bloc saut_fin_while
  ;

init_for 
  : ID ASSIGN expr 
    {
      // sémantique 
      Symbol * id = symtable_get(SYMTAB,$1);
      if (id == NULL){
        fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été déclarée.\n",nb_ligne,$1);
        exit(SEMANTIC_FAILURE);
      }

      if (id->var->type != NAME && id->var->type != INT){
        fprintf(stderr,"Ligne %d : L'itérateur d'une boucle for doit être de type INT.\n",nb_ligne);
        exit(SEMANTIC_FAILURE);
      }

      unsigned type2;
      switch($3.ptr->kind){
        case(NAME):
          assert($3.ptr->var->init);
          type2 = $3.ptr->var->type;
          break;
        case(CONST_INT):
          type2 = INT;
          break;
        default:
          fprintf(stderr,"Ligne %d : L'itérateur d'une boucle for doit être de type INT.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
          break;
      }
      if (type2 != INT){
        fprintf(stderr,"Ligne %d : L'itérateur d'une boucle for doit être de type INT.\n",nb_ligne);
        exit(SEMANTIC_FAILURE);
      }

      // marquer que la variable est initialisée
      id->var->init = true;
      
      gencode(CODE,COPY,id,$3.ptr,NULL);

      $$ = id;
    }
  ; // init d'un itérateur => id (int déjà déclarée) = int

debut_for
  : SEMICOLON
    {
      gencode(CODE,DEBUT_FOR,NULL,NULL,NULL);
    }
  ;

fin_for 
  : SEMICOLON
    {
      gencode(CODE,FIN_FOR,NULL,NULL,NULL);
    }
  ;

jump_debut_for 
  : LCURLY
    {
      gencode(CODE,JUMP_DEBUT_FOR,NULL,NULL,NULL);
      gencode(CODE,DEBUT_BLOC, NULL, NULL, NULL);
    }
  ;

jump_maj_for 
  : RCURLY
    {
      gencode(CODE,JUMP_MAJ_FOR,NULL,NULL,NULL);
      gencode(CODE,FIN_BLOC, NULL, NULL, NULL);
    }
  ;

struct_for 
  : FOR LPAR init_for debut_for expr_bool fin_for affectation RPAR jump_debut_for bloc jump_maj_for
    {
      // vérifier que le id de init_for c'est le meme que le id de affectation
      // l'énoncé précise mise à jour de l'itérateur
      if ($3 != $7){
        fprintf(stderr,"Ligne %d : L'itérateur initialisé dans la première partie du for doit être l'itérateur mis à jour dans la troisième partie du for.\n",nb_ligne);
        exit(SEMANTIC_FAILURE);
      }
    }
  ;

bloc
  : %empty
  | afficher SEMICOLON bloc
  | affectation SEMICOLON bloc
  | struct_if bloc
  | struct_while bloc
  | struct_for bloc
// TO CHANGE
  | affectation_fonction bloc
  
  ;

// RETURN
retour
  : RETURN V_INT SEMICOLON
    {
      $$.ptr = symtable_const_int(SYMTAB,$2); 
    }
  | RETURN V_FLOAT SEMICOLON
    {
      fprintf(stderr,"Ligne %d : Possibilité de renvoyer des int seulement.\n",nb_ligne);
      exit(SEMANTIC_FAILURE);
    }
  | RETURN LPAR expr RPAR SEMICOLON
    {
      unsigned type1;
      switch($3.ptr->kind){
        case(NAME):
          assert($3.ptr->var->init);
          type1 = $3.ptr->var->type;
          break;
        case(CONST_INT):
          type1 = INT;
          break;
        default:
          fprintf(stderr,"Ligne %d : Incompatibilité de type entre la valeur renvoyer et le type de la fonction.\n",nb_ligne);
          exit(SEMANTIC_FAILURE);
          break;
      }
      // exactement le même message dans la regle fonction_principale
      if (type1 != INT){
        fprintf(stderr,"Ligne %d : Incompatibilité de type entre la valeur renvoyer et le type de la fonction.\n",nb_ligne);
        exit(SEMANTIC_FAILURE);
        break;
      }
      $$ = $3;
    }
  ;


%%

void yyerror(const char * s)
{
  fprintf(stderr,"Ligne %d : %s\n",nb_ligne,s);
}

