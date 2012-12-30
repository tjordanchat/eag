/*
   File: main.c
   Defines main program
  
   CVS ID: "$Id: main.c,v 1.5 2007/10/22 12:54:20 marcs Exp $"
*/

/* general includes */
#include <stdio.h>
#include <string.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_error.h>
#include <ebs_memalloc.h>
#include <ebs_textstorage.h>

/* local includes */
#include "tree.h"
#include "lex.h"
#include "parser.h"
#include "numbering.h"
#include "ident.h"
#include "typing.h"
#include "lattice.h"
#include "empty.h"
#include "flow.h"
#include "constant.h"
#include "lexicon.h"
#include "reduce.h"
#include "prepare.h"
#include "eag_unparser.h"
#include "layout.h"
#include "lr.h"
#include "rules.h"
#include "lookahead.h"
#include "leftcorner.h"
#include "topdown.h"
#include "recursion.h"
#include "placeholder.h"
#include "main.h"

int showversion;
int topdown;
int leftcorner;
int interface_to_c;
int lr;
int editor;
int layoutflag;
int placeholderflag;
int indirect_templates;
int traceflag;
int matchflag;
int dumpflag;
int lookahead;
int lookahead_error_messages;
int qstackflag;
int v1_compatibility;
int ignore_lexicon;
static int strstore_size;

static void init_options ()
	{ showversion = 0;
	  topdown = 0;
	  leftcorner = 1;
	  interface_to_c = 0;
	  lr = 0;
	  editor = 0;
	  layoutflag = 0;
	  placeholderflag = 0;
	  indirect_templates = 0;
	  traceflag = 0;
	  matchflag = 0;
	  dumpflag = 0;
	  lookahead = 1;
	  lookahead_error_messages = 0;
	  qstackflag = 0;
	  ignore_lexicon = 0;
	  v1_compatibility = 0;
	  strstore_size = 100000;
	};

static string basename_from_name (char *name)
	{ char basename[MAXFNAME];
	  char *sptr, *dptr;
	  for (sptr = name, dptr = basename;
	       (*sptr != '\0') && (*sptr != '.');
	       sptr++, dptr++)
	     *dptr = *sptr;
	  *dptr = '\0';
	  return (addto_names (basename));
	};

static FILE *open_input_file (char *name, string *ret_fname)
	{ char fname[MAXFNAME];
	  FILE *fd;

	  sprintf (fname, "%s.eag", name);
	  if ((fd = fopen (fname, "r")))
	     { *ret_fname = addto_names (fname);
	       return (fd);
	     };
	  sprintf (fname, "%s.gra", name);
	  if ((fd = fopen (fname, "r")))
	     { *ret_fname = addto_names (fname);
	       return (fd);
	     };
	  sprintf (fname, "%s.lex", name);
	  if ((fd = fopen (fname, "r")))
	     { *ret_fname = addto_names (fname);
	       return (fd);
	     };
	  sprintf (fname, "%s", name);
	  if ((fd = fopen (fname, "r")))
	     { *ret_fname = addto_names (fname);
	       return (fd);
	     };
	  panic ("could not open inputfile %s", name);
	  return ((FILE *) NULL);
	};

#ifndef PATH
#define PATH "/home/marcs/eag/include"
#endif
static FILE *open_predefines_file (char *name, string *ret_fname)
	{ char fname[MAXFNAME];
	  FILE *fd;

	  sprintf (fname, "%s/%s.eag", PATH, name);
	  if ((fd = fopen (fname, "r")))
	     { *ret_fname = addto_names (fname);
	       return (fd);
	     };
	  sprintf (fname, "./%s.eag", name);
	  if ((fd = fopen (fname, "r")))
	     { *ret_fname = addto_names (fname);
	       return (fd);
	     };
	  panic ("could not open predefines file %s.eag", name);
	  return ((FILE *) NULL);
	};

static void print_usage ()
	{ wlog ("usage: eag-compile [flags] [filenames]");
	  wlog ("-h:  provide this help");
	  wlog ("-V:  show version");
	  wlog ("-v:  verbose");
	  wlog ("-fv: full verbose");
	  wlog ("-p [filename]: include filename as extra prelude");
	  wlog ("-ic: generate interface to C");
	  wlog ("-ed: generate editor");
	  wlog ("-td: generate topdown parser");
	  wlog ("-lc: generate leftcorner parser");
	  wlog ("-lr: generate lr parser (not yet implemented)");
	  wlog ("-gl: generate code to report error messages on lookahead failure");
	  wlog ("-il: generate code without lexicon");
	  wlog ("-nl: do not use lookahead");
	  wlog ("-sl: shortcut rule(s) for layout");
	  wlog ("-pp: generate code to parse placeholders");
	  wlog ("-it: generate indirect templates");
	  wlog ("-D:  generate code to dump parse tree");
	  wlog ("-M:  generate code to count number of matches");
	  wlog ("-T:  generate code to enable tracing");
	  wlog ("-qc: generate code to check the runtime qstack");
	  wlog ("-v1: accept eag version 1.6 syntax and semantics");
	  wlog ("-ss [size]: use size as string store size");
	  exit (4);
	};

static void syntax_error (char *syn_error)
	{ error ("error on command line: %s", syn_error);
	  print_usage ();
	};

#define PREDFNAME "eag_stddefs"
static string_list inputs;
static string_list predefs;
static void init_command_line_parsing ()
	{ init_error ();
	  init_options ();
	  inputs = new_string_list ();
	  predefs = new_string_list ();
	  app_string_list (predefs, PREDFNAME);
	};

