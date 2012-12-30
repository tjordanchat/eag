/*
   File: topdown.c
   Defines a topdown recursive backup parser generator
  
   CVS ID: "$Id: topdown.c,v 1.4 2008/06/18 12:29:17 marcs Exp $"
*/

/* general includes */
#include <stdio.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_error.h>

/* libedt includes */
#include <edt_editor.h>

/* local includes */
#include "limits.h"
#include "tree.h"
#include "numbering.h"
#include "main.h"
#include "common.h"
#include "codemeta.h"
#include "gentemplates.h"
#include "typing.h"
#include "topdown.h"

/*
   Forward declare all generated rules
*/
static void generate_rule_declaration (rule srule)
	{ if (!srule -> reachable) return;
	  code_line ("%s void %s_%s ();", srule -> ext?"":"static",
		     rule_qualifier (srule), srule -> nonterminal);
	};

static void generate_rule_declarations ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     generate_rule_declaration (all_rules[ix]);
	};

/*
   Generate code for parsing members
*/
static void generate_call (rule srule, member m, int *sonnr, int *nr_pushes)
	{ rule def = m -> u.call.def;
	  pos_list dpy = m -> u.call.display;
	  if (layoutflag && (def == layout_rule))
	     { code_line ("\t  pushq (rule_%s);", def -> nonterminal);
	       *nr_pushes += 1;
	       return;
	     };
	  if (srule -> kind & rule_predicate)
	     { code_line ("\t  pushi (%d);", *sonnr);
	       code_line ("\t  pusht (pnode);");
	       code_line ("\t  pushq (link_predicate_son);");
	       *nr_pushes += 3;
	     }
	  else 
	     { code_line ("\t  pushi (%d);", *sonnr);
	       code_line ("\t  pushq (link_son);");
	       *nr_pushes += 2;
	     };
	  if (dpy -> size)
	     { generate_display (dpy, nr_pushes);
	       code_line ("\t  pushq (make_affix_link);");
	       *nr_pushes += 1;
	     };
	  code_line ("\t  pushq (%s_%s);", rule_qualifier (def),
		     def -> nonterminal);
	  *nr_pushes += 1;
	  *sonnr -= 1;
	};

static void generate_member (rule srule, member m,
			      int *sonnr, int *nr_pushes, int code_build)
	{ switch (m -> tag)
	     { case tag_call:
		  generate_call (srule, m, sonnr, nr_pushes); break;
	       case tag_terminal:
		  generate_terminal (m -> u.terminal, nr_pushes); break;
	       case tag_semiterminal:
		  generate_semiterminal (m, sonnr, nr_pushes, code_build);
		  break;
	       case tag_cut:
		  generate_cut (nr_pushes);
	       default: break;
	     };
	};

static void generate_rhs (rule srule, member_list mems,
			   int nrsons, int *nr_pushes, int code_build)
	{ int ix;
	  int sonnr = nrsons;
	  for (ix = mems -> size - 1; 0 <= ix; ix--)
	     generate_member (srule, mems -> array[ix], &sonnr,
			nr_pushes, code_build);
	};

static void generate_rule_lhs (rule srule, alt a, int *nr_pushes)
	{ int nrofps = a -> display -> size;
	  if (!(srule -> kind & rule_nonpredicate) || nrofps)
	     generate_display (a -> display, nr_pushes);
	  code_line ("\t  pushi (%d);", a -> nrsons);
	  code_line ("\t  pushi (%d);", a -> nodenr);
	  if (srule -> kind & rule_nonpredicate)
	     code_line ("\t  pushq (make_%s_node);",
			(nrofps)?"normal":"simple");
	  else if (srule -> kind & rule_semipredicate)
	     code_line ("\t  pushq (make_semipredicate_node);");
	  *nr_pushes += 3;
	};

static int alt_contains_cut (member_list mems)
	{ int ix;
	  for (ix = 0; ix < mems -> size; ix++)
	     if (mems -> array[ix] -> tag == tag_cut) return (1);
	  return (0);
	};

static void generate_alt_code (rule srule, int i, alt a, int *needs_leave)
	{ int nrpushes = 0;
	  int code_build = !layoutflag || (srule != layout_rule);
	  int cut = alt_contains_cut (a -> members);
	  if (srule -> kind & rule_nonpredicate)
	     may_generate_lookahead_check (srule, a -> director);
	  generate_alt_header (srule, i, a, code_build, cut);
	  generate_rhs (srule, a -> members, a -> nrsons,
			&nrpushes, code_build);
	  if (code_build) generate_rule_lhs (srule, a, &nrpushes);
	  generate_alt_trailer (srule, i, a, nrpushes, code_build, cut);
	  if (cut) *needs_leave = 1;
	};

