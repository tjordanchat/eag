/*
   File: buildtree.c
   Defines the actions necessary for building the syntax tree
   as well as the linkage to the affix tree

   CVS ID: "$Id: eag_buildtree.c,v 1.5 2004/12/25 22:19:08 marcs Exp $"
*/

/* global includes */
#include <stdio.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_error.h>
#include <ebs_textstorage.h>
#include <ebs_cst.h>
#include <ebs_value.h>
#include <ebs_lexds.h>

/* libeag includes */
#include "eag_ds.h"
#include "eag_nodeinfo.h"
#include "eag_propagate.h"
#include "eag_buildtree.h"

/*
   Building affix_nodes
*/
affix_node value_to_affix (string name, value v)
	{ affix_node new = new_affix_node (name);
	  new -> val = v;
	  return (new);
	};

affix_node make_undefined_affix ()
	{ affix_node new = new_affix_node ("predef_undefined");
	  new -> val = new_undefined_value ();
	  return (new);
	};

/*
   Building links
*/
void add_link (affix_node aff, pos_node pos, int side)
	{ link_node link = new_link_node ();
	  link -> pos = pos;
	  link -> side = side;
	  link -> next = aff -> links;
	  aff -> links = link;
	  if (aff -> val != value_nil)
	     pos -> sides[side].sill -= 1;
	};

void delete_link (affix_node aff, pos_node pos)
	{ link_node current = aff -> links;
	  if (current -> pos == pos)
	     { aff -> links = current -> next;
	       free_link_node (current);
	     }
	  else
	     { link_node next;
	       for (next = current -> next;
		    next != link_node_nil;
		    current = next, next = next -> next)
		  if (next -> pos == pos)
		     { current -> next = next -> next;
		       free_link_node (next);
		       return;
		     };
	     };
	};
/*
   make_affix_link will build the upper side affix expressions
   corresponding to applications of syntax rules (i.e. right
   hand sides). For every position it will pop a representation of the
   corresponding affix expression from the q stack, which it will build
   at the upper side of the position the corresponding affix graph.
   After building it will try and propagate the affixes through
   the affix graph. 
*/
void make_affix_link ()
	{ int ix;
	  cont *lqptr;
	  int nr = popi();		/* redundant wegens topnode! */
	  tree_node topnode = top_tree_node();
	  pos_node *save_ps = new_pos_space (nr);
	  for (ix = 0; ix < nr; ix++)
	     { pos_node save_pos = new_pos_node ();
	       pos_node old_pos = topnode -> affs[ix];
	       int ntag;
	       save_pos -> sides[upper_side].tag =
			old_pos -> sides[upper_side].tag;
	       save_pos -> sides[upper_side].sill =
			old_pos -> sides[upper_side].sill;
	       save_pos -> sides[upper_side].a = old_pos -> sides[upper_side].a;
	       save_ps[ix] = save_pos;
	       ntag = popi ();
	       old_pos -> sides[upper_side].tag = ntag;
	       if (ntag == tag_single)
		  { affix_node affx = popa ();
		    old_pos -> sides[upper_side].sill = 1;
		    old_pos -> sides[upper_side].a.affx = affx;
		    add_link (affx,old_pos,upper_side);
		  }
	       else
		  { int nras = popi ();
		    affix_node *affs = new_affix_space (nras);
		    int j;
		    old_pos -> sides[upper_side].sill = nras;
		    old_pos -> sides[upper_side].a.co.nr = nras;
		    old_pos -> sides[upper_side].a.co.affs = affs;
		    for (j = 0; j < nras; j++)
		       { affix_node affx = popa();
			 affs[j] = affx;
			 add_link (affx,old_pos,upper_side);
		       };
		  };
	     };
	  lqptr = qptr;
	  for (ix = 0; ix < nr; ix++)
	     { pushp (topnode -> affs[ix]);
	       pushq (propagate_affix_value);
	     };
	  callq ();
	  qptr = lqptr;
	  for (ix = nr - 1; 0 <= ix; ix--)
	     { pos_node save_pos = save_ps[ix];
	       pos_node old_pos = topnode -> affs[ix];
	       int otag = old_pos -> sides[upper_side].tag;
	       if (otag == tag_single)
		  { affix_node affx = old_pos -> sides[upper_side].a.affx;
		    delete_link (affx, old_pos);
		    pusha (affx);
		  }
	       else
		  { int nras = old_pos -> sides[upper_side].a.co.nr;
		    affix_node *affs = old_pos -> sides[upper_side].a.co.affs;
		    int j;
		    for (j = nras - 1; 0 <= j; j--)
		        { affix_node affx = affs[j];
			  delete_link (affx, old_pos);
			  pusha (affx);
			};
		    free_affix_space (nras, affs);
		    pushi (nras);
		  };
	       pushi (otag);
	       old_pos -> sides[upper_side].tag =
			save_pos -> sides[upper_side].tag;
	       old_pos -> sides[upper_side].sill =
			save_pos -> sides[upper_side].sill;
	       old_pos -> sides[upper_side].a = save_pos -> sides[upper_side].a;
	       free_pos_node (save_pos);
	     };
	  free_pos_space (nr, save_ps);
	  pushi (nr);
	  pushq (make_affix_link);
	};

