/*
   File: constant.c
   Does constant propagation across all rules and meta rules
   This pass is necessary for implementing the finite lattices
  
   CVS ID: "$Id: constant.c,v 1.4 2008/06/18 12:27:01 marcs Exp $"
*/

/* general includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_error.h>
#include <ebs_textstorage.h>
#include <ebs_value.h>

/* local includes */
#include "tree.h"
#include "gentree.h"
#include "numbering.h"
#include "ident.h"
#include "typing.h"
#include "constant.h"
#include "main.h"

static void init_propagation ()
	{ meta_rule nil_def = lookup_meta_rule ("nil");
	  if (nil_def == meta_rule_nil)
	     panic ("somebody erased nil from stddefs.eag");
	  nil_def -> mvalue = new_tuple_value (new_value_list (0));
	};

static value affix_variable_value (affix a)
	{ meta_rule mrule = a -> u.var.def;
	  if (mrule == meta_rule_nil) return (value_nil);
	  return (rdup_value (mrule -> mvalue));	/* also value_nil */
	};

static value affix_element_value (affix a)
	{ element_def def = a -> u.elt.def;
	  value sval = new_singleton_value (def -> eltnr, def -> set -> defs -> size, NULL);
	  sval -> admin_nr = def -> set -> number;
	  return (sval);
	};

static value affix_value (affix a)
	{ switch (a -> tag)
	     { case tag_affix_variable: return (affix_variable_value (a));
	       case tag_affix_terminal: return (new_string_value (a -> u.str));
	       case tag_affix_integer:	return (new_integer_value (a -> u.inum));
	       case tag_affix_real:	return (new_real_value (a -> u.rnum));
	       case tag_affix_semi:	return (value_nil);
	       case tag_affix_element:	return (affix_element_value (a));
	       default: bad_tag (a -> tag, "affix_value");
	     };
	  return (value_nil);
	};

static value compose_affix_values (affix_list affl)
	{ int ix;
	  value_list vl = new_value_list (affl -> size);
	  for (ix = 0; ix < affl -> size; ix++)
	     { value val = affix_value (affl -> array[ix]);
	       if (val == value_nil)
		  { rfre_value_list (vl);
		    return (value_nil);
	          };
	       app_value_list (vl, val);
	     };
	  return (new_tuple_value (vl));
	};

static value concatenate_affix_values (affix_list affl)
	{ int ix;
	  value_list vl = new_value_list (affl -> size);
	  value new;
	  for (ix = 0; ix < affl -> size; ix++)
	     { value val = affix_value (affl -> array[ix]);
	       if (val == value_nil)
		  { rfre_value_list (vl);
		    return (value_nil);
		  };
	       app_value_list (vl, val);
	       if (val -> tag != vl -> array[0] -> tag)
		  internal_error ("concatenate_affix_values");
	     };
	  new = concatenate_values (vl);
	  rfre_value_list (vl);
	  return (new);
	};

static value unify_affix_values (affix_list affl)
	{ int ix;
	  value_list vl = new_value_list (affl -> size);
	  value new;
	  for (ix = 0; ix < affl -> size; ix++)
	     { value val = affix_value (affl -> array[ix]);
	       if (val == value_nil)
		  { rfre_value_list (vl);
		    return (value_nil);
		  };
	       app_value_list (vl, val);
	       if (val -> tag != vl -> array[0] -> tag)
		  internal_error ("unify_affix_values");
	     };
	  new = join_lattice_values (vl);
	  new -> admin_nr = vl -> array[0] -> admin_nr;
	  rfre_value_list (vl);
	  return (new);
	};

static value expression_value (expr e)
	{ if (e == expr_nil)		/* empty meta alt */
	     return (new_string_value (""));
	  switch (e -> tag)
	     { case tag_single: return (affix_value (e -> u.single));
	       case tag_compos: return (compose_affix_values (e -> u.compos));
	       case tag_concat:
		  return (concatenate_affix_values (e -> u.concat));
	       case tag_union: return (unify_affix_values (e -> u.uni));
	       default: bad_tag (e -> tag, "expression_value");
	     };
	  return (value_nil);
	};

