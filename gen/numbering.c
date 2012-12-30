/*
   File: numbering.c
   Numbers all rules and meta rules so that we may use these
   as indexes in matrices to derive the closure of the left
   corner relation, do left recursion detection, set of items
   construction, etc. Some statistics are also gathered.
  
   CVS ID: "$Id: numbering.c,v 1.3 2004/12/26 00:48:46 marcs Exp $"
*/

/* general includes */
#include <stdio.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_error.h>
#include <ebs_memalloc.h>

/* local includes */
#include "tree.h"
#include "numbering.h"
#include "main.h"

int nr_of_rules;
int nr_of_alts;
int nr_of_members;
int max_members_per_alt;

rule *all_rules;
alt *all_alts;
member *all_members;

int nr_of_meta_rules;
int nr_of_meta_alts;
meta_rule *all_meta_rules;
meta_alt *all_meta_alts;

int nr_of_elements;
element_def *all_elements;

/*
   initialization
*/
static void init_numbering ()
	{ nr_of_rules = 0;
	  nr_of_alts = 0;
	  nr_of_members = 0;
	  max_members_per_alt = 0;
	  nr_of_meta_rules = 0;
	  nr_of_meta_alts = 0;
	  nr_of_elements = 0;
	};

/*
   Count all rules, alternatives and members.
*/
static void number_member (member m)
	{ m -> number = nr_of_members;
	  nr_of_members++;
	};

static void number_alt (int rule_nr, alt a)
	{ member_list mems = a -> members;
	  int ix;

	  a -> number = nr_of_alts;
	  nr_of_alts++;
	  a -> rule_nr = rule_nr;
	  if (mems == member_list_nil) return;		/* external rule */
	  if (mems -> size > max_members_per_alt)
	     max_members_per_alt = mems -> size;
	  for (ix = 0; ix < mems -> size; ix++)
	     number_member (mems -> array[ix]);
	};

static void number_rule (rule rule)
	{ alt_list alts = rule -> alts;
	  int ix;

	  rule -> number = nr_of_rules;
	  nr_of_rules++;
	  for (ix = 0; ix < alts -> size; ix++)
	     number_alt (rule -> number, alts -> array[ix]);
	};

static void number_rules (rule_tree root)
	{ if (root == rule_tree_nil) return;
	  number_rule (root -> node);
	  number_rules (root -> left);
	  number_rules (root -> right);
	};

/*
   Count all meta rules and meta alternatives.
*/
static void number_meta_alt (meta_alt a)
	{ a -> number = nr_of_meta_alts;
	  nr_of_meta_alts++;
	};

static void number_meta_rule (meta_rule mrule)
	{ int ix;
	  meta_alt_list alts = mrule -> alts;
	  mrule -> number = nr_of_meta_rules;
	  nr_of_meta_rules++;
	  if (alts == meta_alt_list_nil) return;	/* external meta rule */
	  for (ix = 0; ix < alts -> size; ix++)
	     number_meta_alt (alts -> array[ix]);
	};

static void number_meta_rules (meta_tree root)
	{ if (root == meta_tree_nil) return;
	  number_meta_rule (root -> node);
	  number_meta_rules (root -> left);
	  number_meta_rules (root -> right);
	};

/*
   Count all elements
*/
static void number_element (element_def node)
	{ node -> number = nr_of_elements;
	  nr_of_elements++;
	};

static void number_elements (element_tree root)
	{ if (root == element_tree_nil) return;
	  number_element (root -> node);
	  number_elements (root -> left);
	  number_elements (root -> right);
	};

/*
   Allocate storage to hold pointers to all rules, alternatives,
   members, meta rules and meta alternatives.
*/
static void allocate_rule_storage ()
	{ all_rules = (rule *) ckcalloc (nr_of_rules, sizeof (rule));
	  all_alts = (alt *) ckcalloc (nr_of_alts, sizeof (alt));
	  all_members = (member *) ckcalloc (nr_of_members, sizeof (member));
	  all_meta_rules = (meta_rule *) ckcalloc
				(nr_of_meta_rules, sizeof (meta_rule));
	  all_meta_alts = (meta_alt *) ckcalloc
				(nr_of_meta_alts, sizeof (meta_alt));
	};

static void allocate_element_storage ()
	{ all_elements = (element_def *) ckcalloc
				(nr_of_elements, sizeof (element_def));
	};

/*
   Insert pointers to all rules, alternatives and members
*/
static void put_member_in_row (member m)
	{ all_members [m -> number] = m;
	};

static void put_alt_in_row (alt a)
	{ int ix;
	  member_list mems = a -> members;
	  all_alts [a -> number] = a;
	  if (mems == member_list_nil) return;		/* external rule */
	  for (ix = 0; ix < mems -> size; ix++)
	     put_member_in_row (mems -> array[ix]);
	};

static void put_rule_in_row (rule srule)
	{ int ix;
	  alt_list alts = srule -> alts;
	  all_rules [srule -> number] = srule;
	  for (ix = 0; ix < alts -> size; ix++)
	     put_alt_in_row (alts -> array[ix]);
	};

static void put_rules_in_row (rule_tree root)
	{ if (root == rule_tree_nil) return;
	  put_rule_in_row (root -> node);
	  put_rules_in_row (root -> left);
	  put_rules_in_row (root -> right);
	};

/*
   Insert pointers to all meta rules and alternatives
*/
static void put_meta_alt_in_row (meta_alt a)
	{ all_meta_alts [a -> number] = a;
	};

static void put_meta_rule_in_row (meta_rule rule)
	{ int ix;
	  meta_alt_list alts = rule -> alts;
	  all_meta_rules [rule -> number] = rule;
	  if (alts == meta_alt_list_nil) return;
	  for (ix = 0; ix < alts -> size; ix++)
	     put_meta_alt_in_row (alts -> array[ix]);
	};

static void put_meta_rules_in_row (meta_tree root)
	{ if (root == meta_tree_nil) return;
	  put_meta_rule_in_row (root -> node);
	  put_meta_rules_in_row (root -> left);
	  put_meta_rules_in_row (root -> right);
	};

/*
   Insert pointers to all elements
*/
static void put_element_in_row (element_def node)
	{ all_elements [node -> number] = node;
	};

static void put_elements_in_row (element_tree root)
	{ if (root == element_tree_nil) return;
	  put_element_in_row (root -> node);
	  put_elements_in_row (root -> left);
	  put_elements_in_row (root -> right);
	};

/*
   numbering driver1
*/
void do_numbering (int pred)
	{ warning ("numbering...");
	  init_numbering ();
	  if (!pred) number_alt (-1, start_alt);
	  number_rules (rule_root);
	  number_meta_rules (meta_root);
	  allocate_rule_storage ();
	  if (!pred) put_alt_in_row (start_alt);
	  put_rules_in_row (rule_root);
	  put_meta_rules_in_row (meta_root);
	  hint ("found %d syntax rules, %d alternatives, %d members",
			nr_of_rules, nr_of_alts, nr_of_members);
	  hint ("found %d meta rules, %d meta alternatives",
			nr_of_meta_rules, nr_of_meta_alts);
	};

void do_element_numbering ()
	{ warning ("numbering elements...");
	  number_elements (element_root);
	  allocate_element_storage ();
	  put_elements_in_row (element_root);
	  hint ("found %d elements", nr_of_elements);
	};
