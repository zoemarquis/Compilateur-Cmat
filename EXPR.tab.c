/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 2 "EXPR.y"

  #include "cmat.h" // pas besoin ?
  #include "error.h"
  #include "symbtab.h"
  #include "generation.h"
  #include <stdio.h>// ?
  #include <stdlib.h>// ?

  extern void yyerror(const char * s);
  extern int yylex();
  extern unsigned nb_ligne;

#line 84 "EXPR.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "EXPR.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_CONST = 3,                      /* CONST  */
  YYSYMBOL_NOT = 4,                        /* NOT  */
  YYSYMBOL_AND = 5,                        /* AND  */
  YYSYMBOL_OR = 6,                         /* OR  */
  YYSYMBOL_DOT = 7,                        /* DOT  */
  YYSYMBOL_MAIN = 8,                       /* MAIN  */
  YYSYMBOL_PRINTF = 9,                     /* PRINTF  */
  YYSYMBOL_PRINTMAT = 10,                  /* PRINTMAT  */
  YYSYMBOL_MATRIX = 11,                    /* MATRIX  */
  YYSYMBOL_INT = 12,                       /* INT  */
  YYSYMBOL_FLOAT = 13,                     /* FLOAT  */
  YYSYMBOL_IF = 14,                        /* IF  */
  YYSYMBOL_ELSE = 15,                      /* ELSE  */
  YYSYMBOL_WHILE = 16,                     /* WHILE  */
  YYSYMBOL_FOR = 17,                       /* FOR  */
  YYSYMBOL_TILDE = 18,                     /* TILDE  */
  YYSYMBOL_PLUSPLUS = 19,                  /* PLUSPLUS  */
  YYSYMBOL_MINUSMINUS = 20,                /* MINUSMINUS  */
  YYSYMBOL_DIV = 21,                       /* DIV  */
  YYSYMBOL_LCURLY = 22,                    /* LCURLY  */
  YYSYMBOL_RCURLY = 23,                    /* RCURLY  */
  YYSYMBOL_LBRACKET = 24,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 25,                  /* RBRACKET  */
  YYSYMBOL_COMMA = 26,                     /* COMMA  */
  YYSYMBOL_EQUAL = 27,                     /* EQUAL  */
  YYSYMBOL_QUOTE = 28,                     /* QUOTE  */
  YYSYMBOL_APOSTROPHE = 29,                /* APOSTROPHE  */
  YYSYMBOL_BACKSLASH = 30,                 /* BACKSLASH  */
  YYSYMBOL_RETURN = 31,                    /* RETURN  */
  YYSYMBOL_EXIT = 32,                      /* EXIT  */
  YYSYMBOL_NOT_EQUAL = 33,                 /* NOT_EQUAL  */
  YYSYMBOL_LESS_THAN = 34,                 /* LESS_THAN  */
  YYSYMBOL_GREATER_THAN = 35,              /* GREATER_THAN  */
  YYSYMBOL_LTOE = 36,                      /* LTOE  */
  YYSYMBOL_GTOE = 37,                      /* GTOE  */
  YYSYMBOL_PRINT = 38,                     /* PRINT  */
  YYSYMBOL_SEMICOLON = 39,                 /* SEMICOLON  */
  YYSYMBOL_PLUS = 40,                      /* PLUS  */
  YYSYMBOL_MINUS = 41,                     /* MINUS  */
  YYSYMBOL_MULT = 42,                      /* MULT  */
  YYSYMBOL_ASSIGN = 43,                    /* ASSIGN  */
  YYSYMBOL_LPAR = 44,                      /* LPAR  */
  YYSYMBOL_RPAR = 45,                      /* RPAR  */
  YYSYMBOL_ID = 46,                        /* ID  */
  YYSYMBOL_V_INT = 47,                     /* V_INT  */
  YYSYMBOL_V_FLOAT = 48,                   /* V_FLOAT  */
  YYSYMBOL_V_STRING = 49,                  /* V_STRING  */
  YYSYMBOL_UMINUS = 50,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 51,                  /* $accept  */
  YYSYMBOL_Start = 52,                     /* Start  */
  YYSYMBOL_def_constantes = 53,            /* def_constantes  */
  YYSYMBOL_def_constante = 54,             /* def_constante  */
  YYSYMBOL_fonction_principale = 55,       /* fonction_principale  */
  YYSYMBOL_fc_main = 56,                   /* fc_main  */
  YYSYMBOL_suite_instructions = 57,        /* suite_instructions  */
  YYSYMBOL_instr = 58,                     /* instr  */
  YYSYMBOL_affectation = 59,               /* affectation  */
  YYSYMBOL_declaration_variable = 60,      /* declaration_variable  */
  YYSYMBOL_type_var = 61,                  /* type_var  */
  YYSYMBOL_liste_var = 62,                 /* liste_var  */
  YYSYMBOL_une_var = 63,                   /* une_var  */
  YYSYMBOL_taille = 64,                    /* taille  */
  YYSYMBOL_matrix_litt = 65,               /* matrix_litt  */
  YYSYMBOL_init_matrix = 66,               /* init_matrix  */
  YYSYMBOL_liste_matrix_ligne = 67,        /* liste_matrix_ligne  */
  YYSYMBOL_matrix_une_ligne = 68,          /* matrix_une_ligne  */
  YYSYMBOL_matrix_remplir_colonne = 69,    /* matrix_remplir_colonne  */
  YYSYMBOL_valeur_matrix = 70,             /* valeur_matrix  */
  YYSYMBOL_afficher = 71,                  /* afficher  */
  YYSYMBOL_expr = 72,                      /* expr  */
  YYSYMBOL_extraction = 73,                /* extraction  */
  YYSYMBOL_intervalle = 74,                /* intervalle  */
  YYSYMBOL_expr_bool = 75,                 /* expr_bool  */
  YYSYMBOL_debut_bloc = 76,                /* debut_bloc  */
  YYSYMBOL_evaluation_if = 77,             /* evaluation_if  */
  YYSYMBOL_fin_bloc_if = 78,               /* fin_bloc_if  */
  YYSYMBOL_saut_fin_if = 79,               /* saut_fin_if  */
  YYSYMBOL_struct_if = 80,                 /* struct_if  */
  YYSYMBOL_debut_while = 81,               /* debut_while  */
  YYSYMBOL_fin_while = 82,                 /* fin_while  */
  YYSYMBOL_saut_fin_while = 83,            /* saut_fin_while  */
  YYSYMBOL_struct_while = 84,              /* struct_while  */
  YYSYMBOL_init_for = 85,                  /* init_for  */
  YYSYMBOL_debut_for = 86,                 /* debut_for  */
  YYSYMBOL_fin_for = 87,                   /* fin_for  */
  YYSYMBOL_jump_debut_for = 88,            /* jump_debut_for  */
  YYSYMBOL_jump_maj_for = 89,              /* jump_maj_for  */
  YYSYMBOL_struct_for = 90,                /* struct_for  */
  YYSYMBOL_bloc = 91,                      /* bloc  */
  YYSYMBOL_retour = 92                     /* retour  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   238

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  97
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  213

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   305


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    58,    58,    59,    60,    65,    66,    70,    82,    94,
     106,   121,   157,   188,   189,   190,   191,   192,   196,   197,
     198,   202,   266,   340,   413,   453,   590,   591,   592,   596,
     600,   607,   613,   620,   628,   636,   644,   655,   666,   676,
     677,   681,   682,   693,   697,   702,   710,   711,   715,   757,
     796,   803,   866,   870,   883,   887,   891,   975,  1109,  1110,
    1114,  1115,  1116,  1127,  1162,  1169,  1175,  1181,  1185,  1240,
    1294,  1347,  1400,  1453,  1509,  1523,  1530,  1538,  1545,  1546,
    1550,  1557,  1563,  1570,  1574,  1617,  1624,  1631,  1639,  1647,
    1659,  1660,  1661,  1662,  1663,  1664,  1670,  1696
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "CONST", "NOT", "AND",
  "OR", "DOT", "MAIN", "PRINTF", "PRINTMAT", "MATRIX", "INT", "FLOAT",
  "IF", "ELSE", "WHILE", "FOR", "TILDE", "PLUSPLUS", "MINUSMINUS", "DIV",
  "LCURLY", "RCURLY", "LBRACKET", "RBRACKET", "COMMA", "EQUAL", "QUOTE",
  "APOSTROPHE", "BACKSLASH", "RETURN", "EXIT", "NOT_EQUAL", "LESS_THAN",
  "GREATER_THAN", "LTOE", "GTOE", "PRINT", "SEMICOLON", "PLUS", "MINUS",
  "MULT", "ASSIGN", "LPAR", "RPAR", "ID", "V_INT", "V_FLOAT", "V_STRING",
  "UMINUS", "$accept", "Start", "def_constantes", "def_constante",
  "fonction_principale", "fc_main", "suite_instructions", "instr",
  "affectation", "declaration_variable", "type_var", "liste_var",
  "une_var", "taille", "matrix_litt", "init_matrix", "liste_matrix_ligne",
  "matrix_une_ligne", "matrix_remplir_colonne", "valeur_matrix",
  "afficher", "expr", "extraction", "intervalle", "expr_bool",
  "debut_bloc", "evaluation_if", "fin_bloc_if", "saut_fin_if", "struct_if",
  "debut_while", "fin_while", "saut_fin_while", "struct_while", "init_for",
  "debut_for", "fin_for", "jump_debut_for", "jump_maj_for", "struct_for",
  "bloc", "retour", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-155)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-78)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      97,   172,     6,    46,    41,    34,  -155,    40,    62,  -155,
      36,  -155,  -155,   115,    77,    84,    67,  -155,    -6,   -12,
     121,    87,  -155,   103,  -155,    79,  -155,  -155,   111,   116,
    -155,  -155,  -155,   123,   131,   137,   148,   -16,   122,   134,
    -155,  -155,   149,  -155,    79,    79,    79,   144,   124,     3,
     174,  -155,     3,   151,   124,   152,    89,   130,   171,    79,
      -1,   175,  -155,  -155,  -155,  -155,   153,   124,   176,  -155,
    -155,    39,     3,     3,   129,    10,  -155,    61,    12,   159,
     164,    81,   179,    76,  -155,  -155,  -155,   165,   124,   147,
    -155,  -155,   160,   124,    13,   149,  -155,   109,    25,   124,
    -155,   183,   105,    20,   124,   124,   124,   124,   124,   124,
       3,     3,  -155,   167,   169,    61,    61,    61,   186,  -155,
     174,   124,  -155,     3,  -155,    15,   143,  -155,  -155,   156,
    -155,   157,  -155,   112,  -155,   185,   165,   189,   170,  -155,
    -155,  -155,   205,   190,   177,  -155,  -155,   165,   165,   165,
     165,   165,   165,   183,   183,    61,    61,  -155,  -155,  -155,
     199,  -155,   202,    61,   165,    27,   173,   124,  -155,   196,
    -155,   143,   180,  -155,  -155,   189,   214,   198,    25,  -155,
    -155,   174,   200,  -155,   178,   201,   165,  -155,  -155,  -155,
    -155,   181,    25,  -155,    61,  -155,  -155,   182,   187,  -155,
     204,   208,   203,   124,  -155,  -155,  -155,  -155,    61,   165,
     209,  -155,  -155
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     0,     0,     0,     0,     4,     0,     0,    12,
       0,     1,     3,     5,     0,     0,     0,     6,     0,     0,
       0,     0,     7,     0,     9,    13,     8,    10,     0,     0,
      28,    26,    27,     0,     0,     0,     0,     0,     0,     0,
      20,    18,     0,    19,    13,    13,    13,     0,     0,     0,
       0,    80,     0,     0,     0,     0,     0,     0,     0,    13,
      31,    25,    29,    15,    16,    17,     0,     0,    53,    54,
      55,     0,     0,     0,    63,     0,    74,    90,     0,     0,
       0,     0,     0,     0,    24,    38,    39,    21,     0,     0,
      11,    14,     0,     0,    33,     0,    50,     0,     0,     0,
      49,    66,    63,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,     0,     0,    90,    90,    90,     0,    81,
       0,     0,    85,     0,    48,     0,     0,    46,    47,     0,
      41,     0,    44,     0,    96,     0,    32,     0,    34,    30,
      52,    60,    61,     0,    58,    51,    67,    72,    73,    68,
      70,    69,    71,    65,    64,    90,    90,    93,    94,    95,
      76,    78,     0,    90,    84,     0,     0,     0,    40,     0,
      43,     0,    52,    37,    35,     0,     0,    56,     0,    92,
      91,     0,     0,    86,     0,     0,    22,    42,    45,    97,
      36,     0,     0,    59,    90,    82,    83,     0,     0,    62,
       0,     0,     0,     0,    57,    76,    79,    87,    90,    23,
       0,    88,    89
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -155,  -155,   220,  -155,   230,  -155,   102,  -155,   -25,  -155,
    -155,  -155,   140,   142,  -135,  -155,  -155,   -82,  -155,    66,
      57,   -45,  -154,  -155,   -42,  -107,  -155,    37,  -155,    60,
    -155,  -155,  -155,    69,  -155,  -155,  -155,  -155,  -155,    85,
    -111,  -155
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     3,     4,     5,     6,    10,    38,    39,   113,    41,
      42,    61,    62,    94,    84,    85,   129,    86,   131,   132,
     114,    74,   143,   144,    75,    77,    50,   161,   162,   115,
      52,   120,   196,   116,    80,   123,   184,   208,   212,   117,
     118,    58
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      40,   130,   174,    71,   157,   158,   159,    72,    55,    81,
      78,    87,    89,   163,     9,   110,   111,   110,   111,    40,
      40,    40,    97,    92,   193,   110,   111,    56,   102,    23,
     101,   103,   110,   111,    40,    21,    24,    92,   200,   166,
     190,    22,    93,   133,   179,   180,    11,    73,   136,    68,
      69,    70,   182,     2,   145,   112,   137,   119,   167,   147,
     148,   149,   150,   151,   152,   146,   183,   141,   153,   154,
      28,    29,   142,    13,   194,    33,   164,    34,    35,    99,
      16,   165,    43,   201,   100,    44,    14,   187,    28,    29,
      30,    31,    32,    33,    45,    34,    35,   210,   126,    36,
       1,    43,    43,    43,    44,    44,    44,    37,    15,     2,
      46,    83,    20,    45,    45,    45,    43,    36,     1,    44,
      18,    99,   186,   127,   128,    37,   124,    19,    45,    46,
      46,    46,   104,    67,    26,    68,    69,    70,   105,   106,
     107,   108,   109,    25,    46,    99,    63,    64,    65,    99,
     140,    27,    99,    57,   140,    47,   104,   172,   209,   197,
      48,    91,   105,   106,   107,   108,   109,    49,    67,    99,
      68,    69,    70,    59,    88,    51,    68,    69,    70,   168,
     170,    53,   169,   171,     7,     8,   134,    99,   110,   111,
     127,   128,    54,    66,    90,    60,    76,    79,    96,    82,
      98,    95,   121,   122,   125,    99,   155,   135,   156,   160,
     173,    83,   176,   175,   -77,   177,   178,   181,   126,   189,
     185,   191,   192,   195,    37,   207,   198,   202,   199,   204,
     203,   205,   211,    17,    12,   139,   138,   188,   206
};

static const yytype_uint8 yycheck[] =
{
      25,    83,   137,    48,   115,   116,   117,     4,    24,    54,
      52,    56,    57,   120,     8,     5,     6,     5,     6,    44,
      45,    46,    67,    24,   178,     5,     6,    43,    73,    41,
      72,    73,     5,     6,    59,    41,    48,    24,   192,    24,
     175,    47,    43,    88,   155,   156,     0,    44,    93,    46,
      47,    48,   163,    12,    99,    45,    43,    45,    43,   104,
     105,   106,   107,   108,   109,    45,    39,    42,   110,   111,
       9,    10,    47,    39,   181,    14,   121,    16,    17,    40,
      44,   123,    25,   194,    45,    25,    46,   169,     9,    10,
      11,    12,    13,    14,    25,    16,    17,   208,    22,    38,
       3,    44,    45,    46,    44,    45,    46,    46,    46,    12,
      25,    22,    45,    44,    45,    46,    59,    38,     3,    59,
      43,    40,   167,    47,    48,    46,    45,    43,    59,    44,
      45,    46,    27,    44,    47,    46,    47,    48,    33,    34,
      35,    36,    37,    22,    59,    40,    44,    45,    46,    40,
      45,    48,    40,    31,    45,    44,    27,    45,   203,   184,
      44,    59,    33,    34,    35,    36,    37,    44,    44,    40,
      46,    47,    48,    39,    44,    44,    46,    47,    48,    23,
      23,    44,    26,    26,    12,    13,    39,    40,     5,     6,
      47,    48,    44,    49,    23,    46,    22,    46,    45,    47,
      24,    26,    43,    39,    25,    40,    39,    47,    39,    23,
      25,    22,     7,    43,    15,    25,    39,    15,    22,    39,
      47,     7,    24,    23,    46,    22,    25,    45,    47,    25,
      43,    23,    23,    13,     4,    95,    94,   171,   201
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    12,    52,    53,    54,    55,    12,    13,     8,
      56,     0,    55,    39,    46,    46,    44,    53,    43,    43,
      45,    41,    47,    41,    48,    22,    47,    48,     9,    10,
      11,    12,    13,    14,    16,    17,    38,    46,    57,    58,
      59,    60,    61,    71,    80,    84,    90,    44,    44,    44,
      77,    44,    81,    44,    44,    24,    43,    31,    92,    39,
      46,    62,    63,    57,    57,    57,    49,    44,    46,    47,
      48,    72,     4,    44,    72,    75,    22,    76,    75,    46,
      85,    72,    47,    22,    65,    66,    68,    72,    44,    72,
      23,    57,    24,    43,    64,    26,    45,    72,    24,    40,
      45,    75,    72,    75,    27,    33,    34,    35,    36,    37,
       5,     6,    45,    59,    71,    80,    84,    90,    91,    45,
      82,    43,    39,    86,    45,    25,    22,    47,    48,    67,
      68,    69,    70,    72,    39,    47,    72,    43,    64,    63,
      45,    42,    47,    73,    74,    72,    45,    72,    72,    72,
      72,    72,    72,    75,    75,    39,    39,    91,    91,    91,
      23,    78,    79,    76,    72,    75,    24,    43,    23,    26,
      23,    26,    45,    25,    65,    43,     7,    25,    39,    91,
      91,    15,    91,    39,    87,    47,    72,    68,    70,    39,
      65,     7,    24,    73,    76,    23,    83,    59,    25,    47,
      73,    91,    45,    43,    25,    23,    78,    22,    88,    72,
      91,    23,    89
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    51,    52,    52,    52,    53,    53,    54,    54,    54,
      54,    55,    56,    57,    57,    57,    57,    57,    58,    58,
      58,    59,    59,    59,    59,    60,    61,    61,    61,    62,
      62,    63,    63,    63,    63,    63,    63,    64,    65,    66,
      66,    67,    67,    68,    69,    69,    70,    70,    71,    71,
      71,    72,    72,    72,    72,    72,    72,    72,    73,    73,
      74,    74,    74,    75,    75,    75,    75,    75,    75,    75,
      75,    75,    75,    75,    76,    77,    78,    79,    80,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    91,    91,    91,    91,    91,    92,    92
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     1,     2,     3,     5,     6,     5,
       6,     8,     1,     0,     3,     2,     2,     2,     1,     1,
       1,     3,     6,     9,     3,     2,     1,     1,     1,     1,
       3,     1,     3,     2,     3,     4,     5,     3,     1,     1,
       3,     1,     3,     3,     1,     3,     1,     1,     4,     4,
       4,     3,     3,     1,     1,     1,     4,     7,     1,     3,
       1,     1,     4,     1,     3,     3,     2,     3,     3,     3,
       3,     3,     3,     3,     1,     3,     1,     1,     5,     9,
       1,     1,     1,     7,     3,     1,     1,     1,     1,    11,
       0,     3,     3,     2,     2,     2,     3,     5
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 7: /* def_constante: CONST INT ID ASSIGN V_INT  */
#line 71 "EXPR.y"
    {
      Symbol * id = symtable_get(GLOBAL,(yyvsp[-2].strval));
      if (id) {
        fprintf(stderr,"Ligne %d : La constante '%s' a déjà été déclarée.\n",nb_ligne, (yyvsp[-2].strval));
        exit(1);
      }
      valeur val;
      val.entier = (yyvsp[0].intval);
      variable * v = creer_variable((yyvsp[-2].strval), INT, true, val);
      symtable_put(GLOBAL, (yyvsp[-2].strval), v);
    }
