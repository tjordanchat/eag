/*
   File: common.c
   Defines some general support routines for output generation
  
   CVS ID: "$Id: common.c,v 1.6 2008/06/18 14:00:33 marcs Exp $"
*/

/* General includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_error.h>
#include <ebs_primio.h>

/* libedt includes */
#include <edt_editor.h>

/* local includes */
#include "limits.h"
#include "tree.h"
#include "lexicon.h"
#include "lattice.h"
#include "numbering.h"
#include "common.h"
#include "main.h"

/*
   Do initial output for generated parsers
*/
FILE *out;
void code_string (char *format, ...)
	{ char buf[MAXSTRLEN];
	  va_list arg_ptr;
	  va_start (arg_ptr, format);
	  vsprintf (buf, format, arg_ptr);
	  va_end (arg_ptr);
	  fprintf (out, "%s", buf);
	};

void code_line (char *format, ...)
	{ char buf[MAXSTRLEN];
	  va_list arg_ptr;
	  va_start (arg_ptr, format);
	  vsprintf (buf, format, arg_ptr);
	  va_end (arg_ptr);
	  fprintf (out, "%s\n", buf);
	};

void open_output_file (string basename, string infix, string suffix)
	{ char totalname[MAXFNAME];
	  time_t thetime;
	  string atime;
	  sprintf (totalname, "%s_%s.%s", basename, infix, suffix);
	  if (!(out = fopen (totalname, "w")))
	    panic ("can't open outputfile %s", totalname);
	  time (&thetime);
	  atime = ctime (&thetime);
	  code_line ("/*");
	  code_line ("   File: %s", totalname);
	  code_string ("   Generated on %s", atime);	/* atime has \n */
	  code_line ("*/\n");
	};

void close_output_file ()
	{ fclose (out);
	};

void generate_std_includes (string_list predefs)
	{ int ix;
	  warning ("coding preamble...");
	  code_line ("/* general includes */");
	  code_line ("#include <stdio.h>\n");
	  code_line ("/* libebs includes */");
	  code_line ("#include <ebs_error.h>");
	  code_line ("#include <ebs_bst.h>");
	  code_line ("#include <ebs_cst.h>");
	  code_line ("#include <ebs_value.h>");
	  if (lexicon_present) code_line ("#include <ebs_lexds.h>");
	  code_line ("");
	  code_line ("/* libeag includes */");
	  code_line ("#include <eag_ds.h>");
	  code_line ("#include <eag_textparsing.h>");
	  code_line ("#include <eag_buildtree.h>");
	  code_line ("#include <eag_propagate.h>");
	  code_line ("#include <eag_trace.h>");
	  code_line ("#include <eag_nodeinfo.h>");
	  code_line ("#include <eag_misc.h>");
	  code_line ("#include <eag_init.h>");
	  if (interface_to_c) code_line ("#include <eag_c_interface.h>");
	  code_line ("");
	  code_line ("/* Predefines includes */");
	  for (ix = 0; ix < predefs -> size; ix++)
	     code_line ("#include <%s.h>", predefs -> array[ix]);
	  code_line ("");
	  if (editor)
	     { code_line ("/* libedt includes */");
	       code_line ("#include <edt_templates.h>");
	       code_line ("#include <edt_cpmerge.h>");
	       code_line ("#include <edt_unparser.h>");
	       code_line ("#include <edt_editmain.h>");
	       code_line ("#include <edt_initedit.h>");
	       code_line ("#include <edt_editorparsing.h>\n");
	     };
	};

string rule_qualifier (rule srule)
	{ if (srule -> kind & rule_predicate) return ("pred");
	  if (srule -> kind & rule_semipredicate) return ("semipred");
	  return ("rule");
	};

/*
   Assign nodenrs to every reachable alternative
   Nodenr 0 is reserved for the root node
   Nodenr 1 is reserved for leaf nodes
*/
static int nodenr;
static void code_nodenr (rule srule)
	{ code_line ("\t       case %d: return (\"%s_%s\");", nodenr,
		     rule_qualifier (srule), srule -> nonterminal);
	  nodenr++;
	};

static void code_nodenr_for_alt (rule srule, alt a)
	{ code_line ("\t       case %d: return (\"%s_%s\");", a -> nodenr,
		     rule_qualifier (srule), srule -> nonterminal);
	  nodenr++;
	};

