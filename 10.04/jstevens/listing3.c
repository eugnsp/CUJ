/******************************************************************************
* Module    :   Lexical Analyzer --- Header file containing token value
*               enumeration, type definitions and function prototypes for
*               the lexical analyzer functions.
*
*               Copyright (C) 1990 John W. M. Stevens, All Rights Reserved
*
* Author    :   John W. M. Stevens
******************************************************************************/

#if     ! defined( LEXICAL_ANALYZER )
#define     LEXICAL_ANALYZER        1

#include    <dos.h>

#define     TRUE        1
#define     FALSE       0
#define     ERROR       -1
#define     OK          0

#define     PATH_SZ     65
typedef     unsigned    int     UINT;
typedef     unsigned    char    UCHAR;
typedef     char        PATH[PATH_SZ];

/* Definition of structure filled in and returned by lex.   */
typedef struct  {
    char    str[257];
    long    no;
    struct  time    ftime;
    struct  date    fdate;
} TOKEN;

/* Token defines.   */
enum    tkn_en  {
    STRING = 128,
    NUMBER,     TIME,       DATE,

    L_PAREN,     R_PAREN,     COMMA,       F_SLASH,     ACTION,
    AFTER,       AND,         ARCHIVE,     ATTRIBUTES,  BEFORE,
    DIRECTORY_T, EXEC,        FILES,       HIDDEN,      LABEL,
    MODIFIED,    NAME,        NOT,         OR,          PRINT,
    READONLY,    RECURS,      SEARCH,      SELECT,      SYSTEM,
    L_BRACE,     BAR,         R_BRACE
};
typedef enum    tkn_en  TKNS;

/* Function prototypes. */
extern  TKNS    Lex(TOKEN *);
extern  void    OpenPrg(char *);
extern  void    ParsErr(char *);

#endif
/* End of File */
