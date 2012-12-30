/*
   File: codeskel.c
   Codes a skeleton library out of a specification
  
   CVS ID: "$Id: codeskel.c,v 1.4 2007/10/22 12:54:20 marcs Exp $"
*/

/* general includes */
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_error.h>

/* local includes */
#include "tree.h"
#include "numbering.h"
#include "main.h"
#include "codeskel.h"

/*
   Transput stuff
*/
static FILE *out;
static void open_output_file (string basename, string suffix)
	{ char totalname[MAXFNAME];
	  time_t thetime;
	  char *atime;
	  sprintf (totalname, "%s.%s", basename, suffix);
	  if (!(out = fopen (totalname, "w")))
	     panic ("can't open outputfile %s", totalname);
	  time (&thetime);
	  atime = ctime (&thetime);
	  fprintf (out, "/*\n");
	  fprintf (out, "   File: %s\n", totalname);
	  fprintf (out, "   Generated on %s", atime);
	  fprintf (out, "   Implements the predefines library '%s'\n",
			basename);

	  fprintf (out, "*/\n\n");
	};
#define close_output_file() fclose (out)

/*
   Code skeleton include file
*/
static char *rule_qualifier (rule srule)
	{ if (srule -> kind & rule_semipredicate) return ("semipred");
	  if (srule -> kind & rule_predicate) return ("pred");
	  return ("rule");	/* should not occur actually */
	};

static void open_conditional (string name)
	{ fprintf (out, "#ifndef Inc_%s\n", name);
	  fprintf (out, "#define Inc_%s\n\n", name);
	};

static void close_conditional (string name)
	{ fprintf (out, "#endif /* Inc_%s */\n", name);
	};

static void code_include_rule (rule srule)
	{ if (!srule -> ext) return;
	  fprintf (out, "void %s_%s ();\n", rule_qualifier (srule), srule -> nonterminal);
	};

static void code_include_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     code_include_rule (all_rules[ix]);
	};

static void code_include_metarule (meta_rule mrule)
	{ if (!mrule -> ext) return;
	  fprintf (out, "affix_node make_%s_affix ();\n", mrule -> nonterminal);
	  fprintf (out, "void rec_%s_value ();\n", mrule -> nonterminal);
	};

static void code_include_metarules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_meta_rules; ix++)
	     code_include_metarule (all_meta_rules[ix]);
	};

static void code_interface_spec (string name)
	{ fprintf (out, "\nchar *%s_name_from_nodenr (int nodenr);\n", name);
	  fprintf (out, "void init_%s (int modnr);\n", name);
	};

static void generate_include_file (string name)
	{ open_output_file (name, "h");
	  open_conditional (name);
	  code_include_rules ();
	  code_include_metarules ();
	  code_interface_spec (name);
	  close_conditional (name);
	  close_output_file();
	};

/*
   Code skeleton file
*/
static string module_name; 
static int cnodenr;
static void code_source_header (string name)
	{ fprintf (out, "/* global includes */\n");
	  fprintf (out, "#include <stdio.h>\n\n");
	  fprintf (out, "/* libebs includes */\n");
	  fprintf (out, "#include <ebs_bst.h>\n");
	  fprintf (out, "#include <ebs_cst.h>\n");
	  fprintf (out, "#include <ebs_error.h>\n");
	  fprintf (out, "#include <ebs_memalloc.h>\n");
	  fprintf (out, "#include <ebs_textstorage.h>\n");
	  fprintf (out, "#include <ebs_value.h>\n\n");
	  fprintf (out, "/* libeag includes */\n");
	  fprintf (out, "#include <eag_ds.h>\n");
	  fprintf (out, "#include <eag_nodeinfo.h>\n\n");
	  fprintf (out, "#include <eag_buildtree.h>\n");
	  fprintf (out, "#include <eag_propagate.h>\n");
	  fprintf (out, "/* local includes */\n");
	  fprintf (out, "#include \"%s.h\"\n\n", name);
	  fprintf (out, "/* our module number */\n");
	  fprintf (out, "static int %s_modnr = 2;\n\n", name);
	  module_name = name;
	  cnodenr = 0;
	};