static void code_nodenrs_in_alts (rule srule, alt_list alts)
	{ int ix;
	  for (ix = 0; ix < alts -> size; ix++)
	     code_nodenr_for_alt (srule, alts -> array[ix]);
	};

static void code_nodenrs_in_rule (rule srule)
	{ if (srule -> ext) return;
	  if (!srule -> reachable) return;
	  if (layoutflag && (srule == layout_rule)) return;
	  if (srule -> kind & rule_predicate)
	    code_nodenr (srule);		/* delayed node */
	  else if (placeholderflag && (srule -> placeholder))
	    { code_nodenr (srule);		/* untyped placeholder */
	      code_nodenr (srule);		/* typed placeholder */
	    };
	  code_nodenrs_in_alts (srule, srule -> alts);
	};

void code_nodenrs (string basename)
	{ int ix;
	  nodenr = 2;
	  code_line ("string %s_name_from_nodenr (int nodenr)", basename);
	  code_line ("\t{ switch (nodenr)");
	  code_line ("\t     { case 0: return (\"root_node\");");
	  code_line ("\t       case 1: return (\"leaf_node\");");
	  for (ix = 0; ix < nr_of_rules; ix++)
	    code_nodenrs_in_rule (all_rules[ix]);
	  code_string ("\t       default: panic (\"strange node ");
	  code_line ("%%d in %s\", nodenr);", basename);
	  code_line ("\t     };");
	  code_line ("\t  return (NULL);");
	  code_line ("\t};\n");
	};

/*
   code lattices
*/
static void code_lattice (element_set set)
	{ int ix;
	  code_line ("static string lattice%d_names [] =", set -> number);
	  code_line ("\t{");
	  for (ix = 0; ix < set -> defs -> size; ix++)
	    code_line ("\t  \"%s\",", set -> defs -> array[ix] -> eltnm);
	  code_line ("\t  NULL");
	  code_line ("\t};\n");
	};

void code_lattices ()
	{ int ix;
	  if (!all_element_sets -> size) return;
	  code_line ("/* code lattice names */");
	  for (ix = 0; ix < all_element_sets -> size; ix++)
	     code_lattice (all_element_sets -> array[ix]);
	  if (lexicon_present)
	     { code_line ("static string *lattice_names [] =");
	       code_line ("\t{");
	       for (ix = 0; ix < all_element_sets -> size; ix++)
		  code_line ("\t  lattice%d_names,", ix + 1);
	       code_line ("\t  NULL");
	       code_line ("\t};\n");
	     };
	};

/*
   code lexicon
*/
void code_lexicon ()
	{ if (!lexicon_present) return;
	  code_line ("/* code lexicon */");
	  code_line ("static lexicon the_lexicon;\n");
	};

/*
   value generation
*/
static int temp_nr;
void reset_temporaries ()
	{ temp_nr = 1;
	};

static void do_indent (int indent)
	{ int ix;
	  code_string ("\t  ");
	  for (ix = 0; ix < indent; ix++) code_string (" ");
	};

int generate_temp_value (value val, int indent)
	{ int my_temp = temp_nr;
	  int ix;
	  temp_nr++;
	  do_indent (indent);
	  switch (val -> tag)
	     { case string_value:
		  { code_string ("value v%d = new_string_value (", my_temp);
		    output_string (out, val -> u.str);
		    code_line (");");
		  }; break;
	       case integer_value:
		  code_line ("value v%d = new_integer_value (%d);", my_temp, val -> u.inum);
		  break;
	       case tuple_value:
		  { value_list vl = val -> u.tuple;
		    int_list il = new_int_list ();
		    if (!vl -> size)
		       { code_string ("value v%d = ", my_temp);
			 code_line ("new_tuple_value (new_value_list (0));");
			 return (my_temp);
		       };
		    for (ix = 0; ix < vl -> size; ix++)
		       app_int_list (il,
			  generate_temp_value (vl -> array[ix], indent));
		    do_indent (indent);
		    code_string ("value v%d[] = { ", temp_nr);
		    temp_nr++;
		    for (ix = 0; ix < vl -> size; ix++)
		       code_string ("%sv%d", ((ix)?", ":""), il -> array[ix]);
		    code_line (" };");
		    do_indent (indent);
		    code_string ("value v%d = ", my_temp);
		    code_line ("new_tuple_value_from_array (%d, v%d);",
			       il -> size, temp_nr - 1);
		  }; break;
	       case small_lattice_value:
		  { code_string ("value v%d = ", my_temp);
		    code_string ("new_small_lattice_value ");
		    code_line ("(0x%08x, lattice%d_names);",
			       val -> u.slat, val -> admin_nr);
		  }; break;
	       case large_lattice_value:
		  { int_list il = val -> u.elat;
		    code_string ("int v%d[] = { ", temp_nr);
		    temp_nr++;
		    for (ix = 0; ix < il -> size; ix++)
		       code_string ("%s0x%08x",
				    ((ix)?", ":""), il -> array[ix]);
		    code_line (" };");
		    do_indent (indent);
		    code_string ("value v%d = ", my_temp);
		    code_string ("new_large_lattice_value ");
		    code_line ("(%d, v%d, lattice%d_names);",
			       il -> size, temp_nr - 1, val -> admin_nr);
		  }; break;
	       default: bad_tag (val -> tag, "generate_temp_value");
	     };
	  return (my_temp);
	};

