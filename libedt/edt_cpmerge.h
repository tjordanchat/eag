/*
   File: edt_cpmerge.h
   Defines the actions for copying the syntax tree or merging it
   with an existing one, possibly leading to 3D trees.

   CVS ID: "$Id: edt_cpmerge.h,v 1.3 2004/12/25 22:40:51 marcs Exp $"
*/
#ifndef IncEdtCpMerge
#define IncEdtCpMerge

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_value.h>

/* libeag includes */
#include <eag_ds.h>

/* local includes */
#include "edt_edit_ds.h"

/* Exported vars and code */
extern etree_node the_root;
void dump_etree (int with);
void copy_tree ();
void init_cpmerge ();

#endif /* IncEdtCpMerge */
