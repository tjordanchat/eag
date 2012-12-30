/*
   File: layout.h
   Analyzes where layout may be inserted
  
   CVS ID: "$Id: layout.h,v 1.3 2004/12/26 00:48:45 marcs Exp $"
*/
#ifndef IncLayout
#define IncLayout

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_value.h>
#include <ebs_lexds.h>

/* local includes */
#include "tree.h"

#define is_layout_member(m) (((m) -> tag == tag_call) && ((m) -> u.call.def == layout_rule))
int is_an_invisible_member (member m);
void analyze_layout ();

#endif /* IncLayout */
