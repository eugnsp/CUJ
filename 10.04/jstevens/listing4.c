/******************************************************************************
* Module    :   Lexical Analyzer --- Process the input text file into tokens
*                   that the parser can understand.
*
*               Copyright (C) 1990 John W. M. Stevens, All Rights Reserved
*
* Routines  :   Lex     - Return the next token from the file.
*               OpenPrg - Open the source file.
*               ParsErr - Report a parsing error.
*
* Author    :   John W. M. Stevens
******************************************************************************/

#include    <stdio.h>
#include    <stdlib.h>
#include    <ctype.h>
#include    <string.h>

#include    "lex.h"

/* Structure of trie branch.    */
typedef struct  key_st  {
    char    c;
    TKNS    token;
    struct  key_st  *child;
} NODE;

/* Constants local to this file.    */
#define     MAX_STR     256
#define     NOT_FND     -2

/* Object Data. */
static  char    word[MAX_STR + 1];  /* Last string analyzed.                */
static  char    PrvWd[MAX_STR + 1]; /* Previous word.                       */
static  int     LnNo = 0;           /* The current line number in the file. */
static  FILE    *PrgFl;             /* File pointer.                        */

/* Trie data structure containing all the keywords and punctuation marks for
*   the language being tokenized.
*/
static
NODE    T5[2] = {
    {   ' ',             2, NULL    },
    {   'n',        ACTION, NULL    }
};

static
NODE    T4[2] = {
    {   ' ',             2, NULL    },
    {   'o',             0, T5  }
};

static
NODE    T3[2] = {
    {   ' ',             2, NULL    },
    {   'i',             0, T4  }
};

static

NODE    T2[2] = {
    {   ' ',             2, NULL    },
    {   't',             0, T3  }
};

static
NODE    T8[2] = {
    {   ' ',             2, NULL    },
    {   'r',         AFTER, NULL    }
};

static
NODE    T7[2] = {
    {   ' ',             2, NULL    },
    {   'e',             0, T8  }
};

static
NODE    T6[2] = {
    {   ' ',             2, NULL    },
    {   't',             0, T7  }
};

static
NODE    T9[2] = {
    {   ' ',             2, NULL    },
    {   'd',           AND, NULL    }
};

static
NODE    Te[2] = {
    {   ' ',             2, NULL    },
    {   'e',       ARCHIVE, NULL    }
};

static
NODE    Td[2] = {
    {   ' ',             2, NULL    },
    {   'v',             0, Te  }
};

static
NODE    Tc[2] = {
    {   ' ',             2, NULL    },
    {   'i',             0, Td  }
};

static
NODE    Tb[2] = {
    {   ' ',             2, NULL    },
    {   'h',             0, Tc  }
};

static
NODE    Ta[2] = {
    {   ' ',             2, NULL    },
    {   'c',             0, Tb  }
};

static

NODE    T16[2] = {
    {   ' ',             2, NULL    },
    {   's',    ATTRIBUTES, NULL    }
};

static
NODE    T15[2] = {
    {   ' ',             2, NULL    },
    {   'e',             0, T16 }
};

static
NODE    T14[2] = {
    {   ' ',             2, NULL    },
    {   't',             0, T15 }
};

static
NODE    T13[2] = {
    {   ' ',             2, NULL    },
    {   'u',             0, T14 }
};

static
NODE    T12[2] = {
    {   ' ',             2, NULL    },
    {   'b',             0, T13 }
};

static
NODE    T11[2] = {
    {   ' ',             2, NULL    },
    {   'i',             0, T12 }
};

static
NODE    T10[2] = {
    {   ' ',             2, NULL    },
    {   'r',             0, T11 }
};

static
NODE    Tf[2] = {
    {   ' ',             2, NULL    },
    {   't',             0, T10 }
};

static
NODE    T1[6] = {
    {   ' ',             6, NULL    },
    {   'c',             0, T2  },
    {   'f',             0, T6  },
    {   'n',             0, T9  },
    {   'r',             0, Ta  },
    {   't',             0, Tf  }
};

static
NODE    T1b[2] = {
    {   ' ',             2, NULL    },

    {   'e',        BEFORE, NULL    }
};

static
NODE    T1a[2] = {
    {   ' ',             2, NULL    },
    {   'r',             0, T1b }
};

static
NODE    T19[2] = {
    {   ' ',             2, NULL    },
    {   'o',             0, T1a }
};

