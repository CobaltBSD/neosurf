/* original parser id follows */
/* yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93" */
/* (use YYMAJOR/YYMINOR for ifdefs dependent on parser version) */

#define YYBYACC 1
#define YYMAJOR 2
#define YYMINOR 0
#define YYPATCH 20230219

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)
#define YYENOMEM       (-2)
#define YYEOF          0
#undef YYBTYACC
#define YYBTYACC 0
#define YYDEBUGSTR YYPREFIX "debug"

#ifndef yyparse
#define yyparse    nsgenbind_parse
#endif /* yyparse */

#ifndef yylex
#define yylex      nsgenbind_lex
#endif /* yylex */

#ifndef yyerror
#define yyerror    nsgenbind_error
#endif /* yyerror */

#ifndef yychar
#define yychar     nsgenbind_char
#endif /* yychar */

#ifndef yyval
#define yyval      nsgenbind_val
#endif /* yyval */

#ifndef yylval
#define yylval     nsgenbind_lval
#endif /* yylval */

#ifndef yydebug
#define yydebug    nsgenbind_debug
#endif /* yydebug */

#ifndef yynerrs
#define yynerrs    nsgenbind_nerrs
#endif /* yynerrs */

#ifndef yyerrflag
#define yyerrflag  nsgenbind_errflag
#endif /* yyerrflag */

#ifndef yylhs
#define yylhs      nsgenbind_lhs
#endif /* yylhs */

#ifndef yylen
#define yylen      nsgenbind_len
#endif /* yylen */

#ifndef yydefred
#define yydefred   nsgenbind_defred
#endif /* yydefred */

#ifndef yystos
#define yystos     nsgenbind_stos
#endif /* yystos */

#ifndef yydgoto
#define yydgoto    nsgenbind_dgoto
#endif /* yydgoto */

#ifndef yysindex
#define yysindex   nsgenbind_sindex
#endif /* yysindex */

#ifndef yyrindex
#define yyrindex   nsgenbind_rindex
#endif /* yyrindex */

#ifndef yygindex
#define yygindex   nsgenbind_gindex
#endif /* yygindex */

#ifndef yytable
#define yytable    nsgenbind_table
#endif /* yytable */

#ifndef yycheck
#define yycheck    nsgenbind_check
#endif /* yycheck */

#ifndef yyname
#define yyname     nsgenbind_name
#endif /* yyname */

#ifndef yyrule
#define yyrule     nsgenbind_rule
#endif /* yyrule */

#ifndef yyloc
#define yyloc      nsgenbind_loc
#endif /* yyloc */

#ifndef yylloc
#define yylloc     nsgenbind_lloc
#endif /* yylloc */

#if YYBTYACC

#ifndef yycindex
#define yycindex   nsgenbind_cindex
#endif /* yycindex */

#ifndef yyctable
#define yyctable   nsgenbind_ctable
#endif /* yyctable */

#endif /* YYBTYACC */

#define YYPREFIX "nsgenbind_"

#define YYPURE 1

