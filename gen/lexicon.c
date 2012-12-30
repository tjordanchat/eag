/*
   File: lexicon.c
   Checks the rules in the grammar that belong to the lexicon
  
   CVS ID: "$Id: lexicon.c,v 1.3 2004/12/26 00:48:46 marcs Exp $"
*/

/* general includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_error.h>
#include <ebs_memalloc.h>
#include <ebs_textstorage.h>
#include <ebs_value.h>
#include <ebs_lexds.h>

/* libeag includes */
#include <eag_nodeinfo.h>

/* local includes */
#include "limits.h"
#include "tree.h"
#include "numbering.h"
#include "main.h"
#include "lexicon.h"

/*
   The first part checks whether the rules in the lexicon comply
   with the following rules:
   1) Their right hand sides should have one or 0 members
   2) When present, the member must be a terminal
   3) All affixes in the left hand side are static
      (they can be evaluated at compile time)
*/
int lexicon_present;
static int lexicon_errors;
static int est_nr_lex_rules;
static void init_lexicon_check ()
	{ lexicon_present = 0;
	  lexicon_errors = 0;
	  est_nr_lex_rules = 0;
	};

static void lexicon_error (char *format, ...)
	{ char buf[MAXSTRLEN];
	  va_list arg_ptr;
	  va_start (arg_ptr, format);
	  vsprintf (buf, format, arg_ptr);	
	  va_end (arg_ptr);
	  lexicon_errors++;
	  error ("lexicon error: %s", buf);
	};

#define lhs_error1 \
"position %d in lexicon rule %s, alt %d does not have a static value"
#define lhs_error2 \
"affix %s in lexicon rule %s, alt %d should have a static value"
static void check_lexicon_lhs_expr (rule lrule, int altnr, int posnr, expr ex)
	{ affix a;
	  if (ex -> tag != tag_single)
	     { lexicon_error (lhs_error1, posnr, lrule -> nonterminal, altnr);
	       return;
	     };
	  a = ex -> u.single;
	  if (a -> val == value_nil)
	     lexicon_error (lhs_error2, a -> name, posnr, lrule -> nonterminal);
	};

static void check_lexicon_lhs (rule lrule, int altnr, alt a)
	{ pos_list dpy = a -> display;
	  int ix;
	  for (ix = 0; ix < dpy -> size; ix++)
	     check_lexicon_lhs_expr (lrule, altnr, ix + 1, 
				     dpy -> array[ix] -> ex);
	};

#define rhs_error1 \
"right hand side of lexicon rule %s, alt %d is not a terminal"
#define rhs_error2 \
"right hand side of lexicon rule %s, alt %d has more than one member"
static void check_lexicon_member (rule lrule, int altnr, alt a)
	{ member_list ml = a -> members;
	  switch (ml -> size)
	     { case 1:
		  if (ml -> array[0] -> tag != tag_terminal)
		     lexicon_error (rhs_error1, lrule -> nonterminal, altnr);
	       case 0: break; /* empty is ok */
	       default:
		  lexicon_error (rhs_error2, lrule -> nonterminal, altnr);
	     };	
	};

static void check_lexicon_alt (rule lrule, int altnr, alt a)
	{ check_lexicon_lhs (lrule, altnr, a);
	  check_lexicon_member (lrule, altnr, a);
	};

static void check_lexicon_rule (rule lrule)
	{ alt_list alts = lrule -> alts;
	  int ix;
	  est_nr_lex_rules++;
	  for (ix = 0; ix < alts -> size; ix++)
	     check_lexicon_alt (lrule, ix + 1, alts -> array[ix]);
	};

static void check_lexicon_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     { rule srule = all_rules[ix];
	       if (srule -> lex) check_lexicon_rule (srule);
	       if (ignore_lexicon) srule -> lex = 0;
	     };
	};

void check_lexicon ()
	{ warning ("checking lexicon rules...");
	  init_lexicon_check ();
	  check_lexicon_rules ();
	  if (lexicon_errors)
	     panic ("%d error%s w%s found in lexicon rules", lexicon_errors,
		    (lexicon_errors == 1)?"":"s",
		    (lexicon_errors == 1)?"as":"ere");
	  hint ("found %d rule%s in lexicon", est_nr_lex_rules,
		(est_nr_lex_rules == 1)?"":"s");
	};

