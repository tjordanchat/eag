/*
   File: eag_misc.c
   Defines some miscellaneous support routines

   CVS ID: "$Id: eag_misc.c,v 1.3 2004/12/25 22:19:08 marcs Exp $"
*/

/* global includes */
#include <stdio.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_memalloc.h>
#include <ebs_textstorage.h>
#include <ebs_value.h>

/* libeag includes */
#include "eag_ds.h"
#include "eag_misc.h"

/*
   output affix value on stdout
*/
void output_affix ()
	{ affix_node affx = popa ();
	  output_value (stdout, affx -> val);
	  callq ();
	  pusha (affx);
	  pushq (output_affix);
	};

/*
   commit cut
*/
void cut ()
	{ int *p = popip ();
	  *p = 1;
	  callq ();
	  puship (p);
	  pushq (cut);
	};

/*
   make empty strstore
*/
void make_empty_strstore ()
	{ char c = strstore[0];
	  strstore[0] = '\0';
	  callq ();
	  strstore[0] = c;
	  pushq (make_empty_strstore);
	};

/*
   The end point of all continuation
*/
void dummy_continuation ()
	{ pushq (dummy_continuation);
	};