#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#line 187 "nsgenbind-parser.y"
typedef union YYSTYPE
{
        char *text;
        struct genbind_node *node;
        long value;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 143 "nsgenbind-parser.c"

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

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(char *filename, struct genbind_node **genbind_ast)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# ifdef YYLEX_PARAM_TYPE
#  define YYLEX_DECL() yylex(YYSTYPE *yylval, YYLTYPE *yylloc, YYLEX_PARAM_TYPE YYLEX_PARAM)
# else
#  define YYLEX_DECL() yylex(YYSTYPE *yylval, YYLTYPE *yylloc, void * YYLEX_PARAM)
# endif
# define YYLEX yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(YYSTYPE *yylval, YYLTYPE *yylloc)
# define YYLEX yylex(&yylval, &yylloc)
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(YYLTYPE *loc, char *filename, struct genbind_node **genbind_ast, const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(&yylloc, filename, genbind_ast, msg)
#endif

extern int YYPARSE_DECL();

#define TOK_BINDING 257
#define TOK_WEBIDL 258
#define TOK_PREFACE 259
#define TOK_PROLOGUE 260
#define TOK_EPILOGUE 261
#define TOK_POSTFACE 262
#define TOK_CLASS 263
#define TOK_PRIVATE 264
#define TOK_INTERNAL 265
#define TOK_FLAGS 266
#define TOK_TYPE 267
#define TOK_UNSHARED 268
#define TOK_SHARED 269
#define TOK_PROPERTY 270
#define TOK_INIT 271
#define TOK_FINI 272
#define TOK_METHOD 273
#define TOK_GETTER 274
#define TOK_SETTER 275
#define TOK_PROTOTYPE 276
#define TOK_DBLCOLON 277
#define TOK_STRUCT 278
#define TOK_UNION 279
#define TOK_UNSIGNED 280
#define TOK_IDENTIFIER 281
#define TOK_STRING_LITERAL 282
#define TOK_CCODE_LITERAL 283
#define YYERRCODE 256
typedef int YYINT;
static const YYINT nsgenbind_lhs[] = {                   -1,
    0,    5,    5,    5,    4,    4,    4,    6,    7,    7,
    8,    8,   18,   22,   22,   22,   22,   21,   21,   20,
   20,    1,    1,   19,   19,   13,   13,   16,   16,   16,
   16,   16,   16,   16,   17,   17,   17,   17,   14,   14,
   14,   15,   15,   15,   15,    9,   10,   10,   11,   11,
   11,   11,   11,   12,   12,    2,    2,    3,    3,    3,
};
static const YYINT nsgenbind_len[] = {                    2,
    1,    1,    2,    2,    1,    1,    1,    6,    1,    2,
    1,    3,    3,    2,    2,    2,    1,    1,    2,    2,
    4,    1,    2,    1,    3,    3,    3,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    6,    5,    4,    3,    6,    0,    2,    3,    3,
    4,    3,    3,    1,    3,    0,    2,    1,    1,    1,
};
static const YYINT nsgenbind_defred[] = {                 0,
    0,    0,   35,   36,   37,   38,    0,   28,   29,   30,
   31,   32,   33,    0,    2,    0,    5,    6,    7,    0,
   34,    4,    0,    0,    3,    0,    0,    0,    0,    0,
    0,   22,   27,    0,    0,    0,    9,    0,   11,    0,
    0,    0,   56,    0,    0,    0,   41,   40,   39,    0,
    0,    0,    0,   17,   45,    0,   24,    0,    0,   23,
    0,    0,   10,    0,    0,    0,   54,    0,    0,    0,
   48,    0,    0,   15,   16,   14,    0,   44,    0,   19,
   13,    8,   12,   49,   50,   52,    0,   58,   59,   60,
    0,   57,   46,   53,   43,    0,   25,    0,   55,   51,
   42,   21,
};
#if defined(YYDESTRUCT_CALL) || defined(YYSTYPE_TOSTRING)
static const YYINT nsgenbind_stos[] = {                   0,
  256,  257,  259,  260,  261,  262,  263,  271,  272,  273,
  274,  275,  276,  285,  289,  290,  291,  294,  298,  301,
  302,   59,  281,  281,  289,  281,  300,  123,  123,  277,
   40,  283,   59,  286,  258,  292,  293,  302,  303,  264,
  265,  266,  270,  295,  296,  302,  267,  273,  281,  299,
  278,  279,  280,  281,   41,  304,  305,  306,  307,  283,
  282,  125,  293,  286,  305,  305,  281,  297,  287,  125,
  295,  286,   40,  281,  281,  281,   44,   41,  281,   42,
   59,   59,   59,   59,   59,   59,   44,  267,  268,  269,
  281,  288,   59,   59,   41,  304,  305,  277,  281,   59,
   41,  281,
};
#endif /* YYDESTRUCT_CALL || YYSTYPE_TOSTRING */
static const YYINT nsgenbind_dgoto[] = {                 14,
   34,   69,   92,   15,   16,   17,   36,   37,   18,   44,
   45,   68,   19,   50,   27,   20,   21,   39,   56,   57,
   58,   59,
};
static const YYINT nsgenbind_sindex[] = {              -245,
  -52, -268,    0,    0,    0,    0, -261,    0,    0,    0,
    0,    0,    0,    0,    0, -238,    0,    0,    0, -241,
    0,    0,  -82,  -69,    0,  -32,  -57, -179, -192, -215,
  -41,    0,    0, -228, -226, -119,    0, -224,    0, -194,
 -194, -218,    0,  -60, -192, -224,    0,    0,    0,   31,
 -206, -205, -204,    0,    0,   20,    0, -191,   50,    0,
   30,   34,    0,  -56,   35,   36,    0,  -12, -219,   37,
    0,  -54,  -37,    0,    0,    0, -194,    0, -180,    0,
    0,    0,    0,    0,    0,    0, -183,    0,    0,    0,
   40,    0,    0,    0,    0,   47,    0, -181,    0,    0,
    0,    0,
};
static const YYINT nsgenbind_rindex[] = {                 0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  101,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  -23,    0,
    0,    0,    0,    1,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  -23,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, -178,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   -2,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,
};
#if YYBTYACC
static const YYINT nsgenbind_cindex[] = {                 0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,
};
#endif
static const YYINT nsgenbind_gindex[] = {                 0,
    7,    0,    0,   88,    0,    0,    0,   69,    0,   61,
    0,    0,    0,    0,    0,    0,   15,    0,   38,  -31,
    0,    0,
};
#define YYTABLESIZE 277
static const YYINT nsgenbind_table[] = {                 55,
   26,   33,   83,   95,   94,   62,   22,   31,   65,   66,
    1,    2,   23,    3,    4,    5,    6,    7,    2,   24,
    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,
   13,   87,    8,    9,   10,   11,   12,   13,   20,   26,
   28,   20,   38,   46,   64,   97,   86,   88,   89,   90,
   38,   47,   72,   29,   60,   61,   20,   48,   32,   46,
   78,   91,   67,   77,   70,   49,    3,    4,    5,    6,
   73,   40,   41,   42,   74,   75,   76,   43,   35,    3,
    4,    5,    6,   51,   52,   53,   54,  101,   81,   79,
   77,   80,   82,   84,   85,   93,   98,   99,  100,  102,
    1,   47,   18,   25,   63,   71,    0,    0,    0,    0,
   96,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   35,    3,
    4,    5,    6,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   32,   60,    0,   60,    0,
    0,    0,    0,    0,    0,    0,   51,   52,   53,   54,
   51,   52,   53,   54,   30,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   26,    0,   26,
   26,   26,   26,   26,    0,    0,    0,    0,    0,    0,
    0,   26,   26,   26,   26,   26,   26,
};
static const YYINT nsgenbind_check[] = {                 41,
    0,   59,   59,   41,   59,  125,   59,   40,   40,   41,
  256,  257,  281,  259,  260,  261,  262,  263,  257,  281,
  259,  260,  261,  262,  263,  271,  272,  273,  274,  275,
  276,   44,  271,  272,  273,  274,  275,  276,   41,  281,
  123,   44,   28,   29,   38,   77,   59,  267,  268,  269,
   36,  267,   46,  123,  283,  282,   59,  273,  283,   45,
   41,  281,  281,   44,  125,  281,  259,  260,  261,  262,
   40,  264,  265,  266,  281,  281,  281,  270,  258,  259,
  260,  261,  262,  278,  279,  280,  281,   41,   59,  281,
   44,   42,   59,   59,   59,   59,  277,  281,   59,  281,
    0,  125,  281,   16,   36,   45,   -1,   -1,   -1,   -1,
   73,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,  259,
  260,  261,  262,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  283,  283,   -1,  283,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  278,  279,  280,  281,
  278,  279,  280,  281,  277,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  257,   -1,  259,
  260,  261,  262,  263,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  271,  272,  273,  274,  275,  276,
};
#if YYBTYACC
static const YYINT nsgenbind_ctable[] = {                -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,
};
#endif
#define YYFINAL 14
#ifndef YYDEBUG
#define YYDEBUG 1
#endif
#define YYMAXTOKEN 283
#define YYUNDFTOKEN 308
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? YYUNDFTOKEN : (a))
#if YYDEBUG
static const char *const nsgenbind_name[] = {

"$end",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,"'('","')'","'*'",0,"','",0,0,0,0,0,0,0,0,0,0,0,0,0,0,"';'",0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'{'",0,"'}'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"error",
"TOK_BINDING","TOK_WEBIDL","TOK_PREFACE","TOK_PROLOGUE","TOK_EPILOGUE",
"TOK_POSTFACE","TOK_CLASS","TOK_PRIVATE","TOK_INTERNAL","TOK_FLAGS","TOK_TYPE",
"TOK_UNSHARED","TOK_SHARED","TOK_PROPERTY","TOK_INIT","TOK_FINI","TOK_METHOD",
"TOK_GETTER","TOK_SETTER","TOK_PROTOTYPE","TOK_DBLCOLON","TOK_STRUCT",
"TOK_UNION","TOK_UNSIGNED","TOK_IDENTIFIER","TOK_STRING_LITERAL",
"TOK_CCODE_LITERAL","$accept","Input","CBlock","Modifiers","Modifier",
"Statement","Statements","Binding","BindingArgs","BindingArg","Class",
"ClassArgs","ClassArg","ClassFlags","Method","MethodName","MethodDeclarator",
"MethodType","BindingAndMethodType","WebIDL","ParameterList","CTypeIdent",
"CType","CTypeSpecifier","illegal-symbol",
};
static const char *const nsgenbind_rule[] = {
"$accept : Input",
"Input : Statements",
"Statements : Statement",
"Statements : Statements Statement",
"Statements : error ';'",
"Statement : Binding",
"Statement : Class",
"Statement : Method",
"Binding : TOK_BINDING TOK_IDENTIFIER '{' BindingArgs '}' ';'",
"BindingArgs : BindingArg",
"BindingArgs : BindingArgs BindingArg",
"BindingArg : WebIDL",
"BindingArg : BindingAndMethodType CBlock ';'",
"WebIDL : TOK_WEBIDL TOK_STRING_LITERAL ';'",
"CTypeSpecifier : TOK_UNSIGNED TOK_IDENTIFIER",
"CTypeSpecifier : TOK_STRUCT TOK_IDENTIFIER",
"CTypeSpecifier : TOK_UNION TOK_IDENTIFIER",
"CTypeSpecifier : TOK_IDENTIFIER",
"CType : CTypeSpecifier",
"CType : CTypeSpecifier '*'",
"CTypeIdent : CType TOK_IDENTIFIER",
"CTypeIdent : CType TOK_IDENTIFIER TOK_DBLCOLON TOK_IDENTIFIER",
"CBlock : TOK_CCODE_LITERAL",
"CBlock : CBlock TOK_CCODE_LITERAL",
"ParameterList : CTypeIdent",
"ParameterList : ParameterList ',' CTypeIdent",
"Method : MethodType MethodDeclarator CBlock",
"Method : MethodType MethodDeclarator ';'",
"MethodType : TOK_INIT",
"MethodType : TOK_FINI",
"MethodType : TOK_METHOD",
"MethodType : TOK_GETTER",
"MethodType : TOK_SETTER",
"MethodType : TOK_PROTOTYPE",
"MethodType : BindingAndMethodType",
"BindingAndMethodType : TOK_PREFACE",
"BindingAndMethodType : TOK_PROLOGUE",
"BindingAndMethodType : TOK_EPILOGUE",
"BindingAndMethodType : TOK_POSTFACE",
"MethodName : TOK_IDENTIFIER",
"MethodName : TOK_METHOD",
"MethodName : TOK_TYPE",
"MethodDeclarator : TOK_IDENTIFIER TOK_DBLCOLON MethodName '(' ParameterList ')'",
"MethodDeclarator : TOK_IDENTIFIER TOK_DBLCOLON MethodName '(' ')'",
"MethodDeclarator : TOK_IDENTIFIER '(' ParameterList ')'",
"MethodDeclarator : TOK_IDENTIFIER '(' ')'",
"Class : TOK_CLASS TOK_IDENTIFIER '{' ClassArgs '}' ';'",
"ClassArgs :",
"ClassArgs : ClassArg ClassArgs",
"ClassArg : TOK_PRIVATE CTypeIdent ';'",
"ClassArg : TOK_INTERNAL CTypeIdent ';'",
"ClassArg : TOK_PROPERTY Modifiers TOK_IDENTIFIER ';'",
"ClassArg : TOK_FLAGS ClassFlags ';'",
"ClassArg : BindingAndMethodType CBlock ';'",
"ClassFlags : TOK_IDENTIFIER",
"ClassFlags : ClassFlags ',' TOK_IDENTIFIER",
"Modifiers :",
"Modifiers : Modifiers Modifier",
"Modifier : TOK_TYPE",
"Modifier : TOK_UNSHARED",
"Modifier : TOK_SHARED",

};
#endif

#if YYDEBUG
int      yydebug;
#endif

/* %code "requires" block start */
#line 21 "nsgenbind-parser.y"

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

/* %code "requires" block end */
#line 553 "nsgenbind-parser.c"

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
#ifndef YYLLOC_DEFAULT
#define YYLLOC_DEFAULT(loc, rhs, n) \
do \
{ \
    if (n == 0) \
    { \
        (loc).first_line   = YYRHSLOC(rhs, 0).last_line; \
        (loc).first_column = YYRHSLOC(rhs, 0).last_column; \
        (loc).last_line    = YYRHSLOC(rhs, 0).last_line; \
        (loc).last_column  = YYRHSLOC(rhs, 0).last_column; \
    } \
    else \
    { \
        (loc).first_line   = YYRHSLOC(rhs, 1).first_line; \
        (loc).first_column = YYRHSLOC(rhs, 1).first_column; \
        (loc).last_line    = YYRHSLOC(rhs, n).last_line; \
        (loc).last_column  = YYRHSLOC(rhs, n).last_column; \
    } \
} while (0)
#endif /* YYLLOC_DEFAULT */
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#if YYBTYACC

#ifndef YYLVQUEUEGROWTH
#define YYLVQUEUEGROWTH 32
#endif
#endif /* YYBTYACC */

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#ifndef YYINITSTACKSIZE
#define YYINITSTACKSIZE 200
#endif

typedef struct {
    unsigned stacksize;
    YYINT    *s_base;
    YYINT    *s_mark;
    YYINT    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  *p_base;
    YYLTYPE  *p_mark;
#endif
} YYSTACKDATA;
#if YYBTYACC

struct YYParseState_s
{
    struct YYParseState_s *save;    /* Previously saved parser state */
    YYSTACKDATA            yystack; /* saved parser stack */
    int                    state;   /* saved parser state */
    int                    errflag; /* saved error recovery status */
    int                    lexeme;  /* saved index of the conflict lexeme in the lexical queue */
    YYINT                  ctry;    /* saved index in yyctable[] for this conflict */
};
typedef struct YYParseState_s YYParseState;
#endif /* YYBTYACC */

/* %code "" block start */
#line 71 "nsgenbind-parser.y"
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

/* %code "" block end */
#line 740 "nsgenbind-parser.c"

/* For use in generated program */
#define yydepth (int)(yystack.s_mark - yystack.s_base)
#if YYBTYACC
#define yytrial (yyps->save)
#endif /* YYBTYACC */

#if YYDEBUG
#include <stdio.h>	/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    YYINT *newss;
    YYSTYPE *newvs;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE *newps;
#endif

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return YYENOMEM;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (YYINT *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return YYENOMEM;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return YYENOMEM;

    data->l_base = newvs;
    data->l_mark = newvs + i;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    newps = (YYLTYPE *)realloc(data->p_base, newsize * sizeof(*newps));
    if (newps == 0)
        return YYENOMEM;

    data->p_base = newps;
    data->p_mark = newps + i;
#endif

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;

#if YYDEBUG
    if (yydebug)
        fprintf(stderr, "%sdebug: stack size increased to %d\n", YYPREFIX, newsize);
#endif
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    free(data->p_base);
#endif
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif /* YYPURE || defined(YY_NO_LEAKS) */
#if YYBTYACC

static YYParseState *
yyNewState(unsigned size)
{
    YYParseState *p = (YYParseState *) malloc(sizeof(YYParseState));
    if (p == NULL) return NULL;

    p->yystack.stacksize = size;
    if (size == 0)
    {
        p->yystack.s_base = NULL;
        p->yystack.l_base = NULL;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        p->yystack.p_base = NULL;
#endif
        return p;
    }
    p->yystack.s_base    = (YYINT *) malloc(size * sizeof(YYINT));
    if (p->yystack.s_base == NULL) return NULL;
    p->yystack.l_base    = (YYSTYPE *) malloc(size * sizeof(YYSTYPE));
    if (p->yystack.l_base == NULL) return NULL;
    memset(p->yystack.l_base, 0, size * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    p->yystack.p_base    = (YYLTYPE *) malloc(size * sizeof(YYLTYPE));
    if (p->yystack.p_base == NULL) return NULL;
    memset(p->yystack.p_base, 0, size * sizeof(YYLTYPE));
#endif

    return p;
}

static void
yyFreeState(YYParseState *p)
{
    yyfreestack(&p->yystack);
    free(p);
}
#endif /* YYBTYACC */

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab
#if YYBTYACC
#define YYVALID        do { if (yyps->save)            goto yyvalid; } while(0)
#define YYVALID_NESTED do { if (yyps->save && \
                                yyps->save->save == 0) goto yyvalid; } while(0)
#endif /* YYBTYACC */

int
YYPARSE_DECL()
{
    int      yyerrflag;
    int      yychar;
    YYSTYPE  yyval;
    YYSTYPE  yylval;
    int      yynerrs;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  yyloc; /* position returned by actions */
    YYLTYPE  yylloc; /* position from the lexer */
#endif

    /* variables for the parser stack */
    YYSTACKDATA yystack;
#if YYBTYACC

    /* Current parser state */
    static YYParseState *yyps = 0;

    /* yypath != NULL: do the full parse, starting at *yypath parser state. */
    static YYParseState *yypath = 0;

    /* Base of the lexical value queue */
    static YYSTYPE *yylvals = 0;

    /* Current position at lexical value queue */
    static YYSTYPE *yylvp = 0;

    /* End position of lexical value queue */
    static YYSTYPE *yylve = 0;

    /* The last allocated position at the lexical value queue */
    static YYSTYPE *yylvlim = 0;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    /* Base of the lexical position queue */
    static YYLTYPE *yylpsns = 0;

    /* Current position at lexical position queue */
    static YYLTYPE *yylpp = 0;

    /* End position of lexical position queue */
    static YYLTYPE *yylpe = 0;

    /* The last allocated position at the lexical position queue */
    static YYLTYPE *yylplim = 0;
#endif

    /* Current position at lexical token queue */
    static YYINT  *yylexp = 0;

    static YYINT  *yylexemes = 0;
#endif /* YYBTYACC */
    int yym, yyn, yystate, yyresult;
#if YYBTYACC
    int yynewerrflag;
    YYParseState *yyerrctx = NULL;
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  yyerror_loc_range[3]; /* position of error start/end (0 unused) */
#endif
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
    if (yydebug)
        fprintf(stderr, "%sdebug[<# of symbols on state stack>]\n", YYPREFIX);
#endif
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    memset(yyerror_loc_range, 0, sizeof(yyerror_loc_range));
#endif

    yyerrflag = 0;
    yychar = 0;
    memset(&yyval,  0, sizeof(yyval));
    memset(&yylval, 0, sizeof(yylval));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    memset(&yyloc,  0, sizeof(yyloc));
    memset(&yylloc, 0, sizeof(yylloc));
#endif

#line 65 "nsgenbind-parser.y"
	{
        yylloc.first_line   = yylloc.last_line   = 1;
        yylloc.first_column = yylloc.last_column = 1;
        yylloc.filename = filename;
}
#if YYBTYACC
    yyps = yyNewState(0); if (yyps == 0) goto yyenomem;
    yyps->save = 0;
#endif /* YYBTYACC */
    yym = 0;
    /* yyn is set below */
    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark = yystack.p_base;
#endif
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
#if YYBTYACC
        do {
        if (yylvp < yylve)
        {
            /* we're currently re-reading tokens */
            yylval = *yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylloc = *yylpp++;
#endif
            yychar = *yylexp++;
            break;
        }
        if (yyps->save)
        {
            /* in trial mode; save scanner results for future parse attempts */
            if (yylvp == yylvlim)
            {   /* Enlarge lexical value queue */
                size_t p = (size_t) (yylvp - yylvals);
                size_t s = (size_t) (yylvlim - yylvals);

                s += YYLVQUEUEGROWTH;
                if ((yylexemes = (YYINT *)realloc(yylexemes, s * sizeof(YYINT))) == NULL) goto yyenomem;
                if ((yylvals   = (YYSTYPE *)realloc(yylvals, s * sizeof(YYSTYPE))) == NULL) goto yyenomem;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                if ((yylpsns   = (YYLTYPE *)realloc(yylpsns, s * sizeof(YYLTYPE))) == NULL) goto yyenomem;
#endif
                yylvp   = yylve = yylvals + p;
                yylvlim = yylvals + s;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp   = yylpe = yylpsns + p;
                yylplim = yylpsns + s;
#endif
                yylexp  = yylexemes + p;
            }
            *yylexp = (YYINT) YYLEX;
            *yylvp++ = yylval;
            yylve++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            *yylpp++ = yylloc;
            yylpe++;
#endif
            yychar = *yylexp++;
            break;
        }
        /* normal operation, no conflict encountered */
#endif /* YYBTYACC */
        yychar = YYLEX;
#if YYBTYACC
        } while (0);
#endif /* YYBTYACC */
        if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            fprintf(stderr, "%s[%d]: state %d, reading token %d (%s)",
                            YYDEBUGSTR, yydepth, yystate, yychar, yys);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
            if (!yytrial)
#endif /* YYBTYACC */
                fprintf(stderr, " <%s>", YYSTYPE_TOSTRING(yychar, yylval));
#endif
            fputc('\n', stderr);
        }
#endif
    }