static void new_prelude (int *i, int argc, char **argv)
	{ *i += 1;
	  if (*i == argc) syntax_error ("missing prelude name");
	  app_string_list (predefs, argv[*i]);
	};

static void new_strstore_size (int *i, int argc, char **argv)
	{ *i += 1;
	  if (*i == argc) syntax_error ("missing size");
	  strstore_size = strtol (argv[*i], (char **)NULL, 0);
	};

static void scan_option (char *ptr, int *i, int argc, char **argv)
	{ if (strcmp (ptr, "td") == 0)      { topdown = 1; leftcorner = 0; lr = 0; }
	  else if (strcmp (ptr, "lc") == 0) { topdown = 0; leftcorner = 1; lr = 0; }
	  else if (strcmp (ptr, "lr") == 0) { topdown = 0; leftcorner = 0; lr = 1; }
	  else if (strcmp (ptr, "ed") == 0)
             { editor = 1; layoutflag = 1; placeholderflag = 1; interface_to_c = 0; }
	  else if (strcmp (ptr, "it") == 0) indirect_templates = 1;
	  else if (strcmp (ptr, "sl") == 0) layoutflag = 1;
	  else if (strcmp (ptr, "il") == 0) ignore_lexicon = 1;
	  else if (strcmp (ptr, "ic") == 0) { interface_to_c = 1; editor = 0; }
	  else if (strcmp (ptr, "pp") == 0) { layoutflag = 1; placeholderflag = 1; }
	  else if (strcmp (ptr, "p") == 0)  new_prelude (i, argc, argv);
	  else if (strcmp (ptr, "h") == 0)  print_usage ();
	  else if (strcmp (ptr, "V") == 0)  showversion = 1;
	  else if (strcmp (ptr, "v") == 0)  { showversion = 1; verbose = 1; }
	  else if (strcmp (ptr, "fv") == 0) { showversion = 1; verbose = 1; full_verbose = 1; }
	  else if (strcmp (ptr, "T") == 0)  traceflag = 1;
	  else if (strcmp (ptr, "M") == 0)  matchflag = 1;
	  else if (strcmp (ptr, "D") == 0)  dumpflag = 1;
	  else if (strcmp (ptr, "nl") == 0) lookahead = 0;
	  else if (strcmp (ptr, "gl") == 0) lookahead_error_messages = 1;
	  else if (strcmp (ptr, "qc") == 0) qstackflag = 1;
	  else if (strcmp (ptr, "v1") == 0) v1_compatibility = 1;
	  else if (strcmp (ptr, "ss") == 0) new_strstore_size (i, argc, argv);
	  else syntax_error ("illegal option specified");
	};

static void report_version ()
	{ if (!showversion) return;
	  wlog ("This is EAG, C Version %s, (C) University of Nijmegen", VERSION);
	};

void add_grammar_to_inputs (string gname)
	{ int ix;
	  for (ix = 0; ix < inputs -> size; ix++)
	     if (streq (inputs -> array[ix], gname)) return;
	  app_string_list (inputs, gname);
	};

static void parse_command_line (int argc, char **argv)
	{ int ix;
	  for (ix = 1; ix < argc; ix++)
	     { char *arg = argv[ix];
	       if (arg[0] == '-') scan_option (arg+1, &ix, argc, argv);
	       else add_grammar_to_inputs (arg);
	     };
	};

static void init_compiler ()
	{ init_textstorage (strstore_size);
	  init_tree ();
	};

static void context_free_analysis (string *basename)
	{ string fname = string_nil;
	  FILE *fd;
	  int ix;
	  init_lex ();
	  init_parser ();
	  for (ix = 0; ix < predefs -> size; ix++)
	     { fd = open_predefines_file (predefs -> array[ix], &fname);
	       parse_eag (fd, fname, 1);
	     };
	  if (!inputs -> size)
	     { *basename = "_stdin";
	       parse_eag (stdin, "standard in", 0);
	     }
	  else *basename = basename_from_name (inputs -> array[0]);
	  for (ix = 0; ix < inputs -> size; ix++)
	     { fd = open_input_file (inputs -> array[ix], &fname);
	       parse_eag (fd, fname, 0);
	     };
	  finish_parser ();
	};

static void context_sensitive_analysis ()
	{ do_numbering (0);
	  identification ();
	  do_element_numbering ();
	  infer_typing ();
	  construct_lattices ();
	  do_empty_classification ();
	  flow_check ();
	  dump_grammar_typing ();
	};

static void prepare_parser_generation (string basename)
	{ propagate_constants ();
	  check_lexicon ();
	  reduce_grammar ();
	  check_recursion ();
	  if (placeholderflag) decide_placeholders ();
	  if (lookahead) determine_lookahead_sets ();
	  prepare_grammar ();
	  prepare_lexicon (basename);
	  if (editor) analyze_layout ();
	  if (editor) generate_unparsing_rules (basename);
	};

static void generate_code (string basename)
	{ if (topdown) generate_topdown_parser (basename, predefs);
	  if (leftcorner) generate_leftcorner_parser (basename, predefs);
	  if (lr) generate_lr_parser (basename, predefs);
	};

int main (int argc, char **argv)
	{ string basename;
	  init_command_line_parsing ();
	  parse_command_line (argc, argv);
	  report_version ();
	  init_compiler ();
	  context_free_analysis (&basename);
	  context_sensitive_analysis ();
	  prepare_parser_generation (basename);
	  unparse_eag ();
	  generate_code (basename);
	  return (0);
	};
