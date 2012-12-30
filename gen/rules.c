/*
   File: rules.c
   Generates the unparsing rules
  
   CVS ID: "$Id: rules.c,v 1.3 2004/12/26 00:48:47 marcs Exp $"
*/

/* general includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_error.h>
#include <ebs_primio.h>

/* libedt includes */
#include <edt_editor.h>

/* local includes */
#include "tree.h"
#include "limits.h"
#include "main.h"
#include "prepare.h"
#include "numbering.h"
#include "common.h"
#include "layout.h"
#include "rules.h"

static FILE *rfile;
static void code_rule (char *format, ...)
	{ char buf[MAXSTRLEN];
	  va_list arg_ptr;
	  va_start (arg_ptr, format);
	  vsprintf (buf, format, arg_ptr);
	  va_end (arg_ptr);
	  fprintf (rfile, buf);
	};

static void open_rule_file (string gram)
	{ char totalname[MAXFNAME];
	  sprintf (totalname, "%s.rules", gram);
	  if (!(rfile = fopen (totalname, "w")))
	     panic ("can't open rulefile %s", totalname);
	  code_rule ("number of rules: %d\n", max_nodenr);
	  code_rule ("number of tuples: %d\n", max_members_per_alt);
	};
#define close_rule_file() fclose (rfile)

static void generate_member (member m)
	{ if (is_an_invisible_member (m)) return;
	  switch (m -> tag)
	     { case tag_call:
		  code_rule ("%s%s%s", typed_open_symbol,
			     m -> u.call.nonterminal, typed_close_symbol);
		  break;
	       case tag_terminal:
		  output_string (rfile, m -> u.terminal);
		  break;
	       case tag_semiterminal: code_rule ("{}");
	       default: break;
	     };
	};

static int rule_is_unoriented (alt a)
	{ int ix;
	  member_list mems = a -> members;
	  if (mems == member_list_nil) return (1);
	  for (ix = 0; ix < mems -> size; ix++)
	     if (mems -> array[ix] -> followlayout) return (0);
	  return (1);
	};

static void generate_unoriented_rule (alt a)
	{ member_list mems = a -> members;
	  int ix;
	  code_rule ("::\n");
	  if (mems != member_list_nil)
	     for (ix = 0; ix < mems -> size; ix++) 
	        generate_member (mems -> array[ix]);
	  code_rule ("\n");
	};

static void generate_default_horizontal_rule (member_list mems)
	{ int ix;
	  for (ix = 0; ix < mems -> size; ix++)
	     { generate_member (mems -> array[ix]);
	       if (mems -> array[ix] -> followlayout) code_rule (".");
	     };
	};

static void generate_default_vertical_rule (member_list mems)
	{ int ix;
	  for (ix = 0; ix < mems -> size; ix++)
	     { generate_member (mems -> array[ix]);
	       if (mems -> array[ix] -> followlayout) code_rule ("\n");
	     };
	};

/*
   Heuristic rules for horizontal layout only kill layout.
   We therefore unmark these members and then generate
   according to the nonheuristic rules. This also has
   the wanted side effect in changing the corresponding
   vertical rule and template.
*/
static int heuristic_horizontal_rule (member_list mems)
	{ int ix;
	  member m;
	  for (ix = 0; ix < mems -> size; ix++)
	     if (!is_an_invisible_member (mems -> array[ix])) break;
	  m = mems -> array[ix];
	  if (m -> tag != tag_terminal) return (0);
	  if (strlen (m -> u.terminal) != 1) return (0);
	  m -> followlayout = 0;
	  for (ix++; ix < mems -> size; ix++)
	     { int iy;
	       if (mems -> array[ix] -> followlayout)
	          for (iy = ix + 1; iy < mems -> size; iy++)
		     { m = mems -> array[iy];
		       if ((m -> tag == tag_terminal) &&
			   (strlen (m -> u.terminal) == 1))
		          mems -> array[ix] -> followlayout = 0;
		       if (!is_an_invisible_member (m)) break;
		     };
	     };
	  generate_default_horizontal_rule (mems);
	  return (1);
	};