/*
   Code lookahead sets
*/
static void generate_lookahead_condition (string str)
	{ if (!str || (strlen (str) == 0)) code_string ("0");
	  else if (strlen (str) == 1)
	     { code_string ("iptr_at ('");
	       output_char (out, *str);
	       code_string ("')");
	     }
	  else
	     { code_string ("in_lookahead (");
	       output_string (out, str);
	       code_string (")");
	     };
	};

void may_generate_lookahead_check (rule srule, string str)
	{ if (!lookahead) return;
	  code_string ("\t  if (");
	  if (lookahead_error_messages) code_string ("!");
	  generate_lookahead_condition (str);
	  if (lookahead_error_messages)
	     { code_line (") lookahead_failure (\"%s\");",
			  srule -> nonterminal);
	       code_line ("\t  else");
	     }
	  else code_line (")");
	};

void generate_terminal (string str, int *nr_pushes)
	{ code_string ("\t  pushs (");
	  output_string (out, str);
	  code_line (");");
	  code_line ("\t  pushq (parse_terminal);");
	  *nr_pushes += 2;
	};

void generate_semiterminal (member m, int *sonnr,
				   int *nrpushes, int code_build)
	{ cset set = m -> u.semi.set;
	  pos_list dpy = m -> u.semi.display;
	  if (code_build)
	     { code_line ("\t  pushi (%d);", *sonnr);
	       code_line ("\t  pushq (link_son);");
	       generate_display (dpy, nrpushes);
	       code_line ("\t  pushq (make_affix_link);");
	       code_line ("\t  pushq (make_leaf_node);");
	       *nrpushes += 4;
	     };
	  code_string ("\t  pushs (");
	  output_string (out, set -> str);
	  code_line (");");
	  code_string ("\t  pushq (parse");
	  if (set -> kind & non) code_string ("_non");
	  code_string ("_set");
	  if (set -> kind & star) code_string ("_star");
	  if (set -> kind & plus) code_string ("_plus");
	  if (set -> kind & strict) code_string ("_strict");
	  code_line (");");
	  *nrpushes += 2;
	  *sonnr -= 1;
	};

void generate_cut (int *nrpushes)
	{ code_line ("\t  puship (&cut_set);");
	  code_line ("\t  pushq (cut);");
	  *nrpushes += 2;
	};

void generate_affix_output (pos_list dpy, int *nrpushes)
	{ int ix;
	  if (dpy == pos_list_nil) return;
	  for (ix = dpy -> size - 1; 0 <= ix; ix--)
	     { pos p = dpy -> array[ix];
	       if (p -> kind != inherited)
		  { affix a = p -> ex -> u.single;
		    code_line ("\t  pusha (%s);", a -> name);
		    code_line ("\t  pushq (output_affix);");
		    *nrpushes += 2;
		  };
	     };
	};

static void generate_collect_affix_output (pos_list dpy, int *nrpushes)
	{ int ix;
	  if (dpy == pos_list_nil) return;
	  for (ix = dpy -> size - 1; 0 <= ix; ix--)
	     { pos p = dpy -> array[ix];
	       if (p -> kind != inherited)
		  { affix a = p -> ex -> u.single;
		    code_line ("\t  pusha (%s);", a -> name);
		    code_line ("\t  pushi (%d);", ix);
		    code_line ("\t  pushq (collect_output_affix);");
		    *nrpushes += 3;
		  };
	     };
	};

