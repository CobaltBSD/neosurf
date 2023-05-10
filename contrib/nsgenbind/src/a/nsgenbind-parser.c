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
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         nsgenbind_parse
#define yylex           nsgenbind_lex
#define yyerror         nsgenbind_error
#define yydebug         nsgenbind_debug
#define yynerrs         nsgenbind_nerrs


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

#include "nsgenbind-parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TOK_BINDING = 3,                /* TOK_BINDING  */
  YYSYMBOL_TOK_WEBIDL = 4,                 /* TOK_WEBIDL  */
  YYSYMBOL_TOK_PREFACE = 5,                /* TOK_PREFACE  */
  YYSYMBOL_TOK_PROLOGUE = 6,               /* TOK_PROLOGUE  */
  YYSYMBOL_TOK_EPILOGUE = 7,               /* TOK_EPILOGUE  */
  YYSYMBOL_TOK_POSTFACE = 8,               /* TOK_POSTFACE  */
  YYSYMBOL_TOK_CLASS = 9,                  /* TOK_CLASS  */
  YYSYMBOL_TOK_PRIVATE = 10,               /* TOK_PRIVATE  */
  YYSYMBOL_TOK_INTERNAL = 11,              /* TOK_INTERNAL  */
  YYSYMBOL_TOK_FLAGS = 12,                 /* TOK_FLAGS  */
  YYSYMBOL_TOK_TYPE = 13,                  /* TOK_TYPE  */
  YYSYMBOL_TOK_UNSHARED = 14,              /* TOK_UNSHARED  */
  YYSYMBOL_TOK_SHARED = 15,                /* TOK_SHARED  */
  YYSYMBOL_TOK_PROPERTY = 16,              /* TOK_PROPERTY  */
  YYSYMBOL_TOK_INIT = 17,                  /* TOK_INIT  */
  YYSYMBOL_TOK_FINI = 18,                  /* TOK_FINI  */
  YYSYMBOL_TOK_METHOD = 19,                /* TOK_METHOD  */
  YYSYMBOL_TOK_GETTER = 20,                /* TOK_GETTER  */
  YYSYMBOL_TOK_SETTER = 21,                /* TOK_SETTER  */
  YYSYMBOL_TOK_PROTOTYPE = 22,             /* TOK_PROTOTYPE  */
  YYSYMBOL_TOK_DBLCOLON = 23,              /* TOK_DBLCOLON  */
  YYSYMBOL_TOK_STRUCT = 24,                /* TOK_STRUCT  */
  YYSYMBOL_TOK_UNION = 25,                 /* TOK_UNION  */
  YYSYMBOL_TOK_UNSIGNED = 26,              /* TOK_UNSIGNED  */
  YYSYMBOL_TOK_IDENTIFIER = 27,            /* TOK_IDENTIFIER  */
  YYSYMBOL_TOK_STRING_LITERAL = 28,        /* TOK_STRING_LITERAL  */
  YYSYMBOL_TOK_CCODE_LITERAL = 29,         /* TOK_CCODE_LITERAL  */
  YYSYMBOL_30_ = 30,                       /* ';'  */
  YYSYMBOL_31_ = 31,                       /* '{'  */
  YYSYMBOL_32_ = 32,                       /* '}'  */
  YYSYMBOL_33_ = 33,                       /* '*'  */
  YYSYMBOL_34_ = 34,                       /* ','  */
  YYSYMBOL_35_ = 35,                       /* '('  */
  YYSYMBOL_36_ = 36,                       /* ')'  */
  YYSYMBOL_YYACCEPT = 37,                  /* $accept  */
  YYSYMBOL_Input = 38,                     /* Input  */
  YYSYMBOL_Statements = 39,                /* Statements  */
  YYSYMBOL_Statement = 40,                 /* Statement  */
  YYSYMBOL_Binding = 41,                   /* Binding  */
  YYSYMBOL_BindingArgs = 42,               /* BindingArgs  */
  YYSYMBOL_BindingArg = 43,                /* BindingArg  */
  YYSYMBOL_WebIDL = 44,                    /* WebIDL  */
  YYSYMBOL_CTypeSpecifier = 45,            /* CTypeSpecifier  */
  YYSYMBOL_CType = 46,                     /* CType  */
  YYSYMBOL_CTypeIdent = 47,                /* CTypeIdent  */
  YYSYMBOL_CBlock = 48,                    /* CBlock  */
  YYSYMBOL_ParameterList = 49,             /* ParameterList  */
  YYSYMBOL_Method = 50,                    /* Method  */
  YYSYMBOL_MethodType = 51,                /* MethodType  */
  YYSYMBOL_BindingAndMethodType = 52,      /* BindingAndMethodType  */
  YYSYMBOL_MethodName = 53,                /* MethodName  */
  YYSYMBOL_MethodDeclarator = 54,          /* MethodDeclarator  */
  YYSYMBOL_Class = 55,                     /* Class  */
  YYSYMBOL_ClassArgs = 56,                 /* ClassArgs  */
  YYSYMBOL_ClassArg = 57,                  /* ClassArg  */
  YYSYMBOL_ClassFlags = 58,                /* ClassFlags  */
  YYSYMBOL_Modifiers = 59,                 /* Modifiers  */
  YYSYMBOL_Modifier = 60                   /* Modifier  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 68 "nsgenbind-parser.y"

/* parser for the binding generation config file 
 *
 * This file is part of nsgenbind.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2012 Vincent Sanders <vince@netsurf-browser.org>
 */