/*
   Code the skeletons of syntax rules
*/
static void code_calculate_inherited_values (pos_list dpy)
	{ int ix;
	  int first = 1;
	  for (ix = 0; ix < dpy -> size; ix++)
	     if (dpy -> array[ix] -> kind == inherited)
		{ fprintf (out,
		"\t%svalue v%d = calc_affix_value (ps%d, lower_side);\n",
			(first)?"{ ":"  ", ix+1, ix+1);
		  first = 0;
		};
	};

static int code_calculate_derived_values (pos_list dpy)
	{ int ix;
	  int first = 1;
	  int der = 0;
	  fprintf (out, "\t  if (/* insert code to test args */ 1)\n");
	  for (ix = 0; ix < dpy -> size; ix++)
	     if (dpy -> array[ix] -> kind != inherited)
	        { fprintf (out, "\t     %svalue v%d;\n",
			(first)?"{ ":"  ", ix+1);
		  first = 0;
		  der = 1;
	        };
	  if (derived) fprintf (out,
		"\t     /* insert code to calculate derived values */;\n");
	  else fprintf (out,
		"\t     /* insert code to use inherited values */;\n");
	  return (der);
	};

static void code_propagate_derived_values (pos_list dpy)
	{ int ix;
	  int nrpushes = 0;
	  for (ix = 0; ix < dpy -> size; ix++)
	     if (dpy -> array[ix] -> kind != inherited)
		{ fprintf (out, "\t       pushp (ps%d);\n", ix+1);
		  fprintf (out, "\t       pushv (v%d);\n", ix+1);
		  fprintf (out,
			"\t       pushq (propagate_predicate_value);\n");
		  nrpushes += 3;
		};
	  fprintf (out, "\t       callq ();\n");
	  fprintf (out, "\t       pop (%d);\n", nrpushes);
	};
 
static void code_detach_derived_values (pos_list dpy)
	{ int ix;
	  for (ix = 0; ix < dpy -> size; ix++)
	     if (dpy -> array[ix] -> kind != inherited)
		fprintf (out, "\t       rfre_value (v%d);\n", ix+1);
	  fprintf (out, "\t     };\n");
	};

static void code_detach_inherited_values (pos_list dpy)
	{ int ix;
	  for (ix = 0; ix < dpy -> size; ix++)
	     if (dpy -> array[ix] -> kind == inherited)
		fprintf (out, "\t  rfre_value (v%d);\n", ix+1);
	};

static void code_actual_predicate (rule srule, pos_list dpy)
	{ int ix;
	  int der;
	  fprintf (out, "static void act_%s (", srule -> nonterminal);
	  for (ix = 0; ix < dpy -> size; ix++)
	     fprintf (out, "%spos_node ps%d", (ix==0)?"":", ", ix+1);
	  fprintf (out, ")\n");
	  code_calculate_inherited_values (dpy);
	  der = code_calculate_derived_values (dpy);
	  if (der)
	     { code_propagate_derived_values (dpy);
	       code_detach_derived_values (dpy);
	     };
	  code_detach_inherited_values (dpy);
	  fprintf (out, "\t};\n\n");
	};

static void generate_check_inherited_affixes (char *cond,
					       pos_list dpy, char *check)
	{ int first = 1;
	  int ix;
	  fprintf (out, "\t  %s", cond);
	  for (ix = 0; ix < dpy -> size; ix++)
	     { if (dpy -> array[ix] -> kind == inherited)
		 { fprintf (out, "%s!%s (ps%d)", (first)?"":" || ",
				check, ix+1);
		   first = 0;
		 };
	     };
	  fprintf (out, ")\n\t     callq ();\n");
	};

