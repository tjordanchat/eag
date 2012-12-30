/*
   File: leftcorner.h
   Defines a leftcorner recursive backup parser generator
  
   CVS ID: "$Id: leftcorner.h,v 1.3 2004/12/26 00:48:45 marcs Exp $"
*/
#ifndef IncLeftCorner
#define IncLeftCorner

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>

void generate_leftcorner_parser (string basename, string_list predefs);

#endif /* IncLeftCorner */
