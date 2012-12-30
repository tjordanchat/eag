/*
   File: lattice.c
   Constructs the actual lattices
  
   CVS ID: "$Id: lattice.c,v 1.3 2004/12/26 00:48:45 marcs Exp $"
*/

/* general includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_error.h>
#include <ebs_memalloc.h>
#include <ebs_textstorage.h>

/* local includes */
#include "tree.h"
#include "gentree.h"
#include "numbering.h"
#include "typing.h"
#include "lattice.h"
#include "main.h"

/*
   Merge the two element groups
*/
static int change;
static element_set merge_lattices (element_set left, element_set right)
	{ int ix;
	  if (left == element_set_nil) return (right);
	  if (right == element_set_nil) return (left);
	  if (left == right) return (left);
	  for (ix = 0; ix < right -> defs -> size; ix++)
	     { element_def relt = right -> defs -> array[ix];
	       app_element_def_list (left -> defs, relt);
	       relt -> set = left;
	     };
	  change = 1;
	  return (left);
	};

static element_set construct_lattice_from_variable (affix a, element_set mset)
	{ meta_rule mrule = a -> u.var.def;
	  element_set hset;
	  if ((mrule == meta_rule_nil) || (mrule -> ext) ||
	      !(mrule -> type & lattice_type))
	       { error ("found inconsistent variable %s", a -> name);
	         internal_error ("construct_lattice_from_variable");
	       };
	  if (mrule -> lat_repr == element_def_nil) hset = element_set_nil;
	  else hset = mrule -> lat_repr -> set;
	  return (merge_lattices (hset, mset));
	};

static element_set construct_lattice_from_element (affix a, element_set mset)
	{ element_def def =  a -> u.elt.def;
	  if ((def -> set == element_set_nil) && (mset == element_set_nil))
	     { element_def_list ndefs = new_element_def_list ();
	       element_set rset = new_element_set (ndefs);	
	       app_element_def_list (ndefs, def);
	       def -> set = rset;
	       return (rset);
	     };
	  if ((def -> set == element_set_nil) && (mset != element_set_nil))
	     { app_element_def_list (mset -> defs, def);
	       def -> set = mset;
	       return (mset);
	     };
	  return (merge_lattices (def -> set, mset));
	};

static element_set construct_lattice_in_affix (affix a, element_set mset)
	{ switch (a -> tag)
	     { case tag_affix_variable:
		  return (construct_lattice_from_variable (a, mset));
	       case tag_affix_element:
		  return (construct_lattice_from_element (a, mset));
	       default: bad_tag (a -> tag, "construct_lattice_in_affix");
	     };
	  return (element_set_nil);
	};

static element_set construct_lattices_in_affixes (affix_list affs,
						   element_set mset)
	{ element_set rset = mset;
	  int ix;
	  for (ix = 0; ix < affs -> size; ix++)
	     rset = construct_lattice_in_affix (affs -> array[ix], rset);
	  return (rset);
	};

static element_set construct_lattices_in_expr (expr e, element_set mset)
	{ if (e == expr_nil)
	     internal_error ("construct_lattices_in_expr");
	  switch (e -> tag)
	     { case tag_single:
		  return (construct_lattice_in_affix (e -> u.single, mset));
	       case tag_union:
		  return (construct_lattices_in_affixes (e -> u.uni, mset));
	       default: bad_tag (e -> tag, "construct_lattices_in_expr");
	     };
	  return (element_set_nil);
	};

/*
   Lattice coalescing in meta rules
*/
static element_set construct_lattices_in_meta_alts (meta_alt_list alts,
						     element_set mset)
	{ int ix;
	  element_set rset = mset;
	  for (ix = 0; ix < alts -> size; ix++)
	     rset = construct_lattices_in_expr (alts -> array[ix] -> e, rset);
	  return (rset);
	};

static void construct_lattices_in_meta_rule (meta_rule mrule)
	{ element_set rset, mset;
	  if (mrule -> ext) return;
	  if (mrule -> type != lattice_type) return;
	  if (mrule -> lat_repr == element_def_nil) mset = element_set_nil;
	  else mset = mrule -> lat_repr -> set;
	  rset = construct_lattices_in_meta_alts (mrule -> alts, mset);
	  if (rset != mset)
	     { mrule -> lat_repr = rset -> defs -> array[0];
	       change = 1;
	     };
	};

