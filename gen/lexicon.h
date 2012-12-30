/*
   File: lexicon.h
   Checks the rules in the grammar that belong to the lexicon
   Prepares the rules in the lexicon for codegeneration
  
   CVS ID: "$Id: lexicon.h,v 1.3 2004/12/26 00:48:46 marcs Exp $"
*/
#ifndef IncLexicon
#define IncLexicon

/* libebs includes */
#include <ebs_bst.h>

extern int lexicon_present;
void check_lexicon ();
void prepare_lexicon (string lexicon);

#endif /* IncLexicon */
