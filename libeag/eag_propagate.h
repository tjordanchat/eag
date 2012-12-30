/*
   File: eag_propagate.h
   Defines the affix propagation mechanism

   CVS ID: "$Id: eag_propagate.h,v 1.3 2004/12/25 22:19:09 marcs Exp $"
*/
#ifndef IncEagPropagate
#define IncEagPropagate

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_value.h>

/* local includes */
#include "eag_ds.h"

#define crit_pos_has_value(p) ((p) -> sides[lower_side].a.affx -> val != value_nil)

/* Exported functions */
value calc_affix_value (pos_node pos, int side);
void unparse_affix_value (value val, pos_node pos, int side);
void propagate_affix_value ();
void propagate_predicate_value ();
void make_node_delayed ();

#endif /* IncEagPropagate */
