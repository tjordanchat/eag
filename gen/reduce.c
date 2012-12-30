/*
   File: reduce.c
   Reduces the grammer
  
   CVS ID: "$Id: reduce.c,v 1.3 2004/12/26 00:48:47 marcs Exp $"
*/

/* general includes */
#include <stdio.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_error.h>

/* local includes */
#include "tree.h"
#include "numbering.h"
#include "main.h"
#include "reduce.h"

static void mark_in_meta_rule (meta_rule mrule);
static void mark_in_affix_variable (affix a)
	{ if (a -> u.var.def != meta_rule_nil)
	     mark_in_meta_rule (a -> u.var.def);
	};

static void mark_in_affix (affix a)
	{ if (a -> tag == tag_affix_variable)
	     mark_in_affix_variable (a);
	  a -> reachable = 1;
	};

static void mark_in_affix_list (affix_list al)
	{ int ix;
	  for (ix = 0; ix < al -> size; ix++)
	     mark_in_affix (al -> array[ix]);
	};

static void mark_in_expression (expr e)
	{ if (e == expr_nil) return;		/* empty meta alt */
	  switch (e -> tag)
	     { case tag_single: mark_in_affix (e -> u.single); break;
	       case tag_concat: mark_in_affix_list (e -> u.concat); break;
	       case tag_compos: mark_in_affix_list (e -> u.compos); break;
	       case tag_union: mark_in_affix_list (e -> u.uni); break;
	       default: bad_tag (e -> tag, "mark_in_expression");
	     };
	};

static void mark_in_meta_alts (meta_alt_list al)
	{ int ix;
	  if (al == meta_alt_list_nil) return;	/* predefined metarule */
	  for (ix = 0; ix < al -> size; ix++)
	     mark_in_expression (al -> array[ix] -> e);
	};

static void mark_in_meta_rule (meta_rule mrule)
	{ if (mrule -> reachable) return;
	  mrule -> reachable = 1;
	  if (mrule -> ext) return;
	  mark_in_meta_alts (mrule -> alts);
	};

static void mark_in_position (pos p)
	{ mark_in_expression (p -> ex);
	};

static void mark_in_display (pos_list dpy)
	{ int ix;
	  if (dpy == pos_list_nil) return;
	  for (ix = 0; ix < dpy -> size; ix++)
	     mark_in_position (dpy -> array[ix]);
	};

static void mark_in_rule (rule srule);
static void mark_in_call (rule srule, member m)
	{ mark_in_rule (m -> u.call.def);
	  mark_in_display (m -> u.call.display);
	  srule -> has_sons = 1;
	};

static void mark_in_semiterminal (rule srule, member m)
	{ mark_in_display (m -> u.semi.display);
	  srule -> has_sons = 1;
	};

static void mark_in_member (rule srule, member m)
	{ switch (m -> tag)
	     { case tag_call: mark_in_call (srule, m);
	       case tag_terminal: break;
	       case tag_semiterminal: mark_in_semiterminal (srule, m);
	       case tag_cut: break;
	       default: bad_tag (m -> tag, "mark_in_member");
	     };
	};

static void mark_in_members (rule srule, member_list mems)
	{ int ix;
	  if (mems == member_list_nil) return;
	  for (ix = 0; ix < mems -> size; ix++)
	     mark_in_member (srule, mems -> array[ix]);
	};

static void mark_in_alt (rule srule, alt a)
	{ mark_in_display (a -> display);
	  mark_in_members (srule, a -> members);
	};

static void mark_in_alts (rule srule, alt_list alts)
	{ int ix;
	  for (ix = 0; ix < alts -> size; ix++)
	     mark_in_alt (srule, alts -> array[ix]);
	};

static void mark_in_rule (rule srule)
	{ if (srule -> reachable) return;
	  srule -> reachable = 1;
	  if (srule -> ext) return;
	  mark_in_alts (srule, srule -> alts);
	};

static void report_on_rule (rule srule)
	{ wlog ("rule %s is %sreachable and has %ssons.",
		srule -> nonterminal, (srule -> reachable)?"":"not ",
		(srule -> has_sons)?"":"no ");
	};

static void report_on_meta_rule (meta_rule mrule)
	{ wlog ("meta rule %s is %sreachable.",
		mrule -> nonterminal, (mrule -> reachable)?"":"not ");
	};

static void try_report_on_grammar_reduction ()
	{ int ix;
	  if (!full_verbose) return;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     report_on_rule (all_rules[ix]);
	  for (ix = 0; ix < nr_of_meta_rules; ix++)
	     report_on_meta_rule (all_meta_rules[ix]);
	};

void reduce_grammar ()
	{ warning ("reducing grammar...");
	  mark_in_rule (start_rule -> u.call.def);
	  mark_in_display (start_rule -> u.call.display);
	  try_report_on_grammar_reduction ();
	};
