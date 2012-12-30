/*
   File: empty.c
   Detects which meta and syntax rules may produce empty.
   Detects which rules always produce empty
   Classifies the rules into (semi) predicates and non predicates
   Marks all alts and members which may produce empty
  
   CVS ID: "$Id: empty.c,v 1.4 2008/06/18 12:27:01 marcs Exp $"
*/

/* general includes */
#include <stdio.h>
#include <string.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_error.h>

/* local includes */
#include "tree.h"
#include "numbering.h"
#include "empty.h"
#include "main.h"

/*
   Nullability of rules in a context free grammar is usually detected
   by the following fixed point algorithm:

   mark all members that directly derive empty;
   do
      mark all rules that have right hand sides,
      consisting entirely of marked members
   until no more marking is possible.

   When no more information is needed, the initial marking can also be
   done by the loop. We will use this algorithm for the empty detection
   in meta rules. For the classification of the rules we must also detect
   which rules always produce empty by a second fixed point calculation.

   Marking in rules and meta rules is always one of 4:
   unknown (U), may_produce_empty (m),
   always_produces_empty (A), never_produces_empty (N).
*/
static int affix_nullability (affix a)
	{ switch (a -> tag)
	     { case tag_affix_variable:
		  { meta_rule def = a -> u.var.def;
		    return (def -> empty);		/* U, m, N */
		  };
	       case tag_affix_terminal:
		  if (strlen (a -> u.str)) return (never_produces_empty);
		  else return (may_produce_empty);	/* actually A */
	       case tag_affix_integer:			/* check this one */
		  if (a -> u.inum <= 0) return (may_produce_empty);
		  else return (never_produces_empty);
	       case tag_affix_real:
		  if (a -> u.rnum == 0.0) return (may_produce_empty);
		  else return (never_produces_empty);	/* sowieso checken */
	       case tag_affix_semi:
		  { cset set = a -> u.semi;
		    if (set -> kind & star) return (may_produce_empty);
		    else return (never_produces_empty);
		  };
	       case tag_affix_element: return (never_produces_empty);
	       default: bad_tag (a -> tag, "affix_may_produce_empty");
	     };
	  return (unknown);
	};

/*
   The nullability of the concatenation is constructed as follows
	+ U m N
        U U U N
        m U m N
        N N N N
*/
static int concat_nullability (affix_list affs)
	{ int null = affix_nullability (affs -> array[0]);
	  int ix = 1;
	  if (null == never_produces_empty) return (never_produces_empty);
	  do
	     { int null2 = affix_nullability (affs -> array[ix]);
	       if (null2 == never_produces_empty) return (never_produces_empty);
	       if (null != null2) null = unknown;
	       ix++;
	     }
	  while (ix < affs -> size);
	  return (null);
	};

static int expr_nullability (expr e)
	{ if (e == expr_nil)		/* empty meta alt */
	     return (may_produce_empty);
	  switch (e -> tag)
	     { case tag_single: return (affix_nullability (e -> u.single));
	       case tag_concat: return (concat_nullability (e -> u.concat));
	       case tag_compos: return (never_produces_empty);
	       case tag_union: return (never_produces_empty);
	       default: bad_tag (e -> tag, "expr_nullability");
	     };
	  return (unknown);
	};

/*
   The nullability of different meta alternatives is constructed as
	; U m N
        U U m U
        m m m m
        N U m N
*/
static int meta_alts_nullability (meta_alt_list alts)
	{ int null = expr_nullability (alts -> array[0] -> e);
	  int ix = 1;
	  if (null == may_produce_empty) return (may_produce_empty);
	  for (ix = 1; ix < alts -> size; ix++)
	     { int null2 = expr_nullability (alts -> array[ix] -> e);
	       if (null2 == may_produce_empty) return (may_produce_empty);
	       if (null != null2) null = unknown;
	     };
	  return (null);
	};

static int change;
static void detect_meta_rule_nullability (meta_rule mrule)
	{ int null;
	  if (mrule -> ext) return;
	  null = meta_alts_nullability (mrule -> alts);
	  if (mrule -> empty == null) return;
	  if (mrule -> empty == unknown)
	     { mrule -> empty = null;
	       change = 1;
	       return;
	     };
	  internal_error ("detect_meta_rule_nullability");
	};

static void finish_meta_rule_nullability (meta_rule mrule)
	{ if (mrule -> ext) return;
	  if (mrule -> empty == unknown)
	     mrule -> empty = never_produces_empty;
	};

