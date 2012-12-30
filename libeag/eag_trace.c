/*
   File: eag_trace.c
   Provides some debugging support

   CVS ID: "$Id: eag_trace.c,v 1.3 2004/12/25 22:19:09 marcs Exp $"
*/

/* global includes */
#include <stdio.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_error.h>
#include <ebs_value.h>
#include <ebs_primio.h>

/* local includes */
#include "eag_ds.h"
#include "eag_trace.h"
#include "eag_nodeinfo.h"
#include "eag_textparsing.h"

int tracing = 0;
static int indent;
void init_trace ()
	{ indent = 0;
	  tracing = 1;
	}

static void trace_char (char c)
	{ if (c == '\t') eprint_log ("\\t");
	  else if (c == '\n') eprint_log ("\\n");
	  else if (c == EOFCHAR) eprint_log ("\\e");
	  else if (c < ' ') eprint_log ("\\%o", c);
	  else eprint_log ("%c", c);
	};

static void output_indent ()
	{ eprint_log ("%d '", indent);
	  trace_char (*iptr);
	  eprint_log ("': ");
	};

void trace_affix (affix_node a)
	{ eprint_log ("%s", a -> name);
	  if (a -> val != value_nil)
	     { eprint_log ("[");
	       dump_value (a -> val);
	       eprint_log ("]");
	     };
	};

void trace_pos_side (pos_node p, int i)
	{ int tag = p -> sides[i].tag;
	  output_indent ();
	  eprint_log ("%s = (", (i == lower_side)?"lo":"hi");
	  eprint_log ("sill = %d, ", p -> sides[i].sill);
	  switch (tag)
	     { case tag_undefined: eprint_log ("_"); break;
	       case tag_single: trace_affix (p -> sides[i].a.affx); break;
	       case tag_compos:
	       case tag_concat:
	       case tag_union:
		  { int iy;
		    int nr = p -> sides[i].a.co.nr;
		    string seps = "*+|";
		    char sep = seps[tag - tag_compos];
		    for (iy = 0; iy < nr; iy++)
		       { trace_affix (p -> sides[i].a.co.affs[iy]);
			 if (iy != nr - 1) eprint_log (" %c ", sep);
		       };
		  };
	     };
	  eprint_log (")\n");
	};

static int find_position_nr (pos_node p, tree_node node)
	{ int ix;
	  for (ix = 0; ix < node -> nraffs; ix++)
	     if (node -> affs[ix] == p) return (ix);
	  return (-1);
	};

void trace_pos (pos_node p)
	{ tree_node node = p -> node;
	  int nodenr = node -> nodenr;
	  int pos_nr = find_position_nr (p, node);
	  output_indent ();
	  eprint_log ("tracing position %d of %s in module %s\n",
		      pos_nr + 1, node -> name,
		      module_name_from_nodenr (nodenr));
	  trace_pos_side (p, lower_side);
	  trace_pos_side (p, upper_side);
	};

void trace_restrict_pos (pos_node p, value mval)
	{ tree_node node = p -> node;
	  int nodenr = node -> nodenr;
	  int pos_nr = find_position_nr (p, node);
	  output_indent ();
	  eprint_log ("restricting position %d of %s in module %s\n",
		      pos_nr + 1, node -> name,
		      module_name_from_nodenr (nodenr));
	  output_indent ();
	  eprint_log ("with value: ");
	  dump_value (mval);
	  eprint_log ("\n");
	  trace_pos_side (p, lower_side);
	  trace_pos_side (p, upper_side);
	};

void trace_enter (char *s)
	{ output_indent ();
	  eprint_log (">%s\n", s);
	  indent++;
	};

void trace_alternative (char *s, int i)
	{ output_indent ();
	  eprint_log ("-%s, alt %d\n", s, i);
	};

void trace_placeholder_alternative (char *s, int untyped)
	{ output_indent ();
	  eprint_log ("-%s, %styped placeholder alt\n", s, (untyped)?"un":"");
	};

void trace_leave (char *s)
	{ indent--;
	  output_indent ();
	  eprint_log ("<%s\n", s);
	};
