/*
   File: lookahead.c
   Determines FIRST and FOLLOW sets (on character basis)
   to form director sets which may limit backtracking.
  
   CVS ID: "$Id: lookahead.c,v 1.3 2004/12/26 00:48:46 marcs Exp $"
*/

/* general includes */
#include <stdio.h>
#include <string.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_error.h>
#include <ebs_textstorage.h>
#include <ebs_primio.h>

/* Libedt includes */
#include <edt_editor.h>

/* Local includes */
#include "main.h"
#include "tree.h"
#include "numbering.h"
#include "lookahead.h"

/*
   Make an initial assumption about the first sets of rules
*/
static void initial_first_sets_in_terminal (member m)
	{ char first[2];
	  if (strlen (m -> u.terminal) != 0)
	     { first[0] = m -> u.terminal[0];
	       first[1] = '\0';
	       m -> first = addto_names (first);
	     }
	  else m -> first = "";
	};

static int char_in_set (char c, char *s)
	{ register char *sptr;
	  for (sptr = s; *sptr; sptr++)
	     if (c == *sptr) return (1);
	  return (0);
	};

static char *make_complementaryset (char *s)
	{ char buf[129];
	  char *dptr = buf;
	  int i;
	  for (i=1; i <= 127; i++)
	     if (!char_in_set (i, s))
		*dptr++ = i;
	  *dptr = '\0';
	  dptr = addto_names (buf);
	  return (dptr);
	};

static void initial_first_sets_in_semi (member m)
	{ cset set = m -> u.semi.set;
	  if (set -> kind & non)
	     m -> first = make_complementaryset (set -> str);
	  else m -> first = set -> str;
	};

static void initial_first_sets_in_member (member m)
	{ switch (m -> tag)
	     { case tag_call: break;
	       case tag_terminal: initial_first_sets_in_terminal (m); break;
	       case tag_semiterminal: initial_first_sets_in_semi (m); break;
	       case tag_cut: m -> first = ""; break;
	       default: bad_tag (m -> tag, "initial_first_sets_in_member");
	     }
	};

static void initial_first_sets_in_members (member_list mems)
	{ int ix;
	  if (mems == member_list_nil) return;
	  for (ix = 0; ix < mems -> size; ix++)
	     initial_first_sets_in_member (mems -> array[ix]);
	};

static void initial_first_sets_in_alts (alt_list alts)
	{ int ix;
	  for (ix = 0; ix < alts -> size; ix++)
	     initial_first_sets_in_members (alts -> array[ix] -> members);
	};

static void initial_first_sets_in_rule (rule srule)
	{ char buf[3];
	  if (srule -> ext) return;
	  if (srule -> kind & rule_predicate) return;
	  if (srule -> kind & rule_semipredicate) return;
	  initial_first_sets_in_alts (srule -> alts);
	  if (!srule -> placeholder) return;
	  buf[0] = untyped_symbol[0];
	  if (typed_open_symbol[0] != untyped_symbol[0])
	     { buf[1] = typed_open_symbol[0];
	       buf[2] = '\0';
	     }
	  else buf[1] = '\0';
	  srule -> first = addto_names (buf);
	};

static void initial_first_sets_in_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     initial_first_sets_in_rule (all_rules[ix]);
	};

/*
   Incrementally add to the first sets of the rules
   until we reach a fixed point
*/
static void addcharto_set (char *dst, char c)
	{ char *dptr;
	  for (dptr = dst; *dptr; dptr++)
	     if (*dptr == c) return;
	  *dptr++ = c;
	  *dptr = '\0';
	};

static void addto_set (char *dst, char *src)
	{ char *sptr;
	  if (src == string_nil) return;
	  for (sptr = src; *sptr; sptr++)
	     addcharto_set (dst, *sptr);
	};

static void add_first_sets_for_call (char *dst, member m)
	{ rule def = m -> u.call.def;
	  addto_set (dst, def -> first);
	};