/*
   make_affix_pos builds the lower side affix positions corresponding
   to the definition of a syntax node (i.e. its left hand side)
   For every position it will pop a representation of the corresponding
   affix expression from the q stack, which it will build at the lower
   side of the position the corresponding affix graph
*/
static void make_affix_pos (tree_node node, int nrps)
	{ int nr;
	  pos_node *ps = new_pos_space (nrps);
	  node -> nraffs = nrps;
	  node -> affs = ps;

	  for (nr = 0; nr < nrps; nr++)
	     { pos_node pos = new_pos_node ();
	       int ntag = popi ();
	       ps[nr] = pos;
	       pos -> node = node;
	       pos -> sides[lower_side].tag = ntag;
	       if (ntag == tag_single)
		  { /* add link will decrease the sill if possible */
		    affix_node affx = popa ();
		    pos -> sides[lower_side].sill = 1;
		    pos -> sides[lower_side].a.affx = affx;
		    add_link (affx, pos, lower_side);
		  }
	       else
		  { int ix;
		    int nras = popi ();
		    affix_node *affs = new_affix_space (nras);
		    pos -> sides[lower_side].sill = nras;
		    pos -> sides[lower_side].a.co.nr = nras;
		    pos -> sides[lower_side].a.co.affs = affs;
		    for (ix = 0; ix < nras; ix++)
		       { affix_node affx = popa ();
			 affs[ix] = affx;
			 add_link (affx, pos, lower_side);
		       };
		  };
	     };
	};

/*
   clear affix pos will undo the actions of make affix pos
*/
static void clear_affix_pos (tree_node node, int nrps)
	{ int nr;
	  pos_node *ps = node -> affs;
	  for (nr = 0; nr < nrps; nr++)
	     { pos_node pos = ps[nr];
	       int tag = pos -> sides[lower_side].tag;
	       if (tag == tag_single)
		  { affix_node affx = pos -> sides[lower_side].a.affx;
		    delete_link (affx, pos);
		    pusha (affx);
		  }
	       else
		  { int ix;
		    int nras = pos -> sides[lower_side].a.co.nr;
		    affix_node *affs = pos -> sides[lower_side].a.co.affs;
		    for (ix = nras - 1; 0 <= ix; ix--)
		       { affix_node affx = affs[ix];
			 delete_link (affx, pos);
			 pusha (affx);
		       };
		    free_affix_space (nras, affs);
		    pushi (nras);
		  };
	       pushi (tag);
	       free_pos_node (pos);
	     };
	  free_pos_space (nrps, ps);
	};

