/*
   File: lex.h
   Does lexical analysis of EAGs
  
   CVS ID: "$Id: lex.h,v 1.4 2008/06/18 12:23:47 marcs Exp $"
*/
#ifndef IncLex
#define IncLex

/* libebs includes */
#include <ebs_bst.h>

/* Define the tokens, to begin with undefined */
typedef enum { UNDEFINED,

/* General categories */
IDENTIFIER, INTEGER, REAL, STRING, SET,

/* Punctuation marks */
DOLLAR, COLON, DOUBLECOLON,
POINT, SEMICOLON, COMMA,
LEFTBRACE, RIGHTBRACE,
LEFTPARENTHESIS, RIGHTPARENTHESIS,
SUB, BUS, FLOW,
UP, STAR, PLUS,
EXCLAMATIONMARK, PIPE, CUT,

/* Reserved words */
GRAMMAR_SYMBOL, LEXICON_SYMBOL,
DEFINES_SYMBOL, USES_SYMBOL,

/* End of file */
EOFSYMBOL } symbol;

/* Exported lexed variables */
extern string current_fname;
extern int line, column;
extern int this_ival;
extern real this_rval;
extern string this_sval;
extern symbol thistoken;
extern symbol nexttoken;
extern int lex_errors;

/* Exported code */
void init_lex ();
void init_lex_file (FILE *fd, string fname);
void insymbol ();

#endif /* IncLex */
