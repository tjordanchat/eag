/*
   File: numbering.h
   Numbers all rules and meta rules so that we may use these numbers
   in matrices to derive the closure of the left corner relation,
   do left recursion detection and other things
  
   CVS ID: "$Id: numbering.h,v 1.3 2004/12/26 00:48:46 marcs Exp $"
*/
#ifndef IncNumbering
#define IncNumbering

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_value.h>
#include <ebs_lexds.h>

/* local includes */
#include "tree.h"

/* Exported global vars */
extern int nr_of_rules;
extern int nr_of_alts;
extern int nr_of_members;
extern int max_members_per_alt;

extern rule *all_rules;
extern alt *all_alts;
extern member *all_members;

extern int nr_of_meta_rules;
extern int nr_of_meta_alts;
extern meta_rule *all_meta_rules;
extern meta_alt *all_meta_alts;

extern int nr_of_elements;
extern element_def *all_elements;

/* Exported code */
void do_numbering (int pred);
void do_element_numbering ();

#endif /* IncNumbering */
