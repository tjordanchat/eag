/*
   File: eag_buildtree.h
   Defines the actions necessary for building the syntax tree
   as well as the linkage to the affix tree

   CVS ID: "$Id: eag_buildtree.h,v 1.4 2004/12/25 22:19:08 marcs Exp $"
*/
#ifndef IncEagBuildTree
#define IncEagBuildTree

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_value.h>
#include <ebs_lexds.h>

/* local includes */
#include "eag_ds.h"

/* building affix nodes */
affix_node value_to_affix (char *name, value v);
affix_node make_undefined_affix ();

/* propagation links */
void add_link (affix_node aff, pos_node pos, int side);
void delete_link (affix_node aff, pos_node pos);
void make_affix_link ();

/* tree nodes */
void make_leaf_node ();
void make_simple_node ();
void make_normal_node ();
void make_lexicon_node (lexend le, value_list vals);
void make_predicate_node ();
void make_semipredicate_node ();
void make_typed_placeholder_node ();
void make_untyped_placeholder_node ();

/* predicates, leftcorner parsing */
void update_predicate_node ();
void exchange_top ();
void link_son ();
void link_predicate_son ();

/* debugging */
void dump_parse_tree_indented (tree_node tnode, int indent);
void dump_parse_tree ();

#endif /* IncEagBuildTree */