#include <stdio.h>
#include <string.h>

#define YYFPRINTF genbind_fprintf
#define YY_LOCATION_PRINT(File, Loc)                            \
  genbind_fprintf(File, "%d.%d-%d.%d",                          \
                  (Loc).first_line, (Loc).first_column,         \
                  (Loc).last_line,  (Loc).last_column)

#include "utils.h"
#include "nsgenbind-lexer.h"
#include "webidl-ast.h"
#include "nsgenbind-ast.h"

static char *errtxt;

static void nsgenbind_error(yyltype *locp,
                            char *filename,
                            struct genbind_node **genbind_ast,
                            const char *str)
{
        int errlen;

        UNUSED(genbind_ast);
        UNUSED(filename);

        errlen = snprintf(NULL, 0, "%s:%d:%s",
                          locp->filename, locp->first_line, str);
        errtxt = malloc(errlen + 1);
        snprintf(errtxt, errlen + 1, "%s:%d:%s",
                          locp->filename, locp->first_line, str);

}

static struct genbind_node *
add_method(struct genbind_node **genbind_ast,
           long methodtype,
           struct genbind_node *declarator,
           char *cdata,
           long lineno,
           char *filename)
{
        struct genbind_node *res_node;
        struct genbind_node *method_node;
        struct genbind_node *class_node;
        struct genbind_node *cdata_node;
        struct genbind_node *location_node;
        char *class_name;

        /* extract the class name from the declarator */
        class_name = genbind_node_gettext(
                genbind_node_find_type(
                        genbind_node_getnode(
                                genbind_node_find_type(
                                        declarator,
                                        NULL,
                                        GENBIND_NODE_TYPE_CLASS)),
                        NULL,
                        GENBIND_NODE_TYPE_IDENT));

        if (cdata == NULL) {
                cdata_node = declarator;
        } else {
                cdata_node = genbind_new_node(GENBIND_NODE_TYPE_CDATA,
                                              declarator,
                                              cdata);
        }

        location_node = genbind_new_node(GENBIND_NODE_TYPE_FILE,
                                genbind_new_number_node(GENBIND_NODE_TYPE_LINE,
                                                        cdata_node,
                                                        lineno),
                                         strdup(filename));

        /* generate method node */
        method_node = genbind_new_node(GENBIND_NODE_TYPE_METHOD,
                                 NULL,
                                 genbind_new_number_node(GENBIND_NODE_TYPE_METHOD_TYPE,
                                                  location_node,
                                                  methodtype));

        class_node = genbind_node_find_type_ident(*genbind_ast,
                                                  NULL,
                                                  GENBIND_NODE_TYPE_CLASS,
                                                  class_name);
        if (class_node == NULL) {
                /* no existing class so manufacture one and attach method */
                res_node = genbind_new_node(GENBIND_NODE_TYPE_CLASS, NULL,
                                      genbind_new_node(GENBIND_NODE_TYPE_IDENT,
                                                       method_node,
                                                       class_name));
        } else {
                /* update the existing class */

                /* link member node into class_node */
                genbind_node_add(class_node, method_node);

                res_node = NULL; /* updating so no need to add a new node */
        }
        return res_node;
}


#line 282 "a/nsgenbind-parser.c"

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
typedef yytype_int8 yy_state_t;

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

