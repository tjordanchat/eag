/*
   File: layout.c
   Analyzes where layout may be inserted
  
   CVS ID: "$Id: layout.c,v 1.3 2004/12/26 00:48:45 marcs Exp $"
*/

/* general includes */
#include <stdio.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_error.h>
#include <ebs_primio.h>

/* local includes */
#include "tree.h"
#include "numbering.h"
#include "main.h"
#include "common.h"
#include "layout.h"

/*
   Analyze the grammar for layout and invisible rules
*/
int is_an_invisible_member (member m)
	{ switch (m -> tag)
	     { case tag_call:
		  { rule def = m -> u.call.def;
		    if (def == layout_rule) return (1);
		    if (def -> ext) return (1);
		    if (def -> kind & (rule_predicate | rule_semipredicate))
			return (1);
		    return (0);
		  };
	       case tag_terminal: return (strcmp (m -> u.terminal, "") == 0);
	       case tag_cut: return (1);
	       default: return (0);
	     };
	};

static void initial_check_if_alt_starts_with_layout (rule srule,
						      member_list mems)
	{ int ix;
	  for (ix = 0; ix < mems -> size; ix++)
	     { member m = mems -> array[ix];
	       if ((m -> tag == tag_call) && (m -> u.call.def == layout_rule))
		  { srule -> startswithlayout = 1;
		    return;
		  };
	       if (m -> empty == never_produces_empty) return;
	     };
	};

static void initial_check_if_alt_ends_in_layout (rule srule,
						  member_list mems)
	{ int ix;
	  for (ix = mems -> size - 1; 0 <= ix; ix--)
	     { member m = mems -> array[ix];
	       if ((m -> tag == tag_call) && (m -> u.call.def == layout_rule))
		  { srule -> endsinlayout = 1;
		    return;
		  };
	       if (m -> empty == never_produces_empty) return;
	     };
	};

static void initial_analyze_layout_in_alt (rule srule, alt a)
	{ member_list mems = a -> members;
	  if (mems == member_list_nil) return;
	  initial_check_if_alt_starts_with_layout (srule, mems);
	  initial_check_if_alt_ends_in_layout (srule, mems);
	};

static void initial_analyze_layout_in_rule (rule srule)
	{ int ix;
	  if (srule -> ext) return;
	  if (srule -> kind & (rule_predicate | rule_semipredicate)) return;
	  if (srule == layout_rule) return;
	  for (ix = 0; ix < srule -> alts -> size; ix++)
	     initial_analyze_layout_in_alt (srule, srule -> alts -> array[ix]);
	};

static void initial_analyze_layout_in_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     initial_analyze_layout_in_rule (all_rules[ix]);
	};

/*
   Incrementally find which members are shielded from
   members by layout
*/
static int change;
static void check_if_alt_starts_with_layout (rule srule, member_list mems)
	{ int ix;
	  if (srule -> startswithlayout) return;
	  for (ix = 0; ix < mems -> size; ix++)
	     { member m = mems -> array[ix];
	       if ((m -> tag == tag_call) &&
		   (m -> u.call.def -> startswithlayout))
		  { srule -> startswithlayout = 1;
		    change = 1;
		    return;
		  };
	       if (m -> empty == never_produces_empty) return;
	     };
	};

static void check_if_alt_ends_in_layout (rule srule, member_list mems)
	{ int ix;
	  if (srule -> endsinlayout) return;
	  for (ix = mems -> size - 1; 0 <= ix; ix--)
	     { member m = mems -> array[ix];
	       if ((m -> tag == tag_call) &&
		   (m -> u.call.def -> endsinlayout))
		  { srule -> endsinlayout = 1;
		    change = 1;
		    return;
		  };
	       if (m -> empty == never_produces_empty) return;
	     };
	};

static void analyze_layout_in_alt (rule srule, alt a)
	{ member_list mems = a -> members;
	  if (mems == member_list_nil) return;
	  check_if_alt_starts_with_layout (srule, mems);
	  check_if_alt_ends_in_layout (srule, mems);
	};

static void analyze_layout_for_rule (rule srule)
	{ int ix;
	  if (srule -> ext) return;
	  if (srule -> kind & (rule_predicate | rule_semipredicate)) return;
	  if (srule == layout_rule) return;
	  for (ix = 0; ix < srule -> alts -> size; ix++)
	     analyze_layout_in_alt (srule, srule -> alts -> array[ix]);
	};

static void analyze_layout_for_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     analyze_layout_for_rule (all_rules[ix]);
	};

