/*
   File: lr.h
   Defines a lr recursive backup parser generator
  
   CVS ID: "$Id: lr.h,v 1.3 2004/12/26 00:48:46 marcs Exp $"
*/
#ifndef IncLr
#define IncLr

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>

void generate_lr_parser (string fname, string_list predefs);

#endif /* IncLr */
