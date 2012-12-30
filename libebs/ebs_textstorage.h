/*
   File: ebs_textstorage.h
   Stores text fragments in a balanced binary tree
   Defines one storage region for string operations

   CVS ID: "$Id: ebs_textstorage.h,v 1.3 2004/12/25 15:17:33 marcs Exp $"
*/
#ifndef IncEbsTextstorage
#define IncEbsTextstorage

/* local includes */
#include "ebs_bst.h"

/* Pointer to the string store */
extern char *strstore;

/* Exported allocators */
string addto_names (char *t);
string new_unique_name ();
string lookup_name (char *t);
void init_textstorage (int strstore_size);
void dump_textstorage ();

#endif /* IncEbsTextstorage */
