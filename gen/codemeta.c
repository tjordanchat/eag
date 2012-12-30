/*
   File: codemeta.c
   Codes the meta rules
  
   CVS ID: "$Id: codemeta.c,v 1.4 2008/06/18 13:59:53 marcs Exp $"
*/

/* general includes */
#include <stdio.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_error.h>
#include <ebs_primio.h>

/* local includes */
#include "tree.h"
#include "numbering.h"
#include "common.h"
#include "main.h"
#include "typing.h"
#include "codemeta.h"

/*
   Forward declare all meta rule creator and parsing routines
*/
static void generate_meta_rule_declaration (meta_rule mrule)
	{ if (!mrule -> reachable) return;
	  if (mrule -> ext)
	     { code_line ("affix_node make_%s_affix ();", mrule -> nonterminal);
	       code_line ("void rec_%s_value ();", mrule -> nonterminal);
	       return;
	     };
          code_line ("static affix_node make_%s_affix ();", mrule -> nonterminal);
	  if (mrule -> needs_rec)
	     code_line ("static void rec_%s_value ();", mrule -> nonterminal);
	  if (mrule -> needs_recbup)
	     code_line ("static void recbup_%s_value ();", mrule -> nonterminal);
	};

static void generate_meta_rule_declarations ()
	{ int ix;
	  code_line ("/* forward declare meta rule routines */");
	  for (ix = 0; ix < nr_of_meta_rules; ix++)
	     generate_meta_rule_declaration (all_meta_rules[ix]);
	  code_line ("");
	};

static void generate_creator_rule (meta_rule mrule)
	{ if (mrule -> ext) return;
	  if (!mrule -> reachable) return;
	  reset_temporaries ();
	  code_line ("static affix_node make_%s_affix ()", mrule -> nonterminal);
	  if (mrule -> mvalue != value_nil)
	     { int valnr;
	       code_line ("\t{");
	       valnr = generate_temp_value (mrule -> mvalue, 0);
	       code_line ("\t  return (value_to_affix (\"meta_%s\", v%d));",
			  mrule -> nonterminal, valnr);
	       code_line ("\t};\n");
	       return;
	     };
	  code_line ("\t{ affix_node new = new_affix_node (\"meta_%s\");",
			  mrule -> nonterminal);
	  code_line ("\t  new -> mfunc = rec_%s_value;", mrule -> nonterminal);
	  code_line ("\t  return (new);");
	  code_line ("\t};\n");
	};

static void generate_creator_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_meta_rules; ix++)
	     generate_creator_rule (all_meta_rules[ix]);
	};

/*
   Generate recognizer routines
*/
static void generate_qstack_mcheck (meta_rule mrule, int i, int body)
	{ char *indent = (body)?"":"     ";
	  code_line ("\t   %sif (lqptr != qptr)", indent);
	  code_line ("\t   %s   panic (\
\"qstack inconsistency detected in metarule %s, alt %d, rec%s\");",
		indent, mrule -> nonterminal, i+1, (body)?"body":"");
	};

static void generate_rec_affix (affix a, char *name, int *nrpushes)
	{ code_line ("\t\tpushv (%s);", name);
	  switch (a -> tag)
	     { case tag_affix_variable:
		  { meta_rule def = a -> u.var.def;
		    code_line ("\t\tpushq (rec_%s_value);", def -> nonterminal);
		    *nrpushes += 2;
		  };
		  break;
	       case tag_affix_terminal:
		  { code_string ("\t\tpushs (");
		    output_string (out, a -> u.str);
		    code_line (");");
		    code_line ("\t\tpushq (rec_parse_terminal);");
		    *nrpushes += 3;
		  };
		  break;
	       case tag_affix_integer:
		  { code_line ("\t\tpushi (%d);", a -> u.inum);
		    code_line ("\t\tpushq (rec_parse_number);");
		    *nrpushes += 3;
		  };
		  break;
	       case tag_affix_real:
		  { code_line ("\t\tpushr (%g);", a -> u.rnum);
		    code_line ("\t\tpushq (rec_parse_real);");
		    *nrpushes += 3;
		  };
		  break;
	       case tag_affix_semi:
		  { code_string ("\t\tpushs (");
		    output_string (out, a -> u.semi -> str);
		    code_line (");");
		    code_string ("\t\tpushq (rec_parse");
		    if (a -> u.semi -> kind & non) code_string ("_non");
		    code_string ("_set");
		    if (a -> u.semi -> kind & star) code_string ("_star");
		    if (a -> u.semi -> kind & plus) code_string ("_plus");
		    if (a -> u.semi -> kind & strict) code_string ("_strict");
		    code_line (");");
		    *nrpushes += 3;
		  }; break;
	       case tag_affix_element:
	       case tag_affix_constant:
		  internal_error ("generate_rec_affix");
	       default: bad_tag (a -> tag, "generate_rec_affix");
	     };
	};

