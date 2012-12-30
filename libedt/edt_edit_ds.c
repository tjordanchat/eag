/*
   File: edt_edit_ds.c
   Keeps track of the editor datastructures

   CVS ID: "$Id: edt_edit_ds.c,v 1.3 2004/12/25 22:40:51 marcs Exp $"
*/

/* general includes */
#include <stdio.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_memalloc.h>
#include <ebs_textstorage.h>
#include <ebs_value.h>

/* libeag includes */
#include <eag_ds.h>
#include <eag_nodeinfo.h>

/* local includes */
#include "edt_edit_ds.h"

/*
   Memory management routines
*/

/* management of tree nodes */
static etree_node free_etree_node_list;
etree_node new_etree_node ()
	{ etree_node new;
	  if (free_etree_node_list == etree_node_nil)
	     new = (etree_node) ckmalloc (sizeof (struct etree_rec));
	  else
	     { new = free_etree_node_list;
	       free_etree_node_list = (etree_node) free_etree_node_list -> name;
	     };
	  new -> name = "";
	  new -> type = undefined;
	  new -> nodenr = 0;
	  new -> father = etree_node_nil;
	  new -> nrsons = 0;
	  new -> sons = etree_array_nil;
	  return (new);
	};

void free_etree_node (etree_node old)
	{ old -> name = (string) free_etree_node_list;
	  free_etree_node_list = old;
	};

/* management of arrays of tree nodes */
#define MAXMANAGEDSONS 64
static etree_node *free_eson_list[MAXMANAGEDSONS];
etree_node *new_eson_space (int nrsons)
	{ int i;
	  etree_node *new;
	  if (nrsons == 0) return (etree_array_nil);

	  if ((nrsons > MAXMANAGEDSONS) ||
	      (free_eson_list[nrsons-1] == etree_array_nil))
	     new = (etree_node *) ckcalloc (nrsons, sizeof (etree_node));
	  else
	     { new = free_eson_list[nrsons-1];
	       free_eson_list[nrsons-1] =
			(etree_node *)(free_eson_list[nrsons-1][0]);
	     };
	  for (i = 0; i < nrsons; i++) new[i] = etree_node_nil;
	  return (new);
	};

void free_eson_space (int nrsons, etree_node *old)
	{ if (nrsons == 0) return;
	  if (nrsons > MAXMANAGEDSONS) free ((char *) old);
	  else
	     { old[0] = (etree_node) free_eson_list[nrsons-1];
	       free_eson_list[nrsons-1] = old;
	     };
	};

etree_node rdup_etree_node (etree_node org)
	{ etree_node new;
	  int ix;
	  if (org == etree_node_nil) return (etree_node_nil);
	  new = new_etree_node ();
	  new -> name = org -> name;
	  new -> type = org -> type;
	  new -> nodenr = org -> nodenr;
	  new -> nrsons = org -> nrsons;
	  new -> sons = new_eson_space (org -> nrsons);
	  new -> father = etree_node_nil;
	  for (ix = 0; ix < org -> nrsons; ix++)
	     { new -> sons[ix] = rdup_etree_node (org -> sons[ix]);
	       new -> sons[ix] -> father = new;
	     }; 
	  return (new);
	};

void rfre_etree_node (etree_node old)
	{ int ix;
	  if (old == etree_node_nil) return;
	  for (ix = 0; ix < old -> nrsons; ix++)
	     rfre_etree_node (old -> sons[ix]);
	  free_eson_space (old -> nrsons, old -> sons);
	  free_etree_node (old);
	};

void init_edit_ds ()
	{ int i;
	  free_etree_node_list = etree_node_nil;
	  for (i = 0; i < MAXMANAGEDSONS; i++)
	     free_eson_list[i] = etree_array_nil;
	};