#line 1323 "EXPR.tab.c"
    break;

  case 8: /* def_constante: CONST INT ID ASSIGN MINUS V_INT  */
#line 83 "EXPR.y"
    {
      Symbol * id = symtable_get(GLOBAL,(yyvsp[-3].strval));
      if (id) {
        fprintf(stderr,"Ligne %d : La constante '%s' a déjà été déclarée.\n",nb_ligne, (yyvsp[-3].strval));
        exit(1);
      }
      valeur val;
      val.entier = -(yyvsp[0].intval);
      variable * v = creer_variable((yyvsp[-3].strval), INT, true, val);
      symtable_put(GLOBAL, (yyvsp[-3].strval), v);
    }
#line 1339 "EXPR.tab.c"
    break;

  case 9: /* def_constante: CONST FLOAT ID ASSIGN V_FLOAT  */
#line 95 "EXPR.y"
    {
      Symbol * id = symtable_get(GLOBAL,(yyvsp[-2].strval));
      if (id) {
        fprintf(stderr,"Ligne %d : La constante '%s' a déjà été déclarée.\n",nb_ligne, (yyvsp[-2].strval));
        exit(1);
      }
      valeur val;
      val.flottant = (yyvsp[0].floatval);
      variable * v = creer_variable((yyvsp[-2].strval), FLOAT, true, val);
      symtable_put(GLOBAL, (yyvsp[-2].strval), v);
    }