static void add_first_sets_for_member (char *dst, member m)
	{ switch (m -> tag)
	     { case tag_call: add_first_sets_for_call (dst, m); break;
	       case tag_terminal:
	       case tag_semiterminal: addto_set (dst, m -> first);
	       case tag_cut: break;
	       default: bad_tag (m -> tag, "add_first_sets_for_member");
	     };
	};

static void add_first_sets_for_members (char *dst, member_list mems)
	{ int ix;
	  if (mems == member_list_nil) return;
	  for (ix = 0; ix < mems -> size; ix++)
	     { add_first_sets_for_member (dst, mems -> array[ix]);
	       if (mems -> array[ix] -> empty == never_produces_empty)
		  break;
	     };
	};

static int change;
static void add_first_sets_for_alt (char *dst, alt a)
	{ char altset[256];
	  altset[0] = '\0';
	  addto_set (altset, a -> first);
	  add_first_sets_for_members (altset, a -> members);
	  if ((a -> first == string_nil) ||
	      (strcmp (a -> first, altset) != 0))
	     { a -> first = addto_names (altset);
	       change = 1;
	     };
	  addto_set (dst, a -> first);
	};

static void find_first_sets_for_rule (rule srule)
	{ char ruleset[256];
	  int ix;
	  if (srule -> ext) return;
	  if (srule -> kind & rule_predicate) return;
	  if (srule -> kind & rule_semipredicate) return;
	  ruleset[0] = '\0';
	  addto_set (ruleset, srule -> first);
	  for (ix = 0; ix < srule -> alts -> size; ix++)
	     add_first_sets_for_alt (ruleset, srule -> alts -> array[ix]);
	  if ((srule -> first == string_nil) ||
	      (strcmp (srule -> first, ruleset) != 0))
	     { srule -> first = addto_names (ruleset);
	       change = 1;
	     };
	};

static void find_first_sets_for_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     find_first_sets_for_rule (all_rules[ix]);
	};

static void determine_first_sets ()
	{ int nrpasses = 0;
	  warning ("determining first sets...");
	  initial_first_sets_in_rules ();
	  do
	     { change = 0;
	       nrpasses++;
	       find_first_sets_for_rules ();
	     }
	  while (change);
	  hint ("used %d passes for first sets", nrpasses);
	};

/*
   Make an initial assumption about the follow sets of all rules
*/
static void initiate_follow_sets ()
	{ char buf[2];
	  buf[0] = EOFCHAR;
	  buf[1] = '\0';
	  start_rule -> u.call.def -> follow = addto_names (buf);
	  if (!placeholderflag) return;
	  buf[0] = untyped_symbol[0];
	  if (typed_open_symbol[0] != buf[0])
	     { buf[1] = typed_open_symbol[0];
	       buf[2] = '\0';
	     }
	  else buf[1] = '\0';
	  lookup_syntax_rule ("layout") -> follow = addto_names (buf);
	};

static void add_first_of_member_to_follow_set (member m, char *s)
	{ switch (m -> tag)
	     { case tag_call:
		  addto_set (s, m -> u.call.def -> first); break;
	       case tag_terminal:
	       case tag_semiterminal: addto_set (s, m -> first);
	       case tag_cut: break;
	       default: bad_tag (m -> tag, "add_first_of_member_to_follow_set");
	     };
	};

static void add_first_of_members_to_follow_set (member_list mems, int from,
						 char *fset)
	{ int ix;
	  for (ix = from; ix < mems -> size; ix++)
	     { add_first_of_member_to_follow_set (mems -> array[ix], fset);
	       if (mems -> array[ix] -> empty == never_produces_empty)
		  return;
	     };
	};

static void try_add_rule_set_to_follow_set (rule srule, member_list mems,
					     int from, char *fset)
	{ int ix;
	  for (ix = from; ix < mems -> size; ix++)
	     if (mems -> array[ix] -> empty == never_produces_empty)
		return;
	  addto_set (fset, srule -> follow);
	};