static void generate_assign_affix_input (pos_list dpy, int *nrpushes)
	{ int ix;
	  if (dpy == pos_list_nil) return;
	  for (ix = dpy -> size - 1; 0 <= ix; ix--)
	     { pos p = dpy -> array[ix];
	       if (p -> kind == inherited)
		  { affix a = p -> ex -> u.single;
		    code_line ("\t  pusha (%s);", a -> name);
		    code_line ("\t  pushi (%d);", ix);
		    code_line ("\t  pushq (assign_input_affix);");
		    *nrpushes += 3;
		  };
	     };
	};

static void generate_reserve_collection_space (pos_list dpy, int *nrpushes)
	{ int nr = (dpy == pos_list_nil)?0:dpy -> size;
	  code_line ("\t  pushi (%d);", nr);
	  code_line ("\t  pushq (reserve_collection_space);");
	  *nrpushes += 2;
	};

/* opsplitsen */
void generate_affix_decls (alt a)
	{ affix ptr;
	  for (ptr = a -> locals; ptr != affix_nil; ptr = ptr -> next)
	     if (ptr -> reachable)
	        switch (ptr -> tag)
		   { case tag_affix_variable:
		        { meta_rule def = ptr -> u.var.def;
		          code_string ("\t  affix_node %s = ", ptr -> name);
		          if (def != meta_rule_nil)
			     code_line ("make_%s_affix ();", def -> nonterminal);
		          else code_line ("new_affix_node (\"%s\");", ptr -> name);
		        }; break;
		     case tag_affix_terminal:
		     case tag_affix_integer:
		     case tag_affix_real:
		     case tag_affix_element:
		     case tag_affix_constant:
		        { value v = ptr -> val;
		          int temp = generate_temp_value (v, 0);
		          code_string ("\t  affix_node %s = ", ptr -> name);
		          code_line ("value_to_affix (\"%s\", v%d);", ptr -> name, temp);
		        };
		        break;
		     default: bad_tag (ptr -> tag, "generate_affix_decls");
		   };
	};

void generate_affix_undecls (alt a)
	{ affix ptr;
	  for (ptr = a -> locals; ptr != affix_nil; ptr = ptr -> next)
	     if (ptr -> reachable)
		{ code_line ("\t  rfre_value (%s -> val);", ptr -> name);
		  code_line ("\t  free_affix_node (%s);", ptr -> name);
		};
	};

static void generate_affix (affix a, int *nr_pushes)
	{ code_line ("\t  pusha (%s);", a -> name);
	  *nr_pushes += 1;
	};

static void generate_affix_list (affix_list affl, int *nr_pushes)
	{ int ix;
	  for (ix = affl -> size - 1; 0 <= ix; ix--)
	     generate_affix (affl -> array[ix], nr_pushes);
	  code_line ("\t  pushi (%d);", affl -> size);
	  *nr_pushes += 1;
	};

static void generate_expression (expr e, int *nr_pushes)
	{ switch (e -> tag)
	     { case tag_single:
		  { generate_affix (e -> u.single, nr_pushes);
		    code_line ("\t  pushi (tag_single);");
		  }; break;
	       case tag_compos:
		  { generate_affix_list (e -> u.compos, nr_pushes);
		    code_line ("\t  pushi (tag_compos);");
		  }; break;
	       case tag_concat:
		  { generate_affix_list (e -> u.concat, nr_pushes);
		    code_line ("\t  pushi (tag_concat);");
		  }; break;
	       case tag_union:
		  { generate_affix_list (e -> u.uni, nr_pushes);
		    code_line ("\t  pushi (tag_union);");
		  }; break;
	       default: bad_tag (e -> tag, "generate_expression");
	     };
	  *nr_pushes += 1;
	};

void generate_display (pos_list dpy, int *nr_pushes)
	{ int ix;
	  for (ix = dpy -> size - 1; 0 <= ix; ix--)
	     generate_expression (dpy -> array[ix] -> ex, nr_pushes);
	  code_line ("\t  pushi (%d);", dpy -> size);
	  *nr_pushes += 1;
	};

