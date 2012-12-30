/*
   File: skelmain.c
   Defines main program of skeleton generator.
  
   CVS ID: "$Id: skelmain.c,v 1.3 2004/12/26 00:48:47 marcs Exp $"
*/

/* general includes */
#include <stdio.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_error.h>
#include <ebs_memalloc.h>
#include <ebs_textstorage.h>

/* local includes */
#include "tree.h"
#include "numbering.h"
#include "parser.h"
#include "codeskel.h"
#include "main.h"

int v1_compatibility;

static FILE *open_input_file (char *name)
	{ char fname[MAXFNAME];
	  FILE *fd;

	  sprintf (fname, "%s.eag", name);
	  if ((fd = fopen (fname, "r"))) return (fd);
	  sprintf (fname, "%s", name);
	  if ((fd = fopen (fname, "r"))) return (fd);
	  panic ("could not open inputfile %s", name);
	  return ((FILE *) NULL);
	};

static void print_usage ()
	{ wlog ("usage: eag-skel [flags] filename");
	  wlog ("-h:  provide this help");
	  wlog ("-v:  verbose");
	  wlog ("-fv: full verbose");
	  wlog ("-v1: version 1.6 syntax");
	  exit (4);
	};

static void syntax_error (char *syn_error)
	{ error ("error on command line: %s", syn_error);
	  print_usage ();
	};

static void scan_option (char *ptr)
	{ if (strcmp (ptr, "h") == 0) print_usage ();
	  else if (strcmp (ptr, "v") == 0) verbose = 1;
	  else if (strcmp (ptr, "fv") == 0) { full_verbose = 1; verbose = 1; }
	  else if (strcmp (ptr, "v1") == 0) v1_compatibility = 1;
	  else syntax_error ("illegal option specified");
	};

void add_grammar_to_inputs (string gname)
	{
	};

int main (int argc, char **argv)
	{ FILE *in;
	  int ix;
	  char *fname = NULL;

	  for (ix = 1; ix < argc; ix++)
	     { char *arg = argv[ix];
	       if (arg[0] == '-') scan_option (arg+1);
	       else if (fname == NULL) fname = arg;
	       else syntax_error ("too many filenames specified");
	     };
	  if (fname == NULL) { fname = "_stdin"; in = stdin; }
	  else in = open_input_file (fname);
	  init_error ();
	  init_textstorage (100000);
	  init_tree ();
	  parse_eag (in, fname, 1);
	  do_numbering (1);
	  generate_skeleton (fname);
	  return (0);
	};
