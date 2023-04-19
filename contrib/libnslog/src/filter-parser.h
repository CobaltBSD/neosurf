#ifndef _filter__defines_h_
#define _filter__defines_h_

#define T_PATTERN 257
#define T_LEVEL 258
#define T_CATEGORY_SPECIFIER 259
#define T_FILENAME_SPECIFIER 260
#define T_LEVEL_SPECIFIER 261
#define T_DIRNAME_SPECIFIER 262
#define T_FUNCNAME_SPECIFIER 263
#define T_OP_AND 264
#define T_OP_OR 265
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union YYSTYPE {
	char *patt;
	nslog_level level;
	nslog_filter_t *filter;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
/* Default: YYLTYPE is the text position type. */
typedef struct YYLTYPE
{
    int first_line;
    int first_column;
    int last_line;
    int last_column;
    unsigned source;
} YYLTYPE;
#define YYLTYPE_IS_DECLARED 1
#endif
#define YYRHSLOC(rhs, k) ((rhs)[k])
extern YYLTYPE filter_lloc;

#endif /* _filter__defines_h_ */