static void generate_horizontal_rule (alt a)
	{ member_list mems = a -> members;
	  code_rule (":H:\n");
	  if (!heuristic_horizontal_rule (mems))
	     generate_default_horizontal_rule (mems);
	  code_rule ("\n");
	};

/*
   A leading or trailing terminal of size >= 2 is treated as
   a bracket. These occur frequently in usual programming
   language constructs
*/
#define is_large_terminal(m)\
	(((m) -> tag == tag_terminal) && (strlen ((m) -> u.terminal) > 1))

static int alt_has_brackets (member_list mems)
	{ int ix;
	  for (ix = 0; ix < mems -> size; ix++)
	     { if (is_large_terminal (mems -> array[ix])) return (1);
	       if (!is_an_invisible_member (mems -> array[ix])) return (0);
	     };
	  for (ix = mems -> size - 1; 0 <= ix; ix--)
	     { if (is_large_terminal (mems -> array[ix])) return (1);
	       if (!is_an_invisible_member (mems -> array[ix])) return (0);
	     };
	  return (0);
	};

static int heuristic_vertical_rule (member_list mems)
	{ int ix;
	  member prev;
	  if (!alt_has_brackets (mems)) return (0);
	  for (ix = 0; ix < mems -> size; ix++)
	     if (!is_an_invisible_member (mems -> array[ix])) break;
	  prev = mems -> array[ix];
	  generate_member (prev);
	  for (ix++; ix < mems -> size; ix++)
	     { member cur = mems -> array[ix];
	       if (!is_an_invisible_member (cur))
		  { if (prev -> followlayout)
		       { if ((cur -> tag == tag_call) &&
			     (prev -> tag == tag_terminal))
			    code_rule ("\n.  ");
		         else code_rule ("\n");
		       };
		    generate_member (cur);
		    prev = cur;
		  };
	     };
	  return (1);
	};

static void generate_vertical_rule (alt a)
	{ member_list mems = a -> members;
	  code_rule (":V:\n");
	  if (!heuristic_vertical_rule (mems))
	     generate_default_vertical_rule (mems);
	  code_rule ("\n");
	};

static void generate_alt_type_and_sonnrs (alt a)
	{ int ix;
	  member_list mems = a -> members;
	  code_rule ("type %d", a -> nodenr);
	  if (mems != member_list_nil)
	     for (ix = 0; ix < mems -> size; ix++)
	        { member m = mems -> array [ix];
	          if (!is_an_invisible_member (m) &&
		      ((m -> tag == tag_call) ||
		       (m -> tag == tag_semiterminal)))
		     code_rule (", %d", m -> sonnr);
	        };
	  code_rule ("\n");
	};

static void generate_unparsing_rule_for_alt (alt a)
	{ generate_alt_type_and_sonnrs (a);
	  if (rule_is_unoriented (a)) generate_unoriented_rule (a);
	  else
	     { generate_horizontal_rule (a);
	       generate_vertical_rule (a);
	     };
	};

static void generate_unparsing_rule_for_rule (rule srule)
	{ int ix;
	  if (!srule -> placeholder) return;
	  for (ix = 0; ix < srule -> alts -> size; ix++)
	     generate_unparsing_rule_for_alt (srule -> alts -> array[ix]);
	};

static void generate_unparsing_rule_for_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     generate_unparsing_rule_for_rule (all_rules[ix]);
	};

static void generate_unparsing_rule_for_startrule ()
	{ code_rule ("type 0, 1\n");
	  code_rule ("::\n");
	  code_rule ("%s%s%s\n", typed_open_symbol,
		     start_rule -> u.call.nonterminal, typed_close_symbol);
	};

void generate_unparsing_rules (string gram)
	{ warning ("generating unparsing rules...");
	  open_rule_file (gram);
	  generate_unparsing_rule_for_startrule ();
	  generate_unparsing_rule_for_rules ();
	  close_rule_file ();
	};
