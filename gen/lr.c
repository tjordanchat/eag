/*
   File: lr.c
   Defines a lr recursive backup parser generator
  
   CVS ID: "$Id: lr.c,v 1.3 2004/12/26 00:48:46 marcs Exp $"
*/

/* general includes */
#include <stdio.h>

/* libeag includes */
#include <ebs_bst.h>
#include <ebs_error.h>
#include <ebs_memalloc.h>

/* Local includes */
#include "tree.h"
#include "numbering.h"
#include "main.h"
#include "typing.h"
#include "warshall.h"
#include "common.h"
#include "codemeta.h"
#include "lr.h"

void generate_lr_parser (string fname, string_list predefs)
	{ full_verbose = 1; verbose = 1;
	  warning ("generating lr parser...");
	  open_output_file (fname, "lr", "c");
	  generate_std_includes (predefs);
	  generate_meta_rules ();
	  close_output_file ();
	};
