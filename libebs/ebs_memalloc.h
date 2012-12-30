/*
   File: ebs_memalloc.h
   Defines a checked malloc and calloc, two variants of realloc and newstring

   CVS ID: "$Id: ebs_memalloc.h,v 1.3 2004/12/25 15:17:33 marcs Exp $"
*/
#ifndef IncEbsMemalloc
#define IncEbsMemalloc

/* Standard headers */
#include <stdlib.h>

/* Exported allocators */
void *ckmalloc (int size);
void *ckcalloc (int nr, int size);
void *ckrealloc (void *ptr, int size);
void *ckrecalloc (void *ptr, int nr, int size);
char *newstring (char *src);
#define ckfree(obj) free((char *) obj)

#endif /* IncEbsMemalloc */
