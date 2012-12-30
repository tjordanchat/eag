/*
   File: topdown.h
   Defines a topdown recursive backup parser generator
   Codes also those rules that must be generated in a topdown fashion
  
   CVS ID: "$Id: topdown.h,v 1.3 2004/12/26 00:48:47 marcs Exp $"
*/
#ifndef IncTopdown
#define IncTopdown

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>

void generate_topdown_parser (string basename, string_list predefs);
void generate_predicate_topdown (rule syntax_rule);
void generate_semipredicate_topdown (rule syntax_rule);

#endif /* IncTopdown */