#if 1

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
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  25
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   134

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  37
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  61
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  104

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   284


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
      35,    36,    33,     2,    34,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    30,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    31,     2,    32,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   256,   256,   263,   265,   270,   279,   281,   283,   287,
     297,   299,   306,   308,   322,   332,   341,   350,   359,   366,
     368,   375,   380,   389,   391,   399,   404,   415,   420,   427,
     432,   437,   442,   447,   452,   457,   461,   466,   471,   476,
     483,   488,   493,   501,   512,   523,   532,   543,   552,   556,
     563,   568,   573,   583,   588,   601,   606,   614,   618,   625,
     630,   635
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "TOK_BINDING",
  "TOK_WEBIDL", "TOK_PREFACE", "TOK_PROLOGUE", "TOK_EPILOGUE",
  "TOK_POSTFACE", "TOK_CLASS", "TOK_PRIVATE", "TOK_INTERNAL", "TOK_FLAGS",
  "TOK_TYPE", "TOK_UNSHARED", "TOK_SHARED", "TOK_PROPERTY", "TOK_INIT",
  "TOK_FINI", "TOK_METHOD", "TOK_GETTER", "TOK_SETTER", "TOK_PROTOTYPE",
  "TOK_DBLCOLON", "TOK_STRUCT", "TOK_UNION", "TOK_UNSIGNED",
  "TOK_IDENTIFIER", "TOK_STRING_LITERAL", "TOK_CCODE_LITERAL", "';'",
  "'{'", "'}'", "'*'", "','", "'('", "')'", "$accept", "Input",
  "Statements", "Statement", "Binding", "BindingArgs", "BindingArg",
  "WebIDL", "CTypeSpecifier", "CType", "CTypeIdent", "CBlock",
  "ParameterList", "Method", "MethodType", "BindingAndMethodType",
  "MethodName", "MethodDeclarator", "Class", "ClassArgs", "ClassArg",
  "ClassFlags", "Modifiers", "Modifier", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-42)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
       6,   -20,    13,   -42,   -42,   -42,   -42,    51,   -42,   -42,
     -42,   -42,   -42,   -42,    91,    26,   -42,   -42,   -42,    65,
     -42,   -42,   -42,    62,    64,   -42,   -42,   -15,    37,    76,
      63,    38,    28,   -42,   -42,    67,    66,    -2,   -42,   -42,
      68,    61,    61,    71,   -42,    68,    69,    63,   -42,   -42,
     -42,    70,    72,    73,    75,   -42,   -42,    74,    77,   -42,
     -17,   -42,    78,    79,   -42,    47,    80,    81,   -42,   -12,
      23,    60,    82,   -42,    36,   -42,   -42,   -42,   -42,    83,
      61,   -42,   -42,   -42,   -42,   -42,   -42,   -42,    86,   -42,
     -42,   -42,    84,   -42,   -42,   -42,   -42,    22,    88,   -42,
     -42,   -42,   -42,   -42
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,    36,    37,    38,    39,     0,    29,    30,
      31,    32,    33,    34,     0,     2,     3,     6,     8,     0,
      35,     7,     5,     0,     0,     1,     4,     0,     0,     0,
      48,     0,     0,    23,    28,    27,     0,     0,    10,    12,
       0,     0,     0,     0,    57,     0,     0,    48,    42,    41,
      40,     0,     0,     0,     0,    18,    46,    19,     0,    25,
       0,    24,     0,     0,    11,     0,     0,     0,    55,     0,
       0,     0,     0,    49,     0,    16,    17,    15,    20,    21,
       0,    45,    14,     9,    13,    50,    51,    53,     0,    59,
      60,    61,     0,    58,    54,    47,    44,     0,     0,    26,
      56,    52,    43,    22
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -42,   -42,   -42,   101,   -42,   -42,    85,   -42,   -42,   -42,
     -41,   -24,    29,   -42,   -42,    12,   -42,   -42,   -42,    87,
     -42,   -42,   -42,   -42
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    14,    15,    16,    17,    37,    38,    39,    57,    58,
      59,    35,    60,    18,    19,    20,    51,    28,    21,    46,
      47,    69,    70,    93
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      66,    67,    36,     3,     4,     5,     6,     1,    31,     2,
      22,     3,     4,     5,     6,     7,    65,    80,    87,    81,
      32,    71,    88,     8,     9,    10,    11,    12,    13,     2,
      63,     3,     4,     5,     6,     7,    89,    90,    91,    99,
      23,    40,    45,     8,     9,    10,    11,    12,    13,    40,
      92,    48,    52,    53,    54,    55,    80,    49,   102,    45,
      52,    53,    54,    55,    56,    50,    33,    34,     3,     4,
       5,     6,    96,    41,    42,    43,    61,    84,    24,    44,
      36,     3,     4,     5,     6,    52,    53,    54,    55,    61,
      94,    25,    27,    29,    62,    30,    61,    33,    68,    75,
      76,    72,    77,    97,    79,    74,    98,    78,    82,    83,
      85,    86,    95,   100,   101,   103,    26,     0,     0,     0,
       0,     0,    64,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    73
};