static value alts_value (meta_alt_list alts)
	{ value val0 = expression_value (alts -> array[0] -> e);
	  value_list vl;
	  value new;
	  int ix;
	  if (val0 == value_nil) return (val0);
	  if (alts -> size <= 1) return (val0);
	  if ((val0 -> tag != small_lattice_value) &&
	      (val0 -> tag != large_lattice_value))
	     { rfre_value (val0);
	       return (value_nil);
	     };
	  vl = new_value_list (alts -> size);
	  app_value_list (vl, val0);
	  for (ix = 1; ix < alts -> size; ix++)
	     { value val = expression_value (alts -> array[ix] -> e);
	       if (val == value_nil)
		  { rfre_value_list (vl);
		    return (value_nil);
		  };
	       app_value_list (vl, val);
	       if (val -> tag != vl -> array[0] -> tag)
		  internal_error ("alts_value");
	     };
	  new = join_lattice_values (vl);
	  new -> admin_nr = vl -> array[0] -> admin_nr;
	  rfre_value_list (vl);
	  return (new);
	};

static int change;
static void propagate_meta_rule_value (meta_rule mrule)
	{ value new;
	  if (mrule -> ext) return;		/* predefined meta_rule */
	  if (mrule -> mvalue != value_nil) return;
	  new = alts_value (mrule -> alts);
	  if (new != value_nil)
	     { mrule -> mvalue = new;
	       change = 1;
	     };
	};

static void propagate_meta_rule_values ()
	{ int ix;
	  for (ix = 0; ix < nr_of_meta_rules; ix++)
	     propagate_meta_rule_value (all_meta_rules[ix]);
	};

static void finish_meta_rule (meta_rule mrule)
	{ if (mrule -> mvalue == value_nil)
	     mrule -> kind = recognizer_meta_value;
	  else
	     { mrule -> kind = single_meta_value;
	       if (full_verbose)
		  { eprint_log ("meta rule %s has value: ",
		    mrule -> nonterminal);
		    dump_value (mrule -> mvalue);
		    eprint_log ("\n");
		  };
	     };
	};

static void finish_meta_rules ()
	{ int ix;
	  hint ("The following meta rules have a single value:");
	  for (ix = 0; ix < nr_of_meta_rules; ix++)
	     finish_meta_rule (all_meta_rules[ix]);
	};

static value evaluate_constant_affix (affix a)
	{ switch (a -> tag)
	     { case tag_affix_variable:
		  { meta_rule def = a -> u.var.def;
		    if (def == meta_rule_nil) return (value_nil);
		    a -> val = rdup_value (def -> mvalue);
		    if (a -> val == value_nil) return (value_nil);
		    if (!(def -> type & lattice_type))
		       return (rdup_value (a -> val));
		    if (a -> ref_count > 1) return (value_nil);
		  }; break;
	       case tag_affix_terminal:
		  a -> val = new_string_value (a -> u.str);
		  break;
	       case tag_affix_integer:
		  a -> val = new_integer_value (a -> u.inum);
		  break;
	       case tag_affix_real:
		  a -> val = new_real_value (a -> u.rnum);
		  break;
	       case tag_affix_element:
		  { element_def def = a -> u.elt.def;
		    value sval = new_singleton_value (def -> eltnr,
						      def -> set -> defs -> size, NULL);
		    sval -> admin_nr = def -> set -> number;
		    a -> val = sval;
		  }; break;
	       default: bad_tag (a -> tag, "evaluate_constant_affix");
	     };
	  return (rdup_value (a -> val));
	};

static affix gather_constant_affix (alt a, expr ex)
	{ string name = generate_anonymous_affixname ();
	  affix new = new_affix_constant (ex);
	  new -> name = name;
	  new -> ref_count = 1;
	  new -> next = a -> locals;
	  a -> locals = new;
	  return (new);
	};

static void evaluate_constants_in_composition (alt a, expr ex)
	{ affix_list affl = ex -> u.compos;
	  value_list vl = new_value_list (affl -> size);
	  affix new;
	  int ix;
	  int eval_all = 1;
	  for (ix = 0; ix < affl -> size; ix++)
	     { value val = evaluate_constant_affix (affl -> array[ix]);
	       if (val == value_nil) eval_all = 0;
	       app_value_list (vl, val);
	     };
	  if (!eval_all)
	     { rfre_value_list (vl);
	       return;
	     };
	  /* replace expression */
	  new = gather_constant_affix (a, new_expr_compos (affl));
	  new -> type = tuple_type;
	  new -> val = new_tuple_value (vl);
	  ex -> tag = tag_single;
	  ex -> u.single = new;
	};

