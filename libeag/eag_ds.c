/*
   File: eag_ds.c
   Keeps track of the incore datastructures

   CVS ID: "$Id: eag_ds.c,v 1.4 2004/12/25 22:19:08 marcs Exp $"
*/

/* global includes */
#include <stdio.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_memalloc.h>
#include <ebs_textstorage.h>
#include <ebs_value.h>

/* libeag includes */
#include "eag_nodeinfo.h"
#include "eag_ds.h"

/*
   Manage the tree stack
*/
tree_node *tptr;
static tree_node *tstack;

/*
   Memory management routines
*/

/* management of tree nodes */
static tree_node free_tree_node_list;
tree_node new_tree_node ()
	{ tree_node new;
	  if (free_tree_node_list == tree_node_nil)
	     new = (tree_node) ckmalloc (sizeof (struct tree_rec));
	  else
	     { new = free_tree_node_list;
	       free_tree_node_list = (tree_node) free_tree_node_list -> name;
	     };
	  new -> name = "";
	  new -> type = undefined;
	  new -> nodenr = 0;
	  new -> nrsons = 0;
	  new -> nraffs = 0;
	  return (new);
	};

void free_tree_node (tree_node old)
	{ old -> name = (string) free_tree_node_list;
	  free_tree_node_list = old;
	};

/* management of arrays of tree nodes */
#define MAXMANAGEDSONS 64
static tree_node *free_son_list[MAXMANAGEDSONS];
tree_node *new_son_space (int nrsons)
	{ int i;
	  tree_node *new;
	  if (nrsons == 0) return (tree_array_nil);

	  if ((nrsons > MAXMANAGEDSONS) ||
	      (free_son_list[nrsons-1] == tree_array_nil))
	     new = (tree_node *) ckcalloc (nrsons, sizeof (tree_node));
	  else
	     { new = free_son_list[nrsons-1];
	       free_son_list[nrsons-1] =
			(tree_node *)(free_son_list[nrsons-1][0]);
	     };
	  for (i = 0; i < nrsons; i++) new[i] = tree_node_nil;
	  return (new);
	};

void free_son_space (int nrsons, tree_node *old)
	{ if (nrsons == 0) return;
	  if (nrsons > MAXMANAGEDSONS) free ((char *) old);
	  else
	     { old[0] = (tree_node) free_son_list[nrsons-1];
	       free_son_list[nrsons-1] = old;
	     };
	};

/* management of affix positions */
static pos_node free_pos_node_list;
pos_node new_pos_node ()
	{ pos_node new;
	  if (free_pos_node_list == pos_node_nil)
	     { new = (pos_node) ckmalloc (sizeof (struct pos_rec));
	     }
	  else
	     { new = free_pos_node_list;
	       free_pos_node_list = (pos_node) free_pos_node_list -> node;
	     };
	  new -> node = tree_node_nil;
	  new -> sides[0].tag = undefined;
	  new -> sides[1].tag = undefined;
	  new -> delayed = 0;
	  new -> args = pos_array_nil;
	  new -> dfunc = NULL;
	  return (new);
	};

void free_pos_node (pos_node old)
	{ old -> node = (tree_node) free_pos_node_list;
	  free_pos_node_list = old;
	}; 

/* management of affix position arrays */
#define MAXMANAGEDPOS 64
static pos_node *free_pos_space_list[MAXMANAGEDPOS];
pos_node *new_pos_space (int nrofps)
	{ int i;
	  pos_node *new;
	  if (nrofps == 0) return (pos_array_nil);

	  if ((nrofps > MAXMANAGEDPOS) ||
	      (free_pos_space_list[nrofps-1] == pos_array_nil))
	     { new = (pos_node *) ckcalloc (nrofps, sizeof (pos_node));
	     }
	  else
	     { new = free_pos_space_list[nrofps-1];
	       free_pos_space_list[nrofps-1] =
			(pos_node *)(free_pos_space_list[nrofps-1][0]);
	     };
	  for (i = 0; i < nrofps; i++) new[i] = pos_node_nil;
	  return (new);
	};