static const yytype_int8 yycheck[] =
{
      41,    42,     4,     5,     6,     7,     8,     1,    23,     3,
      30,     5,     6,     7,     8,     9,    40,    34,    30,    36,
      35,    45,    34,    17,    18,    19,    20,    21,    22,     3,
      32,     5,     6,     7,     8,     9,    13,    14,    15,    80,
      27,    29,    30,    17,    18,    19,    20,    21,    22,    37,
      27,    13,    24,    25,    26,    27,    34,    19,    36,    47,
      24,    25,    26,    27,    36,    27,    29,    30,     5,     6,
       7,     8,    36,    10,    11,    12,    29,    30,    27,    16,
       4,     5,     6,     7,     8,    24,    25,    26,    27,    29,
      30,     0,    27,    31,    28,    31,    29,    29,    27,    27,
      27,    32,    27,    74,    27,    35,    23,    33,    30,    30,
      30,    30,    30,    27,    30,    27,    15,    -1,    -1,    -1,
      -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    47
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     1,     3,     5,     6,     7,     8,     9,    17,    18,
      19,    20,    21,    22,    38,    39,    40,    41,    50,    51,
      52,    55,    30,    27,    27,     0,    40,    27,    54,    31,
      31,    23,    35,    29,    30,    48,     4,    42,    43,    44,
      52,    10,    11,    12,    16,    52,    56,    57,    13,    19,
      27,    53,    24,    25,    26,    27,    36,    45,    46,    47,
      49,    29,    28,    32,    43,    48,    47,    47,    27,    58,
      59,    48,    32,    56,    35,    27,    27,    27,    33,    27,
      34,    36,    30,    30,    30,    30,    30,    30,    34,    13,
      14,    15,    27,    60,    30,    30,    36,    49,    23,    47,
      27,    30,    36,    27
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    37,    38,    39,    39,    39,    40,    40,    40,    41,
      42,    42,    43,    43,    44,    45,    45,    45,    45,    46,
      46,    47,    47,    48,    48,    49,    49,    50,    50,    51,
      51,    51,    51,    51,    51,    51,    52,    52,    52,    52,
      53,    53,    53,    54,    54,    54,    54,    55,    56,    56,
      57,    57,    57,    57,    57,    58,    58,    59,    59,    60,
      60,    60
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     2,     1,     1,     1,     6,
       1,     2,     1,     3,     3,     2,     2,     2,     1,     1,
       2,     2,     4,     1,     2,     1,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     6,     5,     4,     3,     6,     0,     2,
       3,     3,     4,     3,     3,     1,     3,     0,     2,     1,
       1,     1
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
        yyerror (&yylloc, filename, genbind_ast, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location, filename, genbind_ast); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, char *filename, struct genbind_node **genbind_ast)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  YY_USE (filename);
  YY_USE (genbind_ast);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, char *filename, struct genbind_node **genbind_ast)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, filename, genbind_ast);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule, char *filename, struct genbind_node **genbind_ast)
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
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]), filename, genbind_ast);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, filename, genbind_ast); \
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


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, char *filename, struct genbind_node **genbind_ast)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  YY_USE (filename);
  YY_USE (genbind_ast);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (char *filename, struct genbind_node **genbind_ast)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

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

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */


/* User initialization code.  */
#line 62 "nsgenbind-parser.y"
{
        yylloc.first_line   = yylloc.last_line   = 1;
        yylloc.first_column = yylloc.last_column = 1;
        yylloc.filename = filename;
}

#line 1511 "a/nsgenbind-parser.c"

  yylsp[0] = yylloc;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
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
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
      yychar = yylex (&yylval, &yylloc);
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
      yyerror_range[1] = yylloc;
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
  *++yylsp = yylloc;

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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* Input: Statements  */
#line 257 "nsgenbind-parser.y"
        {
                *genbind_ast = (yyvsp[0].node);
        }
#line 1726 "a/nsgenbind-parser.c"
    break;

  case 4: /* Statements: Statements Statement  */
#line 266 "nsgenbind-parser.y"
        {
                (yyval.node) = *genbind_ast = genbind_node_prepend((yyvsp[0].node), (yyvsp[-1].node));
        }
#line 1734 "a/nsgenbind-parser.c"
    break;

  case 5: /* Statements: error ';'  */
#line 271 "nsgenbind-parser.y"
        {
                fprintf(stderr, "%s\n", errtxt);
                free(errtxt);
                YYABORT ;
        }