static void code_delayed_predicate (rule srule, pos_list dpy)
	{ int ix;
	  fprintf (out, "static void delayed_%s (pos_node *ps)\n", srule -> nonterminal);
	  for (ix = 0; ix < dpy -> size; ix++)
	     fprintf (out, "\t%spos_node ps%d = ps[%d];\n",
		(ix==0)?"{ ":"  ", ix+1, ix);
	  generate_check_inherited_affixes ("if (", dpy, "crit_pos_has_value");
	  fprintf (out, "\t  else\n");
	  for (ix = 0; ix < dpy -> size; ix++)
	     fprintf (out, "\t     %sps%d -> delayed = 0;\n",
		(ix==0)?"{ ":"  ", ix+1);
	  fprintf (out, "\t       act_%s (", srule -> nonterminal);
	  for (ix = 0; ix < dpy -> size; ix++)
	     fprintf (out, "%sps%d", (ix==0)?"":", ", ix+1);
	  fprintf (out, ");\n");
	  for (ix = 0; ix < dpy -> size; ix++)
	     fprintf (out, "\t       ps%d -> delayed = 1;\n", ix+1);
	  fprintf (out, "\t     };\n");
	  fprintf (out, "\t};\n\n");
	};

static void code_proper_predicate (rule srule, pos_list dpy)
	{ int ix;
	  fprintf (out, "void pred_%s ()\n", srule -> nonterminal);
	  for (ix = 0; ix < dpy -> size; ix++)
	     fprintf (out,
		"\t%saffix_node af%d = new_affix_node (\"%s_af%d\");\n",
		(ix==0)?"{ ":"  ", ix+1, srule -> nonterminal, ix+1);
	  fprintf (out, "\t  pushq (delayed_%s);\n", srule -> nonterminal);
	  fprintf (out, "\t  pushq (make_node_delayed);\n");
	  for (ix = dpy -> size; 1 <= ix; ix--)
	     { fprintf (out, "\t  pusha (af%d);\n", ix);
	       fprintf (out, "\t  pushi (tag_single);\n");
	     };
	  fprintf (out, "\t  pushi (%d);\n", dpy -> size);
	  fprintf (out, "\t  pushi (0);\n");
	  fprintf (out, "\t  pushi (mk_nodenr (%s_modnr, %d));\n",
		module_name, cnodenr);
	  cnodenr++;
	  fprintf (out, "\t  pushq (make_predicate_node);\n");
	  fprintf (out, "\t  callq ();\n");
	  fprintf (out, "\t  pop (%d);\n", 2 * dpy -> size + 6);
	  for (ix = 0; ix < dpy -> size; ix++)
	     { fprintf (out, "\t  rfre_value (af%d -> val);\n", ix+1);
	       fprintf (out, "\t  free_affix_node (af%d);\n", ix+1);
	     };
	  fprintf (out, "\t  pushq (pred_%s);\n", srule -> nonterminal);
	  fprintf (out, "\t};\n\n");
	};

static void code_predicate (rule srule)
	{ alt a = srule -> alts -> array[0];
	  pos_list dpy = a -> display;
	  if (dpy == pos_list_nil)
	     panic ("predicate %s has 0 positions\n", srule -> nonterminal);
	  code_actual_predicate (srule, dpy);
	  code_delayed_predicate (srule, dpy);
	  code_proper_predicate (srule, dpy);
	};

static void code_semipredicate (rule srule)
	{ alt a = srule -> alts -> array[0];
	  pos_list dpy = a -> display;
	  int nrps = (dpy == pos_list_nil)?0:dpy -> size;
	  int ix; 

	  fprintf (out, "void semipred_%s ()\n", srule -> nonterminal);
	  fprintf (out, "\t{\n");
	  for (ix = 0; ix < nrps; ix++) fprintf (out,
		"\t  affix_node af%d = new_affix_node (\"%s_af%d\");\n",
		ix+1, srule -> nonterminal, ix+1);
	  fprintf (out,
		  "\t  /* insert code for actions or to calculate values */\n");
	  for (ix=nrps; 1 <= ix; ix--)
	     { fprintf (out, "\t  pusha (af%d);\n", ix);
	       fprintf (out, "\t  pushi (tag_single);\n");
	     };
	  fprintf (out, "\t  pushi (%d);\n", nrps);
	  fprintf (out, "\t  pushi (0);\n");
	  fprintf (out, "\t  pushi (mk_nodenr (%s_modnr, %d));\n",
		module_name, cnodenr);
	  cnodenr++;
	  fprintf (out, "\t  pushq (make_semipredicate_node);\n");
	  fprintf (out, "\t  callq ();\n");
	  fprintf (out, "\t  pop (%d);\n", 2 * nrps + 4);
	  for (ix=0; ix < nrps; ix++)
	     { fprintf (out, "\t  rfre_value (af%d -> val);\n", ix+1);
	       fprintf (out, "\t  free_affix_node (af%d);\n", ix+1);
	     };
	  fprintf (out, "\t  pushq (semipred_%s);\n", srule -> nonterminal);
	  fprintf (out, "\t};\n\n");
	};

