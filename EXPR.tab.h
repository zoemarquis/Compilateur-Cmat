/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_EXPR_TAB_H_INCLUDED
# define YY_YY_EXPR_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    CONST = 258,                   /* CONST  */
    NOT = 259,                     /* NOT  */
    AND = 260,                     /* AND  */
    OR = 261,                      /* OR  */
    DOT = 262,                     /* DOT  */
    MAIN = 263,                    /* MAIN  */
    PRINTF = 264,                  /* PRINTF  */
    PRINTMAT = 265,                /* PRINTMAT  */
    MATRIX = 266,                  /* MATRIX  */
    INT = 267,                     /* INT  */
    FLOAT = 268,                   /* FLOAT  */
    IF = 269,                      /* IF  */
    ELSE = 270,                    /* ELSE  */
    WHILE = 271,                   /* WHILE  */
    FOR = 272,                     /* FOR  */
    TILDE = 273,                   /* TILDE  */
    PLUSPLUS = 274,                /* PLUSPLUS  */
    MINUSMINUS = 275,              /* MINUSMINUS  */
    DIV = 276,                     /* DIV  */
    LCURLY = 277,                  /* LCURLY  */
    RCURLY = 278,                  /* RCURLY  */
    LBRACKET = 279,                /* LBRACKET  */
    RBRACKET = 280,                /* RBRACKET  */
    COMMA = 281,                   /* COMMA  */
    EQUAL = 282,                   /* EQUAL  */
    QUOTE = 283,                   /* QUOTE  */
    APOSTROPHE = 284,              /* APOSTROPHE  */
    BACKSLASH = 285,               /* BACKSLASH  */
    RETURN = 286,                  /* RETURN  */
    EXIT = 287,                    /* EXIT  */
    NOT_EQUAL = 288,               /* NOT_EQUAL  */
    LESS_THAN = 289,               /* LESS_THAN  */
    GREATER_THAN = 290,            /* GREATER_THAN  */
    LTOE = 291,                    /* LTOE  */
    GTOE = 292,                    /* GTOE  */
    PRINT = 293,                   /* PRINT  */
    SEMICOLON = 294,               /* SEMICOLON  */
    PLUS = 295,                    /* PLUS  */
    MINUS = 296,                   /* MINUS  */
    MULT = 297,                    /* MULT  */
    ASSIGN = 298,                  /* ASSIGN  */
    LPAR = 299,                    /* LPAR  */
    RPAR = 300,                    /* RPAR  */
    ID = 301,                      /* ID  */
    V_INT = 302,                   /* V_INT  */
    V_FLOAT = 303,                 /* V_FLOAT  */
    V_STRING = 304,                /* V_STRING  */
    UMINUS = 305                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 15 "EXPR.y"

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

#line 130 "EXPR.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_EXPR_TAB_H_INCLUDED  */