#line 1744 "a/nsgenbind-parser.c"
    break;

  case 9: /* Binding: TOK_BINDING TOK_IDENTIFIER '{' BindingArgs '}' ';'  */
#line 288 "nsgenbind-parser.y"
        {
                (yyval.node) = genbind_new_node(GENBIND_NODE_TYPE_BINDING,
                                      NULL,
                                      genbind_new_node(GENBIND_NODE_TYPE_NAME,
                                                       (yyvsp[-2].node), (yyvsp[-4].text)));
        }
#line 1755 "a/nsgenbind-parser.c"
    break;

  case 11: /* BindingArgs: BindingArgs BindingArg  */
#line 300 "nsgenbind-parser.y"
        {
                (yyval.node) = genbind_node_link((yyvsp[0].node), (yyvsp[-1].node));
        }
#line 1763 "a/nsgenbind-parser.c"
    break;

  case 13: /* BindingArg: BindingAndMethodType CBlock ';'  */
#line 309 "nsgenbind-parser.y"
        {
                (yyval.node) = genbind_new_node(GENBIND_NODE_TYPE_METHOD,
                        NULL,
                        genbind_new_number_node(GENBIND_NODE_TYPE_METHOD_TYPE,
                                genbind_new_node(GENBIND_NODE_TYPE_CDATA,
                                                 NULL,
                                                 (yyvsp[-1].text)),
                                (yyvsp[-2].value)));
        }
#line 1777 "a/nsgenbind-parser.c"
    break;

  case 14: /* WebIDL: TOK_WEBIDL TOK_STRING_LITERAL ';'  */
#line 323 "nsgenbind-parser.y"
        {
                (yyval.node) = genbind_new_node(GENBIND_NODE_TYPE_WEBIDL, NULL, (yyvsp[-1].text));
        }
#line 1785 "a/nsgenbind-parser.c"
    break;

  case 15: /* CTypeSpecifier: TOK_UNSIGNED TOK_IDENTIFIER  */
#line 333 "nsgenbind-parser.y"
        {
                (yyval.node) = genbind_new_node(GENBIND_NODE_TYPE_NAME,
                                      genbind_new_node(GENBIND_NODE_TYPE_NAME,
                                                       NULL,
                                                       strdup("unsigned")),
                                      (yyvsp[0].text));
        }
#line 1797 "a/nsgenbind-parser.c"
    break;

  case 16: /* CTypeSpecifier: TOK_STRUCT TOK_IDENTIFIER  */
#line 342 "nsgenbind-parser.y"
        {
                (yyval.node) = genbind_new_node(GENBIND_NODE_TYPE_NAME,
                                      genbind_new_node(GENBIND_NODE_TYPE_NAME,
                                                       NULL,
                                                       strdup("struct")),
                                      (yyvsp[0].text));
        }
#line 1809 "a/nsgenbind-parser.c"
    break;

  case 17: /* CTypeSpecifier: TOK_UNION TOK_IDENTIFIER  */
#line 351 "nsgenbind-parser.y"
        {
                (yyval.node) = genbind_new_node(GENBIND_NODE_TYPE_NAME,
                                      genbind_new_node(GENBIND_NODE_TYPE_NAME,
                                                       NULL,
                                                       strdup("union")),
                                      (yyvsp[0].text));
        }
#line 1821 "a/nsgenbind-parser.c"
    break;

  case 18: /* CTypeSpecifier: TOK_IDENTIFIER  */
#line 360 "nsgenbind-parser.y"
        {
                (yyval.node) = genbind_new_node(GENBIND_NODE_TYPE_NAME, NULL, (yyvsp[0].text));
        }
#line 1829 "a/nsgenbind-parser.c"
    break;

  case 20: /* CType: CTypeSpecifier '*'  */
#line 369 "nsgenbind-parser.y"
        {
                (yyval.node) = genbind_new_node(GENBIND_NODE_TYPE_NAME, (yyvsp[-1].node), strdup("*"));
        }
#line 1837 "a/nsgenbind-parser.c"
    break;

  case 21: /* CTypeIdent: CType TOK_IDENTIFIER  */
#line 376 "nsgenbind-parser.y"
        {
                (yyval.node) = genbind_new_node(GENBIND_NODE_TYPE_IDENT, (yyvsp[-1].node), (yyvsp[0].text));
        }
#line 1845 "a/nsgenbind-parser.c"
    break;

  case 22: /* CTypeIdent: CType TOK_IDENTIFIER TOK_DBLCOLON TOK_IDENTIFIER  */