static void code_rule (rule srule)
	{ if (!srule -> ext) return;
	  if (srule -> kind & rule_predicate) code_predicate (srule);
	  else code_semipredicate (srule);
	};

static void code_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     code_rule (all_rules[ix]);
	};

/*
   Code the skeletons of meta rules
*/
static void code_recognize_rule (meta_rule mrule)
	{ fprintf (out, "void rec_%s_value ()\n", mrule -> nonterminal);
	  fprintf (out, "\t{ value v = popv ();\n");
	  fprintf (out, "\t  /* insert code to recognize v */\n");
	  fprintf (out, "\t  /* end of insertion */\n");
	  fprintf (out, "\t  pushv (v);\n");
	  fprintf (out, "\t  pushq (rec_%s_value);\n",
			mrule -> nonterminal);
	  fprintf (out, "\t};\n\n");
	};

static void code_make_rule (meta_rule mrule)
	{ fprintf (out, "affix_node make_%s_affix ()\n", mrule -> nonterminal);
	  fprintf (out, "\t{ affix_node new = new_affix_node (\"predef_%s\");\n",
			mrule -> nonterminal);
	  fprintf (out, "\t  new -> mfunc = rec_%s_value;\n",
			mrule -> nonterminal);
	  fprintf (out, "\t  return (new);\n");
	  fprintf (out, "\t};\n\n");
	};

static void code_meta_rule (meta_rule mrule)
	{ code_recognize_rule (mrule);
	  code_make_rule (mrule);
	};

static void code_meta_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_meta_rules; ix++)
	     code_meta_rule (all_meta_rules[ix]);
	};

static void code_rule_nodenr (rule srule)
	{ if (!srule -> ext) return;
	  fprintf (out, "\t     %scase %d: return (\"%s_%s\");\n",
			(cnodenr)?"  ":"{ ",
			cnodenr, rule_qualifier (srule), srule -> nonterminal);
	  cnodenr++;
	};

static void code_rule_nodenrs ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     code_rule_nodenr (all_rules[ix]);
	};

static void code_source_trailer (string name)
	{ cnodenr = 0;
	  fprintf (out, "char *%s_name_from_nodenr (int nodenr)\n", name);
	  fprintf (out, "\t{ int lnr = lnodenr_from_nodenr (nodenr);\n");
	  fprintf (out, "\t  switch (lnr)\n");
	  code_rule_nodenrs ();
	  fprintf (out, "\t       default: fprintf (stderr, ");
	  fprintf (out, "\"strange nodenr %%d in %s\\n\", lnr);\n", name);
	  fprintf (out, "\t     };\n");
	  fprintf (out, "\t  exit (4);\n");
	  fprintf (out, "\t};\n\n");
	  fprintf (out, "void init_%s (int modnr)\n", name);
	  fprintf (out, "\t{ %s_modnr = modnr;\n", name);
	  fprintf (out, "\t};\n");
	};

static void generate_source_file (string name)
	{ open_output_file (name, "c");
	  code_source_header (name);
	  code_rules ();
	  code_meta_rules ();
	  code_source_trailer (name);
	  close_output_file();
	};

void generate_skeleton (string name)
	{ generate_include_file (name);
	  generate_source_file (name);
	};