static void detect_empty_producing_meta_rules ()
	{ int nr_passes;
	  int ix;
	  hint ("detecting empty producing meta rules...");
	  nr_passes = 0;
	  do
	     { change = 0;
	       for (ix = 0; ix < nr_of_meta_rules; ix++)
		  detect_meta_rule_nullability (all_meta_rules[ix]);
	       nr_passes++;
	     }
	  while (change);
	  hint ("needed %d pass%s for empty detection of meta rules",
		nr_passes, (nr_passes == 1)?"":"es");
	  for (ix = 0; ix < nr_of_meta_rules; ix++)
	     finish_meta_rule_nullability (all_meta_rules[ix]);
	};

static void initial_terminal_classification (member m)
	{ if (strlen (m -> u.terminal) != 0)
	       m -> empty = never_produces_empty;
	  else m -> empty = always_produces_empty;
	};

static void initial_semiterminal_classification (member m)
	{ cset set = m -> u.semi.set;
	  if (set -> kind & star)
	       m -> empty = may_produce_empty;
	  else m -> empty = never_produces_empty;
	};

static void initial_cut_classification (member m)
	{ m -> empty = always_produces_empty;
	};

static void initial_member_classification (rule srule, member m)
	{ switch (m -> tag)
	     { case tag_call: return;
	       case tag_terminal:
		  initial_terminal_classification (m); break;
	       case tag_semiterminal:
		  initial_semiterminal_classification (m); break;
	       case tag_cut:
		  initial_cut_classification (m); break;
	       default: bad_tag (m -> tag, "initial_member_classification");
	     };
	  if (m -> empty != always_produces_empty)
	     srule -> kind = rule_nonpredicate;
	};

static void initial_alt_classification (rule srule, alt a)
	{ member_list mems = a -> members;
	  int ix;
	  for (ix = 0; ix < mems -> size; ix++)
	     initial_member_classification (srule, mems -> array[ix]);
	};

static void initial_rule_classification (rule srule)
	{ alt_list alts = srule -> alts;
	  int ix;

	  if (srule -> ext) srule -> empty = always_produces_empty;
	  else
	     for (ix = 0; ix < alts -> size; ix++)
	        initial_alt_classification (srule, alts -> array[ix]);
	};

static void do_initial_rule_classification ()
	{ int ix;
	  hint ("initial rule classification...");
	  for (ix = 0; ix < nr_of_rules; ix++)
	     initial_rule_classification (all_rules [ix]);
	};

static int change;
static void unify_empty (rule srule, int empty)
	{ if (srule -> empty) return;
	  if (!empty) return;
	  srule -> empty = may_produce_empty;
	  change = 1;
	};

static int member_may_produce_empty (member m)
	{ switch (m -> tag)
	     { case tag_call:
	          { int empty = m -> u.call.def -> empty;
	            if (empty == may_produce_empty) return (1);
	            return (empty == always_produces_empty);	/* ext */
		  };
	       case tag_terminal:
	       case tag_semiterminal:
	       case tag_cut:
	  	  return (m -> empty != never_produces_empty);
	       default: bad_tag (m -> tag, "member_may_produce_empty");
	     };
	  return (0);
	};

static int alt_may_produce_empty (alt a)
	{ int ix;
	  member_list mems = a -> members;
	  for (ix = 0; ix < mems -> size; ix++)
	     if (!member_may_produce_empty (mems -> array[ix])) return (0);
	  return (1);
	};

static void detect_if_rule_produces_empty (rule srule)
	{ int ix;
	  alt_list alts = srule -> alts;

	  if (srule -> ext) return;
	  for (ix = 0; ix < alts -> size; ix++)
	     unify_empty (srule, alt_may_produce_empty (alts -> array[ix]));
	};

static void detect_empty_producing_rules ()
	{ int nr_passes;
	  int ix;
	  hint ("detecting empty producing rules...");
	  nr_passes = 0;
	  do
	     { change = 0;
	       for (ix = 0; ix < nr_of_rules; ix++)
		  detect_if_rule_produces_empty (all_rules [ix]);
	       nr_passes++;
	     }
	  while (change);
	  hint ("needed %d pass%s for empty detection",
		nr_passes, (nr_passes == 1)?"":"es");
	};

static void mark_if_rule_never_produces_empty (rule srule)
	{ if (srule -> empty) return;
	  srule -> empty = never_produces_empty;
	  srule -> kind = rule_nonpredicate;
	};

