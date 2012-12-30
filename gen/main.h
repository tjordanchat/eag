/*
   File: main.h
   Defines some global flags for the generation
  
   CVS ID: "$Id: main.h,v 1.3 2004/12/26 00:48:46 marcs Exp $"
*/
#ifndef IncMain
#define IncMain

/* libebs includes */
#include <ebs_bst.h>

extern int showversion;
extern int topdown;
extern int leftcorner;
extern int lr;
extern int editor;
extern int layoutflag;
extern int interface_to_c;
extern int placeholderflag;
extern int indirect_templates;
extern int traceflag;
extern int matchflag;
extern int dumpflag;
extern int ignore_lexicon;
extern int lookahead;
extern int lookahead_error_messages;
extern int qstackflag;
extern int v1_compatibility;

void add_grammar_to_inputs (string gname);

#endif /* IncMain */
