
%{
  #include "cmat.h" // pas besoin ?
  #include "error.h"
  #include "symbtab.h"
  #include "generation.h"
  #include <stdio.h>// ?
  #include <stdlib.h>// ?

  extern void yyerror(const char * s);
  extern int yylex();
  extern unsigned nb_ligne;
%}

%union {
  int intval;
  float floatval;
  name_t strval;
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
}

%token NOT AND OR DOT MAIN PRINTF PRINTMAT MATRIX INT FLOAT IF ELSE WHILE FOR TILDE PLUSPLUS MINUSMINUS DIV LCURLY RCURLY LBRACKET RBRACKET COMMA EQUAL QUOTE APOSTROPHE BACKSLASH RETURN EXIT  NOT_EQUAL LESS_THAN GREATER_THAN LTOE GTOE

%token PRINT SEMICOLON PLUS MINUS MULT ASSIGN LPAR RPAR
%token <strval> ID
%token <intval> V_INT 
%token <floatval> V_FLOAT
%token <string> V_STRING

%left MULT DIV EQUAL NOT_EQUAL
%left PLUS MINUS GTOE LTOE GREATER_THAN LESS_THAN
%nonassoc UMINUS

%type <type> type_var
%type<l_var> liste_var
%type<var> une_var
%type<intval> taille 
%type<floatval> valeur_matrix
%type<matrix> init_matrix liste_matrix_ligne matrix_une_ligne matrix_remplir_colonne 
%type <exprval> expr expr_bool matrix_litt
%type <variable> affectation init_for
%type<liste_extract> extraction intervalle

%start Start

%%

Start 
  : %empty
  | fonction_principale
  ;

fonction_principale 
  : INT MAIN LPAR RPAR debut_bloc suite_instructions fin_bloc
  ; 

debut_bloc
  : LCURLY 
  {
    gencode(CODE, DEBUT_BLOC, NULL, NULL, NULL);
  }
  ;

fin_bloc 
  : RCURLY
  {
    gencode(CODE, FIN_BLOC, NULL, NULL, NULL);
  }
  ;


suite_instructions 
  : %empty
  | instr SEMICOLON suite_instructions
  | struct_if suite_instructions
  | struct_while suite_instructions
  | struct_for suite_instructions
  ;

instr 
  : declaration_variable  
  | afficher 
  | affectation
  ;

