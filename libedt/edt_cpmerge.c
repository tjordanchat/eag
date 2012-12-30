/*
   File: edt_cpmerge.c
   Defines the actions for copying the syntax tree or merging it
   with an existing one, possibly leading to 3D trees.

   CVS ID: "$Id: edt_cpmerge.c,v 1.3 2004/12/25 22:40:50 marcs Exp $"
*/

/* general includes */
#include <stdio.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_error.h>
#include <ebs_memalloc.h>
#include <ebs_value.h>

/* libeag includes */
#include <eag_ds.h>
#include <eag_nodeinfo.h>

/* local includes */
#include "edt_edit_ds.h"
#include "edt_cpmerge.h"

etree_node the_root;
static etree_node cptree (tree_node node, etree_node destfather)
	{ int ix;
	  etree_node copy;
	  if (node == tree_node_nil) return (etree_node_nil);
	  copy = new_etree_node ();
	  if (node -> type == leaf_node)
	     { pos_node pos;
	       affix_node affx;
	       if (node -> nraffs != 1) internal_error ("cptree");
	       pos = node -> affs[0];
	       if (pos -> sides[lower_side].tag != tag_single)
		  internal_error ("cptree");
	       affx = pos -> sides[lower_side].a.affx;
	       if (affx -> val == value_nil) internal_error ("cptree");
	       if (affx -> val -> tag != string_value)
		  internal_error ("cptree");
	       copy -> name = affx -> val -> u.str;
	     }
	  else copy -> name = node -> name;
	  copy -> type = node -> type;
	  copy -> nodenr = node -> nodenr;
	  copy -> nrsons = node -> nrsons;
	  copy -> sons = new_eson_space (node -> nrsons);
	  copy -> father = destfather;
	  for (ix = 0; ix < node -> nrsons; ix++)
	     copy -> sons[ix] = cptree (node -> sons[ix], copy);
	  return (copy);
	};

#define copy_subtree(node) cptree(node, etree_node_nil)

/*
   create ambiguous node assumes that the first tree was created
   before the second one and that the destined father therefore
   points to first
*/
static etree_node make_ambiguous_node (etree_node first, etree_node second)
	{ int ix;
	  etree_node ambi = new_etree_node ();
	  ambi -> name = first -> name;
	  ambi -> type = ambiguous_node;
	  ambi -> nodenr = first -> nodenr;
	  ambi -> nrsons = 2;
	  ambi -> sons = new_eson_space (2);
	  ambi -> sons [0] = first;
	  ambi -> sons [1] = second;
	  /* reparenting by searching among the father's sons */
	  for (ix = 0; ix < first -> father -> nrsons; ix++)
	     if (first == first -> father -> sons[ix]) break;
	  first -> father -> sons [ix] = ambi;
	  ambi -> father = first -> father;
	  first -> father = ambi;
	  second -> father = ambi;
	  ambi -> x = -1;
	  ambi -> y = 0;
	  return (ambi);
	};

/*
   addto_ambiguous_node adds a newly found parse tree to an
   already ambiguous node
*/
static void addto_ambiguous_node (etree_node ambi, etree_node new)
	{ int ix;
	  etree_node *newsons;

	  ambi -> nrsons++;
	  newsons = new_eson_space (ambi -> nrsons);
	  for (ix = 0; ix < ambi -> nrsons - 1; ix++)
	     newsons [ix] = ambi -> sons[ix];
	  newsons [ambi -> nrsons - 1] = new;
	  new -> father = ambi;
	  free_eson_space (ambi -> nrsons - 1, ambi -> sons);
	  ambi -> sons = newsons;
	};

/*
   for the following routine to be correct we should actually prove
   that if an ambiguous node is constructed for one of the sons,
   the whole syntax tree of 'node' is merged with that of 'dest'.
*/
static int merge_trees (etree_node dest, etree_node new);
static int try_merge_sons (etree_node dest, etree_node new)
	{ int i;
	  for (i=0; i < dest -> nrsons; i++)
	     if (merge_trees (dest -> sons[i], new -> sons[i]))
		return (1);
	  return (0);
	};

static int merge_trees (etree_node dest, etree_node new)
	{ int i;
	  if (new == etree_node_nil) return (1);	/* ???? */
	  if ((dest -> nodenr == new -> nodenr) &&
	      (dest -> type != ambiguous_node))
	     return (try_merge_sons (dest, new));
	  if (dest -> type == ambiguous_node)
	     { /* This is already an ambiguous node:
		  try and merge with one of its sons; */
	       for (i=0; i < dest -> nrsons; i++)
		  { etree_node dson = dest -> sons[i];
		    if (dson -> nodenr == new -> nodenr)
		       if (try_merge_sons (dson, new))
			  return (1);
		  };
	       addto_ambiguous_node (dest, rdup_etree_node (new));
	       return (0);
	     };
	  /* dest should become ambiguous. however we should be careful
	     if dest has more than one son. if so, climb up in the syntax
	     tree until we reach another alternative.
	  */
	  if (dest -> nrsons > 1 || new -> nrsons > 1)
	     while (dest -> father -> nodenr == dest -> nodenr)
		{ new = new -> father;
		  dest = dest -> father;
		  if (dest -> father -> type == ambiguous_node)
		     dest = dest -> father;
		};
	  make_ambiguous_node (dest, rdup_etree_node (new));
	  return (1);
	};

static void dump_etree_indented (etree_node tnode, int indent, int with)
	{ int ix;
	  if (tnode == etree_node_nil) return;
	  if ((tnode -> type != leaf_node) &&
	      (tnode -> type != normal_node) &&
	      (tnode -> type != typed_placeholder_node) &&
	      (tnode -> type != untyped_placeholder_node) &&
	      (tnode -> type != ambiguous_node)) return;
	  for (ix = 0; ix < indent; ix++) eprint_log ("%c", ' ');
	  switch (tnode -> type)
	     { case leaf_node:
		  eprint_log ("leaf_node (\"%s\")", tnode -> name);
		  break;
	       case normal_node:
		  eprint_log ("%s (%d)", tnode -> name, tnode -> nodenr);
		  break;
	       case typed_placeholder_node:
		  eprint_log ("%s (typed: %d)",
			      tnode -> name, tnode -> nodenr);
		  break;
	       case untyped_placeholder_node:
		  eprint_log ("%s (untyped: %d)",
			      tnode -> name, tnode -> nodenr);
		  break;
	       case ambiguous_node:
		  eprint_log ("ambi: %s (%d)", tnode -> name, tnode -> nodenr);
	       default: break;
	     };
	  if (with)
	     eprint_log ("   x=%d, y=%d, w=%d, h=%d, style = %d",
			 tnode -> x, tnode -> y,
			 tnode -> width, tnode -> height, tnode -> style);
	  eprint_log ("\n");
	  for (ix = 0; ix < tnode -> nrsons; ix++)
	     dump_etree_indented (tnode -> sons[ix], indent + 1, with);
	};

void dump_etree (int with)
	{ wlog ("\nResultant folded tree%s", (with)?" with layout parms":"");
	  dump_etree_indented (the_root, 0, with);
	};

void copy_tree ()
	{ etree_node tnode = copy_subtree (top_tree_node ());
	  if (the_root == etree_node_nil)
	     the_root = tnode;
	  else
	     { (void) merge_trees (the_root, tnode);
	       rfre_etree_node (tnode);
	     };
	  if (debug) dump_etree (0);
	  callq ();
	  pushq (copy_tree);
	};

void init_cpmerge ()
	{ the_root = etree_node_nil;
	};
