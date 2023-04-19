#ifndef _webidl__defines_h_
#define _webidl__defines_h_

#define TOK_ANY 257
#define TOK_ATTRIBUTE 258
#define TOK_BOOLEAN 259
#define TOK_BYTE 260
#define TOK_CALLBACK 261
#define TOK_CONST 262
#define TOK_CONSTRUCTOR 263
#define TOK_CREATOR 264
#define TOK_DATE 265
#define TOK_DELETER 266
#define TOK_DICTIONARY 267
#define TOK_DOUBLE 268
#define TOK_ELLIPSIS 269
#define TOK_ENUM 270
#define TOK_EOL 271
#define TOK_EXCEPTION 272
#define TOK_FALSE 273
#define TOK_FLOAT 274
#define TOK_GETRAISES 275
#define TOK_GETTER 276
#define TOK_IMPLEMENTS 277
#define TOK_IN 278
#define TOK_INFINITY 279
#define TOK_INHERIT 280
#define TOK_INTERFACE 281
#define TOK_ITERABLE 282
#define TOK_LEGACYCALLER 283
#define TOK_LEGACYITERABLE 284
#define TOK_LONG 285
#define TOK_MODULE 286
#define TOK_NAN 287
#define TOK_NATIVE 288
#define TOK_NAMEDCONSTRUCTOR 289
#define TOK_NULL_LITERAL 290
#define TOK_OBJECT 291
#define TOK_OCTET 292
#define TOK_OMITTABLE 293
#define TOK_OPTIONAL 294
#define TOK_OR 295
#define TOK_PARTIAL 296
#define TOK_PROMISE 297
#define TOK_RAISES 298
#define TOK_READONLY 299
#define TOK_REQUIRED 300
#define TOK_SETRAISES 301
#define TOK_SETTER 302
#define TOK_SEQUENCE 303
#define TOK_SHORT 304
#define TOK_STATIC 305
#define TOK_STRING 306
#define TOK_STRINGIFIER 307
#define TOK_TRUE 308
#define TOK_TYPEDEF 309
#define TOK_UNRESTRICTED 310
#define TOK_UNSIGNED 311
#define TOK_VOID 312
#define TOK_POUND_SIGN 313
#define TOK_IDENTIFIER 314
#define TOK_INT_LITERAL 315
#define TOK_FLOAT_LITERAL 316
#define TOK_STRING_LITERAL 317
#define TOK_OTHER_LITERAL 318
#define TOK_JAVADOC 319
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union YYSTYPE
{
    int attr;
    long value;
    bool isit;
    char* text;
    struct webidl_node *node;
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
extern YYLTYPE webidl_lloc;

#endif /* _webidl__defines_h_ */