affectation
  : ID ASSIGN expr
    {
      // sémantique : vérifier que l'id existe
      Symbol * id = symtable_get(SYMTAB,$1);
      if ( id == NULL ){
        fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été déclarée.\n",nb_ligne,$1);
        exit(1);
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
          exit(1);
          break;
      }

      if (type1 == MATRIX && type2 != MATRIX){ 
        fprintf(stderr,"Ligne %d : Incompatibilité de types.\n",nb_ligne);
        exit(1);
      }
      else if (type1 == INT && type2 != INT) {
        fprintf(stderr,"Ligne %d : Incompatibilité de types.\n",nb_ligne);
        exit(1);
      }
      else if (type1 == FLOAT && (type2 != FLOAT && type2 != INT)){
        fprintf(stderr,"Ligne %d : Incompatibilité de types.\n",nb_ligne);
        exit(1);
      }

      if (type1 == MATRIX && type2 == MATRIX){ 
        if ((id->var->val.matrix->l != $3.ptr->var->val.matrix->l) || (id->var->val.matrix->c != $3.ptr->var->val.matrix->c)){
          fprintf(stderr,"Ligne %d : Incompatibilité entre les tailles de matrice lors de l'affectation à %s.\n",nb_ligne, $1);
          exit(1);
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
        exit(1);
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
          exit(1);
          break;
      }

      // vérifier qu'il s'agit d'une matrice
      if (type1 != MATRIX){
        fprintf(stderr,"Ligne %d : %s n'est pas une matrice, on ne peut donc pas accéder à %s[%d].\n",nb_ligne,id->name,id->name,$3);
        exit(1);
      }

      // vérifier que c'est une matrice à une dimension
      if (id->var->val.matrix->l != 1){
        fprintf(stderr,"Ligne %d : La matrice %s a deux dimensions : impossible d'affecter quelque chose à %s[%d].\n",nb_ligne,id->name,id->name,$3);
        exit(1);
      }

      // vérifier que 0 <= $3 < taille colonne
      if (id->var->val.matrix->c <= $3){
        fprintf(stderr,"Ligne %d : La matrice %s a %d colonnes : %s[%d] out of range.\n",nb_ligne,id->name,id->var->val.matrix->c,id->name,$3);
        exit(1);
      }

      // vérifier que le type de l'expr est compatible avec le type de ID
      if (type2 == MATRIX) {
        fprintf(stderr,"Ligne %d : Incompatibilité de types.\n",nb_ligne);
        exit(1);
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
        exit(1);
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
          exit(1);
          break;
      }

      // vérifier qu'il s'agit d'une matrice
      if (type1 != MATRIX){
        fprintf(stderr,"Ligne %d : %s n'est pas une matrice, on ne peut donc pas accéder à %s[%d].\n",nb_ligne,id->name,id->name,$3);
        exit(1);
      }

      // vérifier que 0 <= $3 < taille ligne
      if (id->var->val.matrix->l <= $3){
        fprintf(stderr,"Ligne %d : La matrice %s a %d lignes : %s[%d][%d] out of range.\n",nb_ligne,id->name,id->var->val.matrix->l,id->name,$3,$6);
        exit(1);
      }

      // vérifier que 0 <= $6 < taille colonne
      if (id->var->val.matrix->c <= $6){
        fprintf(stderr,"Ligne %d : La matrice %s a %d colonnes : %s[%d][%d] out of range.\n",nb_ligne,id->name,id->var->val.matrix->c,id->name,$3,$6);
        exit(1);
      }

      // vérifier que le type de l'expr est compatible avec le type de ID
      if (type2 == MATRIX) {
        fprintf(stderr,"Ligne %d : Incompatibilité de types.\n",nb_ligne);
        exit(1);
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
        exit(1);
      }

      // sémantique vérifier que le type de l'expr est compatible avec le type de ID

      unsigned type1;

      type1 = id->var->type;

      if (type1 != MATRIX){ 
        fprintf(stderr,"Ligne %d : Incompatibilité de types.\n",nb_ligne);
        exit(1);
      }

      // marquer que la variable est initialisée
      id->var->init = true;
      
      gencode(CODE,COPY,id,$3.ptr,NULL);
      $$ = id; // utile pour for
    }
  
  //| i ++, i-- ...
 
  ;


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
          fprintf(stderr,"Ligne %d : Ligne %d : La variable '%s' a déjà été déclarée.\n",nb_ligne, nb_ligne,v->name);
          exit(1);
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
              exit(1);
              break;
          }
          assert(type3 == INT || type3 == FLOAT || type3 == MATRIX);
          // fprintf + exit

          // vérifier la compatibilité des types
          if(type1 == MATRIX && (type2 != MATRIX || type3 != MATRIX)){
            if (type2 != MATRIX){
              fprintf(stderr,"Ligne %d : Il est nécessaire de déclarer les dimensions de %s.\n",nb_ligne,v->name);
              exit(1);
            }  // si type 2 != MATRIX : manque les dimensions à la déclaration
            fprintf(stderr,"Ligne %d : Incompatibilité de types.\n",nb_ligne);
            exit(1);
          }

          if(type1 == INT && (type2 != 0 || type3 != INT)){
            fprintf(stderr,"Ligne %d : Incompatibilité de types.\n",nb_ligne);
            exit(1);
          }

          if(type1 == FLOAT && (type2 != 0 || (type3 != INT && type3 != FLOAT))){
            fprintf(stderr,"Ligne %d : Incompatibilité de types.\n",nb_ligne);
            exit(1);
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
              exit(1);
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
            exit(1);
          }

          v->init = true;
          gencode(CODE,COPY,res,s,NULL);

        } else { // pas d'affectation

          // vérifier la compatibilité des types
          if(type1 == MATRIX && type2 != MATRIX){
            fprintf(stderr,"Ligne %d : Il est nécessaire de déclarer les dimensions de %s.\n",nb_ligne,v->name);
            exit(1);
          }

          if(type1 == INT && type2 != 0){
            fprintf(stderr,"Ligne %d : Incompatibilité de types.\n",nb_ligne);
            exit(1);
          }

          if(type1 == FLOAT && type2 != 0){
            fprintf(stderr,"Ligne %d : Incompatibilité de types.\n",nb_ligne);
            exit(1);
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
            exit(1);
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
  ;

taille
  : LBRACKET V_INT RBRACKET 
  { 
    if ($2==0){
      fprintf(stderr, "Ligne %d : Une dimension de matrice ne peut pas être nulle.\n",nb_ligne);
      exit(1);
    }
    $$ = $2; 
  }
  ;

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
            exit(1);
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

afficher 
  : PRINT LPAR ID RPAR {
    Symbol * id = symtable_get(SYMTAB,$3);
    if (id == NULL) {
      fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été déclarée, elle ne peut donc pas être affichée.\n",nb_ligne,$3);
      exit(1);
    } else if (!(id->var->type == INT || id->var->type == FLOAT)){
      fprintf(stderr,"Ligne %d : La variable '%s' n'est pas de type INT ou FLOAT, elle ne peut donc pas être affichée à l'aide de la fonction print.\n",nb_ligne,$3);
      exit(1);
    } else if (!id->var->init){
      fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été initialisée, elle ne peut donc pas être affichée.\n",nb_ligne,$3);
      exit(1);
    }
    gencode(CODE,CALL_PRINT,id,NULL,NULL);
  }

  | PRINTMAT LPAR ID RPAR {
    Symbol * id = symtable_get(SYMTAB,$3);
    if (id == NULL) {
      fprintf(stderr,"Ligne %d : La matrice '%s' n'a jamais été déclarée, elle ne peut donc pas être affichée.\n",nb_ligne,$3);
      exit(1);
    } else if (!(id->var->type == MATRIX)){
      fprintf(stderr,"Ligne %d : La variable '%s' n'est pas de type MATRIX, elle ne peut donc pas être affichée à l'aide de la fonction mat.\n",nb_ligne,$3);
      exit(1);
    } else if (!id->var->init){
      fprintf(stderr,"Ligne %d : La matrice '%s' n'a jamais été initialisée, elle ne peut donc pas être affichée.\n",nb_ligne,$3);
      exit(1);
    }
    gencode(CODE,CALL_PRINTMAT,id,NULL,NULL);
  }

  | PRINTF LPAR V_STRING RPAR {
      Symbol * id = symtable_string(SYMTAB, $3);
      gencode(CODE,CALL_PRINTF,id,NULL,NULL);
    }
  ;

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
        exit(1);
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
        exit(1);
        break;
    }

    valeur val;
    if (type1 == MATRIX || type2 == MATRIX) {
      type = MATRIX;

      // vérifier même tailles si les 2 sont matrices
      if ((type1 == MATRIX && type2 == MATRIX)&&(($1.ptr->var->val.matrix->l != $3.ptr->var->val.matrix->l) || ($1.ptr->var->val.matrix->c != $3.ptr->var->val.matrix->c))){
        fprintf(stderr,"Ligne %d : Incompatibilité entre les tailles de matrice lors d'une addition.\n",nb_ligne);
        exit(1);
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
        exit(1);
    }
    if (!id->var->init){
      fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été initialisée, elle ne peut donc pas être utilisée dans une expression.\n",nb_ligne,$1);
      exit(1);
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
      exit(1);
    }

    if (!id->var->init){
      fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été initialisée, elle ne peut donc pas être utilisée dans une expression.\n",nb_ligne,$1);
      exit(1);
    }

    // 2. vérifier que c'est une matrice
    if (id->var->type != MATRIX){
      fprintf(stderr,"Ligne %d : La variable '%s' n'est pas de type matrix.\n",nb_ligne,$1);
      exit(1);
    }

    // 3. vérifier que c'est bien une matrice une dimension
    if (id->var->val.matrix->l != 1){
      fprintf(stderr,"Ligne %d : La matrix '%s' n'est pas de dimension 1.\n",nb_ligne,$1);
      exit(1);
    }

    // 4. vérifier correspondance dimensions matrice / extract

      Matrix *m = id->var->val.matrix;
      for (int i = 0; i < $3.taille; i++) {
        if ($3.liste[i] != -1 && $3.liste[i] >= m->c) {
          //fprintf(stderr, "l = %d , c = %d, $3.liste[i] = %d\n", m->l, m->c, $3.liste[i]);
          fprintf(stderr, "Ligne %d : Indice de colonne out of range.\n",nb_ligne);
          exit(1);
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
      exit(1);
    }

    if (!id->var->init){
      fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été initialisée, elle ne peut donc pas être utilisée dans une expression.\n",nb_ligne,$1);
      exit(1);
    }

    // 2. vérifier que c'est une matrice
    if (id->var->type != MATRIX){
      fprintf(stderr,"Ligne %d : La variable '%s' n'est pas de type matrix.\n",nb_ligne,$1);
      exit(1);
    }

    // vérifier correspondance dimensions matrice / extract
    Matrix *m = id->var->val.matrix;
    for (int i = 0; i < $3.taille; i++) {
      if ($3.liste[i] != -1 && $3.liste[i] >= m->l) {
        fprintf(stderr, "Ligne %d : Indice de ligne out of range.\n",nb_ligne);
        exit(1);
      }
    }
    for (int i = 0; i < $6.taille; i++) {
      if ($6.liste[i] != -1 && $6.liste[i] >= m->c) {
        fprintf(stderr, "Ligne %d : Indice de colonne out of range.\n",nb_ligne);
        exit(1);
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

  //| expr MINUS expr
    /*{ 
      $$.ptr = newtemp(SYMTAB);
      gencode(CODE,BOP_MINUS,$$.ptr,$1.ptr,$3.ptr); 
    }*/
  //| expr MULT expr
    /*{ 
      $$.ptr = newtemp(SYMTAB);
      gencode(CODE,BOP_MULT,$$.ptr,$1.ptr,$3.ptr); 
    }*/
  //| MINUS expr %prec UMINUS
    /*{ 
      $$.ptr = newtemp(SYMTAB);
      gencode(CODE,UOP_MINUS,$$.ptr,$2.ptr,NULL); 
    }*/
  ; 

extraction 
  : intervalle { $$ = $1; }
  | intervalle SEMICOLON extraction { $$ = concat_extract_liste($1,$3); }
  ;

intervalle 
  : MULT  { $$ = creer_liste_extract(-1); }
  | V_INT { $$ = creer_liste_extract($1) ; }
  | V_INT DOT DOT V_INT 
    { 
      if ($1 > $4) {
        fprintf(stderr, "Ligne %d : Dans une extraction, l'indice de gauche doit être inférieur ou égal à l'indice de droite.\n",nb_ligne);
        exit(1);
      }
      creer_liste_extract_intervalle($1,$4);
    }
  ;

expr_bool
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

        /*
        $$.true = crelist(CODE->nextquad);
        gencode(CODE,IF_ID_PTR_GOTO,$1.ptr,NULL,NULL);
        $$.false = crelist(CODE->nextquad);
        gencode(CODE,GOTO,NULL,NULL,NULL);
        */
      } else {
        fprintf(stderr, "Ligne %d : Une expression booléenne ne manipule que des expressions correspondant à des int ou des float.\n",nb_ligne);
        exit(1);
      }
    }
  | expr_bool OR expr_bool
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
        exit(1);
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
        exit(1);
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
      exit(1);
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
        exit(1);
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
        exit(1);
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
      exit(1);
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
          exit(1);
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
          exit(1);
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
        exit(1);
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
          exit(1);
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
          exit(1);
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
        exit(1);
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
          exit(1);
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
          exit(1);
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
        exit(1);
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
          exit(1);
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
          exit(1);
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
        exit(1);
      }
    }
  ;

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
        exit(1);
      }

      if (id->var->type != NAME && id->var->type != INT){
        fprintf(stderr,"Ligne %d : L'itérateur d'une boucle for doit être de type INT.\n",nb_ligne);
        exit(1);
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
          exit(1);
          break;
      }
      if (type2 != INT){
        fprintf(stderr,"Ligne %d : L'itérateur d'une boucle for doit être de type INT.\n",nb_ligne);
        exit(1);
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
        exit(1);
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
  ;

/*
struct_for 
  : FOR LPAR initialisation SEMICOLON expr_bool SEMICOLON mise_a_jour RPAR debut_bloc bloc fin_for
  // pas de déclaration dans l'initialisation et autoriser seulement les int.
  // condition c'est juste une expr_bool 
  // mise a jour
  ;

initialisation
jump condition
mise_a_jour 
condition
(jump fin_bloc)
bloc
jump mise a jour
fin bloc
// bloc juste 

fin_for 
  : RPAR

  // mise a jour
  // 
  // fin bloc
*/

/*
retour 
  : RETURN V_INT 
  ;
*/

%%

void yyerror(const char * s)
{
  fprintf(stderr,"Ligne %d : %s\n",nb_ligne,s);
}

