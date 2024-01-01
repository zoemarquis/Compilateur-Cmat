
%{

#include "cmat.h"
#include "symbtab.h"

extern void yyerror(const char * s);
extern int yylex();
%}

%union {
    long int intval;
    float floatval;
    name_t strval;
    struct {
        Symbol * ptr;
    } exprval;
    Symbol * variable;
    unsigned type;
    Liste_Variable l_var;
    variable * var;
    Matrix* matrix;
    char * string;
}

%token DOT MAIN PRINTF PRINTMAT MATRIX INT FLOAT STATIC CONST IF ELSE WHILE FOR TILDE PLUSPLUS MINUSMINUS DIV LCURLY RCURLY LBRACKET RBRACKET COMMA EQUAL QUOTE APOSTROPHE BACKSLASH RETURN EXIT  NOT_EQUAL LESS_THAN GREATER_THAN LTOE GTOE

%token PRINT SEMICOLON PLUS MINUS MULT ASSIGN LPAR RPAR
%token <strval> ID
%token <intval> V_INT 
%token <floatval> V_FLOAT
%token <string> V_STRING

%left PLUS MINUS
%left MULT
%nonassoc UMINUS

//%type <exprval> expr
%type <type> type_var
%type<l_var> liste_var
%type<var> une_var
%type<intval> taille 
%type<floatval> valeur_matrix
%type<matrix> init_matrix liste_matrix_ligne matrix_une_ligne matrix_remplir_colonne
%type <exprval> expr

%start Start

%%

Start 
  : %empty
  | fonction_principale
  ;

fonction_principale 
  : INT MAIN LPAR RPAR LCURLY suite_instructions RCURLY
  ; 

suite_instructions 
: instr SEMICOLON suite_instructions
| instr SEMICOLON
;

instr 
  : declaration_variable  
  | afficher 
  | ID ASSIGN expr
    {
      // sémantique : vérifier que l'id existe
      Symbol * id = symtable_get(SYMTAB,$1);
      if ( id == NULL ){
        fprintf(stderr,"La variable '%s' n'a jamais été déclarée.\n",$1);
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
          fprintf(stderr,"Erreur de type.\n");
          exit(1);
          break;
      }

      if (type1 == MATRIX && type2 != MATRIX){ 
        fprintf(stderr,"Incompatibilité de types.\n");
        exit(1);
      }
      else if (type1 == INT && type2 != INT) {
        fprintf(stderr,"Incompatibilité de types.\n");
        exit(1);
      }
      else if (type1 == FLOAT && (type2 != FLOAT && type2 != INT)){
        fprintf(stderr,"Incompatibilité de types.\n");
        exit(1);
      }

      if (type1 == MATRIX && type2 == MATRIX){ 
        if ((id->var->val.matrix->l != $3.ptr->var->val.matrix->l) || (id->var->val.matrix->c != $3.ptr->var->val.matrix->c)){
          fprintf(stderr,"Incompatibilité entre les tailles de matrice lors de l'affectation à %s.\n", $1);
          exit(1);
        }
      }

      // marquer que la variable est initialisée
      id->var->init = true;
      
      gencode(CODE,COPY,id,$3.ptr,NULL);
    }
  ;

declaration_variable
  : type_var liste_var {
      for (int i = 0; i < $2.taille; i++) {
        // vérifier la compatibilité entre $1 et les types de la liste

        if ($2.liste[i]->init && $1 != $2.liste[i]->type){
          if ($1 != FLOAT || $2.liste[i]->type != INT){
            fprintf(stderr,"Incompatibilité de type à la déclaration de %s.\n",$2.liste[i]->name);
            exit(1);
          } else {
            $2.liste[i]->val.flottant = (float)$2.liste[i]->val.entier;
          }
        } else if ($2.liste[i]->type == 0 && $1 == MATRIX){
          fprintf(stderr,"Il est nécessaire de déclarer les dimensions de %s.\n",$2.liste[i]->name);
          exit(1);
        }

        $2.liste[i]->type = $1;

        // vérifier que l'entrée n'existe pas
        Symbol * id = symtable_get(SYMTAB,$2.liste[i]->name);
        if (id) {
          fprintf(stderr,"La variable '%s' a déjà été déclarée.\n",$2.liste[i]->name);
          exit(1);
        }

        // ajouter
        symtable_put(SYMTAB,$2.liste[i]->name,$2.liste[i]);
      }
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
      $$.taille = 1;
      $$.liste = creer_var_liste($1);
    }
  | liste_var COMMA une_var 
    {
      $$.taille = $1.taille +1 ;
      $$.liste = ajouter_var_liste($$.liste, $$.taille, $3);
    }
  ;

