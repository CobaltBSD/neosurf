#ifndef _nsgenbind__defines_h_
#define _nsgenbind__defines_h_

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
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union YYSTYPE
{
        char *text;
        struct genbind_node *node;
        long value;
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
extern YYLTYPE nsgenbind_lloc;

#endif /* _nsgenbind__defines_h_ */