static void evaluate_constants_in_concatenation (alt a, expr ex)
	{ affix_list affl = ex -> u.concat;
	  value_list vl = new_value_list (affl -> size);
	  affix new;
	  int ix;
	  int eval_all = 1;
	  for (ix = 0; ix < affl -> size; ix++)
	     { value val = evaluate_constant_affix (affl -> array[ix]);
	       if (val == value_nil) eval_all = 0;
	       app_value_list (vl, val);
	     };
	  if (!eval_all)
	     { rfre_value_list (vl);
	       return;
	     };
	  /* replace expression */
	  new = gather_constant_affix (a, new_expr_concat (affl));
	  new -> val = concatenate_values (vl);
	  if (new -> val -> tag == string_value) new -> type = string_type;
	  else new -> type = integer_type;
	  ex -> tag = tag_single;
	  ex -> u.single = new;
	};

static void evaluate_constants_in_union (alt a, expr ex)
	{ affix_list affl = ex -> u.uni;
	  value_list vl = new_value_list (affl -> size);
	  affix new;
	  int ix;
	  int eval_all = 1;
	  for (ix = 0; ix < affl -> size; ix++)
	     { value val = affix_value (affl -> array[ix]);
	       if (val == value_nil) eval_all = 0;
	       app_value_list (vl, val);
	     };
	  if (!eval_all)
	     { rfre_value_list (vl);
	       return;
	     };
	  /* replace expression */
	  new = gather_constant_affix (a, new_expr_union (affl));
	  new -> val = join_lattice_values (vl);
	  new -> val -> admin_nr = vl -> array[0] -> admin_nr;
	  new -> type = lattice_type;
	  rfre_value_list (vl);
	  ex -> tag = tag_single;
	  ex -> u.single = new;
	};

static void evaluate_constants_in_expr (alt a, expr ex)
	{ if (ex == expr_nil) internal_error ("evaluate_constants_in_expr");
	  switch (ex -> tag)
	     { case tag_single:
		  rfre_value (evaluate_constant_affix (ex -> u.single));
		  break;
	       case tag_compos:
		  evaluate_constants_in_composition (a, ex);
		  break;
	       case tag_concat:
		  evaluate_constants_in_concatenation (a, ex);
	          break;
	       case tag_union:
		  evaluate_constants_in_union (a, ex);
		  break;
	       default: bad_tag (ex -> tag, "evaluate_constants_in_expr");
	     };
	};

static void evaluate_constants_in_display (alt a, pos_list dpy)
	{ int ix;
	  for (ix = 0; ix < dpy -> size; ix++)
	     evaluate_constants_in_expr (a, dpy -> array[ix] -> ex);
	};

static void evaluate_constants_in_member (alt a, member m)
	{ switch (m -> tag)
	     { case tag_call:
		  evaluate_constants_in_display (a, m -> u.call.display);
	       case tag_terminal: break;
	       case tag_semiterminal:
		  evaluate_constants_in_display (a, m -> u.semi.display);
	       case tag_cut: break;
	       default: bad_tag (m -> tag, "evaluate_constants_in_member");
	     };
	};

static void evaluate_constants_in_alt (alt a)
	{ member_list mems = a -> members;
	  int ix;
	  evaluate_constants_in_display (a, a -> display);
	  for (ix = 0; ix < mems -> size; ix++)
	     evaluate_constants_in_member (a, mems -> array[ix]);
	};

static void evaluate_constants_in_rule (rule srule)
	{ alt_list alts;
	  int ix;
	  if (srule -> ext) return;
	  alts = srule -> alts;
	  for (ix = 0; ix < alts -> size; ix++)
	     evaluate_constants_in_alt (alts -> array[ix]);
	};

static void evaluate_constants_in_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     evaluate_constants_in_rule (all_rules[ix]);
	};

void propagate_constants ()
	{ int nr_passes = 0;
	  warning ("propagating constant values...");
	  init_propagation ();
	  do
	     { change = 0;
	       nr_passes++;
	       propagate_meta_rule_values ();
	     }
	  while (change);
	  hint ("needed %d pass%s for evaluating meta rules",
		nr_passes, (nr_passes == 1)?"":"es");
	  finish_meta_rules ();
	  warning ("evaluating constant affixes...");
	  evaluate_constants_in_rules ();
	  evaluate_constants_in_alt (start_alt);
	};
