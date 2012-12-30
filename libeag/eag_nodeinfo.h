/*
   File: eag_nodeinfo.h
   Defines the information in the nodenrs and types

   CVS ID: "$Id: eag_nodeinfo.h,v 1.3 2004/12/25 22:19:08 marcs Exp $"
*/
#ifndef IncEagNodeInfo
#define IncEagNodeInfo

/* libebs includes */
#include <ebs_bst.h>

/*
   Tree nodes are built for every alternative in the syntax rules
   of the original grammar, as well as for special purposes in the
   editor, semiterminals or rules recognized by a lexicon
*/
#define undefined 0
#define leaf_node 1
#define normal_node 2
#define lexicon_node 3
#define predicate_node 4
#define semipredicate_node 5
#define typed_placeholder_node 6
#define untyped_placeholder_node 7
#define ambiguous_node 8
#define error_node 9

#define mk_nodenr(modnr,lnodenr) (((modnr) << 16) + (lnodenr))
#define modnr_from_nodenr(nodenr) ((nodenr) >> 16)
#define lnodenr_from_nodenr(nodenr) ((nodenr) & 0xffff)

/* Exported code */
string module_name_from_nodenr (int nodenr);
string name_from_nodenr (int nodenr);

#endif /* IncEagNodeInfo */