static
NODE    T18[2] = {
    {   ' ',             2, NULL    },
    {   'f',             0, T19 }
};

static
NODE    T17[2] = {
    {   ' ',             2, NULL    },
    {   'e',             0, T18 }
};

static
NODE    T23[2] = {
    {   ' ',             2, NULL    },
    {   'y',    DIRECTORY_T,    NULL    }
};

static
NODE    T22[2] = {
    {   ' ',             2, NULL    },
    {   'r',             0, T23 }
};

static
NODE    T21[2] = {
    {   ' ',             2, NULL    },
    {   'o',             0, T22 }
};

static
NODE    T20[2] = {
    {   ' ',             2, NULL    },
    {   't',             0, T21 }
};

static
NODE    T1f[2] = {
    {   ' ',             2, NULL    },
    {   'c',             0, T20 }
};

static
NODE    T1e[2] = {
    {   ' ',             2, NULL    },

    {   'e',             0, T1f }
};

static
NODE    T1d[2] = {
    {   ' ',             2, NULL    },
    {   'r',             0, T1e }
};

static
NODE    T1c[2] = {
    {   ' ',             2, NULL    },
    {   'i',             0, T1d }
};

static
NODE    T26[2] = {
    {   ' ',             2, NULL    },
    {   'c',          EXEC, NULL    }
};

static
NODE    T25[2] = {
    {   ' ',             2, NULL    },
    {   'e',             0, T26 }
};

static
NODE    T24[2] = {
    {   ' ',             2, NULL    },
    {   'x',             0, T25 }
};

static
NODE    T2a[2] = {
    {   ' ',             2, NULL    },
    {   's',         FILES, NULL    }
};

static
NODE    T29[2] = {
    {   ' ',             2, NULL    },
    {   'e',             0, T2a }
};

static
NODE    T28[2] = {
    {   ' ',             2, NULL    },
    {   'l',             0, T29 }
};

static
NODE    T27[2] = {
    {   ' ',             2, NULL    },
    {   'i',             0, T28 }
};

static
NODE    T2f[2] = {
    {   ' ',             2, NULL    },

    {   'n',        HIDDEN, NULL    }
};

static
NODE    T2e[2] = {
    {   ' ',             2, NULL    },
    {   'e',             0, T2f }
};

static
NODE    T2d[2] = {
    {   ' ',             2, NULL    },
    {   'd',             0, T2e }
};

static
NODE    T2c[2] = {
    {   ' ',             2, NULL    },
    {   'd',             0, T2d }
};

static
NODE    T2b[2] = {
    {   ' ',             2, NULL    },
    {   'i',             0, T2c }
};

static
NODE    T33[2] = {
    {   ' ',             2, NULL    },
    {   'l',         LABEL, NULL    }
};

static
NODE    T32[2] = {
    {   ' ',             2, NULL    },
    {   'e',             0, T33 }
};

static
NODE    T31[2] = {
    {   ' ',             2, NULL    },
    {   'b',             0, T32 }
};

static
NODE    T30[2] = {
    {   ' ',             2, NULL    },
    {   'a',             0, T31 }
};

static
NODE    T3a[2] = {
    {   ' ',             2, NULL    },
    {   'd',      MODIFIED, NULL    }
};

static
NODE    T39[2] = {
    {   ' ',             2, NULL    },

    {   'e',             0, T3a }
};

static
NODE    T38[2] = {
    {   ' ',             2, NULL    },
    {   'i',             0, T39 }
};

static
NODE    T37[2] = {
    {   ' ',             2, NULL    },
    {   'f',             0, T38 }
};

static
NODE    T36[2] = {
    {   ' ',             2, NULL    },
    {   'i',             0, T37 }
};

static
NODE    T35[2] = {
    {   ' ',             2, NULL    },
    {   'd',             0, T36 }
};

static
NODE    T34[2] = {
    {   ' ',             2, NULL    },
    {   'o',             0, T35 }
};

static
NODE    T3d[2] = {
    {   ' ',             2, NULL    },
    {   'e',          NAME, NULL    }
};

static
NODE    T3c[2] = {
    {   ' ',             2, NULL    },
    {   'm',             0, T3d }
};

static
NODE    T3e[2] = {
    {   ' ',             2, NULL    },
    {   't',           NOT, NULL    }
};

static
NODE    T3b[3] = {
    {   ' ',             3, NULL    },
    {   'a',             0, T3c },
    {   'o',             0, T3e }
};

