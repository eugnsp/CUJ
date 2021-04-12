/*
 *  (C) Copyright 1990 Ron Burk
 *  All Rights Reserved
 *
 *  makehash.c - program to make hashing functions.
 *
 *  Note that this is a C++ program.  It has been compiled with Turbo C++
 *  and Zortech C++.  There is conditional code to hack around the fact that
 *  Zortech does not allow a member function and a const member function of
 *  the same name.
 */

#ifdef __ZTC__
    #define CONST
#else
    #define CONST const
#endif

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char   uchar;
enum    bool            { FALSE, TRUE };

FILE    *OpenFile(const char *FileName, const char *IOMode)
    {
    assert(FileName != 0);
    if(IOMode == 0)
        IOMode  = "r";
    FILE    *FileDescriptor = fopen(FileName, IOMode);
    if(FileDescriptor == 0)
        {
        fprintf(stderr, "'%s': Can't open for mode '%s'\n",
                    FileName, IOMode);
        exit(EXIT_FAILURE);
        }
    return FileDescriptor;
    }

char    *strdup(const char *s, size_t len)
    {
    char    *ret    = (char *)malloc(len+1);
    memcpy(ret, s, len);
    ret[len]      = '\0';
    return ret;
    }

inline  char    *strdup(const char *s)
    {
    return strdup(s, strlen(s));
    }

class   KeyWord
    {
public:
    KeyWord(char *Name=0, char *Value=0);
    char    *Name()
        { return Name_; }
    char    *Value()
        { return Value_; }
    bool    Read(FILE *FileDescriptor);
private:
    char    *Name_;
    char    *Value_;
    };

class   KeyTable
    {
public:
    KeyTable(const char *FileName);
    int     NumberOfKeys()
        { return NumberOfKeys_; }
    KeyWord *operator[](int KeyWordNumber);
    void    MoveTo(int Position1, int Position2);
    static const int MAXKEYWORDS;
private:
    int     NumberOfKeys_;
    KeyWord **Table;
    };
const int KeyTable::MAXKEYWORDS = 100;

class   ByteTable
    {
public:
    ByteTable();
    void    SetAscending();
    void    Shuffle(int Shuffles=1);
    int     Index(int Value);
    uchar   &operator[](int ByteNumber);
#ifndef __ZTC__
    uchar   operator[](int ByteNumber)  const;
#endif
    ByteTable &operator=(int Value);
private:
    uchar   *Table;
    };

// KeyWord member functions
KeyWord::KeyWord(char *Name__, char *Value__)
    : Name_(Name__), Value_(Value__)
    {
    }

/********************************
KeyWord::Read - read a KeyWord from an open file.

    Each line in the KeyWord file must be a KeyWord line, an empty line,
or a comment.  A KeyWord line is a line that contains a KeyWord
optionally followed by white space and a value.  The KeyWord and the
value can contain any characters except white space (except the KeyWord
cannot begin with '#').  An empty line is a line that only contains
white space.  A comment is any line that begins with a '#' (optionally
preceeded by white space). 

********************************/

bool    KeyWord::Read(FILE *FileDescriptor)
    {
    const int MAXLINE = 256;
    char    Line[MAXLINE+1];

    Name_   = Value_    = 0;
    while(fgets(Line, MAXLINE, FileDescriptor))
        {
        char *Scanner = Line;
        while(*Scanner && isspace(*Scanner))    // allow leading white space
            ++Scanner;
        if(*Scanner && *Scanner != '#') // if not empty and not comment
            {
            Name_   = Scanner;      // remember start of KeyWord name
            while(*Scanner && !isspace(*Scanner))
                ++Scanner;          // skip over name
            if(*Scanner)            // if something follows the name
                {
                *Scanner++  = '\0'; // NUL-terminate the name
                while(*Scanner && isspace(*Scanner))
                    ++Scanner;
                if(*Scanner)
                    {
                    Value_  = Scanner;  // remember start of value
                    while(*Scanner && !isspace(*Scanner))
                        ++Scanner;
                    *Scanner    = '\0';
                    Value_  = strdup(Value_);   // make permanent copy
                    }
                }
            Name_   = strdup(Name_);    // make permanent copy
            return TRUE;
            }
        }

    return FALSE;   // hit End-Of-File
    }

// KeyTable member functions

KeyTable::KeyTable(const char *FileName)
    {
    Table   = new KeyWord *[MAXKEYWORDS];
    assert(Table != 0);
    FILE    *KeyWordFile    = OpenFile(FileName, "r");
    KeyWord CurrentKeyWord;
    for(int KeyWordNumber=0; CurrentKeyWord.Read(KeyWordFile); ++KeyWordNumber)
        {
        Table[KeyWordNumber]    = new KeyWord;
        *Table[KeyWordNumber]   = CurrentKeyWord;
        printf( "name='%s', value='%s'\n", Table[KeyWordNumber]->Name(),
            Table[KeyWordNumber]->Value());
        }
    NumberOfKeys_   = KeyWordNumber;
    printf( "read %d keywords\n", KeyWordNumber);
    }

KeyWord *KeyTable::operator[](int KeyWordNumber)
    {
    assert(KeyWordNumber >= 0);
    assert(KeyWordNumber < NumberOfKeys());
    return Table[KeyWordNumber];
    }
