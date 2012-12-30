/*
   File: lookahead.h
   Determines FIRST and FOLLOW sets (on character basis)
   to form director sets which may limit backtracking.
  
   CVS ID: "$Id: lookahead.h,v 1.3 2004/12/26 00:48:46 marcs Exp $"
*/
#ifndef IncLookahead
#define IncLookahead

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_value.h>
#include <ebs_lexds.h>

/* local includes */
#include "tree.h"

void determine_lookahead_sets ();
char *gather_lcin_director_set (rule syntax_rule, member_list mems, int j);

#endif /* IncLookahead */
