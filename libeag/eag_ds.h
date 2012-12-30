/*
   File: eag_ds.h
   Defines the necessary datastructures to store 
   syntax trees and propagate affix values

   CVS ID: "$Id: eag_ds.h,v 1.5 2008/05/28 15:28:33 marcs Exp $"
*/
#ifndef IncEagDs
#define IncEagDs

/* general includes */
#include <stdio.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_value.h>

/* Introduce record type definitions */
typedef struct tree_rec *tree_node;	/* syntax tree node */
typedef struct pos_rec *pos_node;	/* affix position */
typedef struct affix_rec *affix_node;	/* affix */
typedef struct link_rec *link_node;	/* links from affixes to positions */

struct tree_rec
{ string name;				/* voor tracing? */
  int type;				/* type of node */
  int nodenr;				/* nr of the alternative */
  int nrsons;				/* nr of sons */
  tree_node *sons;			/* array of son tree nodes */
  int nraffs;				/* nr of affixes */
  pos_node *affs;			/* array of affix positions */
};
#define tree_node_nil ((tree_node) NULL)
#define tree_array_nil ((tree_node *) NULL)

/*
   While parsing, partial syntax trees are stored on a stack
*/
extern tree_node *tptr;

#define push_tree_node(t) *tptr = t; tptr++
#define pop_tree_node() *(--tptr)
#define top_tree_node() *(tptr-1)

/*
   One side of an affix position corresponds with an affix expression
   in the original grammer. Only when the value at one side is fully
   known will it be propagated to the other side of the position
*/
#define tag_undefined 0
#define tag_single 1
#define tag_compos 2
#define tag_concat 3
#define tag_union 4
typedef struct pos_side_rec
{ int sill;				/* the sill */
  int tag;				/* what kind of affix expr are we */
  union
    { affix_node affx;			/* the single affix */
      struct
	{ int nr;			/* nr of affixes in this position */
  	  affix_node *affs;		/* the affixes */
	} co;				/* a comp/conc/unified affix */
    } a;
} pos_side;

#define lower_side 0
#define upper_side 1
struct pos_rec
{ tree_node node;			/* whose position am i */
					/* doubles for free list adm */
  pos_side sides[2];			/* lower and upper side */
  int delayed;				/* is this position delayed */
  pos_node *args;			/* args of delayed predicate */
  void (*dfunc)();			/* the actual delayed function */
};
#define pos_node_nil ((pos_node) NULL)
#define pos_array_nil ((pos_node *) NULL)

/*
   An affix node corresponds with an affix in the original grammar
   Via the links the affix value will be propagated to other nodes.
*/
struct affix_rec
{ string name;				/* affix name */
  value val;				/* the value of the affix */
  void (*mfunc)();			/* function to check meta definition */
  link_node links;			/* linked list of all links */
};
#define affix_node_nil ((affix_node) NULL)
#define affix_array_nil ((affix_node *) NULL)

/*
   Introduce the structure of affix links
   The list of links points from an affix node to all positions
   in which this affix node occurs. In this way values get propagated
   as soon as the affix node gets a value.
*/
struct link_rec
{ pos_node pos;				/* to which affix position */
  int side;				/* and which side do I propagate */
  link_node next;			/* next link */
};
#define link_node_nil ((link_node) NULL)

/*
   The continuation stack is used for lots of intermediate values
   and continuations
*/
typedef union
{ void (*q)();				/* continuation */
  string s;				/* a string */
  int i;				/* integer */
  int *ip;				/* pointer to integer */
  real r;				/* real */
  real *rp;				/* pointer to real */
  value v;				/* value */
  tree_node t;				/* treenode */
  link_node l;				/* linknode */
  affix_node a;				/* affix */
  affix_node *ap;			/* array of affixes */
  pos_node p;				/* affix position */
  pos_node *pp;				/* array of affix positions */
} cont;

extern cont *qptr;			/* this pointer stores the top */

/*
   Define access macros for the continuation stack
*/
#define pushq(arg) qptr -> q = arg; qptr++
#define pushs(arg) qptr -> s = arg; qptr++
#define pushi(arg) qptr -> i = arg; qptr++
#define pushr(arg) qptr -> r = arg; qptr++
#define pusht(arg) qptr -> t = arg; qptr++
#define pushl(arg) qptr -> l = arg; qptr++
#define pusha(arg) qptr -> a = arg; qptr++
#define pushv(arg) qptr -> v = arg; qptr++
#define pushp(arg) qptr -> p = arg; qptr++
#define puship(arg) qptr -> ip = arg; qptr++
#define pushrp(arg) qptr -> rp = arg; qptr++
#define pushap(arg) qptr -> ap = arg; qptr++
#define pushpp(arg) qptr -> pp = arg; qptr++
#define callq() (--qptr) -> q()
#define topq() ((qptr-1) -> q)
#define tops() ((qptr-1) -> s)
#define topi() ((qptr-1) -> i)
#define popq() ((--qptr) -> q)
#define pops() ((--qptr) -> s)
#define popi() ((--qptr) -> i)
#define popr() ((--qptr) -> r)
#define popt() ((--qptr) -> t)
#define popl() ((--qptr) -> l)
#define popa() ((--qptr) -> a)
#define popv() ((--qptr) -> v)
#define popp() ((--qptr) -> p)
#define popip() ((--qptr) -> ip)
#define poprp() ((--qptr) -> rp)
#define popap() ((--qptr) -> ap)
#define poppp() ((--qptr) -> pp)
#define pop(nr) qptr -= nr

/*
   Memory management routines
*/
tree_node new_tree_node ();
void free_tree_node (tree_node old);
tree_node *new_son_space (int nrsons);
void free_son_space (int nrsons, tree_node *old);
pos_node new_pos_node ();
void free_pos_node (pos_node old);
pos_node *new_pos_space (int nrps);
void free_pos_space (int nrps, pos_node *old);
affix_node new_affix_node (string name);
void free_affix_node (affix_node old);
affix_node *new_affix_space (int nraffs);
void free_affix_space (int nraffs, affix_node *old);
link_node new_link_node ();
void free_link_node (link_node old);

/*
   Initialization
*/
void init_ds (int tstack_size, int qstack_size);	
void reinit_ds ();

#endif /* IncEagDs */