static void generate_alts_code (rule srule, alt_list alts, int *needs_leave)
	{ int ix;
	  for (ix = 0; ix < alts -> size; ix++)
	     generate_alt_code (srule, ix, alts -> array[ix], needs_leave);
	};

static void generate_rule_header (rule srule)
	{ code_line ("static void %s_%s ()", rule_qualifier (srule), srule -> nonterminal);
	  code_line ("\t{");
	};

static void may_generate_trace_enter (rule srule)
	{ if (traceflag)
	     code_line ("\t trace_enter (\"%s_%s\");",
			rule_qualifier (srule), srule -> nonterminal);
	};

static void generate_rule_trailer (rule srule, int needs_leave)
	{ if (needs_leave)
	     code_line ("leave_%s:", srule -> nonterminal);
	  if (traceflag)
	     code_line ("\t trace_leave (\"%s_%s\");",
			rule_qualifier (srule), srule -> nonterminal);
	  code_line ("\t  pushq (%s_%s);", rule_qualifier (srule),
		     srule -> nonterminal);
	  code_line ("\t};\n");
	};

/*
   The following routines generate code to parse placeholders,
*/
static void generate_untyped_placeholder_code (rule srule)
	{ int nrpushes = 4;
	  generate_placeholder_alt_header (srule, 1, untyped_symbol[0]);
	  generate_buildplaceholdernode (srule, 1, &nrpushes);
	  code_line ("\t  pushq (rule_layout);");
	  code_line ("\t  pushs (\"%s\");", untyped_symbol);
	  code_line ("\t  pushq (parse_terminal);");
	  code_line ("\t  pushq (rule_layout);");
	  generate_placeholder_alt_trailer (srule, 1, nrpushes);
	};

static void generate_typed_placeholder_code (rule srule)
	{ char buf[MAXSTRLEN * 3];
	  int nrpushes = 4;
	  generate_placeholder_alt_header (srule, 0, typed_open_symbol[0]);
	  generate_buildplaceholdernode (srule, 0, &nrpushes);
	  sprintf (buf, "%s%s%s", typed_open_symbol,
		   srule -> nonterminal, typed_close_symbol);
	  code_line ("\t  pushq (rule_layout);");
	  code_line ("\t  pushs (\"%s\");", buf);
	  code_line ("\t  pushq (parse_terminal);");
	  code_line ("\t  pushq (rule_layout);");
	  generate_placeholder_alt_trailer (srule, 0, nrpushes);
	};

static void generate_placeholder_code (rule srule)
	{ if (!srule -> placeholder) return;
	  generate_untyped_placeholder_code (srule);
	  generate_typed_placeholder_code (srule);
	};

static void generate_lexicon_code (rule srule)
	{ code_line ("\t  do_lexicon_rule (the_lexicon, %d);",
		     srule -> lex - 1);
	};

static void generate_rule_topdown (rule srule)
	{ int needs_leave = 0;
	  generate_rule_header (srule);
	  may_generate_trace_enter (srule);
	  if (placeholderflag) generate_placeholder_code (srule);
	  if (srule -> lex) generate_lexicon_code (srule);
	  else generate_alts_code (srule, srule -> alts, &needs_leave);
	  generate_rule_trailer (srule, needs_leave);
	};

/*
   Generate code for predicates
*/
static void generate_predicate_alt_code (rule srule, int i, alt a, int *needs_leave)
	{ int nr_pushes = 0;
	  int cut = alt_contains_cut (a -> members);
	  generate_alt_header (srule, i, a, 1, cut);
	  generate_rhs (srule, a -> members, a -> nrsons, &nr_pushes, 1);
	  generate_display (a -> display, &nr_pushes);
	  code_line ("\t  pushpp (args);");
	  code_line ("\t  pushi (%d);", a -> nrsons);
	  code_line ("\t  pushi (%d);", a -> nodenr);
	  code_line ("\t  pushq (update_predicate_node);");
	  generate_alt_trailer (srule, i, a, nr_pushes + 4, 1, cut);
	  if (cut) *needs_leave = 1;
	};

static void generate_actual_predicate_code (rule srule)
	{ int ix;
	  int needs_leave = 0;
	  code_line ("static void act_%s (pos_node *args)", srule -> nonterminal);
	  if (srule -> has_sons)
	       code_line ("\t{ tree_node pnode = args[0] -> node;");
	  else code_line ("\t{");
	  for (ix = 0; ix < srule -> alts -> size; ix++)
	     generate_predicate_alt_code (srule, ix,
				srule -> alts -> array[ix], &needs_leave);
	  if (needs_leave) code_line ("leave_%s: ;", srule -> nonterminal);
	  code_line ("\t}\n");
	};

