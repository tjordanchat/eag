/*
   File: parser.h
   Parses an eag from a file and builds the hyper and meta tree
  
   CVS ID: "$Id: parser.h,v 1.3 2004/12/26 00:48:46 marcs Exp $"
*/
#ifndef IncParser
#define IncParser
#include <stdio.h>

/* libebs includes */
#include <ebs_bst.h>

void init_parser ();
void parse_eag (FILE *fd, string fname, int pred);
void finish_parser ();

#endif /* IncParser */