#line 1355 "EXPR.tab.c"
    break;

  case 10: /* def_constante: CONST FLOAT ID ASSIGN MINUS V_FLOAT  */
#line 107 "EXPR.y"
    {
      Symbol * id = symtable_get(GLOBAL,(yyvsp[-3].strval));
      if (id) {
        fprintf(stderr,"Ligne %d : La constante '%s' a déjà été déclarée.\n",nb_ligne, (yyvsp[-3].strval));
        exit(1);
      }
      valeur val;
      val.flottant = -(yyvsp[0].floatval);
      variable * v = creer_variable((yyvsp[-3].strval), FLOAT, true, val);
      symtable_put(GLOBAL, (yyvsp[-3].strval), v);
    }
#line 1371 "EXPR.tab.c"
    break;

  case 11: /* fonction_principale: INT fc_main LPAR RPAR LCURLY suite_instructions retour RCURLY  */
#line 122 "EXPR.y"
    {
      // vérifier que type de retour est int
      unsigned type1;

      switch((yyvsp[-1].exprval).ptr->kind){
        case(NAME):
          assert((yyvsp[-1].exprval).ptr->var->init);
          type1 = (yyvsp[-1].exprval).ptr->var->type;
          break;
        case(CONST_INT):
          type1 = INT;
          break;
        case(CONST_FLOAT):
          type1 = FLOAT;
          break;
        default:
          fprintf(stderr,"Ligne %d : Possibilité de renvoyer des int ou des float seulement.\n",nb_ligne);
          exit(1);
          break;
      }

      if (type1 != INT){
        fprintf(stderr,"Ligne %d : Incompatibilité de type de la fonction return.\n",nb_ligne);
        exit(1);
        break;
      }

      // gencode ?
    }
#line 1405 "EXPR.tab.c"
    break;

  case 12: /* fc_main: MAIN  */
#line 158 "EXPR.y"
    {
      SYMTAB = symtable_new("main");
      add_symtable(SYMTAB);
      gencode(CODE, FONCTION, NULL, NULL, NULL);
    }
#line 1415 "EXPR.tab.c"
    break;

  case 21: /* affectation: ID ASSIGN expr  */
#line 203 "EXPR.y"
    {
      // sémantique : vérifier que l'id existe
      Symbol * id = symtable_get(SYMTAB,(yyvsp[-2].strval));
      if ( id == NULL ){
        fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été déclarée.\n",nb_ligne,(yyvsp[-2].strval));
        exit(1);
      }
      // vérifier qu'on essaye pas de modifier variable globale
      Symbol * glob = symtable_get(GLOBAL,(yyvsp[-2].strval));
      if ( glob != NULL ){
        fprintf(stderr,"Ligne %d : La constante '%s' ne peut pas être modifiée.\n",nb_ligne,(yyvsp[-2].strval));
        exit(1);
      }

      // sémantique vérifier que le type de l'expr est compatible avec le type de ID

      unsigned type1, type2;

      type1 = id->var->type;

      switch((yyvsp[0].exprval).ptr->kind){
        case(NAME):
          type2 = (yyvsp[0].exprval).ptr->var->type;
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
        if ((id->var->val.matrix->l != (yyvsp[0].exprval).ptr->var->val.matrix->l) || (id->var->val.matrix->c != (yyvsp[0].exprval).ptr->var->val.matrix->c)){
          fprintf(stderr,"Ligne %d : Incompatibilité entre les tailles de matrice lors de l'affectation à %s.\n",nb_ligne, (yyvsp[-2].strval));
          exit(1);
        }
      }

      // marquer que la variable est initialisée
      id->var->init = true;
      
      gencode(CODE,COPY,id,(yyvsp[0].exprval).ptr,NULL);
      (yyval.variable) = id; // utile pour for
    }
