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

#ifndef YY_FILTER_BUILD_X86_64_PC_LINUX_GNU_X86_64_PC_LINUX_GNU_RELEASE_LIB_SHARED_FILTER_PARSER_H_INCLUDED
# define YY_FILTER_BUILD_X86_64_PC_LINUX_GNU_X86_64_PC_LINUX_GNU_RELEASE_LIB_SHARED_FILTER_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef FILTER_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define FILTER_DEBUG 1
#  else
#   define FILTER_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define FILTER_DEBUG 1
# endif /* ! defined YYDEBUG */
#endif  /* ! defined FILTER_DEBUG */
#if FILTER_DEBUG
extern int filter_debug;
#endif

/* Token kinds.  */
#ifndef FILTER_TOKENTYPE
# define FILTER_TOKENTYPE
  enum filter_tokentype
  {
    FILTER_EMPTY = -2,
    FILTER_EOF = 0,                /* "end of file"  */
    FILTER_error = 256,            /* error  */
    FILTER_UNDEF = 257,            /* "invalid token"  */
    T_PATTERN = 258,               /* T_PATTERN  */
    T_LEVEL = 259,                 /* T_LEVEL  */
    T_CATEGORY_SPECIFIER = 260,    /* T_CATEGORY_SPECIFIER  */
    T_FILENAME_SPECIFIER = 261,    /* T_FILENAME_SPECIFIER  */
    T_LEVEL_SPECIFIER = 262,       /* T_LEVEL_SPECIFIER  */
    T_DIRNAME_SPECIFIER = 263,     /* T_DIRNAME_SPECIFIER  */
    T_FUNCNAME_SPECIFIER = 264,    /* T_FUNCNAME_SPECIFIER  */
    T_OP_AND = 265,                /* T_OP_AND  */
    T_OP_OR = 266                  /* T_OP_OR  */
  };
  typedef enum filter_tokentype filter_token_kind_t;
#endif

/* Value type.  */
#if ! defined FILTER_STYPE && ! defined FILTER_STYPE_IS_DECLARED
union FILTER_STYPE
{
#line 30 "src/filter-parser.y"

	char *patt;
	nslog_level level;
	nslog_filter_t *filter;

#line 89 "build-x86_64-pc-linux-gnu-x86_64-pc-linux-gnu-release-lib-shared/filter-parser.h"

};
typedef union FILTER_STYPE FILTER_STYPE;
# define FILTER_STYPE_IS_TRIVIAL 1
# define FILTER_STYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined FILTER_LTYPE && ! defined FILTER_LTYPE_IS_DECLARED
typedef struct FILTER_LTYPE FILTER_LTYPE;
struct FILTER_LTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define FILTER_LTYPE_IS_DECLARED 1
# define FILTER_LTYPE_IS_TRIVIAL 1
#endif




int filter_parse (nslog_filter_t **output);


#endif /* !YY_FILTER_BUILD_X86_64_PC_LINUX_GNU_X86_64_PC_LINUX_GNU_RELEASE_LIB_SHARED_FILTER_PARSER_H_INCLUDED  */
