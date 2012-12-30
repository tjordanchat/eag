/*
   File: edt_editorparsing.c
   Defines all routines necessary for reparsing and handling error nodes

   CVS ID: "$Id: edt_editorparsing.c,v 1.3 2004/12/25 22:40:51 marcs Exp $"
*/

/* global includes */
#include <stdio.h>
#include <string.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_error.h>
#include <ebs_textstorage.h>
#include <ebs_primio.h>

/* libeag includes */
#include <eag_ds.h>
#include <eag_nodeinfo.h>
#include <eag_textparsing.h>

/* local includes */
#include "edt_editor.h"
#include "edt_editmain.h"
#include "edt_cpmerge.h"
#include "edt_unparser.h"
#include "edt_focus.h"
#include "edt_editorparsing.h"

static etree_node mk_error_node (etree_node father)
	{ char *ptr;
	  etree_node new = new_etree_node ();
	  new -> type = error_node;
	  for (ptr = parsebuffer; *ptr != EOFCHAR; ptr++);
	  *ptr = '\0';
	  new -> name = addto_names (parsebuffer);
	  new -> nodenr = 1;
	  new -> nrsons = 0;
	  new -> sons = etree_array_nil;
	  new -> father = father;
	  new -> x = -1;
	  new -> y = 0;
	  return (new);
	};

static void mk_error_tree ()
	{ etree_node tree = new_etree_node ();
	  tree -> type = normal_node;
	  tree -> name = "rootnode";
	  tree -> nodenr = 0;
	  tree -> nrsons = 1;
	  tree -> sons = new_eson_space (1);
	  tree -> sons[0] = mk_error_node (tree);
	  tree -> father = etree_node_nil;
	  tree -> x = -1;
	  tree -> y = 0;
	  the_root = tree;
	};

extern void transduce ();
void initial_transduce_and_unparse (int status, char *init_buf)
	{ if (!status) copy_into_parsebuffer (init_buf, strlen (init_buf), 0);
	  transduce ();
	  if (!nrofparses) mk_error_tree ();
	  unparse ();
	  changed = 1;
	  reset_root_focus ();
	};

static void reparse_parsebuffer ()
	{ rfre_etree_node (the_root);
	  reinit_ds ();
	  reinit_textparsing ();
	  init_cpmerge ();
	  transduce ();
	  if (!nrofparses) mk_error_tree ();
	  unparse ();
	  changed = 1;
	  reset_root_focus ();
	};

void reparse ()
	{ copy_into_parsebuffer (unparse_buffer, strlen (unparse_buffer), 0);
	  reparse_parsebuffer ();
	};

static char *start_of_focus ()
	{ int line = 0;
	  int col = 0;
	  char *ptr = unparse_buffer;
	  while (1)
	     { if ((line == current_focus -> begin_y) &&
		   (col == current_focus -> begin_x)) return (ptr);
	       if (!*ptr) return (NULL);
	       if (*ptr == '\n')
		  { line++;
		    col = 0;
		  }
	       else col++;
	       ptr++;
	     };
	};

static void replace_text (char *begin, int width, char *s)
	{ char *dptr, *sptr;
	  char *end = begin + width;
	  int diff = strlen (s) - width;
	  if (diff > 0)
	     { /* copy backwards */
	       sptr = end + strlen (end) + 1;
	       dptr = sptr + diff;
	       while (sptr != end - 1) *dptr-- = *sptr--;
	     }
	  else if (diff < 0)
	     { sptr = end;
	       dptr = end + diff;
	       while (*sptr) *dptr++ = *sptr++;
	       *dptr = '\0';
	     };
	  for (dptr = begin, sptr = s; *sptr; sptr++, dptr++) *dptr = *sptr;
	};

void replace_focus_by_text (char *s)
	{ char *begin = start_of_focus ();
	  replace_text (begin,
		current_focus -> end_x - current_focus -> begin_x, s);
	};

void save_unparse_buffer (char *sname)
	{ FILE *out = fopen (sname, "w");
	  if (!out) error ("could not write in file '%s'", sname);
	  else
	     { fputs (unparse_buffer, out);
	       fclose (out);
	       changed = 0;
	     };
	};

void load_parsebuffer (char *sname)
	{ FILE *in = fopen (sname, "r");
	  if (!in) error ("could not read from file '%s'", sname);
	  else
	     { Filename = addto_names (sname);
	       copy_file_into_parsebuffer (in);
	       reparse_parsebuffer ();
	     };
	};