static void generate_recbup_affix (affix a, int *nrpushes)
	{ switch (a -> tag)
	     { case tag_affix_variable:
		  { meta_rule def = a -> u.var.def;
		    code_line ("\t\tpushq (recbup_%s_value);",
			       def -> nonterminal);
		    *nrpushes += 1;
		  };
		  break;
	       case tag_affix_terminal:
		  { code_string ("\t\tpushs (");
		    output_string (out, a -> u.str);
		    code_line (");");
		    code_line ("\t\tpushq (recbup_parse_terminal);");
		    *nrpushes += 2;
		  };
		  break;
	       case tag_affix_integer:
		  { code_line ("\t\tpushi (%d);\n", a -> u.inum);
		    code_line ("\t\tpushq (recbup_parse_integer);\n");
		    *nrpushes += 2;
		  };
		  break;
	       case tag_affix_real:
		  { code_line ("\t\tpushr (%d);\n", a -> u.rnum);
		    code_line ("\t\tpushq (recbup_parse_real);\n");
		    *nrpushes += 2;
		  };
		  break;
	       case tag_affix_semi:
		  { code_string ("\t\tpushs (");
		    output_string (out, a -> u.semi -> str);
		    code_line (");");
		    code_string ("\t\tpushq (recbup_parse");
		    if (a -> u.semi -> kind & non) code_string ("_non");
		    code_string ("_set");
		    if (a -> u.semi -> kind & star) code_string ("_star");
		    if (a -> u.semi -> kind & plus) code_string ("_plus");
		    if (a -> u.semi -> kind & strict) code_string ("_strict");
		    code_line (");\n");
		    *nrpushes += 2;
		  };
		  break;
	       case tag_affix_element:
	       case tag_affix_constant:
		  internal_error ("generate_recbup_affix");
	       default: bad_tag (a -> tag, "generate_recbup_affix");
	     };
	};

static void generate_rec_single (affix a)
	{ int nrpushes = 0;
	  generate_rec_affix (a, "v", &nrpushes);
	  code_line ("\t\tcallq ();");
	  code_line ("\t\tpop (%d);", nrpushes);
	};

static void generate_rec_compos (affix_list affs)
	{ char buffer[40];
	  int ix;
	  int nrpushes = 0;
	  code_line ("\t\tif (v -> u.tuple -> size == %d)", affs -> size);
	  code_line ("\t\t   {");
	  for (ix = affs -> size - 1; 0 <= ix; ix--)
	     { sprintf (buffer, "v -> u.tuple -> array[%d]", ix);
	       generate_rec_affix (affs -> array[ix], buffer, &nrpushes);
	     };
	  code_line ("\t\t     callq ();");
	  code_line ("\t\t     pop (%d);", nrpushes);
	  code_line ("\t\t   };");
	};

static void generate_rec_concat (meta_rule mrule, affix_list affs)
	{ int ix;
	  int nrpushes = 1;
	  if (mrule -> type == string_type)
	     { code_line ("\t\tmiptr = v -> u.str;");
	       code_line ("\t\tpushq (meta_endofsentence);");
	     }
	  else
	     { code_line ("\t\tmtotal = v -> u.nr;");
	       code_line ("\t\tpushq (meta_endofnumber);");
	     };
	  for (ix = affs -> size - 1; 0 <= ix; ix--)
	     generate_recbup_affix (affs -> array[ix], &nrpushes);
	  code_line ("\t\tcallq ();");
	  code_line ("\t\tpop (%d);", nrpushes);
	};

static void generate_rec_expr (meta_rule mrule, int i, expr e)
	{ if (qstackflag) code_line ("\t      { cont *lqptr = qptr;");
	  else code_line ("\t      {");
	  if (e == expr_nil)
	     { code_line ("\t\tpushv (v);");
	       code_line ("\t\tpushq (rec_empty_value);");
	       code_line ("\t\tcallq ();");
	       code_line ("\t\tpop (2);");
	     }
	  else
	     switch (e -> tag)
	        { case tag_single: generate_rec_single (e -> u.single); break;
	          case tag_compos: generate_rec_compos (e -> u.compos); break;
	          case tag_concat: generate_rec_concat (mrule, e -> u.concat);
				   break;
		  case tag_union: internal_error ("generate_rec_expr");
		  default: bad_tag (e -> tag, "generate_rec_expr");
	        };
	  if (qstackflag) generate_qstack_mcheck (mrule, i, 0);
	  code_line ("\t      };");
	};