#if YYBTYACC

    /* Do we have a conflict? */
    if (((yyn = yycindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
        yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        YYINT ctry;

        if (yypath)
        {
            YYParseState *save;
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%s[%d]: CONFLICT in state %d: following successful trial parse\n",
                                YYDEBUGSTR, yydepth, yystate);
#endif
            /* Switch to the next conflict context */
            save = yypath;
            yypath = save->save;
            save->save = NULL;
            ctry = save->ctry;
            if (save->state != yystate) YYABORT;
            yyFreeState(save);

        }
        else
        {

            /* Unresolved conflict - start/continue trial parse */
            YYParseState *save;
#if YYDEBUG
            if (yydebug)
            {
                fprintf(stderr, "%s[%d]: CONFLICT in state %d. ", YYDEBUGSTR, yydepth, yystate);
                if (yyps->save)
                    fputs("ALREADY in conflict, continuing trial parse.\n", stderr);
                else
                    fputs("Starting trial parse.\n", stderr);
            }
#endif
            save                  = yyNewState((unsigned)(yystack.s_mark - yystack.s_base + 1));
            if (save == NULL) goto yyenomem;
            save->save            = yyps->save;
            save->state           = yystate;
            save->errflag         = yyerrflag;
            save->yystack.s_mark  = save->yystack.s_base + (yystack.s_mark - yystack.s_base);
            memcpy (save->yystack.s_base, yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            save->yystack.l_mark  = save->yystack.l_base + (yystack.l_mark - yystack.l_base);
            memcpy (save->yystack.l_base, yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            save->yystack.p_mark  = save->yystack.p_base + (yystack.p_mark - yystack.p_base);
            memcpy (save->yystack.p_base, yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            ctry                  = yytable[yyn];
            if (yyctable[ctry] == -1)
            {
#if YYDEBUG
                if (yydebug && yychar >= YYEOF)
                    fprintf(stderr, "%s[%d]: backtracking 1 token\n", YYDEBUGSTR, yydepth);
#endif
                ctry++;
            }
            save->ctry = ctry;
            if (yyps->save == NULL)
            {
                /* If this is a first conflict in the stack, start saving lexemes */
                if (!yylexemes)
                {
                    yylexemes = (YYINT *) malloc((YYLVQUEUEGROWTH) * sizeof(YYINT));
                    if (yylexemes == NULL) goto yyenomem;
                    yylvals   = (YYSTYPE *) malloc((YYLVQUEUEGROWTH) * sizeof(YYSTYPE));
                    if (yylvals == NULL) goto yyenomem;
                    yylvlim   = yylvals + YYLVQUEUEGROWTH;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpsns   = (YYLTYPE *) malloc((YYLVQUEUEGROWTH) * sizeof(YYLTYPE));
                    if (yylpsns == NULL) goto yyenomem;
                    yylplim   = yylpsns + YYLVQUEUEGROWTH;
#endif
                }
                if (yylvp == yylve)
                {
                    yylvp  = yylve = yylvals;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpp  = yylpe = yylpsns;
#endif
                    yylexp = yylexemes;
                    if (yychar >= YYEOF)
                    {
                        *yylve++ = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                        *yylpe++ = yylloc;
#endif
                        *yylexp  = (YYINT) yychar;
                        yychar   = YYEMPTY;
                    }
                }
            }
            if (yychar >= YYEOF)
            {
                yylvp--;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp--;
#endif
                yylexp--;
                yychar = YYEMPTY;
            }
            save->lexeme = (int) (yylvp - yylvals);
            yyps->save   = save;
        }
        if (yytable[yyn] == ctry)
        {
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%s[%d]: state %d, shifting to state %d\n",
                                YYDEBUGSTR, yydepth, yystate, yyctable[ctry]);
#endif
            if (yychar < 0)
            {
                yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp++;
#endif
                yylexp++;
            }
            if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM)
                goto yyoverflow;
            yystate = yyctable[ctry];
            *++yystack.s_mark = (YYINT) yystate;
            *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            *++yystack.p_mark = yylloc;
#endif
            yychar  = YYEMPTY;
            if (yyerrflag > 0) --yyerrflag;
            goto yyloop;
        }
        else
        {
            yyn = yyctable[ctry];
            goto yyreduce;
        }
    } /* End of code dealing with conflicts */
#endif /* YYBTYACC */
    if (((yyn = yysindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
#if YYDEBUG
        if (yydebug)
            fprintf(stderr, "%s[%d]: state %d, shifting to state %d\n",
                            YYDEBUGSTR, yydepth, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        *++yystack.p_mark = yylloc;
#endif
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if (((yyn = yyrindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag != 0) goto yyinrecovery;
#if YYBTYACC

    yynewerrflag = 1;
    goto yyerrhandler;
    goto yyerrlab; /* redundant goto avoids 'unused label' warning */

yyerrlab:
    /* explicit YYERROR from an action -- pop the rhs of the rule reduced
     * before looking for error recovery */
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark -= yym;
#endif

    yynewerrflag = 0;
yyerrhandler:
    while (yyps->save)
    {
        int ctry;
        YYParseState *save = yyps->save;
#if YYDEBUG
        if (yydebug)
            fprintf(stderr, "%s[%d]: ERROR in state %d, CONFLICT BACKTRACKING to state %d, %d tokens\n",
                            YYDEBUGSTR, yydepth, yystate, yyps->save->state,
                    (int)(yylvp - yylvals - yyps->save->lexeme));
#endif
        /* Memorize most forward-looking error state in case it's really an error. */
        if (yyerrctx == NULL || yyerrctx->lexeme < yylvp - yylvals)
        {
            /* Free old saved error context state */
            if (yyerrctx) yyFreeState(yyerrctx);
            /* Create and fill out new saved error context state */
            yyerrctx                 = yyNewState((unsigned)(yystack.s_mark - yystack.s_base + 1));
            if (yyerrctx == NULL) goto yyenomem;
            yyerrctx->save           = yyps->save;
            yyerrctx->state          = yystate;
            yyerrctx->errflag        = yyerrflag;
            yyerrctx->yystack.s_mark = yyerrctx->yystack.s_base + (yystack.s_mark - yystack.s_base);
            memcpy (yyerrctx->yystack.s_base, yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            yyerrctx->yystack.l_mark = yyerrctx->yystack.l_base + (yystack.l_mark - yystack.l_base);
            memcpy (yyerrctx->yystack.l_base, yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yyerrctx->yystack.p_mark = yyerrctx->yystack.p_base + (yystack.p_mark - yystack.p_base);
            memcpy (yyerrctx->yystack.p_base, yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            yyerrctx->lexeme         = (int) (yylvp - yylvals);
        }
        yylvp          = yylvals   + save->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        yylpp          = yylpsns   + save->lexeme;
#endif
        yylexp         = yylexemes + save->lexeme;
        yychar         = YYEMPTY;
        yystack.s_mark = yystack.s_base + (save->yystack.s_mark - save->yystack.s_base);
        memcpy (yystack.s_base, save->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
        yystack.l_mark = yystack.l_base + (save->yystack.l_mark - save->yystack.l_base);
        memcpy (yystack.l_base, save->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        yystack.p_mark = yystack.p_base + (save->yystack.p_mark - save->yystack.p_base);
        memcpy (yystack.p_base, save->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
        ctry           = ++save->ctry;
        yystate        = save->state;
        /* We tried shift, try reduce now */
        if ((yyn = yyctable[ctry]) >= 0) goto yyreduce;
        yyps->save     = save->save;
        save->save     = NULL;
        yyFreeState(save);

        /* Nothing left on the stack -- error */
        if (!yyps->save)
        {
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%sdebug[%d,trial]: trial parse FAILED, entering ERROR mode\n",
                                YYPREFIX, yydepth);
#endif
            /* Restore state as it was in the most forward-advanced error */
            yylvp          = yylvals   + yyerrctx->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylpp          = yylpsns   + yyerrctx->lexeme;
#endif
            yylexp         = yylexemes + yyerrctx->lexeme;
            yychar         = yylexp[-1];
            yylval         = yylvp[-1];
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylloc         = yylpp[-1];
#endif
            yystack.s_mark = yystack.s_base + (yyerrctx->yystack.s_mark - yyerrctx->yystack.s_base);
            memcpy (yystack.s_base, yyerrctx->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            yystack.l_mark = yystack.l_base + (yyerrctx->yystack.l_mark - yyerrctx->yystack.l_base);
            memcpy (yystack.l_base, yyerrctx->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yystack.p_mark = yystack.p_base + (yyerrctx->yystack.p_mark - yyerrctx->yystack.p_base);
            memcpy (yystack.p_base, yyerrctx->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            yystate        = yyerrctx->state;
            yyFreeState(yyerrctx);
            yyerrctx       = NULL;
        }
        yynewerrflag = 1;
    }
    if (yynewerrflag == 0) goto yyinrecovery;
#endif /* YYBTYACC */

    YYERROR_CALL("syntax error");
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yyerror_loc_range[1] = yylloc; /* lookahead position is error start position */
#endif

#if !YYBTYACC
    goto yyerrlab; /* redundant goto avoids 'unused label' warning */
yyerrlab:
#endif
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if (((yyn = yysindex[*yystack.s_mark]) != 0) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    fprintf(stderr, "%s[%d]: state %d, error recovery shifting to state %d\n",
                                    YYDEBUGSTR, yydepth, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                /* lookahead position is error end position */
                yyerror_loc_range[2] = yylloc;
                YYLLOC_DEFAULT(yyloc, yyerror_loc_range, 2); /* position of error span */
                *++yystack.p_mark = yyloc;
#endif
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    fprintf(stderr, "%s[%d]: error recovery discarding state %d\n",
                                    YYDEBUGSTR, yydepth, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                /* the current TOS position is the error start position */
                yyerror_loc_range[1] = *yystack.p_mark;
#endif
#if defined(YYDESTRUCT_CALL)
#if YYBTYACC
                if (!yytrial)
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    YYDESTRUCT_CALL("error: discarding state",
                                    yystos[*yystack.s_mark], yystack.l_mark, yystack.p_mark);
#else
                    YYDESTRUCT_CALL("error: discarding state",
                                    yystos[*yystack.s_mark], yystack.l_mark);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#endif /* defined(YYDESTRUCT_CALL) */
                --yystack.s_mark;
                --yystack.l_mark;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                --yystack.p_mark;
#endif
            }
        }
    }
    else
    {
        if (yychar == YYEOF) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            fprintf(stderr, "%s[%d]: state %d, error recovery discarding token %d (%s)\n",
                            YYDEBUGSTR, yydepth, yystate, yychar, yys);
        }
#endif
#if defined(YYDESTRUCT_CALL)
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            YYDESTRUCT_CALL("error: discarding token", yychar, &yylval, &yylloc);
#else
            YYDESTRUCT_CALL("error: discarding token", yychar, &yylval);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#endif /* defined(YYDESTRUCT_CALL) */
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
    yym = yylen[yyn];
#if YYDEBUG
    if (yydebug)
    {
        fprintf(stderr, "%s[%d]: state %d, reducing by rule %d (%s)",
                        YYDEBUGSTR, yydepth, yystate, yyn, yyrule[yyn]);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
            if (yym > 0)
            {
                int i;
                fputc('<', stderr);
                for (i = yym; i > 0; i--)
                {
                    if (i != yym) fputs(", ", stderr);
                    fputs(YYSTYPE_TOSTRING(yystos[yystack.s_mark[1-i]],
                                           yystack.l_mark[1-i]), stderr);
                }
                fputc('>', stderr);
            }
#endif
        fputc('\n', stderr);
    }
#endif
    if (yym > 0)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)

    /* Perform position reduction */
    memset(&yyloc, 0, sizeof(yyloc));
#if YYBTYACC
    if (!yytrial)
#endif /* YYBTYACC */
    {
        YYLLOC_DEFAULT(yyloc, &yystack.p_mark[-yym], yym);
        /* just in case YYERROR is invoked within the action, save
           the start of the rhs as the error start position */
        yyerror_loc_range[1] = yystack.p_mark[1-yym];
    }
#endif

    switch (yyn)
    {
case 1:
#line 260 "nsgenbind-parser.y"
	{
                *genbind_ast = yystack.l_mark[0].node;
        }
#line 1476 "nsgenbind-parser.c"
break;
case 3:
#line 269 "nsgenbind-parser.y"
	{
                yyval.node = *genbind_ast = genbind_node_prepend(yystack.l_mark[0].node, yystack.l_mark[-1].node);
        }
#line 1483 "nsgenbind-parser.c"
break;
case 4:
#line 274 "nsgenbind-parser.y"
	{
                fprintf(stderr, "%s\n", errtxt);
                free(errtxt);
                YYABORT ;
        }
#line 1492 "nsgenbind-parser.c"
break;
case 8:
#line 291 "nsgenbind-parser.y"
	{
                yyval.node = genbind_new_node(GENBIND_NODE_TYPE_BINDING,
                                      NULL,
                                      genbind_new_node(GENBIND_NODE_TYPE_NAME,
                                                       yystack.l_mark[-2].node, yystack.l_mark[-4].text));
        }
#line 1502 "nsgenbind-parser.c"
break;
case 10:
#line 303 "nsgenbind-parser.y"
	{
                yyval.node = genbind_node_link(yystack.l_mark[0].node, yystack.l_mark[-1].node);
        }
#line 1509 "nsgenbind-parser.c"
break;
case 12:
#line 312 "nsgenbind-parser.y"
	{
                yyval.node = genbind_new_node(GENBIND_NODE_TYPE_METHOD,
                        NULL,
                        genbind_new_number_node(GENBIND_NODE_TYPE_METHOD_TYPE,
                                genbind_new_node(GENBIND_NODE_TYPE_CDATA,
                                                 NULL,
                                                 yystack.l_mark[-1].text),
                                yystack.l_mark[-2].value));
        }
#line 1522 "nsgenbind-parser.c"
break;
case 13:
#line 326 "nsgenbind-parser.y"
	{
                yyval.node = genbind_new_node(GENBIND_NODE_TYPE_WEBIDL, NULL, yystack.l_mark[-1].text);
        }
#line 1529 "nsgenbind-parser.c"
break;
case 14:
#line 336 "nsgenbind-parser.y"
	{
                yyval.node = genbind_new_node(GENBIND_NODE_TYPE_NAME,
                                      genbind_new_node(GENBIND_NODE_TYPE_NAME,
                                                       NULL,
                                                       strdup("unsigned")),
                                      yystack.l_mark[0].text);
        }
#line 1540 "nsgenbind-parser.c"
break;
case 15:
#line 345 "nsgenbind-parser.y"
	{
                yyval.node = genbind_new_node(GENBIND_NODE_TYPE_NAME,
                                      genbind_new_node(GENBIND_NODE_TYPE_NAME,
                                                       NULL,
                                                       strdup("struct")),
                                      yystack.l_mark[0].text);
        }
#line 1551 "nsgenbind-parser.c"
break;
case 16:
#line 354 "nsgenbind-parser.y"
	{
                yyval.node = genbind_new_node(GENBIND_NODE_TYPE_NAME,
                                      genbind_new_node(GENBIND_NODE_TYPE_NAME,
                                                       NULL,
                                                       strdup("union")),
                                      yystack.l_mark[0].text);
        }
#line 1562 "nsgenbind-parser.c"
break;
case 17:
#line 363 "nsgenbind-parser.y"
	{
                yyval.node = genbind_new_node(GENBIND_NODE_TYPE_NAME, NULL, yystack.l_mark[0].text);
        }
#line 1569 "nsgenbind-parser.c"
break;
case 19:
#line 372 "nsgenbind-parser.y"
	{
                yyval.node = genbind_new_node(GENBIND_NODE_TYPE_NAME, yystack.l_mark[-1].node, strdup("*"));
        }
#line 1576 "nsgenbind-parser.c"
break;
case 20:
#line 379 "nsgenbind-parser.y"
	{
                yyval.node = genbind_new_node(GENBIND_NODE_TYPE_IDENT, yystack.l_mark[-1].node, yystack.l_mark[0].text);
        }
#line 1583 "nsgenbind-parser.c"
break;
case 21:
#line 384 "nsgenbind-parser.y"
	{
                yyval.node = genbind_new_node(GENBIND_NODE_TYPE_IDENT,
                        genbind_new_node(GENBIND_NODE_TYPE_IDENT, yystack.l_mark[-3].node, yystack.l_mark[-2].text), yystack.l_mark[0].text);
        }
#line 1591 "nsgenbind-parser.c"
break;
case 23:
#line 395 "nsgenbind-parser.y"
	{
          yyval.text = genbind_strapp(yystack.l_mark[-1].text, yystack.l_mark[0].text);
        }
#line 1598 "nsgenbind-parser.c"
break;
case 24:
#line 403 "nsgenbind-parser.y"
	{
                yyval.node = genbind_new_node(GENBIND_NODE_TYPE_PARAMETER, NULL, yystack.l_mark[0].node);
        }
#line 1605 "nsgenbind-parser.c"
break;
case 25:
#line 408 "nsgenbind-parser.y"
	{
                yyval.node = genbind_node_prepend(yystack.l_mark[-2].node,
                                          genbind_new_node(
                                                  GENBIND_NODE_TYPE_PARAMETER,
                                                  NULL,
                                                  yystack.l_mark[0].node));
        }
#line 1616 "nsgenbind-parser.c"
break;
case 26:
#line 419 "nsgenbind-parser.y"
	{
                yyval.node = add_method(genbind_ast, yystack.l_mark[-2].value, yystack.l_mark[-1].node, yystack.l_mark[0].text, yystack.p_mark[-2].first_line, yystack.p_mark[-2].filename);
        }
#line 1623 "nsgenbind-parser.c"
break;
case 27:
#line 424 "nsgenbind-parser.y"
	{
                yyval.node = add_method(genbind_ast, yystack.l_mark[-2].value, yystack.l_mark[-1].node, NULL, yystack.p_mark[-2].first_line, yystack.p_mark[-2].filename);
        }
#line 1630 "nsgenbind-parser.c"
break;
case 28:
#line 431 "nsgenbind-parser.y"
	{
                yyval.value = GENBIND_METHOD_TYPE_INIT;
        }
#line 1637 "nsgenbind-parser.c"
break;
case 29:
#line 436 "nsgenbind-parser.y"
	{
                yyval.value = GENBIND_METHOD_TYPE_FINI;
        }
#line 1644 "nsgenbind-parser.c"
break;
case 30:
#line 441 "nsgenbind-parser.y"
	{
                yyval.value = GENBIND_METHOD_TYPE_METHOD;
        }
#line 1651 "nsgenbind-parser.c"
break;
case 31:
#line 446 "nsgenbind-parser.y"
	{
                yyval.value = GENBIND_METHOD_TYPE_GETTER;
        }
#line 1658 "nsgenbind-parser.c"
break;
case 32:
#line 451 "nsgenbind-parser.y"
	{
                yyval.value = GENBIND_METHOD_TYPE_SETTER;
        }
#line 1665 "nsgenbind-parser.c"
break;
case 33:
#line 456 "nsgenbind-parser.y"
	{
                yyval.value = GENBIND_METHOD_TYPE_PROTOTYPE;
        }
#line 1672 "nsgenbind-parser.c"
break;
case 35:
#line 465 "nsgenbind-parser.y"
	{
                yyval.value = GENBIND_METHOD_TYPE_PREFACE;
        }
#line 1679 "nsgenbind-parser.c"
break;
case 36:
#line 470 "nsgenbind-parser.y"
	{
                yyval.value = GENBIND_METHOD_TYPE_PROLOGUE;
        }
#line 1686 "nsgenbind-parser.c"
break;
case 37:
#line 475 "nsgenbind-parser.y"
	{
                yyval.value = GENBIND_METHOD_TYPE_EPILOGUE;
        }
#line 1693 "nsgenbind-parser.c"
break;
case 38:
#line 480 "nsgenbind-parser.y"
	{
                yyval.value = GENBIND_METHOD_TYPE_POSTFACE;
        }
#line 1700 "nsgenbind-parser.c"
break;
case 39:
#line 487 "nsgenbind-parser.y"
	{
                yyval.text = yystack.l_mark[0].text;
        }
#line 1707 "nsgenbind-parser.c"
break;
case 40:
#line 492 "nsgenbind-parser.y"
	{
                yyval.text = strdup("method");
        }
#line 1714 "nsgenbind-parser.c"
break;
case 41:
#line 497 "nsgenbind-parser.y"
	{
                yyval.text = strdup("type");
        }
#line 1721 "nsgenbind-parser.c"
break;
case 42:
#line 505 "nsgenbind-parser.y"
	{
                yyval.node = genbind_new_node(GENBIND_NODE_TYPE_CLASS,
                                      genbind_new_node(GENBIND_NODE_TYPE_IDENT,
                                                       yystack.l_mark[-1].node,
                                                       yystack.l_mark[-3].text),
                                      genbind_new_node(GENBIND_NODE_TYPE_IDENT,
                                                       NULL,
                                                       yystack.l_mark[-5].text));
        }
#line 1734 "nsgenbind-parser.c"
break;
case 43:
#line 516 "nsgenbind-parser.y"
	{
                yyval.node = genbind_new_node(GENBIND_NODE_TYPE_CLASS,
                                      genbind_new_node(GENBIND_NODE_TYPE_IDENT,
                                                       NULL,
                                                       yystack.l_mark[-2].text),
                                      genbind_new_node(GENBIND_NODE_TYPE_IDENT,
                                                       NULL,
                                                       yystack.l_mark[-4].text));
        }
#line 1747 "nsgenbind-parser.c"
break;
case 44:
#line 527 "nsgenbind-parser.y"
	{
                yyval.node = genbind_new_node(GENBIND_NODE_TYPE_CLASS,
                                      yystack.l_mark[-1].node,
                                      genbind_new_node(GENBIND_NODE_TYPE_IDENT,
                                                       NULL,
                                                       yystack.l_mark[-3].text));
        }
#line 1758 "nsgenbind-parser.c"
break;
case 45:
#line 536 "nsgenbind-parser.y"
	{
                yyval.node = genbind_new_node(GENBIND_NODE_TYPE_CLASS, NULL,
                                      genbind_new_node(GENBIND_NODE_TYPE_IDENT,
                                                       NULL,
                                                       yystack.l_mark[-2].text));
        }
#line 1768 "nsgenbind-parser.c"
break;
case 46:
#line 547 "nsgenbind-parser.y"
	{
                yyval.node = genbind_new_node(GENBIND_NODE_TYPE_CLASS, NULL,
                        genbind_new_node(GENBIND_NODE_TYPE_IDENT, yystack.l_mark[-2].node, yystack.l_mark[-4].text));
        }
#line 1776 "nsgenbind-parser.c"
break;
case 47:
#line 555 "nsgenbind-parser.y"
	{
                yyval.node = NULL;
        }
#line 1783 "nsgenbind-parser.c"
break;
case 48:
#line 560 "nsgenbind-parser.y"
	{
                yyval.node = genbind_node_prepend(yystack.l_mark[0].node, yystack.l_mark[-1].node);
        }
#line 1790 "nsgenbind-parser.c"
break;
case 49:
#line 567 "nsgenbind-parser.y"
	{
                yyval.node = genbind_new_node(GENBIND_NODE_TYPE_PRIVATE, NULL, yystack.l_mark[-1].node);
        }
#line 1797 "nsgenbind-parser.c"
break;
case 50:
#line 572 "nsgenbind-parser.y"
	{
                yyval.node = genbind_new_node(GENBIND_NODE_TYPE_INTERNAL, NULL, yystack.l_mark[-1].node);
        }
#line 1804 "nsgenbind-parser.c"
break;
case 51:
#line 577 "nsgenbind-parser.y"
	{
                yyval.node = genbind_new_node(GENBIND_NODE_TYPE_PROPERTY, NULL,
                        genbind_new_number_node(GENBIND_NODE_TYPE_MODIFIER,
                                genbind_new_node(GENBIND_NODE_TYPE_IDENT,
                                                 NULL,
                                                 yystack.l_mark[-1].text),
                                         yystack.l_mark[-2].value));
        }
#line 1816 "nsgenbind-parser.c"
break;
case 52:
#line 587 "nsgenbind-parser.y"
	{
                yyval.node = genbind_new_node(GENBIND_NODE_TYPE_FLAGS, NULL, yystack.l_mark[-1].node);
        }
#line 1823 "nsgenbind-parser.c"
break;
case 53:
#line 592 "nsgenbind-parser.y"
	{
                yyval.node = genbind_new_node(GENBIND_NODE_TYPE_METHOD,
                        NULL,
                        genbind_new_number_node(GENBIND_NODE_TYPE_METHOD_TYPE,
                                genbind_new_node(GENBIND_NODE_TYPE_CDATA,
                                                 NULL,
                                                 yystack.l_mark[-1].text),
                                yystack.l_mark[-2].value));
        }
#line 1836 "nsgenbind-parser.c"
break;
case 54:
#line 605 "nsgenbind-parser.y"
	{
                yyval.node = genbind_new_node(GENBIND_NODE_TYPE_IDENT, NULL, yystack.l_mark[0].text);
        }
#line 1843 "nsgenbind-parser.c"
break;
case 55:
#line 610 "nsgenbind-parser.y"
	{
                yyval.node = genbind_new_node(GENBIND_NODE_TYPE_IDENT, yystack.l_mark[-2].node, yystack.l_mark[0].text);
        }
#line 1850 "nsgenbind-parser.c"
break;
case 56:
#line 617 "nsgenbind-parser.y"
	{
            yyval.value = GENBIND_TYPE_NONE;
        }
#line 1857 "nsgenbind-parser.c"
break;
case 57:
#line 622 "nsgenbind-parser.y"
	{
            yyval.value |= yystack.l_mark[0].value;
        }
#line 1864 "nsgenbind-parser.c"
break;
case 58:
#line 629 "nsgenbind-parser.y"
	{
            yyval.value = GENBIND_TYPE_TYPE;
        }
#line 1871 "nsgenbind-parser.c"
break;
case 59:
#line 634 "nsgenbind-parser.y"
	{
            yyval.value = GENBIND_TYPE_UNSHARED;            
        }
#line 1878 "nsgenbind-parser.c"
break;
case 60:
#line 639 "nsgenbind-parser.y"
	{
            yyval.value = GENBIND_TYPE_NONE;            
        }
#line 1885 "nsgenbind-parser.c"
break;
#line 1887 "nsgenbind-parser.c"
    default:
        break;
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark -= yym;
#endif
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
        {
            fprintf(stderr, "%s[%d]: after reduction, ", YYDEBUGSTR, yydepth);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
            if (!yytrial)
#endif /* YYBTYACC */
                fprintf(stderr, "result is <%s>, ", YYSTYPE_TOSTRING(yystos[YYFINAL], yyval));
#endif
            fprintf(stderr, "shifting from state 0 to final state %d\n", YYFINAL);
        }
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        *++yystack.p_mark = yyloc;
#endif
        if (yychar < 0)
        {
#if YYBTYACC
            do {
            if (yylvp < yylve)
            {
                /* we're currently re-reading tokens */
                yylval = *yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylloc = *yylpp++;
#endif
                yychar = *yylexp++;
                break;
            }
            if (yyps->save)
            {
                /* in trial mode; save scanner results for future parse attempts */
                if (yylvp == yylvlim)
                {   /* Enlarge lexical value queue */
                    size_t p = (size_t) (yylvp - yylvals);
                    size_t s = (size_t) (yylvlim - yylvals);

                    s += YYLVQUEUEGROWTH;
                    if ((yylexemes = (YYINT *)realloc(yylexemes, s * sizeof(YYINT))) == NULL)
                        goto yyenomem;
                    if ((yylvals   = (YYSTYPE *)realloc(yylvals, s * sizeof(YYSTYPE))) == NULL)
                        goto yyenomem;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    if ((yylpsns   = (YYLTYPE *)realloc(yylpsns, s * sizeof(YYLTYPE))) == NULL)
                        goto yyenomem;
#endif
                    yylvp   = yylve = yylvals + p;
                    yylvlim = yylvals + s;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpp   = yylpe = yylpsns + p;
                    yylplim = yylpsns + s;
#endif
                    yylexp  = yylexemes + p;
                }
                *yylexp = (YYINT) YYLEX;
                *yylvp++ = yylval;
                yylve++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                *yylpp++ = yylloc;
                yylpe++;
#endif
                yychar = *yylexp++;
                break;
            }
            /* normal operation, no conflict encountered */
#endif /* YYBTYACC */
            yychar = YYLEX;
#if YYBTYACC
            } while (0);
#endif /* YYBTYACC */
            if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
            if (yydebug)
            {
                if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
                fprintf(stderr, "%s[%d]: state %d, reading token %d (%s)\n",
                                YYDEBUGSTR, yydepth, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == YYEOF) goto yyaccept;
        goto yyloop;
    }
    if (((yyn = yygindex[yym]) != 0) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
    {
        fprintf(stderr, "%s[%d]: after reduction, ", YYDEBUGSTR, yydepth);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
            fprintf(stderr, "result is <%s>, ", YYSTYPE_TOSTRING(yystos[yystate], yyval));
#endif
        fprintf(stderr, "shifting from state %d to state %d\n", *yystack.s_mark, yystate);
    }
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    *++yystack.s_mark = (YYINT) yystate;
    *++yystack.l_mark = yyval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    *++yystack.p_mark = yyloc;
#endif
    goto yyloop;
#if YYBTYACC

    /* Reduction declares that this path is valid. Set yypath and do a full parse */
yyvalid:
    if (yypath) YYABORT;
    while (yyps->save)
    {
        YYParseState *save = yyps->save;
        yyps->save = save->save;
        save->save = yypath;
        yypath = save;
    }
#if YYDEBUG
    if (yydebug)
        fprintf(stderr, "%s[%d]: state %d, CONFLICT trial successful, backtracking to state %d, %d tokens\n",
                        YYDEBUGSTR, yydepth, yystate, yypath->state, (int)(yylvp - yylvals - yypath->lexeme));
#endif
    if (yyerrctx)
    {
        yyFreeState(yyerrctx);
        yyerrctx = NULL;
    }
    yylvp          = yylvals + yypath->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yylpp          = yylpsns + yypath->lexeme;
#endif
    yylexp         = yylexemes + yypath->lexeme;
    yychar         = YYEMPTY;
    yystack.s_mark = yystack.s_base + (yypath->yystack.s_mark - yypath->yystack.s_base);
    memcpy (yystack.s_base, yypath->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
    yystack.l_mark = yystack.l_base + (yypath->yystack.l_mark - yypath->yystack.l_base);
    memcpy (yystack.l_base, yypath->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark = yystack.p_base + (yypath->yystack.p_mark - yypath->yystack.p_base);
    memcpy (yystack.p_base, yypath->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
    yystate        = yypath->state;
    goto yyloop;
#endif /* YYBTYACC */

yyoverflow:
    YYERROR_CALL("yacc stack overflow");
#if YYBTYACC
    goto yyabort_nomem;
yyenomem:
    YYERROR_CALL("memory exhausted");
yyabort_nomem:
#endif /* YYBTYACC */
    yyresult = 2;
    goto yyreturn;

yyabort:
    yyresult = 1;
    goto yyreturn;

yyaccept:
#if YYBTYACC
    if (yyps->save) goto yyvalid;
#endif /* YYBTYACC */
    yyresult = 0;

yyreturn:
#if defined(YYDESTRUCT_CALL)
    if (yychar != YYEOF && yychar != YYEMPTY)
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        YYDESTRUCT_CALL("cleanup: discarding token", yychar, &yylval, &yylloc);
#else
        YYDESTRUCT_CALL("cleanup: discarding token", yychar, &yylval);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */

    {
        YYSTYPE *pv;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        YYLTYPE *pp;

        for (pv = yystack.l_base, pp = yystack.p_base; pv <= yystack.l_mark; ++pv, ++pp)
             YYDESTRUCT_CALL("cleanup: discarding state",
                             yystos[*(yystack.s_base + (pv - yystack.l_base))], pv, pp);
#else
        for (pv = yystack.l_base; pv <= yystack.l_mark; ++pv)
             YYDESTRUCT_CALL("cleanup: discarding state",
                             yystos[*(yystack.s_base + (pv - yystack.l_base))], pv);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
    }
#endif /* defined(YYDESTRUCT_CALL) */

#if YYBTYACC
    if (yyerrctx)
    {
        yyFreeState(yyerrctx);
        yyerrctx = NULL;
    }
    while (yyps)
    {
        YYParseState *save = yyps;
        yyps = save->save;
        save->save = NULL;
        yyFreeState(save);
    }
    while (yypath)
    {
        YYParseState *save = yypath;
        yypath = save->save;
        save->save = NULL;
        yyFreeState(save);
    }
#endif /* YYBTYACC */
    yyfreestack(&yystack);
    return (yyresult);
}
