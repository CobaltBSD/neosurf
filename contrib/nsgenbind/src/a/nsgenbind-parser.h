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

#ifndef YY_NSGENBIND_A_NSGENBIND_PARSER_H_INCLUDED
# define YY_NSGENBIND_A_NSGENBIND_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int nsgenbind_debug;
#endif
/* "%code requires" blocks.  */
#line 18 "nsgenbind-parser.y"


#ifndef YYLTYPE
/* Bison <3.0 */
#define YYLTYPE yyltype
#else
/* Bison 3.0 or later */
/* Keep in sync with the defined API prefix */
#define NSGENBIND_LTYPE_IS_DECLARED 1
#endif

typedef struct yyltype {
        struct yyltype *next;
        int start_line;
        char *filename;

        int first_line;
        int first_column;
        int last_line;
        int last_column;
} yyltype;


#define YYLLOC_DEFAULT(Current, Rhs, N)                                \
        do                                                             \
                if (N) {                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;       \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;     \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;        \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;      \
          (Current).filename     = YYRHSLOC (Rhs, 1).filename;         \
          (Current).start_line   = YYRHSLOC (Rhs, 1).start_line;       \
                } else { /* empty RHS */                               \
          (Current).first_line   = (Current).last_line   =             \
            YYRHSLOC (Rhs, 0).last_line;                               \
          (Current).first_column = (Current).last_column =             \
            YYRHSLOC (Rhs, 0).last_column;                             \
          (Current).filename  = YYRHSLOC (Rhs, 0).filename;            \
          (Current).start_line  = YYRHSLOC (Rhs, 0).start_line;        \
                }                                                      \
        while (0)


#line 93 "a/nsgenbind-parser.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    TOK_BINDING = 258,             /* TOK_BINDING  */
    TOK_WEBIDL = 259,              /* TOK_WEBIDL  */
    TOK_PREFACE = 260,             /* TOK_PREFACE  */
    TOK_PROLOGUE = 261,            /* TOK_PROLOGUE  */
    TOK_EPILOGUE = 262,            /* TOK_EPILOGUE  */
    TOK_POSTFACE = 263,            /* TOK_POSTFACE  */
    TOK_CLASS = 264,               /* TOK_CLASS  */
    TOK_PRIVATE = 265,             /* TOK_PRIVATE  */
    TOK_INTERNAL = 266,            /* TOK_INTERNAL  */
    TOK_FLAGS = 267,               /* TOK_FLAGS  */
    TOK_TYPE = 268,                /* TOK_TYPE  */
    TOK_UNSHARED = 269,            /* TOK_UNSHARED  */
    TOK_SHARED = 270,              /* TOK_SHARED  */
    TOK_PROPERTY = 271,            /* TOK_PROPERTY  */
    TOK_INIT = 272,                /* TOK_INIT  */
    TOK_FINI = 273,                /* TOK_FINI  */
    TOK_METHOD = 274,              /* TOK_METHOD  */
    TOK_GETTER = 275,              /* TOK_GETTER  */
    TOK_SETTER = 276,              /* TOK_SETTER  */
    TOK_PROTOTYPE = 277,           /* TOK_PROTOTYPE  */
    TOK_DBLCOLON = 278,            /* TOK_DBLCOLON  */
    TOK_STRUCT = 279,              /* TOK_STRUCT  */
    TOK_UNION = 280,               /* TOK_UNION  */
    TOK_UNSIGNED = 281,            /* TOK_UNSIGNED  */
    TOK_IDENTIFIER = 282,          /* TOK_IDENTIFIER  */
    TOK_STRING_LITERAL = 283,      /* TOK_STRING_LITERAL  */
    TOK_CCODE_LITERAL = 284        /* TOK_CCODE_LITERAL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define TOK_BINDING 258
#define TOK_WEBIDL 259
#define TOK_PREFACE 260
#define TOK_PROLOGUE 261
#define TOK_EPILOGUE 262
#define TOK_POSTFACE 263
#define TOK_CLASS 264
#define TOK_PRIVATE 265
#define TOK_INTERNAL 266
#define TOK_FLAGS 267
#define TOK_TYPE 268
#define TOK_UNSHARED 269
#define TOK_SHARED 270
#define TOK_PROPERTY 271
#define TOK_INIT 272
#define TOK_FINI 273
#define TOK_METHOD 274
#define TOK_GETTER 275
#define TOK_SETTER 276
#define TOK_PROTOTYPE 277
#define TOK_DBLCOLON 278
#define TOK_STRUCT 279
#define TOK_UNION 280
#define TOK_UNSIGNED 281
#define TOK_IDENTIFIER 282
#define TOK_STRING_LITERAL 283
#define TOK_CCODE_LITERAL 284

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 185 "nsgenbind-parser.y"

        char *text;
        struct genbind_node *node;
        long value;

#line 177 "a/nsgenbind-parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif




int nsgenbind_parse (char *filename, struct genbind_node **genbind_ast);


#endif /* !YY_NSGENBIND_A_NSGENBIND_PARSER_H_INCLUDED  */
