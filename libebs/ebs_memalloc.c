/*
   File: memalloc.c
   Defines a checked malloc and calloc

   CVS ID: "$Id: ebs_memalloc.c,v 1.5 2008/05/28 14:00:06 marcs Exp $"
*/

/* include config.h if autoconfigured */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* general includes */
#include <stdio.h>
#include <string.h>

#ifdef HAVE_MALLOC_H
#include <malloc.h>
#else
#include <stdlib.h>
#endif

/* local includes */
#include "ebs_bst.h"
#include "ebs_error.h"
#include "ebs_memalloc.h"

void *ckmalloc (int size)
{ void *nsp = (void *) malloc ((unsigned) size);
  if (nsp != NULL) return (nsp);
  panic ("could not malloc %d bytes", size);
  return ((void *) NULL);
};

void *ckcalloc (int nr, int size)
{ void *nsp;
  if (nr == 0) return ((void *) NULL);
  nsp = (void *) calloc ((unsigned) nr, (unsigned) size);
  if (nsp != NULL) return (nsp);
  panic ("could not calloc %d chunks of %d bytes", nr, size);
  return ((void *) NULL);
};

void *ckrealloc (void *ptr, int size)
{ void *nsp = (void *) realloc ((char *) ptr, (unsigned) size);
  if (nsp != NULL) return (nsp);
  panic ("could not realloc %d bytes", size);
  return ((void *) NULL);
};

void *ckrecalloc (void *ptr, int nr, int size)
{ void *nsp = (void *) realloc ((char *) ptr, (unsigned) (nr * size));
  if (nsp != NULL) return (nsp);
  panic ("could not realloc %d chunks of %d bytes", nr, size);
  return ((void *) NULL);
};

char *newstring (char *src)
{ char *nsp = (char *) ckmalloc (strlen (src) + 1);
  strcpy (nsp, src);
  return (nsp);
};