static void report_on_layout_for_rule (rule srule)
	{ if (srule -> startswithlayout)
	     wlog ("rule %s may start with layout", srule -> nonterminal);
	  if (srule -> endsinlayout)
	     wlog ("rule %s may end with layout", srule -> nonterminal);
	};

static void report_on_layout ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     report_on_layout_for_rule (all_rules[ix]);
	};

static void analyze_layout_in_grammar ()
	{ int nrpasses = 0;
	  warning ("analyzing layout...");
	  initial_analyze_layout_in_rules ();
	  do
	     { change = 0;
	       nrpasses++;
	       analyze_layout_for_rules ();
	     }
	  while (change);
	  hint ("needed %d pass%s for layout analysis", nrpasses,
		(nrpasses == 1)?"":"es");
	  if (full_verbose)
	     { wlog ("dumping layout analysis");
	       report_on_layout ();
	     };
	};

/*
   Determine layout insertion points
*/
static void mark_member_if_followed (member m, member_list mems, int from)
	{ int k;
	  for (k = from; k < mems -> size; k++)
	     { if (!is_an_invisible_member (mems -> array[k]))
		  { m -> followlayout = 1;
		    return;
		  };
	     };
	};

static void determine_layout_insertion_point (member_list mems, int i)
	{ int j;
	  member m = mems -> array[i];
	  if (is_an_invisible_member (m)) return;
	  if ((m -> tag == tag_call) && (m -> u.call.def -> endsinlayout))
	     { mark_member_if_followed (m, mems, i+1);
	       return;
	     };
	  for (j = i + 1; j < mems -> size; j++)
	     { member m2 = mems -> array[j];
	       if (is_layout_member (m2))
		  { mark_member_if_followed (m, mems, j+1);
		    return;
		  };
	       if ((m2 -> tag == tag_call) &&
		   (m2 -> u.call.def -> startswithlayout))
		  { m -> followlayout = 1; return; };
	       if (!is_an_invisible_member (m2)) return;
	     };
	};

static void determine_layout_insertion_points_in_alt (alt a)
	{ int ix;
	  member_list mems = a -> members;
	  if (mems == member_list_nil) return;
	  for (ix = 0; ix < mems -> size - 1; ix++)
	     determine_layout_insertion_point (mems, ix);
	};

static void determine_layout_insertion_points_in_rule (rule srule)
	{ int ix;
	  if (srule -> ext) return;
	  if (srule -> kind & (rule_predicate | rule_semipredicate)) return;
	  for (ix = 0; ix < srule -> alts -> size; ix++)
	     determine_layout_insertion_points_in_alt
		(srule -> alts -> array[ix]);
	};

static void determine_layout_insertion_points_in_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     determine_layout_insertion_points_in_rule (all_rules[ix]);
	};

/*
   Dump insertion points
*/
static void dump_member (member m)
	{ if (is_an_invisible_member (m)) return;
	  switch (m -> tag)
	     { case tag_call: wlog (m -> u.call.nonterminal); break;
	       case tag_terminal: output_string (stderr, m -> u.terminal);
				  break;
	       case tag_semiterminal: wlog ("{}");
	       default: break;
	     };
	};

static void dump_insertion_points_of_alt (member_list mems)
	{ int ix;
	  if (mems == member_list_nil) return;
	  for (ix = 0; ix < mems -> size; ix++)
	     { member m = mems -> array[ix];
	       dump_member (m);
	       if (m -> followlayout) eprint_log (".");
	     };
	};

static void dump_insertion_points_of_rule (rule srule)
	{ int ix;
	  if (srule -> ext) return;
	  if (srule -> kind & (rule_predicate | rule_semipredicate)) return;
	  if (srule == layout_rule) return;
	  for (ix = 0; ix < srule -> alts -> size; ix++)
	     { eprint_log ("%s: ", srule -> nonterminal);
	       dump_insertion_points_of_alt (srule -> alts ->
					     array[ix] -> members);
	       eprint_log ("\n");
	     };
	};

static void dump_insertion_points_of_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     dump_insertion_points_of_rule (all_rules[ix]);
	};

static void determine_layout_insertion_points ()
	{ warning ("determining layout insertion points...");
	  determine_layout_insertion_points_in_rules ();
	  if (full_verbose)
	     { wlog ("dumping layout insertion points");
	       dump_insertion_points_of_rules ();
	     };
	};

void analyze_layout ()
	{ analyze_layout_in_grammar ();
	  determine_layout_insertion_points ();
	};