void free_pos_space (int nrofps, pos_node *old)
	{ if (nrofps == 0) return;
	  if (nrofps > MAXMANAGEDPOS) free ((char *) old);
	  else
	     { old[0] = (pos_node) free_pos_space_list[nrofps-1];
	       free_pos_space_list[nrofps-1] = old;
	     };
	};

/* management of affixes */
static affix_node free_affix_node_list;
affix_node new_affix_node (string name)
	{ affix_node new;
	  if (free_affix_node_list == affix_node_nil)
	     new = (affix_node) ckmalloc (sizeof (struct affix_rec));
	  else
	     { new = free_affix_node_list;
	       free_affix_node_list =
			(affix_node) free_affix_node_list -> links;
	     };
	  new -> name = addto_names (name);
	  new -> val = value_nil;
	  new -> mfunc = NULL;
	  new -> links = link_node_nil;
	  return (new);
	};

void free_affix_node (affix_node old)
	{ old -> links = (link_node) free_affix_node_list;
	  free_affix_node_list = old;
	};

/* management of affix arrays */
#define MAXMANAGEDAFFIXES 64
static affix_node *free_affix_space_list[MAXMANAGEDAFFIXES];
affix_node *new_affix_space (int nraffs)
	{ int i;
	  affix_node *new;
	  if (nraffs == 0) return (affix_array_nil);

	  if ((nraffs > MAXMANAGEDAFFIXES) ||
	      (free_affix_space_list[nraffs-1] == affix_array_nil))
	     new = (affix_node *) ckcalloc (nraffs, sizeof (affix_node));
	  else
	     { new = free_affix_space_list[nraffs-1];
	       free_affix_space_list[nraffs-1] =
			(affix_node *)(free_affix_space_list[nraffs-1][0]);
	     };
	  for (i = 0; i < nraffs; i++) new[i] = affix_node_nil;
	  return (new);
	};

void free_affix_space (int nraffs, affix_node *old)
	{ if (nraffs == 0) return;
	  if (nraffs > MAXMANAGEDAFFIXES) free ((char *) old);
	  else
	     { old[0] = (affix_node) free_affix_space_list[nraffs-1];
	       free_affix_space_list[nraffs-1] = old;
	     };
	};

/* management of links */
static link_node free_link_node_list;
link_node new_link_node ()
	{ link_node new;
	  if (free_link_node_list == link_node_nil)
	     new = (link_node) ckmalloc (sizeof (struct link_rec));
	  else
	     { new = free_link_node_list;
	       free_link_node_list = free_link_node_list -> next;
	     };
	  new -> pos = pos_node_nil;
	  new -> side = 0;
	  new -> next = link_node_nil;
	  return (new);
	};

void free_link_node (link_node old)
	{ old -> next = free_link_node_list;
	  free_link_node_list = old;
	};

/*
   Manage the continuation stack
*/
cont *qptr;
static cont *qstack;

void reinit_ds ()
	{ tptr = tstack;
	  qptr = qstack;
	};

/*
   will improve
*/
void init_ds (int tstack_size, int qstack_size)
	{ int i;
	  tstack = (tree_node *) ckcalloc (tstack_size, sizeof (tree_node));
	  tptr = tstack;
	  qstack = (cont *) ckcalloc (qstack_size, sizeof (cont));
	  qptr = qstack;
	  free_tree_node_list = tree_node_nil;
	  for (i = 0; i < MAXMANAGEDSONS; i++)
	     free_son_list[i] = tree_array_nil;
	  free_pos_node_list = pos_node_nil;
	  for (i = 0; i < MAXMANAGEDPOS; i++)
	     free_pos_space_list[i] = pos_array_nil;
	  free_affix_node_list = affix_node_nil;
	  for (i = 0; i < MAXMANAGEDAFFIXES; i++)
	     free_affix_space_list[i] = affix_array_nil;
	  free_link_node_list = link_node_nil;
	};
