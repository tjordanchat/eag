/*
   File: placeholder.c
   Defines for which rules code will be generated to parse placeholders.
   No placeholders will be generated for rules that are external,
   predicate or semipredicate. No placeholder will be generated
   for the rule "layout".
  
   CVS ID: "$Id: placeholder.c,v 1.3 2004/12/26 00:48:46 marcs Exp $"
*/

/* general includes */
#include <stdio.h>

/* libebs includes */
#include <ebs_bst.h>

/* local includes */
#include "tree.h"
#include "numbering.h"
#include "placeholder.h"

static void decide_placeholder_rule (rule srule)
	{ if (srule -> ext) return;
	  if (srule -> kind & rule_predicate) return;
	  if (srule -> kind & rule_semipredicate) return;
	  if (srule == layout_rule) return;
	  if (!srule -> reachable) return;
	  srule -> placeholder = 1;		/* too optimistic */
	};

void decide_placeholders ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     decide_placeholder_rule (all_rules[ix]);
	};
