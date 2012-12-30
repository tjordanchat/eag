/*
   File: lattice.h
   Constructs the actual lattices
  
   CVS ID: "$Id: lattice.h,v 1.3 2004/12/26 00:48:45 marcs Exp $"
*/
#ifndef IncLattice
#define IncLattice

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_value.h>
#include <ebs_lexds.h>

/* local includes */
#include "tree.h"

extern element_set_list all_element_sets;
void construct_lattices ();

#endif /* IncLattice */