static void construct_lattices_in_meta_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_meta_rules; ix++)
	     construct_lattices_in_meta_rule (all_meta_rules[ix]);
	};

/*
   Lattice coalescing in syntax rules
*/
static void construct_lattices_in_position (pos protopos, pos p)
	{ element_set rset, mset;
	  if (protopos -> lat_repr == element_def_nil) mset = element_set_nil;
	  else mset = protopos -> lat_repr -> set;
	  rset = construct_lattices_in_expr (p -> ex, mset);
	  if (rset != mset)
	     { protopos -> lat_repr = rset -> defs -> array[0];
	       change = 1;
	     };
	};

static void construct_lattices_in_display (pos_list protodpy, pos_list dpy)
	{ int ix;
	  for (ix = 0; ix < dpy -> size; ix++)
	     { pos protopos = protodpy -> array[ix];
	       if (protopos -> type & lattice_type)
		  construct_lattices_in_position (protopos, dpy -> array[ix]);
	     };
	};

static void construct_lattices_in_call (member m)
	{ rule def = m -> u.call.def;
	  pos_list protodpy = def -> proto_display;
	  pos_list display = m -> u.call.display;
	  construct_lattices_in_display (protodpy, display);
	};

static void construct_lattices_in_member (member m)
	{ if (m -> tag != tag_call) return;
	  construct_lattices_in_call (m);
	};

static void construct_lattices_in_members (member_list mems)
	{ int ix;
	  for (ix = 0; ix < mems -> size; ix++)
	     construct_lattices_in_member (mems -> array[ix]);
	};

static void construct_lattices_in_alt (rule srule, alt a)
	{ pos_list protodpy = srule -> proto_display;
	  construct_lattices_in_display (protodpy, a -> display);
	  construct_lattices_in_members (a -> members);
	};

static void construct_lattices_in_rule (rule srule)
	{ int ix;
	  if (srule -> ext) return;
	  for (ix = 0; ix < srule -> alts -> size; ix++)
	     construct_lattices_in_alt (srule, srule -> alts -> array[ix]);
	};

static void construct_lattices_in_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     construct_lattices_in_rule (all_rules[ix]);
	};

element_set_list all_element_sets;
static void assign_numbers_to_group (element_def elt_def)
	{ element_set set = elt_def -> set;
	  int ix;
	  if (elt_def -> eltnr != -1) return;
	  app_element_set_list (all_element_sets, set);
	  set -> number = all_element_sets -> size;
	  for (ix = 0; ix < set -> defs -> size; ix++)
	     { element_def elt = set -> defs -> array[ix];
	       elt -> eltnr = ix;
	     };
	};

static void assign_element_numbers ()
	{ int ix;
	  all_element_sets = new_element_set_list ();
	  for (ix = 0; ix < nr_of_elements; ix++)
	     assign_numbers_to_group (all_elements[ix]);
	};

static void dump_lattices ()
	{ int ix;
	  if (!full_verbose) return;
	  wlog ("%d (super) lattice%s ha%s been found:",
		all_element_sets -> size,
		(all_element_sets -> size == 1)?"":"s",
		(all_element_sets -> size == 1)?"s":"ve");
	  for (ix = 0; ix < all_element_sets -> size; ix++)
	     { element_def_list defs = all_element_sets -> array[ix] -> defs;
	       int iy;
	       eprint_log ("{ ");
	       for (iy = 0; iy < defs -> size; iy++)
		  eprint_log ("%s%s (%d)", ((!iy)?"":(!(iy % 5))?",\n  ":", "),
					   defs -> array[iy] -> eltnm, iy);
	       eprint_log (" }\n");
	     };
	};

void construct_lattices ()
	{ int nr_passes = 0;
	  warning ("coalescing lattices in meta rules...");
	  do
	     { change = 0;
	       nr_passes++;
	       construct_lattices_in_meta_rules ();
	     }
	  while (change);
	  hint ("needed %d pass%s for coalescing lattices in meta rules",
		nr_passes, (nr_passes == 1)?"":"es");
	  nr_passes = 0;
	  warning ("coalescing lattices in rules...");
	  do
	     { change = 0;
	       nr_passes++;
	       construct_lattices_in_rules ();
	     }
	  while (change);
	  hint ("needed %d pass%s for coalescing lattices in rules",
		nr_passes, (nr_passes == 1)?"":"es");
	  assign_element_numbers ();
	  dump_lattices ();
	};