static void generate_startcall (member m, int *nr_pushes)
	{ code_line ("\t  pushi (1);");
	  code_line ("\t  pushq (link_son);");
	  *nr_pushes += 2;
	  generate_display (m -> u.call.display, nr_pushes);
	  code_line ("\t  pushq (make_affix_link);");
	  code_line ("\t  pushq (%s_%s);", rule_qualifier (m -> u.call.def),
		     m -> u.call.nonterminal);
	  *nr_pushes += 2;
	};

/*
   The following routines generate code to enter alternatives
*/
void generate_alt_header (rule srule, int altnr, alt a,
				 int code_build, int cut)
	{ code_line ("\t  {");
	  if (cut)        code_line ("\t  int cut_set = 0;");
	  if (qstackflag) code_line ("\t  cont* lqptr = qptr;");
	  if (code_build) generate_affix_decls (a);
	  if (traceflag)
	     code_line ("\t  trace_alternative (\"%s_%s\", %d);",
		rule_qualifier (srule), srule -> nonterminal, altnr + 1);
	};

static void generate_qstack_check (rule srule, int altnr)
	{ code_line ("\t  if (lqptr != qptr)");
	  code_line ("\t    panic (\"qstack inconsistency detected in rule %s, alt %d\");",
		     srule -> nonterminal, altnr);
	};

void generate_alt_trailer (rule srule, int altnr, alt a, int nrpushes, int code_build, int cut)
	{ code_line ("\t  callq ();");
	  code_line ("\t  pop (%d);", nrpushes);
	  if (code_build) generate_affix_undecls (a);
	  if (qstackflag) generate_qstack_check (srule, altnr + 1);
	  if (cut) code_line ("\t  if (cut_set) goto leave_%s;", srule -> nonterminal);
	  code_line ("\t  };");
	};

/*
   The following routines generate code to parse placeholders,
*/
void generate_placeholder_alt_header (rule srule, int untyped, char look)
	{ int nraffs = srule -> proto_display -> size;
	  int ix;
	  char fset[2];
	  fset[0] = look;
	  fset[1] = '\0';
	  may_generate_lookahead_check (srule, fset);
	  code_line ("\t {");
	  if (qstackflag) code_line ("\t  cont *lqptr = qptr;");
	  for (ix = 0; ix < nraffs; ix++)
	     code_line ( "\t  affix_node uaf%d = make_undefined_affix ();", ix);
	  if (traceflag)
	     code_line ("\t  trace_placeholder_alternative (\"%s_%s\", %d);",
			rule_qualifier (srule), srule -> nonterminal, untyped);
	};

void generate_placeholder_alt_trailer (rule srule,
					      int untyped, int nrpushes)
	{ int nraffs = srule -> proto_display -> size;
	  int ix;
	  code_line ("\t  callq ();");
	  code_line ("\t  pop (%d);", nrpushes);
	  for (ix = 0; ix < nraffs; ix++)
	     { code_line ("\t  rfre_value (uaf%d -> val);", ix);
	       code_line ("\t  free_affix_node (uaf%d);", ix);
	     };
	  if (qstackflag) generate_qstack_check (srule, (untyped)?-2:-1);
	  code_line ("\t };");
	};

void generate_buildplaceholdernode (rule srule,
					   int untyped, int *nrpushes)
	{ int nraffs = srule -> proto_display -> size;
	  int nodenr = srule -> alts -> array[0] -> nodenr - 1;
	  int ix;
	  if (untyped) nodenr--;
	  for (ix = nraffs - 1; 0 <= ix; ix--)
	     { code_line ("\t  pusha (uaf%d);", ix);
	       code_line ("\t  pushi (tag_single);");
	       *nrpushes += 2;
	     };
	  code_line ("\t  pushi (%d);", nraffs);
	  code_line ("\t  pushi (%d);", nodenr);
	  code_line ("\t  pushq (make_%styped_placeholder_node);", (untyped)?"un":"");
	  *nrpushes += 3;
	};