/*
   Collect all values in a simple list using a binary tree.
   When entered, addto_values will return the index in the list.
*/
typedef struct value_tree_rec
	{ value val;
	  int nr;
	  struct value_tree_rec *left;
	  struct value_tree_rec *right;
	} *value_tree;
#define value_tree_nil ((value_tree) NULL)

static lexicon the_lexicon;
static value_tree value_root;
static void init_lexicon_preparation ()
	{ int_list lex_lnames = new_int_list ();
	  value_list lex_vals = new_value_list (nr_of_alts); /* heuristics */
	  lextree_list lex_trees = new_lextree_list (est_nr_lex_rules);
	  the_lexicon = new_lexicon (lex_lnames, lex_vals, lex_trees);
	  value_root = value_tree_nil;
	};

static int addto_values (value v)
	{ value_tree *ptr = &value_root;
	  while (1)
	     { if (*ptr == value_tree_nil)
	          { int nr = the_lexicon -> lex_vals -> size;
		    *ptr = (value_tree)
				ckmalloc (sizeof (struct value_tree_rec));
		    (*ptr) -> val = rdup_value (v);
		    (*ptr) -> nr = nr;
	            (*ptr) -> left = value_tree_nil;
		    (*ptr) -> right = value_tree_nil;
		    app_int_list (the_lexicon -> lex_lnames, v -> admin_nr);
		    app_value_list (the_lexicon -> lex_vals, rdup_value (v));
		    return (nr);
		  };
	       if (less_value (v, (*ptr) -> val)) ptr = &((*ptr) -> left);
	       else if (less_value ((*ptr) -> val, v))
		  ptr = &((*ptr) -> right);
	       else return ((*ptr) -> nr);
	     };
	};

static int prepare_lexicon_expr (expr ex)
	{ affix a = ex -> u.single;
	  return (addto_values (a -> val));
	};

static lexend prepare_lexicon_lhs (alt a)
	{ int_list valuenrs = new_int_list ();
	  pos_list dpy = a -> display;
	  int ix;
	  for (ix = 0; ix < dpy -> size; ix++)
	     { expr ex = dpy -> array[ix] -> ex;
	       app_int_list (valuenrs, prepare_lexicon_expr (ex));
	     };
	  return (new_lexend (a -> nodenr, valuenrs));
	};

static lextree make_initial_lextree (int room)
	{ string empty_string = addto_names ("");
	  lexend_list loc_prods = new_lexend_list ();
	  lextree_list loc_tails = new_lextree_list (room);
	  return (new_lextree (empty_string, loc_prods, loc_tails));
	};

static int is_initial_lextree (lextree curr_tree)
	{ return ((strlen (curr_tree -> remainder) == 0) &&
		  (curr_tree -> prods -> size == 0) &&
		  (curr_tree -> tails -> size == 0));
	};

static void make_deeper_lextree (lextree curr_tree)
	{ string empty_string = addto_names ("");
	  string new_remainder;
	  lextree_list new_tails;
	  lextree new;
	  int curr, ix;
	  if (!strlen (curr_tree -> remainder) || curr_tree -> tails -> size)
	     internal_error ("make_deeper_lextree");

	  /* make subtree */
	  curr = curr_tree -> remainder[0];	/* pick head */
	  new_remainder = addto_names (curr_tree -> remainder + 1);
	  new_tails = new_lextree_list (1);
	  new = new_lextree (new_remainder, curr_tree -> prods, new_tails);

	  /* update the current tree */
	  curr_tree -> remainder = empty_string;
	  curr_tree -> prods = new_lexend_list ();
	  for (ix = 0; ix < curr; ix++)
	     app_lextree_list (curr_tree -> tails, make_initial_lextree (1));
	  app_lextree_list (curr_tree -> tails, new);
	};

static void make_wider_lextree (lextree curr_tree, int new_index)
	{ lextree_list tails = curr_tree -> tails;
	  int ix;
	  for (ix = tails -> size; ix <= new_index; ix++)
	     app_lextree_list (tails, make_initial_lextree (1));
	};