void    KeyTable::MoveTo(int MovePosition, int ToPosition)
    {
    KeyWord *Value  = Table[MovePosition];
    int     Position;
    for(Position=MovePosition; Position < NumberOfKeys()-1; ++Position)
        Table[Position]     = Table[Position+1];
    for(Position=NumberOfKeys()-1; Position >= ToPosition; --Position)
        Table[Position]     = Table[Position-1];
    Table[ToPosition]       = Value;
    }


// ByteTable member functions

ByteTable::ByteTable()
    {
    Table   = new uchar[256];
    assert(Table != 0);
    memset(Table, 0, 256);
    }

void    ByteTable::SetAscending()
    {
    for(int ByteNumber=0; ByteNumber < 256; ++ByteNumber)
        Table[ByteNumber]   = ByteNumber;
    }

void    ByteTable::Shuffle(int Shuffles)
    {
    // swap each entry in the table with a randomly chosen entry

    for(int PassNumber=0; PassNumber < Shuffles; ++PassNumber)
        {
        for(int ByteNumber=0; ByteNumber < 256; ++ByteNumber)
            {
            int     RandomPartner   = rand() % 256;
            uchar   OtherValue      = Table[RandomPartner];
            Table[RandomPartner]    = Table[ByteNumber];
            Table[ByteNumber]       = OtherValue;
            }
        }
    }

uchar   &ByteTable::operator[](int ByteNumber)
    {
    assert(ByteNumber >= 0);
    assert(ByteNumber <= 255);

    return Table[ByteNumber];
    }
#ifndef __ZTC__
uchar   ByteTable::operator[](int ByteNumber) const
    {
    assert(ByteNumber >= 0);
    assert(ByteNumber <= 255);

    return Table[ByteNumber];
    }
#endif

int     ByteTable::Index(int Value)
    {
    uchar    *Found  = (uchar *)memchr(Table, Value, 256);
    if(Found)
        return Found - Table;
    else
        return -1;
    }

inline
ByteTable &ByteTable::operator=(int Value)
    {
    assert(Value >= 0);
    assert(Value <= 255);
    memset(Table, Value, 256);
    return *this;
    }

int     ByteHash(const char *Text, const ByteTable &HashTable)
    {
    int     Hash    = 0;
    while(*Text)
        Hash    =   HashTable[*Text++ ^ Hash];
    return Hash;
    }

int     MakeHash(const char *KeyWordFileName);

void    Usage()
    {
    fprintf(stderr, "Usage: makehash keyfile\n");
    exit(EXIT_FAILURE);
    }

int     main(int argc, char **argv)
    {
    if(argc < 2)
        Usage();
    char    *KeyWordFileName    = argv[1];
    exit(MakeHash(KeyWordFileName));
    }

int     MakeHash(const char *KeyWordFileName)
    {
    KeyTable    InputTable(KeyWordFileName);
    int         iKeyWord;
    int         *Failures   = new int[InputTable.NumberOfKeys()];
    assert(Failures != 0);
    ByteTable   HashBytes;
    for(int Attempts=0; Attempts < 999; ++Attempts)
        {
        for(int xx=0; xx < InputTable.NumberOfKeys(); ++xx) Failures[xx]=0;
        for(int TableBase = 0; TableBase < 256; ++TableBase)
            {
        HashBytes.SetAscending();   // set equal to 0,1,2,...255
        HashBytes.Shuffle();        // randomize
            int         NotEligible[256];
            for(int x=0; x < 256; ++x) NotEligible[x]   = 0;
            for(int iKeyWord = 0; iKeyWord < InputTable.NumberOfKeys(); ++iKeyWord)
                {
                const char *Text    = InputTable[iKeyWord]->Name();
                int     TextLength  = strlen(Text);
                int         RandomWalk[99];
                int Hash = 0;
                uchar   H[99];
                for(int i = 0; i < TextLength; ++i)
                    {
                    int   j = Hash ^ Text[i];
                    Hash    =   H[i]    = HashBytes[j];
                    ++NotEligible[j];
                    RandomWalk[i]   = j;
                    }
                int     DesiredValue   = (iKeyWord + TableBase) % 256;
                for(i = TextLength-1; i >= 0; --i)
                    {
                    int     Pos = RandomWalk[i];
                    --NotEligible[Pos];
                    int Other   = HashBytes.Index(DesiredValue);
    
                    assert(Other >= 0);
                    if(NotEligible[Pos] == 0 && NotEligible[Other] == 0)
                        {
                        HashBytes[Other]    = HashBytes[Pos];
                        HashBytes[Pos]     = DesiredValue;
                        ++NotEligible[Other];
                        ++NotEligible[Pos];
                        assert(ByteHash(Text, HashBytes)
                                == (iKeyWord + TableBase)%256);
                        break;
                        }
                    else                        // else not eligible for swapping
                        {
                        DesiredValue    = Other ^ Text[i];
                        ++NotEligible[Other];
                        }
                    }
                if(i < 0)
                    break;
                }
            if(iKeyWord >= InputTable.NumberOfKeys())
                {
                for(iKeyWord = 0; iKeyWord < InputTable.NumberOfKeys(); ++iKeyWord)
                    {
                    const char *Name = InputTable[iKeyWord]->Name();
                    printf( "Hash('%s') = %d\n", Name, ByteHash(Name, HashBytes));
                    }
                return EXIT_SUCCESS;
                }
            else
                {
                if(++Failures[iKeyWord] > 20)
                    {
                    InputTable.MoveTo(iKeyWord, 0);
                    break;
                    }
                }
            }
        }
    
    return EXIT_FAILURE;
    }