void generate_start_rule (string_list predefs)
	{ int nrpushes = 0;
	  int ix;
	  code_line ("void transduce ()");
	  code_line ("\t{");
	  generate_affix_decls (start_alt);
	  code_line ("\t  pushq (dummy_continuation);");
	  code_line ("\t  pushq (increment_nrparses);");
	  if (editor)
	    { code_line ("\t  pushq (copy_tree);");
	      nrpushes += 1;
	    }
	  else if (interface_to_c)
	    generate_collect_affix_output (start_rule -> u.call.display, &nrpushes);
	  else generate_affix_output (start_rule -> u.call.display, &nrpushes);
	  if (dumpflag)
	    { code_line ("\t  pushq (dump_parse_tree);");
	      nrpushes += 1;
	    };
	  code_line ("\t  pushq (endofsentence);");
	  nrpushes += 2;
	  generate_startcall (start_rule, &nrpushes);
	  generate_display (start_rule -> u.call.display, &nrpushes);
	  code_line ("\t  pushi (1);\t/* one son only */");
	  code_line ("\t  pushi (0);\t/* the rootnode */");
	  code_line ("\t  pushq (make_normal_node);");
	  nrpushes += 3;
	  if (interface_to_c)
	    { generate_assign_affix_input (start_rule -> u.call.display, &nrpushes);
	      generate_reserve_collection_space (start_rule -> u.call.display, &nrpushes);
	    };
	  if (traceflag) code_line ("\t  init_trace ();");
	  for (ix = 0; ix < predefs -> size; ix++)
	    code_line ("\t  init_%s (%d);", predefs -> array[ix], ix+1);
	  code_line ("\t  callq ();");
	  code_line ("\t  pop (%d);", nrpushes);
	  generate_affix_undecls (start_alt);
	  code_line ("\t};\n");
	};

void generate_module_interface (string basename, string_list predefs)
	{ int ix;
	  code_line ("string module_name_from_nodenr (int nodenr)");
	  code_line ("\t{ int mnr = modnr_from_nodenr (nodenr);");
	  code_line ("\t  switch (mnr)");
	  code_line ("\t     { case 0: return (\"%s\");", basename);
	  for (ix = 0; ix < predefs -> size; ix++)
	     code_line ("\t       case %d: return (\"%s\");", ix + 1, predefs -> array[ix]);
	  code_string ("\t       default: panic (\"strange module");
	  code_line (" nr %%d\", mnr);");
	  code_line ("\t     };");
	  code_line ("\t  return (NULL);");
	  code_line ("\t};\n");
	  code_line ("char *name_from_nodenr (int nodenr)");
	  code_line ("\t{ int mnr = modnr_from_nodenr (nodenr);");
	  code_line ("\t  switch (mnr)");
	  code_line ("\t     { case 0: return (%s_name_from_nodenr (nodenr));", basename);
	  for (ix = 0; ix < predefs -> size; ix++)
	     code_line ("\t       case %d: return (%s_name_from_nodenr (nodenr));", ix + 1,
			predefs -> array[ix]);
	  code_string ("\t       default: panic (\"strange module");
	  code_line (" nr %%d\", mnr);");
	  code_line ("\t     };");
	  code_line ("\t  return (NULL);");
	  code_line ("\t};\n");
	};

static void code_finish_parsing ()
{ code_line ("void finish_parsing ()");
  code_line ("{ complain_on_found_parses ();");
  if (matchflag) code_line ("  dump_matches ();");
  code_line ("  exit(0);");
  code_line ("};\n");
}

/*
   This will fail if 'basename' contains quotes (")
*/
void generate_main (string basename)
	{ if (interface_to_c) return;
	  code_finish_parsing ();
	  code_line ("int main (int argc, char **argv)");	
	  code_line ("\t{ int status;");
	  if (!editor)
	     { code_line ("\t  string fname;");
	       code_line ("\t  int usize;");
	     };
	  code_line ("\t  init_error ();");
	  if (lexicon_present)
	     { code_string ("\t  cload_lexicon (\"%s.clx\", ", basename);
	       code_line ("&the_lexicon, lattice_names);");
	     };
	  if (editor) code_line (
	"\t  status = init_editor (\"%s\", &argc, argv);", basename);
	  else code_line (
	"\t  status = init_transducer (argc, argv, 0, &usize, &fname);");
	  if (editor)
	     { code_line ("\t  enter_templates ();");
	       code_string ("\t  initial_transduce_and_unparse (status, ");
	       code_line ("\"%s%s%s\");", typed_open_symbol,
			start_rule -> u.call.nonterminal, typed_close_symbol);
	       code_line ("\t  start_editor ();");
	     }
	  else
	     { code_line ("\t  transduce ();");
	       code_line ("\t  complain_on_found_parses ();");
	       if (matchflag) code_line ("\t  dump_matches ();");
	     };
	  code_line ("\t  return (0);");
	  code_line ("\t};\n");
	};