static void generate_check_inherited_affixes (char *cond, rule srule, int nrps, char *check)
	{ int first = 1;
	  int ix;
	  code_string ("\t  %s", cond);
	  for (ix = 0; ix < nrps; ix++)
	     { pos p = srule -> proto_display -> array[ix];
	       if (p -> kind == inherited)
		  { code_string ("%s!%s (paf%d)", (first)?"":" || ", check, ix);
	            first = 0;
		  };
	     };
	  code_line (")");
	  code_line ("\t     callq();");
	};

static void generate_delayed_predicate_code (rule srule)
	{ int ix;
	  int nrps = srule -> proto_display -> size;
	  code_line ("static void delayed_%s (pos_node *args)", srule -> nonterminal);
	  code_line ("\t{");
	  for (ix = 0; ix < nrps; ix++)
	     code_line ("\t  pos_node paf%d = args[%d];", ix, ix);
	  generate_check_inherited_affixes
		("if (", srule, nrps, "crit_pos_has_value");
	  code_line ("\t  else {");
	  for (ix = 0; ix < nrps; ix++)
	     code_line ("\t     paf%d -> delayed = 0;", ix);
	  code_line ("\t     act_%s (args);", srule -> nonterminal);
	  for (ix = 0; ix < nrps; ix++)
	     code_line ("\t     paf%d -> delayed = 1;", ix);
	  code_line ("\t  };");
	  code_line ("\t}\n");
	};

static void generate_enter_predicate_code (rule srule)
	{ int ix;
	  int nrps = srule -> proto_display -> size;
	  generate_rule_header (srule);
	  for (ix = 0; ix < nrps; ix++)
	     code_line (
	     "\t  affix_node paf%d = new_affix_node (\"pred_%s_paf%d\");",
		ix, srule -> nonterminal, ix);
	  may_generate_trace_enter (srule);
	  code_line ("\t  pushq (delayed_%s);", srule -> nonterminal);
	  code_line ("\t  pushq (make_node_delayed);");
	  for (ix = nrps - 1; 0 <= ix; ix--)
	     { code_line ("\t  pusha (paf%d);", ix);
	       code_line ("\t  pushi (tag_single);");
	     };
	  code_line ("\t  pushi (%d);", nrps);
	  code_line ("\t  pushi (0);");
	  code_line ("\t  pushi (%d);",
		     srule -> alts -> array[0] -> nodenr - 1);
	  code_line ("\t  pushq (make_predicate_node);");
	  code_line ("\t  callq ();");
	  code_line ("\t  pop (%d);", 2 * nrps + 6);
	  for (ix = 0; ix < nrps; ix++)
	     { code_line ("\t  rfre_value (paf%d -> val);", ix);
	       code_line ("\t  free_affix_node (paf%d);", ix);
	     };
	  generate_rule_trailer (srule, 0);
	};

void generate_predicate_topdown (rule srule)
	{ generate_actual_predicate_code (srule);
	  generate_delayed_predicate_code (srule);
	  generate_enter_predicate_code (srule);
	};

void generate_semipredicate_topdown (rule srule)
	{ generate_rule_topdown (srule);
	};

/*
   Generate code for topdown parsers
*/
static void generate_rule (rule srule)
	{ if (srule -> ext) return;
	  if (!srule -> reachable) return;
	  if (srule -> kind & rule_predicate)
	     generate_predicate_topdown (srule);
	  else if (srule -> kind & rule_semipredicate)
	     generate_semipredicate_topdown (srule);
	  else generate_rule_topdown (srule);
	};

static void generate_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     generate_rule (all_rules[ix]);
	};

void generate_topdown_parser (string basename, string_list predefs)
	{ warning ("generating topdown parser...");
	  open_output_file (basename, "topdown", "c");
	  generate_std_includes (predefs);
	  code_lattices ();
	  code_lexicon ();
	  generate_meta_rules ();
	  generate_rule_declarations ();
	  code_nodenrs (basename);
	  if (editor) generate_enter_templates ();
	  warning ("coding syntax rules...");
	  generate_rules ();
	  warning ("coding postamble and main...");
	  generate_start_rule (predefs);
	  generate_module_interface (basename, predefs);
	  generate_main (basename);
	  close_output_file ();
	};