/*
   leaf nodes are used to administer the values delivered by parsing
   semi terminals: they always have one affix position
*/
void make_leaf_node ()
	{ tree_node new = new_tree_node ();
	  value strval = new_string_value (strstore);
	  pos_node *ps = new_pos_space (1);
	  pos_node newpos = new_pos_node ();
	  affix_node aff = value_to_affix ("predef_leaf", strval); 

	  new -> type = leaf_node;
	  new -> name = "leaf_node";
	  new -> nodenr = 1;
	  new -> nrsons = 0;
	  new -> nraffs = 1;
	  new -> affs = ps;
	  ps[0] = newpos;
	  newpos -> node = new;
	  newpos -> sides[lower_side].tag = tag_single;
	  /* add_link must decrease the sill! */
	  newpos -> sides[lower_side].sill = 1;
	  newpos -> sides[lower_side].a.affx = aff;
	  newpos -> delayed = 0;
	  add_link (aff, newpos, lower_side);	/* can be optimized */
	  push_tree_node (new);
	  callq ();
	  (void) pop_tree_node ();
	  delete_link (aff, newpos);
	  free_pos_node (newpos);
	  free_pos_space (1, ps);
	  rfre_value (strval);
	  free_affix_node (aff);
	  free_tree_node (new);
	  pushq (make_leaf_node);
	};

/*
   make typed node will create a new node for the syntax tree and
   attribute it with the necessary affixes. After backup call it
   will undo any action necessary
*/
static void make_typed_node (int type)
	{ tree_node new = new_tree_node ();
	  int nrsons;
	  int nrps;

	  new -> type = type;
	  new -> nodenr = popi();
	  new -> name = name_from_nodenr (new -> nodenr);
	  nrsons = popi();
	  new -> nrsons = nrsons;
	  new -> sons = new_son_space (nrsons);
	  nrps = popi ();
	  make_affix_pos (new, nrps);
	  push_tree_node (new);
	  callq ();
	  (void) pop_tree_node ();
	  clear_affix_pos (new, nrps);
	  pushi (nrps);
	  free_son_space (nrsons, new -> sons);
	  pushi (nrsons);
	  pushi (new -> nodenr);
	  free_tree_node (new);
	};

void make_normal_node ()
	{ make_typed_node (normal_node);
	  pushq (make_normal_node);
	};

void make_predicate_node ()
	{ make_typed_node (predicate_node);
	  pushq (make_predicate_node);
	};

void make_semipredicate_node ()
	{ make_typed_node (semipredicate_node);
	  pushq (make_semipredicate_node);
	};

void make_lexicon_node (lexend le, value_list vals)
	{ int nrps = le -> valuenrs -> size;
	  tree_node new = new_tree_node ();
	  pos_node *ps = new_pos_space (nrps);
	  int ix;
	  new -> type = lexicon_node;
	  new -> nodenr = le -> nodenr;
	  new -> name = name_from_nodenr (le -> nodenr);
	  new -> nrsons = 0;
	  new -> sons = NULL;
	  new -> nraffs = nrps;
	  new -> affs = ps;
	  for (ix = 0; ix < nrps; ix++)
	     { value aval = vals -> array[le -> valuenrs -> array[ix]];
	       pos_node pos = new_pos_node ();
	       char affxname[32];
	       affix_node affx;
	       sprintf (affxname, "lex_gen_%d", ix + 1);
	       affx = value_to_affix (affxname, aval);
	       ps[ix] = pos;
	       pos -> node = new;
	       pos -> sides[lower_side].tag = tag_single;
	       pos -> sides[lower_side].sill = 1;
	       pos -> sides[lower_side].a.affx = affx;
	       add_link (affx, pos, lower_side);
	     };
	  push_tree_node (new);
	  callq ();
	  (void) pop_tree_node ();
	  for (ix = 0; ix < nrps; ix++)
	     { pos_node pos = ps[ix];
	       affix_node affx = pos -> sides[lower_side].a.affx;
	       delete_link (affx, pos);
	       free_affix_node (affx);
	       free_pos_node (pos);
	     };
	  free_pos_space (nrps, ps);
	  free_tree_node (new);
	};