#line 1482 "EXPR.tab.c"
    break;

  case 22: /* affectation: ID LBRACKET V_INT RBRACKET ASSIGN expr  */
#line 267 "EXPR.y"
    {
      // sémantique : vérifier que l'id existe
      Symbol * id = symtable_get(SYMTAB,(yyvsp[-5].strval));
      if ( id == NULL ){
        fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été déclarée.\n",nb_ligne,(yyvsp[-5].strval));
        exit(1);
      }

      // vérifier qu'on essaye pas de modifier variable globale
      Symbol * glob = symtable_get(GLOBAL,(yyvsp[-5].strval));
      if ( glob != NULL ){
        fprintf(stderr,"Ligne %d : La constante '%s' ne peut pas être modifiée.\n",nb_ligne,(yyvsp[-5].strval));
        exit(1);
      }

      unsigned type1, type2;

      assert(id->kind == NAME);
      type1 = id->var->type;

      switch((yyvsp[0].exprval).ptr->kind){
        case(NAME):
          type2 = (yyvsp[0].exprval).ptr->var->type;
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
        fprintf(stderr,"Ligne %d : %s n'est pas une matrice, on ne peut donc pas accéder à %s[%d].\n",nb_ligne,id->name,id->name,(yyvsp[-3].intval));
        exit(1);
      }

      // vérifier que c'est une matrice à une dimension
      if (id->var->val.matrix->l != 1){
        fprintf(stderr,"Ligne %d : La matrice %s a deux dimensions : impossible d'affecter quelque chose à %s[%d].\n",nb_ligne,id->name,id->name,(yyvsp[-3].intval));
        exit(1);
      }

      // vérifier que 0 <= $3 < taille colonne
      if (id->var->val.matrix->c <= (yyvsp[-3].intval)){
        fprintf(stderr,"Ligne %d : La matrice %s a %d colonnes : %s[%d] out of range.\n",nb_ligne,id->name,id->var->val.matrix->c,id->name,(yyvsp[-3].intval));
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
      Symbol * t = symtable_indices(SYMTAB,(Indices){0,(yyvsp[-3].intval)});
      gencode(CODE,PUT_ELEMENT,id,(yyvsp[0].exprval).ptr,t);

      (yyval.variable) = id; // utile pour for
    }
#line 1559 "EXPR.tab.c"
    break;

  case 23: /* affectation: ID LBRACKET V_INT RBRACKET LBRACKET V_INT RBRACKET ASSIGN expr  */
#line 341 "EXPR.y"
    {
      // sémantique : vérifier que l'id existe
      Symbol * id = symtable_get(SYMTAB,(yyvsp[-8].strval));
      if ( id == NULL ){
        fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été déclarée.\n",nb_ligne,(yyvsp[-8].strval));
        exit(1);
      }

      // vérifier qu'on essaye pas de modifier variable globale
      Symbol * glob = symtable_get(GLOBAL,(yyvsp[-8].strval));
      if ( glob != NULL ){
        fprintf(stderr,"Ligne %d : La constante '%s' ne peut pas être modifiée.\n",nb_ligne,(yyvsp[-8].strval));
        exit(1);
      }

      unsigned type1, type2;

      assert(id->kind == NAME);
      type1 = id->var->type;

      switch((yyvsp[0].exprval).ptr->kind){
        case(NAME):
          type2 = (yyvsp[0].exprval).ptr->var->type;
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
        fprintf(stderr,"Ligne %d : %s n'est pas une matrice, on ne peut donc pas accéder à %s[%d].\n",nb_ligne,id->name,id->name,(yyvsp[-6].intval));
        exit(1);
      }

      // vérifier que 0 <= $3 < taille ligne
      if (id->var->val.matrix->l <= (yyvsp[-6].intval)){
        fprintf(stderr,"Ligne %d : La matrice %s a %d lignes : %s[%d][%d] out of range.\n",nb_ligne,id->name,id->var->val.matrix->l,id->name,(yyvsp[-6].intval),(yyvsp[-3].intval));
        exit(1);
      }

      // vérifier que 0 <= $6 < taille colonne
      if (id->var->val.matrix->c <= (yyvsp[-3].intval)){
        fprintf(stderr,"Ligne %d : La matrice %s a %d colonnes : %s[%d][%d] out of range.\n",nb_ligne,id->name,id->var->val.matrix->c,id->name,(yyvsp[-6].intval),(yyvsp[-3].intval));
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
      Symbol * t = symtable_indices(SYMTAB,(Indices){(yyvsp[-6].intval),(yyvsp[-3].intval)});
      gencode(CODE,PUT_ELEMENT,id,(yyvsp[0].exprval).ptr,t);

      (yyval.variable) = id; // utile pour for 
    }
#line 1636 "EXPR.tab.c"
    break;

  case 24: /* affectation: ID ASSIGN matrix_litt  */
#line 414 "EXPR.y"
    {
      // sémantique : vérifier que l'id existe
      Symbol * id = symtable_get(SYMTAB,(yyvsp[-2].strval));
      if ( id == NULL ){
        fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été déclarée.\n",nb_ligne,(yyvsp[-2].strval));
        exit(1);
      }

      // vérifier qu'on essaye pas de modifier variable globale
      Symbol * glob = symtable_get(GLOBAL,(yyvsp[-2].strval));
      if ( glob != NULL ){
        fprintf(stderr,"Ligne %d : La constante '%s' ne peut pas être modifiée.\n",nb_ligne,(yyvsp[-2].strval));
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
      
      gencode(CODE,COPY,id,(yyvsp[0].exprval).ptr,NULL);
      (yyval.variable) = id; // utile pour for
    }
#line 1673 "EXPR.tab.c"
    break;

  case 25: /* declaration_variable: type_var liste_var  */
#line 453 "EXPR.y"
                       {

      unsigned type1, type2, type3;
      type1 = (yyvsp[-1].type);
      
      for (int i = 0; i < (yyvsp[0].l_var).taille; i++) {
        // vérifier la compatibilité entre $1 et les types gauche (peut-etre indéfini) et les types droits
        // et faire les COPY si c'est correct

        variable * v = (yyvsp[0].l_var).liste[i].gauche;
        Symbol * s = (yyvsp[0].l_var).liste[i].droite;
    
        // 1. vérifier que l'entrée n'existe pas
        Symbol * id = symtable_get(SYMTAB,v->name);
        if (id) {
          fprintf(stderr,"Ligne %d : La variable '%s' a déjà été déclarée.\n",nb_ligne, v->name);
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
      free((yyvsp[0].l_var).liste);
  }
#line 1812 "EXPR.tab.c"
    break;

  case 26: /* type_var: INT  */
#line 590 "EXPR.y"
              { (yyval.type) = INT; }
#line 1818 "EXPR.tab.c"
    break;

  case 27: /* type_var: FLOAT  */
#line 591 "EXPR.y"
              { (yyval.type) = FLOAT; }
#line 1824 "EXPR.tab.c"
    break;

  case 28: /* type_var: MATRIX  */
#line 592 "EXPR.y"
              { (yyval.type) = MATRIX; }
#line 1830 "EXPR.tab.c"
    break;

  case 29: /* liste_var: une_var  */
#line 597 "EXPR.y"
    {
      (yyval.l_var) = creer_liste_tuple_declaration((yyvsp[0].var));
    }
#line 1838 "EXPR.tab.c"
    break;

  case 30: /* liste_var: liste_var COMMA une_var  */
#line 601 "EXPR.y"
    {
      (yyval.l_var) = ajouter_tuple((yyvsp[-2].l_var), (yyvsp[0].var));
    }
#line 1846 "EXPR.tab.c"
    break;

  case 31: /* une_var: ID  */
#line 608 "EXPR.y"
    { 
      valeur v;
      (yyval.var).gauche = creer_variable((yyvsp[0].strval), 0, false, v);
      (yyval.var).droite = NULL;
    }
#line 1856 "EXPR.tab.c"
    break;

  case 32: /* une_var: ID ASSIGN expr  */
#line 614 "EXPR.y"
    {
      valeur v;
      (yyval.var).gauche = creer_variable((yyvsp[-2].strval), 0, false, v);
      (yyval.var).droite = (yyvsp[0].exprval).ptr;
    }
#line 1866 "EXPR.tab.c"
    break;

  case 33: /* une_var: ID taille  */
#line 621 "EXPR.y"
    {
      Matrix * m = create_matrix(1,(yyvsp[0].intval)); // créer la matrice meme si pas initialisée
      valeur v;
      v.matrix = m;
      (yyval.var).gauche = creer_variable((yyvsp[-1].strval), MATRIX, false, v);
      (yyval.var).droite = NULL;
    }
#line 1878 "EXPR.tab.c"
    break;

  case 34: /* une_var: ID taille taille  */
#line 629 "EXPR.y"
    {
      Matrix *m = create_matrix((yyvsp[-1].intval),(yyvsp[0].intval));
      valeur v;
      v.matrix = m;
      (yyval.var).gauche = creer_variable((yyvsp[-2].strval), MATRIX, false, v);
      (yyval.var).droite = NULL;
    }
#line 1890 "EXPR.tab.c"
    break;

  case 35: /* une_var: ID taille ASSIGN matrix_litt  */
#line 637 "EXPR.y"
    { 
      Matrix * m = create_matrix(1,(yyvsp[-2].intval)); 
      valeur v;
      v.matrix = m;
      (yyval.var).gauche = creer_variable((yyvsp[-3].strval), MATRIX, true, v);
      (yyval.var).droite = (yyvsp[0].exprval).ptr;
    }
#line 1902 "EXPR.tab.c"
    break;

  case 36: /* une_var: ID taille taille ASSIGN matrix_litt  */
#line 645 "EXPR.y"
    { 
      Matrix *m = create_matrix((yyvsp[-3].intval),(yyvsp[-2].intval));
      valeur v;
      v.matrix = m;
      (yyval.var).gauche = creer_variable((yyvsp[-4].strval), MATRIX, true, v);
      (yyval.var).droite = (yyvsp[0].exprval).ptr;
    }
#line 1914 "EXPR.tab.c"
    break;

  case 37: /* taille: LBRACKET V_INT RBRACKET  */
#line 656 "EXPR.y"
  { 
    if ((yyvsp[-1].intval)==0){
      fprintf(stderr, "Ligne %d : Une dimension de matrice ne peut pas être nulle.\n",nb_ligne);
      exit(1);
    }
    (yyval.intval) = (yyvsp[-1].intval); 
  }
#line 1926 "EXPR.tab.c"
    break;

  case 38: /* matrix_litt: init_matrix  */
#line 667 "EXPR.y"
    {
      // généré temporaire
      valeur val;
      val.matrix = (yyvsp[0].matrix);
      (yyval.exprval).ptr = newtemp(SYMTAB, MATRIX, val);
    }
#line 1937 "EXPR.tab.c"
    break;

  case 39: /* init_matrix: matrix_une_ligne  */
#line 676 "EXPR.y"
                          { (yyval.matrix) = (yyvsp[0].matrix); }
#line 1943 "EXPR.tab.c"
    break;

  case 40: /* init_matrix: LCURLY liste_matrix_ligne RCURLY  */
#line 677 "EXPR.y"
                                        { (yyval.matrix) = (yyvsp[-1].matrix); }
#line 1949 "EXPR.tab.c"
    break;

  case 41: /* liste_matrix_ligne: matrix_une_ligne  */
#line 681 "EXPR.y"
                        { (yyval.matrix) = (yyvsp[0].matrix); }
#line 1955 "EXPR.tab.c"
    break;

  case 42: /* liste_matrix_ligne: liste_matrix_ligne COMMA matrix_une_ligne  */
#line 682 "EXPR.y"
                                                {
        if ((yyvsp[-2].matrix)->c != (yyvsp[0].matrix)->c){
            fprintf(stderr, "Ligne %d : Chaque ligne d'une matrice doit avoir le même nombre de colonne.\n",nb_ligne);
            exit(1);
        } 
        add_ligne((yyvsp[-2].matrix), (yyvsp[0].matrix)); // ajouter $3 en derniere ligne de $1
        (yyval.matrix) = (yyvsp[-2].matrix);
    }
#line 1968 "EXPR.tab.c"
    break;

  case 43: /* matrix_une_ligne: LCURLY matrix_remplir_colonne RCURLY  */
#line 693 "EXPR.y"
                                           { (yyval.matrix) = (yyvsp[-1].matrix); }
#line 1974 "EXPR.tab.c"
    break;

  case 44: /* matrix_remplir_colonne: valeur_matrix  */
#line 697 "EXPR.y"
                      {
        Matrix *m = create_matrix(1,1); // créer une ligne 
        put_value_at(m, 0, 0, (yyvsp[0].floatval)); 
        (yyval.matrix) = m;
    }
#line 1984 "EXPR.tab.c"
    break;

  case 45: /* matrix_remplir_colonne: matrix_remplir_colonne COMMA valeur_matrix  */
#line 702 "EXPR.y"
                                                {
        add_colonne((yyvsp[-2].matrix)); // ajoute une colonne
        put_value_at((yyvsp[-2].matrix), 0, ((yyvsp[-2].matrix)->c)-1, (yyvsp[0].floatval)); // mets la valeur dans cette colonne
        (yyval.matrix) = (yyvsp[-2].matrix);
    }
#line 1994 "EXPR.tab.c"
    break;

  case 46: /* valeur_matrix: V_INT  */
#line 710 "EXPR.y"
                { (yyval.floatval) = (float) (yyvsp[0].intval); }
#line 2000 "EXPR.tab.c"
    break;

  case 47: /* valeur_matrix: V_FLOAT  */
#line 711 "EXPR.y"
                { (yyval.floatval) = (yyvsp[0].floatval); }
#line 2006 "EXPR.tab.c"
    break;

  case 48: /* afficher: PRINT LPAR expr RPAR  */
#line 716 "EXPR.y"
    {
      unsigned type1;

      switch((yyvsp[-1].exprval).ptr->kind){
        case(NAME):
          assert((yyvsp[-1].exprval).ptr->var->init);
          type1 = (yyvsp[-1].exprval).ptr->var->type;
          break;
        case(CONST_INT):
          type1 = INT;
          break;
        case(CONST_FLOAT):
          type1 = FLOAT;
          break;
        default:
          fprintf(stderr,"Ligne %d : La fonction print ne peut qu'afficher des expressions correspondantes à des INT ou des FLOAT.\n",nb_ligne);
          exit(1);
          break;
      }

      if (type1 != INT && type1 != FLOAT){
        fprintf(stderr,"Ligne %d : La fonction print ne peut qu'afficher des expressions correspondantes à des INT ou des FLOAT.\n",nb_ligne);
        exit(1);
      }

    /*
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
    */
      gencode(CODE,CALL_PRINT,(yyvsp[-1].exprval).ptr,NULL,NULL);
  }
#line 2051 "EXPR.tab.c"
    break;

  case 49: /* afficher: PRINTMAT LPAR expr RPAR  */
#line 758 "EXPR.y"
    {

    /*
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
  */
      unsigned type1;

      switch((yyvsp[-1].exprval).ptr->kind){
        case(NAME):
          assert((yyvsp[-1].exprval).ptr->var->init);
          type1 = (yyvsp[-1].exprval).ptr->var->type;
          break;
        default:
          fprintf(stderr,"Ligne %d : La fonction printmat ne peut qu'afficher des expressions correspondantes à des MATRIX.\n",nb_ligne);
          exit(1);
          break;
      }

      if (type1 != MATRIX){
        fprintf(stderr,"Ligne %d : La fonction printmat ne peut qu'afficher des expressions correspondantes à des MATRIX.\n",nb_ligne);
        exit(1);
      }

      gencode(CODE,CALL_PRINTMAT,(yyvsp[-1].exprval).ptr,NULL,NULL);
  }
#line 2093 "EXPR.tab.c"
    break;

  case 50: /* afficher: PRINTF LPAR V_STRING RPAR  */
#line 796 "EXPR.y"
                              {
      Symbol * id = symtable_string(SYMTAB, (yyvsp[-1].string));
      gencode(CODE,CALL_PRINTF,id,NULL,NULL);
    }
#line 2102 "EXPR.tab.c"
    break;

  case 51: /* expr: expr PLUS expr  */
#line 804 "EXPR.y"
  { 
    // vérifier compatibilité de type

    unsigned type1, type2, type;
    switch((yyvsp[-2].exprval).ptr->kind){
      case(NAME):
        type1 = (yyvsp[-2].exprval).ptr->var->type;
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
    switch((yyvsp[0].exprval).ptr->kind){
      case(NAME):
        type2 = (yyvsp[0].exprval).ptr->var->type;
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
      if ((type1 == MATRIX && type2 == MATRIX)&&(((yyvsp[-2].exprval).ptr->var->val.matrix->l != (yyvsp[0].exprval).ptr->var->val.matrix->l) || ((yyvsp[-2].exprval).ptr->var->val.matrix->c != (yyvsp[0].exprval).ptr->var->val.matrix->c))){
        fprintf(stderr,"Ligne %d : Incompatibilité entre les tailles de matrice lors d'une addition.\n",nb_ligne);
        exit(1);
      }

      // créer une matrix pour la var temporaire
      if (type1 == MATRIX){
        Matrix *m = create_matrix((yyvsp[-2].exprval).ptr->var->val.matrix->l, (yyvsp[-2].exprval).ptr->var->val.matrix->c);
        val.matrix = m;
      } else {
        Matrix *m = create_matrix((yyvsp[0].exprval).ptr->var->val.matrix->l, (yyvsp[0].exprval).ptr->var->val.matrix->c);
        val.matrix = m;
      }
    }
    else if (type1 == FLOAT || type2 == FLOAT) 
      type = FLOAT;
    else 
      type = INT;

    (yyval.exprval).ptr = newtemp(SYMTAB, type, val);
    gencode(CODE,BOP_PLUS,(yyval.exprval).ptr,(yyvsp[-2].exprval).ptr,(yyvsp[0].exprval).ptr); 
  }
#line 2169 "EXPR.tab.c"
    break;

  case 52: /* expr: LPAR expr RPAR  */
#line 867 "EXPR.y"
  { 
     (yyval.exprval).ptr = (yyvsp[-1].exprval).ptr;
  }
#line 2177 "EXPR.tab.c"
    break;

  case 53: /* expr: ID  */
#line 871 "EXPR.y"
  { 
    Symbol * id = symtable_get(SYMTAB,(yyvsp[0].strval));
    if (id==NULL){
        fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été déclarée.\n",nb_ligne,(yyvsp[0].strval));
        exit(1);
    }
    if (!id->var->init){
      fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été initialisée, elle ne peut donc pas être utilisée dans une expression.\n",nb_ligne,(yyvsp[0].strval));
      exit(1);
    }
    (yyval.exprval).ptr = id;
  }
#line 2194 "EXPR.tab.c"
    break;

  case 54: /* expr: V_INT  */
#line 884 "EXPR.y"
    { 
      (yyval.exprval).ptr = symtable_const_int(SYMTAB,(yyvsp[0].intval)); 
    }
#line 2202 "EXPR.tab.c"
    break;

  case 55: /* expr: V_FLOAT  */
#line 888 "EXPR.y"
    { 
      (yyval.exprval).ptr = symtable_const_float(SYMTAB,(yyvsp[0].floatval)); 
    }
#line 2210 "EXPR.tab.c"
    break;

  case 56: /* expr: ID LBRACKET extraction RBRACKET  */
#line 892 "EXPR.y"
  {
    // TODO : si taille de extraction = 1  (et que c'est pas -1)-> float 

    // 1. vérifier que ID est déclarée
    Symbol * id = symtable_get(SYMTAB,(yyvsp[-3].strval));
    if (id == NULL){
      fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été déclarée.\n",nb_ligne,(yyvsp[-3].strval));
      exit(1);
    }

    if (!id->var->init){
      fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été initialisée, elle ne peut donc pas être utilisée dans une expression.\n",nb_ligne,(yyvsp[-3].strval));
      exit(1);
    }

    // 2. vérifier que c'est une matrice
    if (id->var->type != MATRIX){
      fprintf(stderr,"Ligne %d : La variable '%s' n'est pas de type matrix.\n",nb_ligne,(yyvsp[-3].strval));
      exit(1);
    }

    // 3. vérifier que c'est bien une matrice une dimension
    if (id->var->val.matrix->l != 1){
      fprintf(stderr,"Ligne %d : La matrix '%s' n'est pas de dimension 1.\n",nb_ligne,(yyvsp[-3].strval));
      exit(1);
    }

    // 4. vérifier correspondance dimensions matrice / extract

      Matrix *m = id->var->val.matrix;
      for (int i = 0; i < (yyvsp[-1].liste_extract).taille; i++) {
        if ((yyvsp[-1].liste_extract).liste[i] != -1 && (yyvsp[-1].liste_extract).liste[i] >= m->c) {
          //fprintf(stderr, "l = %d , c = %d, $3.liste[i] = %d\n", m->l, m->c, $3.liste[i]);
          fprintf(stderr, "Ligne %d : Indice de colonne out of range.\n",nb_ligne);
          exit(1);
        }
      }

      // pour tous les -1, remplacer par la dimension de ID
      int *c = (int *) malloc(sizeof(int)* (yyvsp[-1].liste_extract).taille);
      if (c == NULL) { // Échec de l'allocation, gérer l'erreur
        fprintf(stderr,"Allocation mémoire échouée\n");
        exit(MEMORY_FAILURE);
      }
      int c_taille = 0;
      int tmp = (yyvsp[-1].liste_extract).taille; // pour réallouer la bonne taille
      for (int i = 0; i < (yyvsp[-1].liste_extract).taille; i++){
        if ((yyvsp[-1].liste_extract).liste[i]==-1){
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
          c[c_taille] = (yyvsp[-1].liste_extract).liste[i];
          c_taille++;
        }
      }

    valeur val;
    if (c_taille == 1){
      // on va juste chercher le float correspondant (ici le type c'est float et pas matrix)
      (yyval.exprval).ptr = newtemp(SYMTAB, FLOAT, val);
      Symbol * t = symtable_indices(SYMTAB,(Indices){0,c[0]});
      gencode(CODE,GET_ELEMENT,(yyval.exprval).ptr,id,t);
      free(c);
    } else {
      Symbol * colonne = symtable_extract(SYMTAB,(Extract){c,c_taille});

      // créer la matrice temporaire de bonnes dimensions
      Matrix *mat = create_matrix(1, c_taille);
      val.matrix = mat;
      (yyval.exprval).ptr = newtemp(SYMTAB, MATRIX, val);
  
      gencode(CODE,EXTR_COLONNE,(yyval.exprval).ptr,id,colonne); 
    }
    free((yyvsp[-1].liste_extract).liste);
  }
#line 2298 "EXPR.tab.c"
    break;

  case 57: /* expr: ID LBRACKET extraction RBRACKET LBRACKET extraction RBRACKET  */
#line 976 "EXPR.y"
  {
    // 1. vérifier que ID est déclarée
    Symbol * id = symtable_get(SYMTAB,(yyvsp[-6].strval));
    if (id == NULL){
      fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été déclarée.\n",nb_ligne,(yyvsp[-6].strval));
      exit(1);
    }

    if (!id->var->init){
      fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été initialisée, elle ne peut donc pas être utilisée dans une expression.\n",nb_ligne,(yyvsp[-6].strval));
      exit(1);
    }

    // 2. vérifier que c'est une matrice
    if (id->var->type != MATRIX){
      fprintf(stderr,"Ligne %d : La variable '%s' n'est pas de type matrix.\n",nb_ligne,(yyvsp[-6].strval));
      exit(1);
    }

    // vérifier correspondance dimensions matrice / extract
    Matrix *m = id->var->val.matrix;
    for (int i = 0; i < (yyvsp[-4].liste_extract).taille; i++) {
      if ((yyvsp[-4].liste_extract).liste[i] != -1 && (yyvsp[-4].liste_extract).liste[i] >= m->l) {
        fprintf(stderr, "Ligne %d : Indice de ligne out of range.\n",nb_ligne);
        exit(1);
      }
    }
    for (int i = 0; i < (yyvsp[-1].liste_extract).taille; i++) {
      if ((yyvsp[-1].liste_extract).liste[i] != -1 && (yyvsp[-1].liste_extract).liste[i] >= m->c) {
        fprintf(stderr, "Ligne %d : Indice de colonne out of range.\n",nb_ligne);
        exit(1);
      }
    }

    // pour tous les -1, remplacer par la dimension de ID : ligne
    int *l = (int *) malloc(sizeof(int)* (yyvsp[-4].liste_extract).taille);
    if (l == NULL) { // Échec de l'allocation, gérer l'erreur
      fprintf(stderr,"Allocation mémoire échouée\n");
      exit(MEMORY_FAILURE);
    }
    int l_taille = 0;
    int tmp = (yyvsp[-4].liste_extract).taille; // pour réallouer la bonne taille
    for (int i = 0; i < (yyvsp[-4].liste_extract).taille; i++){
      if ((yyvsp[-4].liste_extract).liste[i]==-1){
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
        l[l_taille] = (yyvsp[-4].liste_extract).liste[i];
        l_taille++;
      }
    }

    // pour tous les -1, remplacer par la dimension de ID : colonne
    int *c = (int *) malloc(sizeof(int)* (yyvsp[-1].liste_extract).taille);
    if (c == NULL) { // Échec de l'allocation, gérer l'erreur
      fprintf(stderr,"Allocation mémoire échouée\n");
      exit(MEMORY_FAILURE);
    }
    int c_taille = 0;
    tmp = (yyvsp[-1].liste_extract).taille;
    for (int i = 0; i < (yyvsp[-1].liste_extract).taille; i++){
      if ((yyvsp[-1].liste_extract).liste[i]==-1){
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
        c[c_taille] = (yyvsp[-1].liste_extract).liste[i];
        c_taille++;
      }
    }

    if (l_taille == 1 && c_taille == 1){
      // on va juste chercher le float correspondant (ici le type c'est float et pas matrix)
      valeur val;
      (yyval.exprval).ptr = newtemp(SYMTAB, FLOAT, val);
      Symbol * t = symtable_indices(SYMTAB,(Indices){l[0],c[0]});
      gencode(CODE,GET_ELEMENT,(yyval.exprval).ptr,id,t);
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
      (yyval.exprval).ptr = newtemp(SYMTAB, MATRIX, val2);
      gencode(CODE,EXTR_COLONNE,(yyval.exprval).ptr,intermediaire,colonne); 
    }
    free((yyvsp[-4].liste_extract).liste);
    free((yyvsp[-1].liste_extract).liste);
  }
#line 2417 "EXPR.tab.c"
    break;

  case 58: /* extraction: intervalle  */
#line 1109 "EXPR.y"
               { (yyval.liste_extract) = (yyvsp[0].liste_extract); }
#line 2423 "EXPR.tab.c"
    break;

  case 59: /* extraction: intervalle SEMICOLON extraction  */
#line 1110 "EXPR.y"
                                    { (yyval.liste_extract) = concat_extract_liste((yyvsp[-2].liste_extract),(yyvsp[0].liste_extract)); }
#line 2429 "EXPR.tab.c"
    break;

  case 60: /* intervalle: MULT  */
#line 1114 "EXPR.y"
          { (yyval.liste_extract) = creer_liste_extract(-1); }
#line 2435 "EXPR.tab.c"
    break;

  case 61: /* intervalle: V_INT  */
#line 1115 "EXPR.y"
          { (yyval.liste_extract) = creer_liste_extract((yyvsp[0].intval)) ; }
#line 2441 "EXPR.tab.c"
    break;

  case 62: /* intervalle: V_INT DOT DOT V_INT  */
#line 1117 "EXPR.y"
    { 
      if ((yyvsp[-3].intval) > (yyvsp[0].intval)) {
        fprintf(stderr, "Ligne %d : Dans une extraction, l'indice de gauche doit être inférieur ou égal à l'indice de droite.\n",nb_ligne);
        exit(1);
      }
      creer_liste_extract_intervalle((yyvsp[-3].intval),(yyvsp[0].intval));
    }
#line 2453 "EXPR.tab.c"
    break;

  case 63: /* expr_bool: expr  */
#line 1128 "EXPR.y"
    {
      unsigned type;
      switch((yyvsp[0].exprval).ptr->kind){
      case(NAME):
        assert((yyvsp[0].exprval).ptr->var->init);
        type = (yyvsp[0].exprval).ptr->var->type;
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
        (yyval.exprval).ptr = newtemp(SYMTAB, INT, val);
        gencode(CODE,B_EVAL,(yyval.exprval).ptr,(yyvsp[0].exprval).ptr, NULL);

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
#line 2492 "EXPR.tab.c"
    break;

  case 64: /* expr_bool: expr_bool OR expr_bool  */
#line 1163 "EXPR.y"
    {
      valeur val;
      (yyval.exprval).ptr = newtemp(SYMTAB, INT, val);
      gencode(CODE,B_OU,(yyval.exprval).ptr,(yyvsp[-2].exprval).ptr,(yyvsp[0].exprval).ptr);
      // complete($1.false, $4);
    }
#line 2503 "EXPR.tab.c"
    break;

  case 65: /* expr_bool: expr_bool AND expr_bool  */
#line 1170 "EXPR.y"
    {
      valeur val;
      (yyval.exprval).ptr = newtemp(SYMTAB, INT, val);
      gencode(CODE,B_ET,(yyval.exprval).ptr,(yyvsp[-2].exprval).ptr,(yyvsp[0].exprval).ptr);
    }
#line 2513 "EXPR.tab.c"
    break;

  case 66: /* expr_bool: NOT expr_bool  */
#line 1176 "EXPR.y"
    {
      valeur val;
      (yyval.exprval).ptr = newtemp(SYMTAB, INT, val);
      gencode(CODE,B_NOT,(yyval.exprval).ptr,(yyvsp[0].exprval).ptr,NULL);
    }
#line 2523 "EXPR.tab.c"
    break;

  case 67: /* expr_bool: LPAR expr_bool RPAR  */
#line 1182 "EXPR.y"
    {
      (yyval.exprval).ptr = (yyvsp[-1].exprval).ptr;
    }
#line 2531 "EXPR.tab.c"
    break;

  case 68: /* expr_bool: expr LESS_THAN expr  */
#line 1186 "EXPR.y"
    {
    unsigned type1, type2;

    switch((yyvsp[-2].exprval).ptr->kind){
      case(NAME):
        assert((yyvsp[-2].exprval).ptr->var->init);
        type1 = (yyvsp[-2].exprval).ptr->var->type;
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

    switch((yyvsp[0].exprval).ptr->kind){
      case(NAME):
        assert((yyvsp[0].exprval).ptr->var->init);
        type2 = (yyvsp[0].exprval).ptr->var->type;
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
      (yyval.exprval).ptr = newtemp(SYMTAB, INT, val);
      gencode(CODE,B_LT,(yyval.exprval).ptr,(yyvsp[-2].exprval).ptr,(yyvsp[0].exprval).ptr);
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
#line 2590 "EXPR.tab.c"
    break;

  case 69: /* expr_bool: expr LTOE expr  */
#line 1241 "EXPR.y"
    {
    unsigned type1, type2;

    switch((yyvsp[-2].exprval).ptr->kind){
      case(NAME):
        assert((yyvsp[-2].exprval).ptr->var->init);
        type1 = (yyvsp[-2].exprval).ptr->var->type;
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

    switch((yyvsp[0].exprval).ptr->kind){
      case(NAME):
        assert((yyvsp[0].exprval).ptr->var->init);
        type2 = (yyvsp[0].exprval).ptr->var->type;
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
      (yyval.exprval).ptr = newtemp(SYMTAB, INT, val);
      gencode(CODE,B_LTOE,(yyval.exprval).ptr,(yyvsp[-2].exprval).ptr,(yyvsp[0].exprval).ptr);
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
#line 2647 "EXPR.tab.c"
    break;

  case 70: /* expr_bool: expr GREATER_THAN expr  */
#line 1295 "EXPR.y"
    {
      unsigned type1, type2;

      switch((yyvsp[-2].exprval).ptr->kind){
        case(NAME):
          assert((yyvsp[-2].exprval).ptr->var->init);
          type1 = (yyvsp[-2].exprval).ptr->var->type;
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

      switch((yyvsp[0].exprval).ptr->kind){
        case(NAME):
          assert((yyvsp[0].exprval).ptr->var->init);
          type2 = (yyvsp[0].exprval).ptr->var->type;
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
        (yyval.exprval).ptr = newtemp(SYMTAB, INT, val);
        gencode(CODE,B_GT,(yyval.exprval).ptr,(yyvsp[-2].exprval).ptr,(yyvsp[0].exprval).ptr);
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
#line 2704 "EXPR.tab.c"
    break;

  case 71: /* expr_bool: expr GTOE expr  */
#line 1348 "EXPR.y"
    {
      unsigned type1, type2;

      switch((yyvsp[-2].exprval).ptr->kind){
        case(NAME):
          assert((yyvsp[-2].exprval).ptr->var->init);
          type1 = (yyvsp[-2].exprval).ptr->var->type;
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

      switch((yyvsp[0].exprval).ptr->kind){
        case(NAME):
          assert((yyvsp[0].exprval).ptr->var->init);
          type2 = (yyvsp[0].exprval).ptr->var->type;
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
        (yyval.exprval).ptr = newtemp(SYMTAB, INT, val);
        gencode(CODE,B_GTOE,(yyval.exprval).ptr,(yyvsp[-2].exprval).ptr,(yyvsp[0].exprval).ptr);
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
#line 2761 "EXPR.tab.c"
    break;

  case 72: /* expr_bool: expr EQUAL expr  */
#line 1401 "EXPR.y"
    {
      unsigned type1, type2;

      switch((yyvsp[-2].exprval).ptr->kind){
        case(NAME):
          //assert($1.ptr->var->init);
          type1 = (yyvsp[-2].exprval).ptr->var->type;
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

      switch((yyvsp[0].exprval).ptr->kind){
        case(NAME):
          assert((yyvsp[0].exprval).ptr->var->init);
          type2 = (yyvsp[0].exprval).ptr->var->type;
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
        (yyval.exprval).ptr = newtemp(SYMTAB, INT, val);
        gencode(CODE,B_EQUAL,(yyval.exprval).ptr,(yyvsp[-2].exprval).ptr,(yyvsp[0].exprval).ptr);
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
#line 2818 "EXPR.tab.c"
    break;

  case 73: /* expr_bool: expr NOT_EQUAL expr  */
#line 1454 "EXPR.y"
    {
      unsigned type1, type2;

      switch((yyvsp[-2].exprval).ptr->kind){
        case(NAME):
          assert((yyvsp[-2].exprval).ptr->var->init);
          type1 = (yyvsp[-2].exprval).ptr->var->type;
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

      switch((yyvsp[0].exprval).ptr->kind){
        case(NAME):
          assert((yyvsp[0].exprval).ptr->var->init);
          type2 = (yyvsp[0].exprval).ptr->var->type;
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
        (yyval.exprval).ptr = newtemp(SYMTAB, INT, val);
        gencode(CODE,B_NOT_EQUAL,(yyval.exprval).ptr,(yyvsp[-2].exprval).ptr,(yyvsp[0].exprval).ptr);
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
#line 2875 "EXPR.tab.c"
    break;

  case 74: /* debut_bloc: LCURLY  */
#line 1510 "EXPR.y"
  {
    gencode(CODE, DEBUT_BLOC, NULL, NULL, NULL);
  }
#line 2883 "EXPR.tab.c"
    break;

  case 75: /* evaluation_if: LPAR expr_bool RPAR  */
#line 1524 "EXPR.y"
  {
    gencode(CODE,DEBUT_IF,(yyvsp[-1].exprval).ptr,NULL,NULL);
  }
#line 2891 "EXPR.tab.c"
    break;

  case 76: /* fin_bloc_if: RCURLY  */
#line 1531 "EXPR.y"
  {
    gencode(CODE, FIN_BLOC, NULL, NULL, NULL);
    gencode(CODE, FIN_IF, NULL, NULL, NULL);
  }
#line 2900 "EXPR.tab.c"
    break;

  case 77: /* saut_fin_if: RCURLY  */
#line 1539 "EXPR.y"
  {
    gencode(CODE, JUMP_FIN_IF, NULL, NULL, NULL);
    gencode(CODE, FIN_BLOC, NULL, NULL, NULL);
  }
#line 2909 "EXPR.tab.c"
    break;

  case 80: /* debut_while: LPAR  */
#line 1551 "EXPR.y"
  {
    gencode(CODE,DEBUT_WHILE,NULL,NULL,NULL);
  }
#line 2917 "EXPR.tab.c"
    break;

  case 81: /* fin_while: RPAR  */
#line 1558 "EXPR.y"
  {
    gencode(CODE,FIN_WHILE,NULL,NULL,NULL);
  }
#line 2925 "EXPR.tab.c"
    break;

  case 82: /* saut_fin_while: RCURLY  */
#line 1564 "EXPR.y"
  {
    gencode(CODE, JUMP_DEBUT_WHILE, NULL, NULL, NULL);
    gencode(CODE, FIN_BLOC, NULL, NULL, NULL);
  }
#line 2934 "EXPR.tab.c"
    break;

  case 84: /* init_for: ID ASSIGN expr  */
#line 1575 "EXPR.y"
    {
      // sémantique 
      Symbol * id = symtable_get(SYMTAB,(yyvsp[-2].strval));
      if (id == NULL){
        fprintf(stderr,"Ligne %d : La variable '%s' n'a jamais été déclarée.\n",nb_ligne,(yyvsp[-2].strval));
        exit(1);
      }

      if (id->var->type != NAME && id->var->type != INT){
        fprintf(stderr,"Ligne %d : L'itérateur d'une boucle for doit être de type INT.\n",nb_ligne);
        exit(1);
      }

      unsigned type2;
      switch((yyvsp[0].exprval).ptr->kind){
        case(NAME):
          assert((yyvsp[0].exprval).ptr->var->init);
          type2 = (yyvsp[0].exprval).ptr->var->type;
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
      
      gencode(CODE,COPY,id,(yyvsp[0].exprval).ptr,NULL);

      (yyval.variable) = id;
    }
#line 2978 "EXPR.tab.c"
    break;

  case 85: /* debut_for: SEMICOLON  */
#line 1618 "EXPR.y"
    {
      gencode(CODE,DEBUT_FOR,NULL,NULL,NULL);
    }
#line 2986 "EXPR.tab.c"
    break;

  case 86: /* fin_for: SEMICOLON  */
#line 1625 "EXPR.y"
    {
      gencode(CODE,FIN_FOR,NULL,NULL,NULL);
    }
#line 2994 "EXPR.tab.c"
    break;

  case 87: /* jump_debut_for: LCURLY  */
#line 1632 "EXPR.y"
    {
      gencode(CODE,JUMP_DEBUT_FOR,NULL,NULL,NULL);
      gencode(CODE,DEBUT_BLOC, NULL, NULL, NULL);
    }
#line 3003 "EXPR.tab.c"
    break;

  case 88: /* jump_maj_for: RCURLY  */
#line 1640 "EXPR.y"
    {
      gencode(CODE,JUMP_MAJ_FOR,NULL,NULL,NULL);
      gencode(CODE,FIN_BLOC, NULL, NULL, NULL);
    }
#line 3012 "EXPR.tab.c"
    break;

  case 89: /* struct_for: FOR LPAR init_for debut_for expr_bool fin_for affectation RPAR jump_debut_for bloc jump_maj_for  */
#line 1648 "EXPR.y"
    {
      // vérifier que le id de init_for c'est le meme que le id de affectation
      // l'énoncé précise mise à jour de l'itérateur
      if ((yyvsp[-8].variable) != (yyvsp[-4].variable)){
        fprintf(stderr,"Ligne %d : L'itérateur initialisé dans la première partie du for doit être l'itérateur mis à jour dans la troisième partie du for.\n",nb_ligne);
        exit(1);
      }
    }
#line 3025 "EXPR.tab.c"
    break;

  case 96: /* retour: RETURN expr SEMICOLON  */
#line 1671 "EXPR.y"
    {
      // vérifier que c'est soit un int ou un float
      unsigned type1;

      switch((yyvsp[-1].exprval).ptr->kind){
        case(NAME):
          assert((yyvsp[-1].exprval).ptr->var->init);
          type1 = (yyvsp[-1].exprval).ptr->var->type;
          break;
        case(CONST_INT):
          type1 = INT;
          break;
        case(CONST_FLOAT):
          type1 = FLOAT;
          break;
        default:
          fprintf(stderr,"Ligne %d : Possibilité de renvoyer des int ou des float seulement.\n",nb_ligne);
          exit(1);
          break;
      }

      (yyval.exprval) = (yyvsp[-1].exprval);

      // gencode ?
    }
#line 3055 "EXPR.tab.c"
    break;

  case 97: /* retour: RETURN LPAR expr RPAR SEMICOLON  */
#line 1697 "EXPR.y"
    {
      // vérifier que c'est soit un int ou un float
      unsigned type1;

      switch((yyvsp[-2].exprval).ptr->kind){
        case(NAME):
          assert((yyvsp[-2].exprval).ptr->var->init);
          type1 = (yyvsp[-2].exprval).ptr->var->type;
          break;
        case(CONST_INT):
          type1 = INT;
          break;
        case(CONST_FLOAT):
          type1 = FLOAT;
          break;
        default:
          fprintf(stderr,"Ligne %d : Possibilité de renvoyer des int ou des float seulement.\n",nb_ligne);
          exit(1);
          break;
      }

      (yyval.exprval) = (yyvsp[-2].exprval);

      // gencode ?
    }
#line 3085 "EXPR.tab.c"
    break;


#line 3089 "EXPR.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1724 "EXPR.y"


void yyerror(const char * s)
{
  fprintf(stderr,"Ligne %d : %s\n",nb_ligne,s);
}