static void try_update_follow_set (rule srule, member_list mems,
				    int from, rule target)
	{ char fset[256];
	  fset[0] = '\0';
	  if ((target -> kind & rule_predicate) ||
	      (target -> kind & rule_semipredicate)) return;
	  addto_set (fset, target -> follow);	/* first yourself */
	  add_first_of_members_to_follow_set (mems, from, fset);
	  try_add_rule_set_to_follow_set (srule, mems, from, fset);
	  if (strcmp (target -> follow, fset) != 0)
	     { target -> follow = addto_names (fset);
	       change = 1;
	     };
	};

static void determine_follow_sets_in_members (rule srule, member_list mems)
	{ int ix;
	  if (mems == member_list_nil) return;
	  for (ix = 0; ix < mems -> size; ix++)
	     if (mems -> array[ix] -> tag == tag_call)
	        try_update_follow_set (srule, mems, ix + 1,
			mems -> array[ix] -> u.call.def);
	};

static void determine_follow_sets_in_alts (rule srule, alt_list alts)
	{ int ix;
	  for (ix = 0; ix < alts -> size; ix++)
	     determine_follow_sets_in_members (srule,
					       alts -> array[ix] -> members);
	};

static void determine_follow_sets_in_rule (rule srule)
	{ if ((srule -> kind & rule_predicate) ||
	      (srule -> kind & rule_semipredicate)) return;
	  determine_follow_sets_in_alts (srule, srule -> alts);
	};

static void determine_follow_sets_in_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     determine_follow_sets_in_rule (all_rules[ix]);
	};

static void determine_follow_sets ()
	{ int nrpasses = 0;
	  warning ("determining follow sets...");
	  initiate_follow_sets ();
	  nrpasses = 0;
	  do
	     { change = 0;
	       nrpasses++;
	       determine_follow_sets_in_rules ();
	     }
	  while (change);
	  hint ("used %d passes for follow sets", nrpasses);
	};

/*
   Combine first and follow sets into director sets
*/
static void determine_director_sets_in_alt (rule srule, alt a)
	{ char fset[256];
	  fset [0] = '\0';
	  addto_set (fset, a -> first);
	  if (a -> empty == may_produce_empty)
	     addto_set (fset, srule -> follow);
	  a -> director = addto_names (fset);
	};

static void determine_director_set (rule srule)
	{ int ix;
	  if ((srule -> kind & rule_predicate) ||
	      (srule -> kind & rule_semipredicate)) return;
	  for (ix = 0; ix < srule -> alts -> size; ix++)
	     determine_director_sets_in_alt (srule, srule -> alts -> array[ix]);
	};

static void determine_director_sets ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     determine_director_set (all_rules[ix]);
	};

/*
   Report on director sets
*/
static void report_director_set (rule srule)
	{ int ix;
	  wlog ("first (%s) = %s", srule -> nonterminal, srule -> first);
	  wlog ("follow (%s) = %s", srule -> nonterminal, srule -> follow);
	  for (ix = 0; ix < srule -> alts -> size; ix++)
	     wlog ("direct_alt (%d) = %s", ix + 1, 
		   srule -> alts -> array[ix] -> director);
	};

static void report_director_sets ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     report_director_set (all_rules[ix]);
	};

void determine_lookahead_sets ()
	{ determine_first_sets ();
	  determine_follow_sets ();
	  determine_director_sets ();
	  if (full_verbose) report_director_sets ();
	};

char *gather_lcin_director_set (rule srule, member_list mems, int j)
	{ char fset[256];
	  fset[0] = '\0';
	  add_first_of_members_to_follow_set (mems, j + 1, fset);
	  try_add_rule_set_to_follow_set (srule, mems, j + 1, fset);
	  return (addto_names (fset));
	};
