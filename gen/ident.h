/*
   File: ident.h
   identifies all hyper_rule applications and metadefined affix values
   checks the number of affix positions for every application,
   gathers local affixes, generates proper metadefinitions
   for affix sets occurring in hyper rules.
  
   CVS ID: "$Id: ident.h,v 1.3 2004/12/26 00:48:45 marcs Exp $"
*/
#ifndef IncIdent
#define IncIdent

/* libebs includes */
#include <ebs_bst.h>

string generate_anonymous_affixname ();
void identification ();

#endif /* IncIdent */