static
NODE    T3f[2] = {

    {   ' ',             2, NULL    },
    {   'r',            OR, NULL    }
};

static
NODE    T43[2] = {
    {   ' ',             2, NULL    },
    {   't',         PRINT, NULL    }
};

static
NODE    T42[2] = {
    {   ' ',             2, NULL    },
    {   'n',             0, T43 }
};

static
NODE    T41[2] = {
    {   ' ',             2, NULL    },
    {   'i',             0, T42 }
};

static
NODE    T40[2] = {
    {   ' ',             2, NULL    },
    {   'r',             0, T41 }
};

static
NODE    T4a[2] = {
    {   ' ',             2, NULL    },
    {   'y',      READONLY, NULL    }
};

static
NODE    T49[2] = {
    {   ' ',             2, NULL    },
    {   'l',             0, T4a }
};

static
NODE    T48[2] = {
    {   ' ',             2, NULL    },
    {   'n',             0, T49 }
};

static
NODE    T47[2] = {
    {   ' ',             2, NULL    },
    {   'o',             0, T48 }
};

static
NODE    T46[2] = {
    {   ' ',             2, NULL    },
    {   'd',             0, T47 }
};

static
NODE    T4d[2] = {

    {   ' ',             2, NULL    },
    {   's',        RECURS, NULL    }
};

static
NODE    T4c[2] = {
    {   ' ',             2, NULL    },
    {   'r',             0, T4d }
};

static
NODE    T4b[2] = {
    {   ' ',             2, NULL    },
    {   'u',             0, T4c }
};

static
NODE    T45[3] = {
    {   ' ',             3, NULL    },
    {   'a',             0, T46 },
    {   'c',             0, T4b }
};

static
NODE    T44[2] = {
    {   ' ',             2, NULL    },
    {   'e',             0, T45 }
};

static
NODE    T52[2] = {
    {   ' ',             2, NULL    },
    {   'h',        SEARCH, NULL    }
};

static
NODE    T51[2] = {
    {   ' ',             2, NULL    },
    {   'c',             0, T52 }
};

static
NODE    T50[2] = {
    {   ' ',             2, NULL    },
    {   'r',             0, T51 }
};

static
NODE    T55[2] = {
    {   ' ',             2, NULL    },
    {   't',        SELECT, NULL    }
};

static
NODE    T54[2] = {
    {   ' ',             2, NULL    },
    {   'c',             0, T55 }
};

static

NODE    T53[2] = {
    {   ' ',             2, NULL    },
    {   'e',             0, T54 }
};

static
NODE    T4f[3] = {
    {   ' ',             3, NULL    },
    {   'a',             0, T50 },
    {   'l',             0, T53 }
};

static
NODE    T59[2] = {
    {   ' ',             2, NULL    },
    {   'm',        SYSTEM, NULL    }
};

static
NODE    T58[2] = {
    {   ' ',             2, NULL    },
    {   'e',             0, T59 }
};

static
NODE    T57[2] = {
    {   ' ',             2, NULL    },
    {   't',             0, T58 }
};

static
NODE    T56[2] = {
    {   ' ',             2, NULL    },
    {   's',             0, T57 }
};

static
NODE    T4e[3] = {
    {   ' ',             3, NULL    },
    {   'e',             0, T4f },
    {   'y',             0, T56 }
};

static
NODE    T0[21] = {
    {   ' ',            21, NULL    },
    {   '(',       L_PAREN, NULL    },
    {   ')',       R_PAREN, NULL    },
    {   ',',         COMMA, NULL    },
    {   '/',       F_SLASH, NULL    },
    {   'a',             0, T1  },
    {   'b',             0, T17 },
    {   'd',             0, T1c },
    {   'e',             0, T24 },
    {   'f',             0, T27 },
    {   'h',             0, T2b },
    {   'l',             0, T30 },
    {   'm',             0, T34 },
    {   'n',             0, T3b },
    {   'o',             0, T3f },

    {   'p',             0, T40 },
    {   'r',             0, T44 },
    {   's',             0, T4e },
    {   '{',       L_BRACE, NULL    },
    {   '|',           BAR, NULL    },
    {   '}',       R_BRACE, NULL    }
};

/*-----------------------------------------------------------------------------
| Routine   :   OpenPrg() --- Open the ASCII text file that contains the
|                   back up program.
|
| Inputs    :   FileNm  - File name of source file.
-----------------------------------------------------------------------------*/

