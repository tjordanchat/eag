/*
   File: warshall.c
   Implements Warshalls algorithm
  
   CVS ID: "$Id: warshall.c,v 1.3 2004/12/26 00:48:47 marcs Exp $"
*/

/* general includes */
#include <stdio.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_memalloc.h>

/* local includes */
#include "warshall.h"

char *warshall (int n, char *rel)
	{ char *result = (char *) ckcalloc (n*n, sizeof (char));
	  int i,j,k;
	  for (i=0; i<n; i++)
	     for (j=0; j<n; j++) result[i * n + j] = rel[i * n + j];
	  for (k=0; k<n; k++)
	     for (i=0; i<n; i++)
		for (j=0; j<n; j++)
		   result[i * n + j] = result[i * n + j] ||
			(result[i * n + k] && result [k * n + j]);
	  return (result);
	};