#line 381 "nsgenbind-parser.y"
        {
                (yyval.node) = genbind_new_node(GENBIND_NODE_TYPE_IDENT,
                        genbind_new_node(GENBIND_NODE_TYPE_IDENT, (yyvsp[-3].node), (yyvsp[-2].text)), (yyvsp[0].text));
        }
#line 1854 "a/nsgenbind-parser.c"
    break;

  case 24: /* CBlock: CBlock TOK_CCODE_LITERAL  */
#line 392 "nsgenbind-parser.y"
        {
          (yyval.text) = genbind_strapp((yyvsp[-1].text), (yyvsp[0].text));
        }
#line 1862 "a/nsgenbind-parser.c"
    break;

  case 25: /* ParameterList: CTypeIdent  */
#line 400 "nsgenbind-parser.y"
        {
                (yyval.node) = genbind_new_node(GENBIND_NODE_TYPE_PARAMETER, NULL, (yyvsp[0].node));
        }
#line 1870 "a/nsgenbind-parser.c"
    break;

  case 26: /* ParameterList: ParameterList ',' CTypeIdent  */
#line 405 "nsgenbind-parser.y"
        {
                (yyval.node) = genbind_node_prepend((yyvsp[-2].node),
                                          genbind_new_node(
                                                  GENBIND_NODE_TYPE_PARAMETER,
                                                  NULL,
                                                  (yyvsp[0].node)));
        }
#line 1882 "a/nsgenbind-parser.c"
    break;

  case 27: /* Method: MethodType MethodDeclarator CBlock  */
#line 416 "nsgenbind-parser.y"
        {
                (yyval.node) = add_method(genbind_ast, (yyvsp[-2].value), (yyvsp[-1].node), (yyvsp[0].text), (yylsp[-2]).first_line, (yylsp[-2]).filename);
        }
#line 1890 "a/nsgenbind-parser.c"
    break;

  case 28: /* Method: MethodType MethodDeclarator ';'  */
#line 421 "nsgenbind-parser.y"
        {
                (yyval.node) = add_method(genbind_ast, (yyvsp[-2].value), (yyvsp[-1].node), NULL, (yylsp[-2]).first_line, (yylsp[-2]).filename);
        }
#line 1898 "a/nsgenbind-parser.c"
    break;

  case 29: /* MethodType: TOK_INIT  */
#line 428 "nsgenbind-parser.y"
        {
                (yyval.value) = GENBIND_METHOD_TYPE_INIT;
        }
#line 1906 "a/nsgenbind-parser.c"
    break;

  case 30: /* MethodType: TOK_FINI  */
#line 433 "nsgenbind-parser.y"
        {
                (yyval.value) = GENBIND_METHOD_TYPE_FINI;
        }
#line 1914 "a/nsgenbind-parser.c"
    break;

  case 31: /* MethodType: TOK_METHOD  */
#line 438 "nsgenbind-parser.y"
        {
                (yyval.value) = GENBIND_METHOD_TYPE_METHOD;
        }
#line 1922 "a/nsgenbind-parser.c"
    break;

  case 32: /* MethodType: TOK_GETTER  */
#line 443 "nsgenbind-parser.y"
        {
                (yyval.value) = GENBIND_METHOD_TYPE_GETTER;
        }
#line 1930 "a/nsgenbind-parser.c"
    break;

  case 33: /* MethodType: TOK_SETTER  */
#line 448 "nsgenbind-parser.y"
        {
                (yyval.value) = GENBIND_METHOD_TYPE_SETTER;
        }
#line 1938 "a/nsgenbind-parser.c"
    break;

  case 34: /* MethodType: TOK_PROTOTYPE  */
#line 453 "nsgenbind-parser.y"
        {
                (yyval.value) = GENBIND_METHOD_TYPE_PROTOTYPE;
        }
#line 1946 "a/nsgenbind-parser.c"
    break;

  case 36: /* BindingAndMethodType: TOK_PREFACE  */
#line 462 "nsgenbind-parser.y"
        {
                (yyval.value) = GENBIND_METHOD_TYPE_PREFACE;
        }
#line 1954 "a/nsgenbind-parser.c"
    break;

  case 37: /* BindingAndMethodType: TOK_PROLOGUE  */
#line 467 "nsgenbind-parser.y"
        {
                (yyval.value) = GENBIND_METHOD_TYPE_PROLOGUE;
        }
#line 1962 "a/nsgenbind-parser.c"
    break;

  case 38: /* BindingAndMethodType: TOK_EPILOGUE  */
#line 472 "nsgenbind-parser.y"
        {
                (yyval.value) = GENBIND_METHOD_TYPE_EPILOGUE;
        }