void    OpenPrg(char    *FileNm)
{
    /* Open the program script file.    */
    if ((PrgFl = fopen(FileNm, "rt")) == NULL)
    {
        fprintf(stderr, "OpenPrg (fopen) : Could not open file '%s' for "
            "reading.\n", FileNm);
        exit( -1 );
    }

    /* Initialize object variables. */
    *word = *PrvWd = '\0';
}

/*-----------------------------------------------------------------------------
| Routine   :   ParsErr() --- Report a parse error.
|
| Inputs    :   Err - Error string.
-----------------------------------------------------------------------------*/

void    ParsErr(char    *Err)
{
    /* Print line number and error message. */
    fprintf(stderr, "Error in Line: %d, %s.\n", LnNo + 1, Err);

    /* If there is a previous word, show it.    */
    if ( *word )
        fprintf(stderr, "\tOn or after word '%s'\n", word);
    exit( -1 );
}

/*-----------------------------------------------------------------------------
| Routine   :   TrieSrch() --- Search the trie for a key word.
|
| Inputs    :   Trie    - The trie level pointer.
|               ch      - The current character to search for.
|               WordPtr - The pointer to the current byte of the word buffer.
| Returns   :   Returns either a token value or
|                   NOT_FND - For key word not found.
|                   EOF     - For end of file.
-----------------------------------------------------------------------------*/

static
int     TrieSrch(NODE   *Trie,
                 int    ch,

                 char   *WordPtr)
{
    register    int     mid;        /* Mid point of array piece.            */
    register    TKNS    ret;        /* Return value of comparison.          */

    auto        int     lo;         /* Limits of current array piece for    */
    auto        int     hi;         /*      binary search.                  */

    /* Make sure that input is lower case.  */
    ch = tolower( ch );

    /* Search for a token.  */
    hi = Trie[0].token - 1;
    lo = 1;
    do
    {
        /* Find mid point of current array piece.   */
        mid = (lo + hi) >> 1;

        /* Do character comparison. */
        ret = ch - Trie[mid].c;

        /* Fix the array limits.    */
        if (ret <= 0)
            hi = mid - 1;
        if (ret >= 0)
            lo = mid + 1;

    }  while (hi >= lo);

    /* If the character matches one of the entries in this level and this
    *   entry has a child, recurse.  If a match is found but the matching
    *   entry has no child, return the token value associated with the
    *   match.  If the return value from the recursive call indicates that
    *   no match was found at a lower level, return the token value
    *   associated with the match at this level of the trie.
    */
    if (ret == 0)
    {
        /* Save the current character in the buffer for error reporting.    */
        *WordPtr++ = ch;

        /* Are we looking for more characters in this string?   */
        if ( Trie[mid].child )
        {
            /* Get the next character.  */
            if ((ch = fgetc( PrgFl )) == EOF)
                return( EOF );

            /* Search next level.   */
            if ((ret = TrieSrch(Trie[mid].child, ch, WordPtr)) == NOT_FND)
            {
                ungetc(ch, PrgFl);
                return( Trie[mid].token );
            }
            return( ret );
        }
        else
        {
            /* Properly NUL terminate the buffer that the keyword is

            *   being saved in and return the token value.
            */
            *WordPtr = '\0';
            return( Trie[mid].token );
        }
    }

    /* Terminate string in keyword buffer and return not found. */
    *WordPtr = '\0';
    return( NOT_FND );

}

/*-----------------------------------------------------------------------------
| Routine   :   GetNo --- Get a number from the file.
|
| Inputs    :   word    - Pointer to word buffer for error reporting.
| Outputs   :   RetNo   - Returns the number read from the file.
|
| Returns   :   Returns the last character read from the file or EOF.
-----------------------------------------------------------------------------*/

static
int     GetNo(char  **word,
              long  *RetNo)
{
    auto        int     c;

    /* Get number.  */
    *RetNo = 0L;
    while ((c = fgetc( PrgFl )) >= '0' && c <= '9')
    {
        /* Save character in word buffer.   */
        *(*word)++ = c;

        /* Calculate value of number.   */
        *RetNo = *RetNo * 10L + (long) (c - '0');
    }
    return( c );
}

/*-----------------------------------------------------------------------------
| Routine   :   Lex() --- Get the next key word from the input file.
|
| Outputs   :   sym - The symbolic data read from the file.
|
| Returns   :   Returns the token read or EOF.
-----------------------------------------------------------------------------*/