static void enter_terminal (lextree rev_tree, lexend le, string terminal)
	{ lextree curr_tree = rev_tree;
	  string tptr = terminal;
	  while (*tptr)
	     { int curr;

	       /* strlen (tptr) > 0 */
	       if (is_initial_lextree (curr_tree))
		  { /* make it a remaindered one */
		    curr_tree -> remainder = addto_names (tptr);
		    app_lexend_list (curr_tree -> prods, le);
		    return;
		  };
	       if (streq (curr_tree -> remainder, tptr))
		  { /* it has the same remainder as we, add production */
		    app_lexend_list (curr_tree -> prods, le);
		    return;
		  };
	       /* if it has a remainder, make the tree deeper */
	       if (strlen (curr_tree -> remainder))
		  make_deeper_lextree (curr_tree);
	       /* make sure it can fit our index */
	       curr = *tptr++;
	       make_wider_lextree (curr_tree, curr);
	       curr_tree = curr_tree -> tails -> array[curr];
	     };
	  /*
	     we have exhausted the terminal; if the current tree is
	     a remaindered one, make it deeper and add my production
	  */
	  if (strlen (curr_tree -> remainder))
	     make_deeper_lextree (curr_tree);
	  app_lexend_list (curr_tree -> prods, le);
	};

static void prepare_lexicon_rhs (lextree rev_tree, lexend le,
				  member_list members)
	{ switch (members -> size)
	     { case 1:
		  enter_terminal (rev_tree, le,
				  members -> array[0] -> u.terminal);
		  break;
	       case 0:
		  { if (strlen (rev_tree -> remainder))
		       make_deeper_lextree (rev_tree);
		    app_lexend_list (rev_tree -> prods, le);
		  }; break;
	       default: bad_tag (members -> size, "enter_alt_rhs");
	     };
	};

static void prepare_lexicon_alt (lextree rev_tree, alt a)
	{ lexend le = prepare_lexicon_lhs (a);
	  prepare_lexicon_rhs (rev_tree, le, a -> members);
	};

static void prepare_lexicon_rule (rule lrule)
	{ lextree rev_tree = make_initial_lextree (128);
	  alt_list alts = lrule -> alts;
	  int ix;

	  app_lextree_list (the_lexicon -> lex_trees, rev_tree);
	  lrule -> lex = the_lexicon -> lex_trees -> size;	/* > 0 */
	  /* if (placeholders) add_placeholder_alts */
	  for (ix = 0; ix < alts -> size; ix++)
	     prepare_lexicon_alt (rev_tree, alts -> array[ix]);
	  lexicon_present = 1;
	};

static void prepare_lexicon_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     { rule srule = all_rules[ix];
	       if (srule -> ext || !srule -> reachable) continue;
	       if (srule -> lex) prepare_lexicon_rule (srule);
	     };
	};

static void save_lexicon_rules (string basename)
	{ char fname[MAXFNAME];
	  sprintf (fname, "%s.clx", basename);
	  csave_lexicon (fname, the_lexicon);
	};

static void dump_lexicon_rules ()
	{ value_list lex_vals = the_lexicon -> lex_vals;
	  int_list lex_lnames = the_lexicon -> lex_lnames;
	  int lex_size = the_lexicon -> lex_trees -> size;
	  int ix;
	  if (!full_verbose) return;
	  if (lex_size)
	     hint ("compiled %d nonterminal%s into lexicon", lex_size,
		   (lex_size == 1)?"":"s");
	  if (!lex_vals -> size) return;
	  hint ("collected %d different values in lexicon", lex_vals -> size);
	  for (ix = 0; ix < lex_vals -> size; ix++)
	     { eprint_log ("value %d, lnr %d: ", ix, lex_lnames -> array[ix]);
	       dump_value (lex_vals -> array[ix]);
	       eprint_log ("\n");
	     };
	}; 

void prepare_lexicon (string basename)
	{ if (ignore_lexicon) return;
	  if (!est_nr_lex_rules) return;
	  warning ("preparing lexicon rules...");
	  init_lexicon_preparation ();
	  prepare_lexicon_rules ();
	  dump_lexicon_rules ();
	  if (lexicon_present)
	     save_lexicon_rules (basename);
	};
