/*
   File: edt_edit_ds.h
   Defines the editor datastructures 

   CVS ID: "$Id: edt_edit_ds.h,v 1.3 2004/12/25 22:40:51 marcs Exp $"
*/
#ifndef IncEdtEditDs
#define IncEdtEditDs

/* general includes */
#include <stdio.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_value.h>

/* libeag includes */
#include <eag_ds.h>

/* Introduce record type definitions */
typedef struct etree_rec *etree_node;	/* syntax tree node */

struct etree_rec
	{ string name;			/* voor tracing */
	  int type;			/* type of node */
	  int nodenr;			/* nr of the alternative? */
	  int nrsons;			/* nr of sons */
	  etree_node *sons;		/* array of son tree nodes */
	  etree_node father;		/* father node */
	  int style;			/* layout style */
	  int width;			/* width */
	  int height;			/* height */
	  int x;			/* x */
	  int y;			/* y */
	};
#define etree_node_nil ((etree_node) NULL)
#define etree_array_nil ((etree_node *) NULL)

/*
   Memory management routines
*/
extern etree_node new_etree_node ();
extern void free_etree_node (etree_node old);
extern etree_node *new_eson_space (int nrsons);
extern void free_eson_space (int nrsons, etree_node *old);
extern etree_node rdup_etree_node (etree_node org);
extern void rfre_etree_node (etree_node old);

/*
   Initialization
*/
extern void init_edit_ds ();

#endif /* IncEdtEditDs */
