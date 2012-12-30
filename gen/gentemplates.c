/*
   File: gentemplates.c
   Generates the template list
  
   CVS ID: "$Id: gentemplates.c,v 1.3 2004/12/26 00:48:45 marcs Exp $"
*/

/* general includes */
#include <stdio.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_error.h>
#include <ebs_primio.h>

/* libedt includes */
#include <edt_editor.h>

/* local includes */
#include "tree.h"
#include "main.h"
#include "prepare.h"
#include "numbering.h"
#include "common.h"
#include "layout.h"
#include "gentemplates.h"

/*
   editor template list generation
*/
static int alt_contains_semiterminals (alt a)
	{ int ix;
	  member_list mems = a -> members;
	  if (mems != member_list_nil) 
	     for (ix = 0; ix < mems -> size; ix++)
		if (mems -> array[ix] -> tag == tag_semiterminal) return (1);
	  return (0);
	};

static int find_first_call (member_list mems, rule *def,
			     int from, int *found)
	{ int ix;
	  for (ix = from; ix < mems -> size; ix++)
	     { member m = mems -> array[ix];
	       if (!is_an_invisible_member (m))
		  { if (m -> tag != tag_call) return (0);
		    *found = ix;
		    *def = m -> u.call.def;
		    return (1);
	          };
	     };
	  return (0);
	};

static int alt_is_single_call (alt a, rule *def)
	{ int ix;
	  member_list mems = a -> members;
	  if (mems == member_list_nil) return (0);
	  if (!find_first_call (mems, def, 0, &ix)) return (0);
	  for (ix++; ix < mems -> size; ix++)
	     if (!is_an_invisible_member (mems -> array[ix])) return (0);
	  return (1);
	};

static void generate_template_member (member m)
	{ if (is_an_invisible_member (m)) return;
	  switch (m -> tag)
	     { case tag_call:
		  code_string ("%s%s%s", typed_open_symbol,
			       m -> u.call.nonterminal, typed_close_symbol);
		  break;
	       case tag_terminal:
		  output_unquoted_string (out, m -> u.terminal);
	       default: break;
	     };
	};

static void generate_template_members (member_list mems)
	{ int ix;
	  if (mems == member_list_nil) return;
	  for (ix = 0; ix < mems -> size; ix++)
	     { member m = mems -> array[ix];
	       generate_template_member (m);
	       if (m -> followlayout) fputc (' ', out);
	     };
	};

static void generate_text_template (int nodenr, alt a)
	{ fprintf (out, "\t  enter_template_in_list_text (%d, \"", nodenr);
	  generate_template_members (a -> members);
	  fprintf (out, "\");\n");
	};

static void enter_templates_of_rule_with_nodenr (int nodenr, rule srule,
						  int indirect);
static void check_to_generate_indirect_templates (int nodenr, rule def)
	{ if (indirect_templates)
	     enter_templates_of_rule_with_nodenr (nodenr, def, 1);
	  fprintf (out, "\t  enter_template_in_list_text (%d, \"<|%s|>\");\n",
				nodenr, def -> nonterminal);
	};

static void enter_templates_of_alt (int nodenr, alt a)
	{ rule def;
	  if (alt_contains_semiterminals (a)) return;
	  if (alt_is_single_call (a, &def))
	     check_to_generate_indirect_templates (nodenr, def);
	  else generate_text_template (nodenr, a);
	};

static void enter_templates_of_rule_with_nodenr (int nodenr, rule srule,
						  int indirect)
	{ int ix;
	  for (ix = 0; ix < srule -> alts -> size; ix++)
	     enter_templates_of_alt (nodenr, srule -> alts -> array[ix]);
	};

static void enter_templates_of_rule (rule srule)
	{ int nodenr;
	  if (!srule -> placeholder) return;
	  nodenr = srule -> alts -> array[0] -> nodenr - 1;
	  enter_templates_of_rule_with_nodenr (nodenr, srule, 0);
	};

static void enter_templates_of_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     enter_templates_of_rule (all_rules[ix]);
	};

void generate_enter_templates ()
	{ warning ("coding templates table...");
	  fprintf (out, "static void enter_templates ()\n");
	  fprintf (out, "\t{\n");
	  enter_templates_of_rules ();
	  fprintf (out, "\t};\n\n");
	};