static int member_always_produces_empty (member m)
	{ if (m -> tag == tag_call)
	     { rule def = m -> u.call.def;
	       if (def -> empty == never_produces_empty) return (0);
	       return (def -> kind != rule_nonpredicate);
	     }
	  else return (m -> empty == always_produces_empty);
	};

static int alt_always_produces_empty (alt a)
	{ int ix;
	  member_list mems = a -> members;
	  if (mems == member_list_nil) return (1);

	  for (ix = 0; ix < mems -> size; ix++)
	     if (!member_always_produces_empty (mems -> array[ix]))
		return (0);
	  return (1);
	};

static void detect_if_rule_always_produces_empty (rule srule)
	{ alt_list alts = srule -> alts;
	  int ix;

	  if (srule -> ext) return;
	  if (srule -> kind == rule_nonpredicate) return;
	  for (ix = 0; ix < alts -> size; ix++)
	     if (!alt_always_produces_empty (alts -> array[ix]))
	        { srule -> kind = rule_nonpredicate;
		  change = 1;
		};
	};

static void mark_if_rule_always_produces_empty (rule srule)
	{ if (srule -> ext) return;
	  if (srule -> kind == rule_nonpredicate) return;
	  srule -> empty = always_produces_empty;
	  srule -> kind = rule_predicate;
	};

static void determine_user_predicates ()
	{ int nr_passes;
	  int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     mark_if_rule_never_produces_empty (all_rules [ix]);
	  hint ("detecting user defined predicates...");
	  nr_passes = 0;
	  do { change = 0;
	       for (ix = 0; ix < nr_of_rules; ix++)
		  detect_if_rule_always_produces_empty (all_rules [ix]);
	       nr_passes++;
	     }
	  while (change);
	  hint ("needed %d pass%s for predicate detection",
		nr_passes, (nr_passes == 1)?"":"es");
	  for (ix = 0; ix < nr_of_rules; ix++)
	     mark_if_rule_always_produces_empty (all_rules [ix]);
	};

static void finish_marking_in_member (member m)
	{ if (m -> tag != tag_call) return;
	  m -> empty = m -> u.call.def -> empty;
	};

static void finish_marking_in_alt (alt a)
	{ member_list mems = a -> members;
	  int ix;
	  if (mems == member_list_nil) return;
	  for (ix = 0; ix < mems -> size; ix++)
	     finish_marking_in_member (mems -> array[ix]);
	  for (ix = 0; ix < mems -> size; ix++)
	     if (mems -> array[ix] -> empty == never_produces_empty) return;
	  a -> empty = may_produce_empty;
	};

static void finish_marking (rule srule)
	{ alt_list alts = srule -> alts;
	  int ix;
	  if (srule -> ext) return;
	  for (ix = 0; ix < alts -> size; ix++)
	     finish_marking_in_alt (alts -> array[ix]);
	};

static void finish_rule_marking ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     finish_marking (all_rules [ix]);
	};

static void check_layout_rule ()
	{ if (!layoutflag) return;
	  if (layout_rule -> kind & rule_nonpredicate) return;
	  panic ("rule layout should be a non predicate");
	};

static void report_empty_detection_for_rule (rule srule)
	{ if (srule -> kind & rule_predicate) eprint_log ("predicate");
	  else if (srule -> kind & rule_semipredicate)
					  eprint_log ("semipredicate");
	  else if (srule -> kind & rule_nonpredicate) eprint_log ("rule");
	  else eprint_log ("?? rule ??");
	  eprint_log (" %s ", srule -> nonterminal);
	  switch (srule -> empty)
	     { case may_produce_empty:
		  eprint_log ("may produce empty");
		  break;
	       case never_produces_empty:
		  eprint_log ("never produces empty");
		  break;
	       case always_produces_empty:
		  eprint_log ("always produces empty");
		  break;
	       default: eprint_log ("may or may not produce empty");
	     };
	  eprint_log ("\n");
	};

static void try_report_empty_detection ()
	{ int ix;
	  if (!full_verbose) return;
	  wlog ("Rules have been classified as follows:");
	  for (ix = 0; ix < nr_of_rules; ix++)
	     report_empty_detection_for_rule (all_rules [ix]);
	};

void do_empty_classification ()
	{ warning ("empty detection and rule classification...");
	  detect_empty_producing_meta_rules ();
	  do_initial_rule_classification ();
	  detect_empty_producing_rules ();
	  determine_user_predicates ();
	  finish_rule_marking ();
	  check_layout_rule ();
	  try_report_empty_detection ();
	};