une_var 
  : ID 
    { 
      valeur v;
      $$ = creer_variable($1, 0, false, v);
    }     
  | ID ASSIGN V_INT 
    {
      valeur v;
      v.entier = $3;
      $$ = creer_variable($1, INT, true, v);
    }
  | ID ASSIGN V_FLOAT   
    {
      valeur v;
      v.flottant = $3;
      $$ = creer_variable($1, FLOAT, true, v);
    }

  | ID taille 
    {
      if ($2==0){
        fprintf(stderr, "Une dimension de la matrice \"%s\" ne peut pas être nulle.\n",$1);
        exit(1);
      }
      else {
        Matrix * m = create_matrix(1,$2); // créer la matrice meme si pas initialisée
        valeur v;
        v.matrix = m;
        $$ = creer_variable($1, MATRIX, false, v);
      }
    }
  | ID taille taille
    {
      if ($2==0 || $3==0){
        fprintf(stderr, "Une dimension de la matrice \"%s\" ne peut pas être nulle.\n",$1);
        exit(1);
      }
      else {
        Matrix *m = create_matrix($2,$3);
        valeur v;
        v.matrix = m;
        $$ = creer_variable($1, MATRIX, false, v);
      }
    }
  | ID taille ASSIGN init_matrix 
        { 
          if ($2==0){
            fprintf(stderr, "Une dimension de la matrice \"%s\" ne peut pas être nulle.\n",$1);
            exit(1);
          }
          else if ($4->l != 1 || $4->c != $2){ // dimensions identiques
            fprintf(stderr, "Les dimensions spécifiées ne correspondent pas aux dimensions déclarées de la matrice \"%s\".\n",$1);
            exit(1);
          }
          else {
            Matrix * m = $4;
            valeur v;
            v.matrix = m;
            $$ = creer_variable($1, MATRIX, true, v);
          }
        }
  | ID taille taille ASSIGN init_matrix   
    { 
      if ($2==0 || $3==0){
        fprintf(stderr, "Une dimension de la matrice \"%s\" ne peut pas être nulle.\n",$1);
        exit(1);
      }
      else if ($5->l != $2 || $5->c != $3){ // dimensions identiques
        fprintf(stderr, "Les dimensions spécifiées ne correspondent pas aux dimensions déclarées de la matrice \"%s\".\n",$1);
        exit(1);
      }
      else {
        Matrix *m = $5;
        valeur v;
            v.matrix = m;
        $$ = creer_variable($1, MATRIX, true, v);
      }
    }
  ;

taille
  : LBRACKET V_INT RBRACKET { $$ = $2; }
  ;

init_matrix 
    : matrix_une_ligne    { $$ = $1; }
    | LCURLY liste_matrix_ligne RCURLY  { $$ = $2; }
    ;

liste_matrix_ligne 
    : matrix_une_ligne  { $$ = $1; } // condition d'arret
    | liste_matrix_ligne COMMA matrix_une_ligne {
        if ($1->c != $3->c){
            fprintf(stderr, "Chaque ligne d'une matrice doit avoir le même nombre de colonne.\n");
            exit(1);
        } else {
            add_ligne($1, $3); // ajouter $3 en derniere ligne de $1
            $$ = $1;
        }
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
      fprintf(stderr,"La variable '%s' n'a jamais été déclarée, elle ne peut donc pas être affichée.\n",$3);
      exit(1);
    } else if (!(id->var->type == INT || id->var->type == FLOAT)){
      fprintf(stderr,"La variable '%s' n'est pas de type INT ou FLOAT, elle ne peut donc pas être affichée à l'aide de la fonction print.\n",$3);
      exit(1);
    } else if (!id->var->init){
      fprintf(stderr,"La variable '%s' n'a jamais été initialisée, elle ne peut donc pas être affichée.\n",$3);
      exit(1);
    }
    gencode(CODE,CALL_PRINT,id,NULL,NULL);
  }

  | PRINTMAT LPAR ID RPAR {
    Symbol * id = symtable_get(SYMTAB,$3);
    if (id == NULL) {
      fprintf(stderr,"La matrice '%s' n'a jamais été déclarée, elle ne peut donc pas être affichée.\n",$3);
      exit(1);
    } else if (!(id->var->type == MATRIX)){
      fprintf(stderr,"La variable '%s' n'est pas de type MATRIX, elle ne peut donc pas être affichée à l'aide de la fonction mat.\n",$3);
      exit(1);
    } else if (!id->var->init){
      fprintf(stderr,"La matrice '%s' n'a jamais été initialisée, elle ne peut donc pas être affichée.\n",$3);
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
        fprintf(stderr,"Erreur de type dans une addition.\n");
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
        fprintf(stderr,"Erreur de type dans une addition.\n");
        exit(1);
        break;
    }

    valeur val;
    if (type1 == MATRIX || type2 == MATRIX) {
      type = MATRIX;

      // vérifier même tailles si les 2 sont matrices
      if ((type1 == MATRIX && type2 == MATRIX)&&(($1.ptr->var->val.matrix->l != $3.ptr->var->val.matrix->l) || ($1.ptr->var->val.matrix->c != $3.ptr->var->val.matrix->c))){
        fprintf(stderr,"Incompatibilité entre les tailles de matrice lors d'une addition.\n");
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
| LPAR expr RPAR
  { 
     $$.ptr = $2.ptr;
  }
| ID
  { 
    Symbol * id = symtable_get(SYMTAB,$1);
    if ( id == NULL ){
        fprintf(stderr,"La variable '%s' n'a jamais été déclarée.\n",$1);
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
;

  
/*
retour 
  : RETURN V_INT 
  ;

*/

%%

void yyerror(const char * s)
{
    fprintf(stderr,"%s\n",s);
}