static void generate_rec_alts (meta_rule mrule)
	{ int ix;
	  for (ix = 0; ix < mrule -> alts -> size; ix++)
	     generate_rec_expr (mrule, ix, mrule -> alts -> array[ix] -> e);
	};

static void generate_recognizer_body (meta_rule mrule)
	{ if ((mrule -> mvalue != value_nil) && (mrule -> mvalue -> tag == string_value))
	     { code_string ("\t     { if (strcmp (v -> u.str, ");
	       output_string (out, mrule -> mvalue -> u.str);
	       code_line (") == 0) callq ();");
	     }
	  else if ((mrule -> mvalue != value_nil) && (mrule -> mvalue -> tag == integer_value))
	     code_line ("\t     { if (v -> u.inum == %d) callq ();", mrule -> mvalue -> u.inum);
	  else
	     { code_line ("\t     {");
	       generate_rec_alts (mrule);
	     };
	  code_line ("\t     };");
	};

static void generate_recognizer_rule (meta_rule mrule)
	{ if (mrule -> ext) return;
	  if (!mrule -> reachable) return;
	  if (!mrule -> needs_rec) return;
	  code_line ("static void rec_%s_value ()", mrule -> nonterminal);
	  code_line ("\t{ value v = popv ();");
	  code_line ("\t  if (v -> tag == undefined_value) callq ();");
	  if ((mrule -> type & integer_type) == integer_type)
	       code_line ("\t  else if (v -> tag == integer_value)");
	  else code_line ("\t  else if (v -> tag == %s_value)", string_from_type (mrule -> type));
	  generate_recognizer_body (mrule);
	  code_line ("\t  pushv (v);");
	  code_line ("\t  pushq (rec_%s_value);", mrule -> nonterminal);
	  code_line ("\t};\n");
	};

static void generate_recognizer_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_meta_rules; ix++)
	     generate_recognizer_rule (all_meta_rules[ix]);
	};

/*
   Generate parser routines
*/
static void generate_recbup_concat (affix_list affs, int *nrpushes)
	{ int ix;
	  for (ix = affs -> size - 1; 0 <= ix; ix--)
	     generate_recbup_affix (affs -> array[ix], nrpushes);
	};

static void generate_parser_expr (expr e, int *nrpushes)
	{ if (e == expr_nil) return;
	  switch (e -> tag)
	     { case tag_single: 
		  generate_recbup_affix (e -> u.single, nrpushes);
		  break;
	       case tag_concat:
		  generate_recbup_concat (e -> u.concat, nrpushes);
		  break;
	       case tag_compos:
	       case tag_union: internal_error ("generate_parser_expr");
	       default: bad_tag (e -> tag, "generate_parser_expr");
	     };
	};

static void generate_parser_alt (meta_rule mrule, int i, expr e)
	{ int nrpushes = 0;
	  if (qstackflag) code_line ("\t { cont *lqptr = qptr;");
	  else code_line ("\t {");
	  generate_parser_expr (e, &nrpushes);
	  code_line ("\t   callq ();");
	  code_line ("\t   pop (%d);", nrpushes);
	  if (qstackflag) generate_qstack_mcheck (mrule, i, 1);
	  code_line ("\t };");
	};

static void generate_parser_alts (meta_rule mrule)
	{ int ix;
	  meta_alt_list alts = mrule -> alts;
	  for (ix = 0; ix < alts -> size; ix++)
	     generate_parser_alt (mrule, ix, alts -> array[ix] -> e);
	};

static void generate_parser_rule (meta_rule mrule)
	{ if (mrule -> ext) return;
	  if (!mrule -> reachable) return;
	  if (!mrule -> needs_recbup) return;
	  code_line ("static void recbup_%s_value ()", mrule -> nonterminal);
	  code_line ("\t{");
	  generate_parser_alts (mrule);
	  code_line ("\t  pushq (recbup_%s_value);", mrule -> nonterminal);
	  code_line ("\t};\n");
	};

static void generate_parser_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_meta_rules; ix++)
	     generate_parser_rule (all_meta_rules[ix]);
	};

void generate_meta_rules ()
	{ warning ("coding meta rules...");
	  generate_meta_rule_declarations ();
	  generate_creator_rules ();
	  generate_recognizer_rules ();
	  generate_parser_rules ();
	};