/*
   make simple node will create a new (normal) node for the syntax tree.
   It is intended to build those tree nodes that have zero affixes.
   After backup call it will undo any action necessary.
*/
void make_simple_node ()
	{ tree_node new = new_tree_node ();
	  int nrsons;
	  new -> type = normal_node;
	  new -> nodenr = popi ();
	  new -> name = name_from_nodenr (new -> nodenr);
	  nrsons = popi ();
	  new -> nrsons = nrsons;
	  new -> sons = new_son_space (nrsons);
	  push_tree_node (new);
	  callq ();
	  (void) pop_tree_node ();
	  free_son_space (nrsons, new -> sons);
	  pushi (nrsons);
	  pushi (new -> nodenr);
	  free_tree_node (new);
	  pushq (make_simple_node);
	};

static void make_placeholder_node (int type)
	{ tree_node new = new_tree_node ();
	  int nrps;

	  new -> type = type;
	  new -> nodenr = popi ();
	  new -> name = name_from_nodenr (new -> nodenr);
	  new -> nrsons = 0;
	  new -> sons = tree_array_nil;
	  nrps = popi ();
	  make_affix_pos (new, nrps);
	  push_tree_node (new);
	  callq ();
	  (void) pop_tree_node ();
	  clear_affix_pos (new, nrps);
	  pushi (nrps);
	  pushi (new -> nodenr);
	  free_tree_node (new);
	};

void make_typed_placeholder_node ()
	{ make_placeholder_node (typed_placeholder_node);
	  pushq (make_typed_placeholder_node);
	};

void make_untyped_placeholder_node ()
	{ make_placeholder_node (untyped_placeholder_node);
	  pushq (make_untyped_placeholder_node);
	};

void update_predicate_node ()
	{ int nodenr = popi ();
	  int nrsons = popi ();
	  pos_node *args = poppp();
	  tree_node node = args[0] -> node;
	  int old_nodenr = node -> nodenr;
	  int nraffs = popi();
	  pos_node *save_pafs;
	  cont *lqptr;
	  int ix;

	  node -> nodenr = nodenr;
	  node -> nrsons = nrsons;
	  node -> sons = new_son_space (nrsons);
	  save_pafs = new_pos_space (nraffs);
	  for (ix = 0; ix < nraffs; ix++)
	     { pos_node save_pos = new_pos_node ();
	       pos_node old_pos = args[ix];
	       int ntag;
	       save_pos -> sides[lower_side].tag =
			old_pos -> sides[lower_side].tag;
	       save_pos -> sides[lower_side].sill =
			old_pos -> sides[lower_side].sill;
	       save_pos -> sides[lower_side].a = old_pos -> sides[lower_side].a;
	       save_pafs[ix] = save_pos;
	       ntag = popi ();
	       old_pos -> sides[lower_side].tag = ntag;
	       if (ntag == tag_single)
		  { affix_node affx = popa();
		    old_pos -> sides[lower_side].sill = 1;
		    old_pos -> sides[lower_side].a.affx = affx;
		    add_link (affx,old_pos,lower_side);
		  }
	       else
		  { int nras = popi ();
		    affix_node *affs = new_affix_space (nras);
		    int j;
		    old_pos -> sides[lower_side].sill = nras;
		    old_pos -> sides[lower_side].a.co.nr = nras;
		    old_pos -> sides[lower_side].a.co.affs = affs;
		    for (j = 0; j < nras; j++)
		       { affix_node affx = popa();
			 affs[j] = affx;
			 add_link (affx,old_pos,lower_side);
		       };
		  };
	     };
	  lqptr = qptr;
	  for (ix = 0; ix < nraffs; ix++)
	     { pushp (args[ix]);
	       pushq (propagate_affix_value);
	     };
	  callq ();
	  qptr = lqptr;
	  for (ix = nraffs - 1; 0 <= ix; ix--)
	     { pos_node save_pos = save_pafs[ix];
	       pos_node old_pos = args[ix];
	       int otag = old_pos -> sides[lower_side].tag;
	       if (otag == tag_single)
		  { affix_node affx = old_pos -> sides[lower_side].a.affx;
		    delete_link (affx, old_pos);
		    pusha (affx);
		  }
	       else
		  { int nras = old_pos -> sides[lower_side].a.co.nr;
		    affix_node *affs = old_pos -> sides[lower_side].a.co.affs;
		    int j;
		    for (j = nras - 1; 0 <= j; j--)
		        { affix_node affx = affs[j];
			  delete_link (affx, old_pos);
			  pusha (affx);
			};
		    free_affix_space (nras, affs);
		    pushi (nras);
		  };
	       pushi (otag);
	       old_pos -> sides[lower_side].tag =
			save_pos -> sides[lower_side].tag;
	       old_pos -> sides[lower_side].sill =
			save_pos -> sides[lower_side].sill;
	       old_pos -> sides[lower_side].a = save_pos -> sides[lower_side].a;
	       free_pos_node (save_pos);
	     };
	  free_pos_space (nraffs, save_pafs);
	  free_son_space (nrsons, node -> sons);
	  node -> sons = tree_array_nil;
	  node -> nrsons = 0;
	  node -> nodenr = old_nodenr;
	  pushi (nraffs);
	  pushpp (args);
	  pushi (nrsons);
	  pushi (nodenr);
	  pushq (update_predicate_node);
	};