#line 1970 "a/nsgenbind-parser.c"
    break;

  case 39: /* BindingAndMethodType: TOK_POSTFACE  */
#line 477 "nsgenbind-parser.y"
        {
                (yyval.value) = GENBIND_METHOD_TYPE_POSTFACE;
        }
#line 1978 "a/nsgenbind-parser.c"
    break;

  case 40: /* MethodName: TOK_IDENTIFIER  */
#line 484 "nsgenbind-parser.y"
        {
                (yyval.text) = (yyvsp[0].text);
        }
#line 1986 "a/nsgenbind-parser.c"
    break;

  case 41: /* MethodName: TOK_METHOD  */
#line 489 "nsgenbind-parser.y"
        {
                (yyval.text) = strdup("method");
        }
#line 1994 "a/nsgenbind-parser.c"
    break;

  case 42: /* MethodName: TOK_TYPE  */
#line 494 "nsgenbind-parser.y"
        {
                (yyval.text) = strdup("type");
        }
#line 2002 "a/nsgenbind-parser.c"
    break;

  case 43: /* MethodDeclarator: TOK_IDENTIFIER TOK_DBLCOLON MethodName '(' ParameterList ')'  */
#line 502 "nsgenbind-parser.y"
        {
                (yyval.node) = genbind_new_node(GENBIND_NODE_TYPE_CLASS,
                                      genbind_new_node(GENBIND_NODE_TYPE_IDENT,
                                                       (yyvsp[-1].node),
                                                       (yyvsp[-3].text)),
                                      genbind_new_node(GENBIND_NODE_TYPE_IDENT,
                                                       NULL,
                                                       (yyvsp[-5].text)));
        }
#line 2016 "a/nsgenbind-parser.c"
    break;

  case 44: /* MethodDeclarator: TOK_IDENTIFIER TOK_DBLCOLON MethodName '(' ')'  */
#line 513 "nsgenbind-parser.y"
        {
                (yyval.node) = genbind_new_node(GENBIND_NODE_TYPE_CLASS,
                                      genbind_new_node(GENBIND_NODE_TYPE_IDENT,
                                                       NULL,
                                                       (yyvsp[-2].text)),
                                      genbind_new_node(GENBIND_NODE_TYPE_IDENT,
                                                       NULL,
                                                       (yyvsp[-4].text)));
        }
#line 2030 "a/nsgenbind-parser.c"
    break;

  case 45: /* MethodDeclarator: TOK_IDENTIFIER '(' ParameterList ')'  */
#line 524 "nsgenbind-parser.y"
        {
                (yyval.node) = genbind_new_node(GENBIND_NODE_TYPE_CLASS,
                                      (yyvsp[-1].node),
                                      genbind_new_node(GENBIND_NODE_TYPE_IDENT,
                                                       NULL,
                                                       (yyvsp[-3].text)));
        }
#line 2042 "a/nsgenbind-parser.c"
    break;

  case 46: /* MethodDeclarator: TOK_IDENTIFIER '(' ')'  */
#line 533 "nsgenbind-parser.y"
        {
                (yyval.node) = genbind_new_node(GENBIND_NODE_TYPE_CLASS, NULL,
                                      genbind_new_node(GENBIND_NODE_TYPE_IDENT,
                                                       NULL,
                                                       (yyvsp[-2].text)));
        }
#line 2053 "a/nsgenbind-parser.c"
    break;

  case 47: /* Class: TOK_CLASS TOK_IDENTIFIER '{' ClassArgs '}' ';'  */
#line 544 "nsgenbind-parser.y"
        {
                (yyval.node) = genbind_new_node(GENBIND_NODE_TYPE_CLASS, NULL,
                        genbind_new_node(GENBIND_NODE_TYPE_IDENT, (yyvsp[-2].node), (yyvsp[-4].text)));
        }
#line 2062 "a/nsgenbind-parser.c"
    break;

  case 48: /* ClassArgs: %empty  */
#line 552 "nsgenbind-parser.y"
        {
                (yyval.node) = NULL;
        }
#line 2070 "a/nsgenbind-parser.c"
    break;

  case 49: /* ClassArgs: ClassArg ClassArgs  */
#line 557 "nsgenbind-parser.y"
        {
                (yyval.node) = genbind_node_prepend((yyvsp[0].node), (yyvsp[-1].node));
        }
#line 2078 "a/nsgenbind-parser.c"
    break;

  case 50: /* ClassArg: TOK_PRIVATE CTypeIdent ';'  */
