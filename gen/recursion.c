/*
   File: recursion.c
   Determines the primary leftcorner relation and
   checks left recursion for topdown parsers
  
   CVS ID: "$Id: recursion.c,v 1.5 2008/06/18 12:27:01 marcs Exp $"
*/

/* general includes */
#include <stdio.h>
#include <string.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_error.h>
#include <ebs_memalloc.h>

/* local includes */
#include "tree.h"
#include "numbering.h"
#include "main.h"
#include "typing.h"
#include "recursion.h"
#include "warshall.h"

/*
   Analyze the leftrecursiveness of meta rules
*/
static char *mcalls_left;
static char *mcalls_left_closure;
static void allocate_space_for_mcalls ()
	{ int i,j;
	  mcalls_left = (char *) ckcalloc (nr_of_meta_rules * nr_of_meta_rules, sizeof (char));
	  for (i=0; i < nr_of_meta_rules; i++)
	     for (j=0; j < nr_of_meta_rules; j++)
		mcalls_left[i * nr_of_meta_rules + j] = 0;
	};

static int detect_lcall_in_affix (meta_rule mrule, affix a)
	{ switch (a -> tag)
	     { case tag_affix_variable:
		  { meta_rule def = a -> u.var.def;
		    int rulenumber = mrule -> number;
		    int calleenumber = def -> number;
		    mcalls_left [rulenumber * nr_of_meta_rules + calleenumber] = 1;
		    return (def -> empty == may_produce_empty);
		  };
	       case tag_affix_terminal: return (strlen (a -> u.str) == 0);
	       case tag_affix_integer:	return (a -> u.inum <= 0);
	       case tag_affix_real:	return (a -> u.rnum <= 0.0);
	       case tag_affix_semi:
		  { cset set = a -> u.semi;
		    return ((set -> kind & star) ||
		     (!(set -> kind & non) && (strlen (set -> str) == 0)));
		  };
	       case tag_affix_element: return (0);
	       default: bad_tag (a -> tag, "detect_lcall_in_affix");
	     };
	  return (0);
	};

static void detect_lcalls_in_concat (meta_rule mrule, affix_list affs)
	{ int ix = 0;
	  int empty;
	  do
	     { empty = detect_lcall_in_affix (mrule, affs -> array[ix]);
	       ix++;
	     }
	  while (empty && (ix < affs -> size));
	};

static void detect_lcalls_in_meta_alt (meta_rule mrule, expr e)
	{ if (e == expr_nil) return;
	  switch (e -> tag)
	     { case tag_single:
		  (void) detect_lcall_in_affix (mrule, e -> u.single);
		  break;
	       case tag_concat:
		  detect_lcalls_in_concat (mrule, e -> u.concat);
	          break;
	       default:
		  internal_error ("detect lcalls in meta alt");
	     };
	};

static void detect_lcalls_in_meta_alts (meta_rule mrule, meta_alt_list alts)
	{ int ix;
	  if (alts == meta_alt_list_nil) return;
	  for (ix = 0; ix < alts -> size; ix++)
	     detect_lcalls_in_meta_alt (mrule, alts -> array[ix] -> e);
	};

static void detect_lcalls_in_metarule (meta_rule mrule)
	{ if (mrule -> ext) return;
	  if (mrule -> type == error_type) return;
	  if (mrule -> type & lattice_type) return;
	  if (mrule -> type & tuple_type) return;
	  detect_lcalls_in_meta_alts (mrule, mrule -> alts);
	};

static void detect_lcalls_in_metarules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_meta_rules; ix++)
	     detect_lcalls_in_metarule (all_meta_rules[ix]);
	};

static void take_lcalls_closure ()
	{ mcalls_left_closure = warshall (nr_of_meta_rules, mcalls_left);
	};

static void complain_on_leftrecursive_meta_rules ()
	{ int ix;
	  int leftrecursive = 0;
	  for (ix = 0; ix < nr_of_meta_rules; ix++)
	     if (mcalls_left_closure [ix * nr_of_meta_rules + ix])
		{ error ("meta rule %s is left recursive",
			 all_meta_rules[ix] -> nonterminal);
		  leftrecursive++;
		};
	  if (leftrecursive)
	     panic ("found %d left recursive meta rule%s", leftrecursive,
		    (leftrecursive > 1)?"s":"");
	};