TKNS    Lex(TOKEN   *sym)
{
    register    int     i;
    register    int     tkn;
    auto        int     ch;
    auto        char    *bf;

    /* Strip comments and white space.  If the character read is a '#',
    *   every thing to the end of the line is a comment.
    */
    ch = fgetc( PrgFl );

    while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '#')
    {
        /* Process the special characters '#' and '\n'.     */
        if (ch == '\n')
            /* End of line, increment the line number.  */
            LnNo++;
        else if (ch == '#')
        {
            /* Found a comment character, strip all characters to end
            *   of line and increment the line number.
            */
            while (fgetc( PrgFl ) != '\n')
                ;
            LnNo++;
        }

        /* Get the next character.  */
        ch = fgetc( PrgFl );
    }

    /* Get strings, etc.    */
    if (ch == '"')
    {
        /* Get contents of string.  */
        bf = sym->str;
        for (i = 0; i < MAX_STR; i++)
            if ((ch = fgetc( PrgFl )) != '"' && ch != EOF)
                *bf++ = ch;
            else
                break;
        *bf = '\0';

        /* Return string token. */
        strcpy(word, sym->str);
        return( STRING );
    }
    else if (ch >= '0' && ch <= '9')
    {
        auto        long        no;

        /* Establish a pointer to the word buffer and unget the
        *   numeric character for re-reading.
        */
        bf = word;
        ungetc(ch, PrgFl);

        /* Get number, time or date.    */
        if ((ch = GetNo(&bf, &no)) == ':')
        {
            /* Getting time, not number.    */
            *bf++ = ch;
            sym->ftime.ti_hour = (unsigned char) no;
            sym->ftime.ti_hund = (unsigned char) 0;

            /* Get minutes.     */
            if ((ch = GetNo(&bf, &no)) == ':')
            {
                /* Save minutes.    */
                *bf++ = ch;
                sym->ftime.ti_min = (unsigned char) no;


                /* Get seconds. */
                if ((ch = GetNo(&bf, &no)) == '.')
                {
                    *bf = '\0';
                    ParsErr( "Hundredths of seconds not allowed in "
                        "time expressions" );
                }
                sym->ftime.ti_sec = (unsigned char) no;
            }
            else
            {
                /* No seconds to get.   */
                sym->ftime.ti_min = (unsigned char) no;
                sym->ftime.ti_sec  = (unsigned char) 0;
            }

            /* This is a time.  */
            tkn = TIME;
        }
        else if (ch == '/')
        {
            /* Getting date, not number.    */
            *bf++ = ch;
            sym->fdate.da_mon = (char) no;

            /* Get day.     */
            if ((ch = GetNo(&bf, &no)) == '/')
            {
                /* Save character.  */
                *bf++ = ch;
                sym->fdate.da_day = (char) no;

                /* Get year.    */
                ch = GetNo(&bf, &no);
                if (no > 1980L)
                    no -= 1980L;
                else if (no > 80L && no < 100L)
                    no -= 80L;
                else
                {
                    *bf = '\0';
                    ParsErr( "Error, bad year value in date expression." );
                }
                sym->fdate.da_year = (int) no;
            }
            else
            {
                *bf = '\0';
                ParsErr( "Missing year in date expression" );
            }

            /* This is a date.  */
            tkn = DATE;
        }
        else
        {
            /* Just an integer constant.    */
            sym->no = no;
            tkn = NUMBER;

        }

        /* Return the unused character. */
        *bf = '\0';
        ungetc(ch, PrgFl);
        return( tkn );
    }
    else if (ch == EOF)
        return( EOF );

    /* Call the trie search routine to return the next token, EOF
    *   or NOT_FND.  If not found, print an error and quit.
    */
    if ((tkn = TrieSrch(T0, ch, word)) == NOT_FND)
    {
        /* Illegal first character in word. */
        if ( *PrvWd )
            fprintf(stderr, "Parse - Error in Line: %d, cannot identify "
                "word after '%s'.\n", LnNo + 1, PrvWd);
        else
            fprintf(stderr, "Parse - Error in Line: %d, cannot identify "
                "first word in file.\n", LnNo + 1);
        exit( -1 );
    }
    else if (tkn == 0)
    {
        /* Illegal word.    */
        fprintf(stderr, "Parse - Error in Line: %d, cannot identify "
            "word '%s'.\n", LnNo + 1, word);
        exit( -1 );
    }
    strcpy(PrvWd, word);

    /* Return the token found.  */
    return( tkn );
}
/* End of File */