#line 564 "nsgenbind-parser.y"
        {
                (yyval.node) = genbind_new_node(GENBIND_NODE_TYPE_PRIVATE, NULL, (yyvsp[-1].node));
        }
#line 2086 "a/nsgenbind-parser.c"
    break;

  case 51: /* ClassArg: TOK_INTERNAL CTypeIdent ';'  */
#line 569 "nsgenbind-parser.y"
        {
                (yyval.node) = genbind_new_node(GENBIND_NODE_TYPE_INTERNAL, NULL, (yyvsp[-1].node));
        }
#line 2094 "a/nsgenbind-parser.c"
    break;

  case 52: /* ClassArg: TOK_PROPERTY Modifiers TOK_IDENTIFIER ';'  */
#line 574 "nsgenbind-parser.y"
        {
                (yyval.node) = genbind_new_node(GENBIND_NODE_TYPE_PROPERTY, NULL,
                        genbind_new_number_node(GENBIND_NODE_TYPE_MODIFIER,
                                genbind_new_node(GENBIND_NODE_TYPE_IDENT,
                                                 NULL,
                                                 (yyvsp[-1].text)),
                                         (yyvsp[-2].value)));
        }
#line 2107 "a/nsgenbind-parser.c"
    break;

  case 53: /* ClassArg: TOK_FLAGS ClassFlags ';'  */
#line 584 "nsgenbind-parser.y"
        {
                (yyval.node) = genbind_new_node(GENBIND_NODE_TYPE_FLAGS, NULL, (yyvsp[-1].node));
        }
#line 2115 "a/nsgenbind-parser.c"
    break;

  case 54: /* ClassArg: BindingAndMethodType CBlock ';'  */
#line 589 "nsgenbind-parser.y"
        {
                (yyval.node) = genbind_new_node(GENBIND_NODE_TYPE_METHOD,
                        NULL,
                        genbind_new_number_node(GENBIND_NODE_TYPE_METHOD_TYPE,
                                genbind_new_node(GENBIND_NODE_TYPE_CDATA,
                                                 NULL,
                                                 (yyvsp[-1].text)),
                                (yyvsp[-2].value)));
        }
#line 2129 "a/nsgenbind-parser.c"
    break;

  case 55: /* ClassFlags: TOK_IDENTIFIER  */
#line 602 "nsgenbind-parser.y"
        {
                (yyval.node) = genbind_new_node(GENBIND_NODE_TYPE_IDENT, NULL, (yyvsp[0].text));
        }
#line 2137 "a/nsgenbind-parser.c"
    break;

  case 56: /* ClassFlags: ClassFlags ',' TOK_IDENTIFIER  */
#line 607 "nsgenbind-parser.y"
        {
                (yyval.node) = genbind_new_node(GENBIND_NODE_TYPE_IDENT, (yyvsp[-2].node), (yyvsp[0].text));
        }
#line 2145 "a/nsgenbind-parser.c"
    break;

  case 57: /* Modifiers: %empty  */
#line 614 "nsgenbind-parser.y"
        {
            (yyval.value) = GENBIND_TYPE_NONE;
        }
#line 2153 "a/nsgenbind-parser.c"
    break;

  case 58: /* Modifiers: Modifiers Modifier  */
#line 619 "nsgenbind-parser.y"
        {
            (yyval.value) |= (yyvsp[0].value);
        }
#line 2161 "a/nsgenbind-parser.c"
    break;

  case 59: /* Modifier: TOK_TYPE  */
#line 626 "nsgenbind-parser.y"
        {
            (yyval.value) = GENBIND_TYPE_TYPE;
        }
#line 2169 "a/nsgenbind-parser.c"
    break;

  case 60: /* Modifier: TOK_UNSHARED  */
#line 631 "nsgenbind-parser.y"
        {
            (yyval.value) = GENBIND_TYPE_UNSHARED;            
        }
#line 2177 "a/nsgenbind-parser.c"
    break;

  case 61: /* Modifier: TOK_SHARED  */
#line 636 "nsgenbind-parser.y"
        {
            (yyval.value) = GENBIND_TYPE_NONE;            
        }
#line 2185 "a/nsgenbind-parser.c"
    break;


#line 2189 "a/nsgenbind-parser.c"

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
  *++yylsp = yyloc;

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
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (&yylloc, filename, genbind_ast, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
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
                      yytoken, &yylval, &yylloc, filename, genbind_ast);
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp, filename, genbind_ast);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

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
  yyerror (&yylloc, filename, genbind_ast, YY_("memory exhausted"));
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
                  yytoken, &yylval, &yylloc, filename, genbind_ast);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp, filename, genbind_ast);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 641 "nsgenbind-parser.y"

