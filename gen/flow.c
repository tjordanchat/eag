/*
   File: flow.c
   Defines a flow check of the eag
  
   CVS ID: "$Id: flow.c,v 1.3 2004/12/26 00:48:45 marcs Exp $"
*/

/* general includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_error.h>

/* local includes */
#include "limits.h"
#include "tree.h"
#include "numbering.h"
#include "main.h"
#include "typing.h"
#include "flow.h"

/*
   check indicated flow in rules
*/
static int flow_errors;
static void init_flow ()
	{ flow_errors = 0;
	};

static void flow_error (char *format, ...)
	{ char buf[MAXSTRLEN];
	  va_list arg_ptr;
	  va_start (arg_ptr, format);
	  vsprintf (buf, format, arg_ptr);
	  va_end (arg_ptr);

	  flow_errors++;
	  error ("flow error: %s", buf);
	};

#define flow_mismatch_error \
"Flow mismatch between alternatives in rule %s, alt %d"
static void merge_flow_in_position (rule srule, int altnr, pos protops, pos p)
	{ if (protops -> kind == undefinedflow)
	     protops -> kind = p -> kind;
	  else if (protops -> kind != p -> kind)
	     flow_error (flow_mismatch_error, srule -> nonterminal, altnr);
	};

static void merge_flow_in_displays (rule srule, int altnr, pos_list proto, 
				     pos_list dpy)
	{ int ix;
	  if (dpy == pos_list_nil) return;
	  for (ix = 0; ix < proto -> size; ix++)
	     merge_flow_in_position (srule, altnr,
				     proto -> array[ix], dpy -> array[ix]);
	};

static void deduce_initial_flow_in_alt (rule srule, int altnr, alt a)
	{ merge_flow_in_displays (srule, altnr,
				  srule -> proto_display, a -> display);
	};

static void deduce_initial_flow_in_alts (rule srule, alt_list alts)
	{ int ix;
	  for (ix = 0; ix < alts -> size; ix++)
	     deduce_initial_flow_in_alt (srule, ix + 1, alts -> array[ix]);
	};

static void deduce_initial_flow_in_rule (rule srule)
	{ deduce_initial_flow_in_alts (srule, srule -> alts);
	};

static void deduce_initial_flow_in_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     deduce_initial_flow_in_rule (all_rules[ix]);
	};

/*
   Determine which predicates are actually semipredicates
   by noting if they have no inherited positions at all
*/
static int no_inherited_positions (pos_list proto)
	{ int ix;
	  for (ix = 0; ix < proto -> size; ix++)
	     if (proto -> array[ix] -> kind == inherited)
		return (0);
	  return (1);
	};

static void check_if_semipredicate (rule srule)
	{ if (srule -> ext) return;
	  if (!(srule -> kind & rule_predicate)) return;
	  if (no_inherited_positions (srule -> proto_display))
	     { srule -> kind &= ~rule_predicate;
	       srule -> kind |= rule_semipredicate;
	     };
	};

static void check_for_semipredicates ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     check_if_semipredicate (all_rules[ix]);
	};

void flow_check ()
	{ warning ("checking flow...");
	  init_flow ();
	  deduce_initial_flow_in_rules ();
	  if (flow_errors)
	     panic ("%d flow error%s w%s found", flow_errors,
		    (flow_errors == 1)?"":"s", (flow_errors == 1)?"as":"ere");
	  hint ("determing semipredicates...");
	  check_for_semipredicates ();
	};