/*
   Determine which non predicate rules call each other either on the left
   calls_left [j * nr_of_syntax_rules + i] = 1 <=>
   j is called by i in one step as a (possibly) leftmost member <=>
   rule j <LC1 rule i
*/
static char *calls_left;
static void allocate_space_for_calls ()
	{ int i,j;
	  calls_left = (char *) ckcalloc (nr_of_rules * nr_of_rules, sizeof (char));
	  for (i=0; i < nr_of_rules; i++)
	     for (j = 0; j < nr_of_rules; j++)
	        calls_left[i * nr_of_rules + j] = 0;
	};

static void detect_call_in_member (rule srule, member m)
	{ if (m -> tag == tag_call)
	     { int rulenumber = srule -> number;
	       rule callee = m -> u.call.def;
	       if (!callee -> ext)
	          calls_left [callee -> number * nr_of_rules + rulenumber] = 1;
	     };
	};

static void detect_calls_in_alt (rule srule, alt a)
	{ member_list mems = a -> members;
	  int ix;
	  if (mems == member_list_nil) return;
	  for (ix = 0; ix < mems -> size; ix++)
	     { detect_call_in_member (srule, mems -> array[ix]);
	       if (mems -> array[ix] -> empty == never_produces_empty)
		  return;
	     };
	};

static void detect_calls_in_rule (rule srule)
	{ alt_list alts = srule -> alts;
	  int ix;
	  if (srule -> ext) return;
	  if (srule -> kind & (rule_predicate | rule_semipredicate)) return;
	  if (!srule -> reachable) return;
	  for (ix = 0; ix < alts -> size; ix++)
	     detect_calls_in_alt (srule, srule -> alts -> array[ix]);
	};

static void detect_calls_in_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     detect_calls_in_rule (all_rules[ix]);
	};

/*
   Determine which non predicate rules call each other either on the left
   in one or more steps by taking the closure of the calls_left relation
   leftc_relation [j * nr_of_syntax_rules + i] = 1 <=>
   j is called by i in one or more steps as a (possibly) leftmost member <=>
   rule j <LC+ rule i
*/
char *leftc_relation;
static void take_calls_closure ()
	{ leftc_relation = warshall (nr_of_rules, calls_left);
	};

/*
   report primary lc relation
*/
static void try_report_primary_lc_relation ()
	{ int i,j;
	  if (!full_verbose) return;
	  warning ("Dump of primary lc relation:");
	  for (i = 0; i < nr_of_rules; i++)
	     { string nont_i = all_rules[i] -> nonterminal;
	       for (j = 0; j < nr_of_rules; j++)
		  if (leftc_relation [i * nr_of_rules + j])
		     { string nont_j = all_rules[j] -> nonterminal;
		       warning ("rule %s <LC+ rule %s", nont_i, nont_j);
		     };
	     };
	};

static void complain_on_left_recursive_rules ()
	{ int i;
	  int leftrecursive = 0;
	  for (i = 0; i < nr_of_rules; i++)
	     if (leftc_relation [i * nr_of_rules + i])
	        { rule srule = all_rules[i];
	          error ("rule %s is left recursive", srule -> nonterminal);
		  leftrecursive ++;
		};
	  if (leftrecursive)
	     panic ("found %d left recursive rule%s", leftrecursive,
		    (leftrecursive > 1)?"s":"");
	};

void check_recursion ()
	{ warning ("detecting left recursive meta rules...");
	  allocate_space_for_mcalls ();
	  detect_lcalls_in_metarules ();
	  take_lcalls_closure ();
	  complain_on_leftrecursive_meta_rules ();
	  warning ("computing primary leftcorner relation...");
	  allocate_space_for_calls ();
	  detect_calls_in_rules ();
	  take_calls_closure ();
	  try_report_primary_lc_relation ();
	  if (!topdown) return;
	  complain_on_left_recursive_rules ();
	};
