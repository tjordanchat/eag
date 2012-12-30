/*
   File: prepare.c
   Prepares the syntax trees for code generation
   Detects the number of sons per rule
  
   CVS ID: "$Id: prepare.c,v 1.3 2004/12/26 00:48:47 marcs Exp $"
*/

/* general includes */
#include <stdio.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_error.h>
#include <ebs_memalloc.h>
#include <ebs_textstorage.h>

/* local includes */
#include "tree.h"
#include "numbering.h"
#include "main.h"
#include "prepare.h"

int max_nodenr;
static void init_prepare ()
	{ max_nodenr = 2;
	};

static void prepare_member (alt a, member m, int *nrsons)
	{ switch (m -> tag)
	     { case tag_call:
		  if (!layoutflag || (m -> u.call.def != layout_rule))
		     { *nrsons += 1;
		       m -> sonnr = *nrsons;
		     };
		  break;
	       case tag_terminal: break;
	       case tag_semiterminal:
		  { *nrsons += 1;
		    m -> sonnr = *nrsons;
		  };
	       default: break;
	     };
	};

static void prepare_members (alt a, member_list mems, int *nrsons)
	{ int ix;
	  if (mems == member_list_nil) return;
	  for (ix = 0; ix < mems -> size; ix++)
	     prepare_member (a, mems -> array[ix], nrsons);
	};

static void assign_node_nr (alt a)
	{ a -> nodenr = max_nodenr;
	  max_nodenr++;
	};

static void prepare_alt (rule srule, alt a)
	{ prepare_members (a, a -> members, &a -> nrsons);
	  if (layoutflag && (srule == layout_rule)) return;
	  assign_node_nr (a);
	};

static void prepare_alts (rule srule, alt_list alts)
	{ int ix;
	  for (ix = 0; ix < alts -> size; ix++)
	     prepare_alt (srule, alts -> array[ix]);
	};

static void prepare_rule (rule srule)
	{ if (srule -> ext) return;
	  if (!srule -> reachable) return;
	  if (srule -> kind & rule_predicate) max_nodenr++;
	  else if (placeholderflag && (srule -> placeholder)) max_nodenr += 2;
	  prepare_alts (srule, srule -> alts);
	};

static void prepare_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     prepare_rule (all_rules[ix]);
	};

static void mark_rec_in_meta_rule (meta_rule mrule);
static void mark_recbup_in_meta_rule (meta_rule mrule);
static void mark_rec_in_affix (affix a)
	{ if (a -> tag == tag_affix_variable)
	     { meta_rule def = a -> u.var.def;
	       mark_rec_in_meta_rule (def);
	     };
	};

static void mark_recbup_in_affix (affix a)
	{ if (a -> tag == tag_affix_variable)
	     { meta_rule def = a -> u.var.def;
	       mark_recbup_in_meta_rule (def);
	     };
	};

static void mark_rec_in_affix_list (affix_list affl)
	{ int ix;
	  for (ix = 0; ix < affl -> size; ix++)
	     mark_rec_in_affix (affl -> array[ix]);
	};

static void mark_recbup_in_affix_list (affix_list affl)
	{ int ix;
	  for (ix = 0; ix < affl -> size; ix++)
	     mark_recbup_in_affix (affl -> array[ix]);
	};

static void mark_rec_in_expr (expr e)
	{ if (e == expr_nil) return;
	  switch (e -> tag)
	     { case tag_single: mark_rec_in_affix (e -> u.single); break;
	       case tag_compos: mark_rec_in_affix_list (e -> u.compos); break;
	       case tag_concat:
		  mark_recbup_in_affix_list (e -> u.concat); break;
	       case tag_union: internal_error ("mark_rec_in_expr");
	       default: bad_tag (e -> tag, "mark_rec_in_expr");
	     };
	};

static void mark_recbup_in_expr (expr e)
	{ if (e == expr_nil) return;
	  switch (e -> tag)
	     { case tag_single: mark_recbup_in_affix (e -> u.single); break;
	       case tag_concat:
		  mark_recbup_in_affix_list (e -> u.concat); break;
	       case tag_compos: 
	       case tag_union: internal_error ("mark_rec_in_expr");
	       default: bad_tag (e -> tag, "mark_rec_in_expr");
	     };
	};

static void mark_rec_in_meta_rule (meta_rule mrule)
	{ int ix;
	  meta_alt_list alts = mrule -> alts;
	  if (mrule -> ext) return;
	  if (!mrule -> reachable) return;	/* sic */
	  if (mrule -> needs_rec) return;
	  mrule -> needs_rec = 1;
	  for (ix = 0; ix < alts -> size; ix++)
	     mark_rec_in_expr (alts -> array[ix] -> e);
	};

static void mark_recbup_in_meta_rule (meta_rule mrule)
	{ int ix;
	  meta_alt_list alts = mrule -> alts;
	  if (mrule -> ext) return;
	  if (!mrule -> reachable) return;	/* sic */
	  if (mrule -> needs_recbup) return;
	  mrule -> needs_recbup = 1;
	  for (ix = 0; ix < alts -> size; ix++)
	     mark_recbup_in_expr (alts -> array[ix] -> e);
	};

static void prepare_meta_rule (meta_rule mrule)
	{ if (mrule -> ext) return;
	  if (!mrule -> reachable) return;
	  if (mrule -> mvalue == value_nil)
	     mark_rec_in_meta_rule (mrule);
	};

static void prepare_meta_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_meta_rules; ix++)
	     prepare_meta_rule (all_meta_rules[ix]);
	};

void prepare_grammar ()
	{ warning ("preparing rules for codegeneration...");
	  init_prepare ();
	  prepare_rules ();
	  warning ("preparing meta rules for codegeneration...");
	  prepare_meta_rules ();
	};