/*
   exchange tops of tree stacks
*/
void exchange_top ()
	{ tree_node top = pop_tree_node ();
	  tree_node top2 = pop_tree_node ();
	  push_tree_node (top);
	  push_tree_node (top2);
	  callq ();
	  tptr -= 2;
	  push_tree_node (top2);
	  push_tree_node (top);
	  pushq (exchange_top);
	};
 
/*
   link son will pop a son from the tree stack and insert him
   as son of the top node of the tree stack.
*/
void link_son ()
	{ tree_node son;
	  int nr = popi();
	  son = pop_tree_node ();
	  if (nr != 0)		/* ????? */
	     { tree_node father = top_tree_node ();
	       tree_node save_son = father -> sons[nr-1];
	       father -> sons[nr-1] = son;
	       callq();
	       father -> sons[nr-1] = save_son;
	     }
	  else callq();
	  push_tree_node (son);
	  pushi (nr);
	  pushq (link_son);
	};

void link_predicate_son ()
	{ tree_node son;
	  tree_node father = popt ();
	  int nr = popi ();
	  son = pop_tree_node ();
	  if (nr != 0)		/* ????? */
	     { tree_node save_son = father -> sons[nr-1];
	       father -> sons[nr-1] = son;
	       callq ();
	       father -> sons[nr-1] = save_son;
	     }
	  else callq ();
	  push_tree_node (son);
	  pushi (nr);
	  pusht (father);
	  pushq (link_predicate_son);
	};

void dump_parse_tree_indented (tree_node tnode, int indent)
	{ int i;
	  if (tnode == tree_node_nil) return;	/* layout? */
	  if ((tnode -> type != normal_node) &&
	      (tnode -> type != typed_placeholder_node) &&
	      (tnode -> type != untyped_placeholder_node)) return;
	  for (i=0; i < indent; i++) eprint_log ("%c", ' ');
	  eprint_log ("%s (%d)\n", tnode -> name, tnode -> nodenr);
	  for (i=0; i < tnode -> nrsons; i++)
	     dump_parse_tree_indented (tnode -> sons[i], indent+1);
	};

void dump_parse_tree ()
	{ wlog ("\nDump of parse tree:");
	  dump_parse_tree_indented (top_tree_node (),0);
	  callq ();
	  pushq (dump_parse_tree);
	};
