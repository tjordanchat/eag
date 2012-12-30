/*
   File: eag_init.c
   Initializes the transducer or recognizer routines

   CVS ID: "$Id: eag_init.c,v 1.4 2008/06/18 13:59:18 marcs Exp $"
*/

/* global includes */
#include <stdio.h>
#include <stdlib.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_error.h>
#include <ebs_textstorage.h>
#include <ebs_value.h>

/* libeag includes */
#include "eag_ds.h"
#include "eag_textparsing.h"
#include "eag_init.h"

static int strstore_size;
static int tstack_size;
static int qstack_size;
static int pbuffer_size;
static int ubuffer_size;
static int max_nr_parses;
static void init_options ()
	{ strstore_size = 100000;
	  tstack_size = 10000;
	  qstack_size = 100000;
	  pbuffer_size = 100000;
	  ubuffer_size = 100000;
	  max_nr_parses = -1;
	};

static void print_usage ()
	{ wlog ("usage: transducer [flags] [filename]");
	  wlog ("or:    editor [flags] [Xtoolkitoptions] [filename]");
	  wlog ("-h:  provide this help");
	  wlog ("-v:  verbose");
	  wlog ("-db: enable debugging");
	  wlog ("-fv: full verbose");
	  wlog ("-ps [size]: use size as parse buffer size");
	  wlog ("-qs [size]: use size as continuation stack size");
	  wlog ("-ss [size]: use size as string store size");
	  wlog ("-ts [size]: use size as tree stack size");
	  wlog ("-us [size]: use size as unparse buffer size (editor only)");
	  wlog ("-P number:  maximum number of parses");
	  exit (4);
	};

static void syntax_error (string error_string)
	{ error ("error on command line: %s", error_string);
	  print_usage ();
	};

static void scan_integer_value (int *i, int argc, char **argv, int *trgt)
	{ int value;
	  *i += 1;
	  if (*i == argc) syntax_error ("missing integer argument");
	  value = strtol (argv[*i], (char **)NULL, 0);
	  if (value <= 0) syntax_error ("illegal value");
	  if (*trgt < 0) *trgt = value;
	  else if (value > *trgt) *trgt = value;
	  else warning ("specified value < default value, no effect");
	};

static void scan_option (char *ptr, int *i, int argc, char **argv)
	{ if (strcmp (ptr, "h") == 0) print_usage ();
	  else if (strcmp (ptr, "v") == 0) verbose = 1;
	  else if (strcmp (ptr, "db") == 0) debug = 1;
	  else if (strcmp (ptr, "fv") == 0) { verbose = 1; full_verbose = 1; }
	  else if (strcmp (ptr, "ps") == 0) scan_integer_value (i, argc, argv, &pbuffer_size);
	  else if (strcmp (ptr, "qs") == 0) scan_integer_value (i, argc, argv, &qstack_size);
	  else if (strcmp (ptr, "ss") == 0) scan_integer_value (i, argc, argv, &strstore_size);
	  else if (strcmp (ptr, "ts") == 0) scan_integer_value (i, argc, argv, &tstack_size);
	  else if (strcmp (ptr, "us") == 0) scan_integer_value (i, argc, argv, &ubuffer_size);
	  else if (strcmp (ptr, "P") == 0)  scan_integer_value (i, argc, argv, &max_nr_parses);
	  else syntax_error ("illegal option specified");
	};

static FILE *scanargs (int argc, char **argv, char **fname)
	{ FILE *input;
	  int ix;
	  char *name = NULL;
	  for (ix = 1; ix < argc; ix++)
	     { char *arg = argv[ix];
	       if (arg[0] == '-') scan_option (arg+1, &ix, argc, argv);
	       else if (name == NULL) name = arg;
	       else syntax_error ("too many input files specified");
	     };
	  if (name == NULL) input = stdin;
	  else if (!(input = fopen (name, "r")))
	     syntax_error ("could not open input file");
	  *fname = name;
	  return (input);
	};

int init_transducer (int argc, char **argv, int edit, int *usize, char **fname)
	{ FILE *input;

	  /* primary initialization */
	  init_error ();
	  init_options ();

	  /* command line parsing */
	  input = scanargs (argc, argv, fname);

	  /* initialize data structures */
	  init_textstorage (strstore_size);
	  init_ds (tstack_size, qstack_size);
	  init_textparsing (pbuffer_size, max_nr_parses);
	  *usize = ubuffer_size;

	  if (edit && (input == stdin)) return (0);
	  copy_file_into_parsebuffer (input);
	  return (1);
	};
