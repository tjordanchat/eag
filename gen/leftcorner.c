/*
   File: leftcorner.c
   Defines a leftcorner recursive backup parser generator
  
   CVS ID: "$Id: leftcorner.c,v 1.4 2007/10/17 08:27:04 marcs Exp $"
*/

/* general includes */
#include <stdio.h>
#include <string.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_error.h>
#include <ebs_memalloc.h>
#include <ebs_primio.h>

/* libedt includes */
#include <edt_editor.h>

/* local includes */
#include "limits.h"
#include "tree.h"
#include "main.h"
#include "common.h"
#include "typing.h"
#include "numbering.h"
#include "recursion.h"
#include "codemeta.h"
#include "warshall.h"
#include "eag_unparser.h"
#include "gentemplates.h"
#include "topdown.h"
#include "lookahead.h"
#include "leftcorner.h"

/*
   Allocate space and calculate the leftcorner relation
   Note that:
   member_leftcorner[i * nr_of_syntax_rules + j] = 1 <=>
   member i is a leftcorner of rule j <=>
   member i <LC rule j
*/
static char *member_leftcorner;
static char *member_leftcorner_closure;
static void allocate_space_for_relations ()
	{ int i,j;
	  member_leftcorner = (char *) ckcalloc
			(nr_of_members * nr_of_rules, sizeof (char));
	  member_leftcorner_closure = (char *) ckcalloc
			(nr_of_members * nr_of_rules, sizeof (char));
	  for (i=0; i < nr_of_members; i++)
	     for (j=0; j < nr_of_rules; j++)
		{ member_leftcorner [i*nr_of_rules + j] = 0;
		  member_leftcorner_closure [i*nr_of_rules + j] = 0;
		};
	};

static void compute_member_relation_in_mem (rule srule, member m)
	{ int mem_nr = m -> number;
	  int rule_nr = srule -> number;
	  switch (m -> tag)
	     { case tag_call:
		  { rule def = m -> u.call.def;
		    if (def -> kind & (rule_predicate | rule_semipredicate))
		       return;
		  }; break;
	       case tag_terminal:
		  if (strlen (m -> u.terminal) == 0) return;
		  break;
	       case tag_cut: return;
	       default: break;
	     };
	  member_leftcorner [mem_nr * nr_of_rules + rule_nr] = 1;
	};

static void compute_member_relation_in_mems (rule srule, member_list mems)
	{ int ix;
	  if (mems == member_list_nil) return;
	  for (ix = 0; ix < mems -> size; ix++)
	     { compute_member_relation_in_mem (srule, mems -> array[ix]);
	       if (mems -> array[ix] -> empty == never_produces_empty)
		  return;
	     };
	};

static void compute_member_relation (rule srule)
	{ int ix;
	  alt_list alts = srule -> alts;
	  if (srule -> ext) return;
	  if (srule -> kind & (rule_predicate | rule_semipredicate)) return;
	  for (ix = 0; ix < alts -> size; ix++)
	     compute_member_relation_in_mems (srule,
					      alts -> array[ix] -> members);
	};

static void compute_member_relations ()
	{ int i;
	  for (i = 0; i < nr_of_rules; i++)
	     compute_member_relation (all_rules[i]);
	};

static void dump_leftcorner_relation ()
	{ int i,j;
	  wlog ("dumping member left corner relation");
	  for (i = 0; i < nr_of_members; i++)
	     for (j = 0; j < nr_of_rules; j++)
		if (member_leftcorner [i*nr_of_rules + j])
		   { unparse_member (all_members[i]);
		     eprint_log ("  < %s\n", all_rules[j] -> nonterminal);
		   };
	};

/*
   refl_leftc_relation [j * nr_of_syntax_rules + i] = 1 <=>
   j is called by i in zero or more steps as a (possibly) leftmost member <=>
   rule j <LC* rule i <=>
   rule j <LC+ rule i V j = i
*/
static char *refl_leftc_relation;
static void build_reflexive_leftcorner_relation ()
	{ int i,j;
	  refl_leftc_relation = (char *) ckcalloc (nr_of_rules * nr_of_rules, sizeof (char));
	  for (i=0; i < nr_of_rules; i++)
	     for (j=0; j < nr_of_rules; j++)
		refl_leftc_relation [i * nr_of_rules + j] = leftc_relation [i * nr_of_rules + j];
	  /* add reflexivety */
	  for (i=0; i < nr_of_rules; i++)
	     refl_leftc_relation [i * nr_of_rules + i] = 1;
	};

/*
   Compute the reflexive closure of the member_leftcorner relation
   Note that:
   member_leftcorner_closure[i * nr_of_syntax_rules + j] = 1 <=>
   member i <LC* rule j
*/
static void compute_closure_of_member_relation ()
	{ int i,j,k;
	  for (i = 0; i < nr_of_members; i++)
	     for (j = 0; j < nr_of_rules; j++)
		if (member_leftcorner[i*nr_of_rules + j])
		   for (k=0; k < nr_of_rules; k++)
		      if (refl_leftc_relation[j * nr_of_rules + k])
			 member_leftcorner_closure[i*nr_of_rules + k] = 1;
	};

static void dump_leftcorner_closure_relation ()
	{ int i,j;
	  wlog ("dumping closure of left corner relation");
	  for (i = 0; i < nr_of_members; i++)
	     for (j = 0; j < nr_of_rules; j++)
		if (member_leftcorner_closure [i*nr_of_rules + j])
		   { unparse_member (all_members[i]);
		     eprint_log ("  <* %s\n",all_rules[j] -> nonterminal);
		   };
	};

static void compute_leftcorner_relations ()
	{ warning ("computing left corner relations...");
	  allocate_space_for_relations ();
	  compute_member_relations ();
	  if (full_verbose) dump_leftcorner_relation ();
	  build_reflexive_leftcorner_relation ();
	  compute_closure_of_member_relation ();
	  if (full_verbose) dump_leftcorner_closure_relation ();
	};

/*
   Code goals
*/
static int goalnr;
static void generate_goal_for_rule (rule srule)
	{ if (srule -> ext) return;
	  if (!srule -> reachable) return;
	  if (srule -> kind & (rule_predicate | rule_semipredicate)) return;
	  srule -> goal = goalnr;
	  goalnr++;
	};

static void generate_goal_for_rules ()
	{ int ix;
	  warning ("generating goals for syntax rules");
	  goalnr = 0;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     generate_goal_for_rule (all_rules[ix]);
	};

static char *goal_relation;
static void allocate_goal_relation ()
	{ int i,j;
	  goal_relation = ckcalloc (goalnr * goalnr, sizeof (char));
	  for (i = 0; i < goalnr; i++)
	     for (j = 0; j < goalnr; j++)
		goal_relation [i * goalnr + j] = 0;
	};

static void fill_goal_relation_for_rules (int i, int j)
	{ rule rule_i = all_rules[i];
	  rule rule_j = all_rules[j];
	  int goal_i = rule_i -> goal;
	  int goal_j = rule_j -> goal;

	  if (rule_i -> ext) return;
	  if (!rule_i -> reachable) return;
	  if (rule_i -> kind & (rule_predicate | rule_semipredicate)) return;
	  if (rule_j -> ext) return;
	  if (!rule_j -> reachable) return;
	  if (rule_j -> kind & (rule_predicate | rule_semipredicate)) return;
	  goal_relation [goal_i * goalnr + goal_j] = 
	     refl_leftc_relation [i * nr_of_rules + j];
	};

static void generate_goal_relation ()
	{ int i,j;
	  warning ("generating needed subset of leftcorner relation");
	  allocate_goal_relation ();
	  for (i = 0; i < nr_of_rules; i++)
	     for (j = 0; j < nr_of_rules; j++)
		fill_goal_relation_for_rules (i,j);
	};

/*
   Forward declare parser routines for every rule
*/
static void generate_declarations_for_rule (rule srule)
	{ if (!srule -> reachable) return;
	  if (srule -> kind & (rule_predicate | rule_semipredicate))
	     code_line ("%s void %s_%s ();", srule -> ext?"":"static",
		        rule_qualifier (srule), srule -> nonterminal);
	  else
	     { code_line ("static void rule_%s ();", srule -> nonterminal);
	       code_line ("static void get_%s ();", srule -> nonterminal);
	       code_line ("static void red_%s ();", srule -> nonterminal);
	       if (srule -> empty == may_produce_empty)
		  { code_line ("static void empv_%s ();",
			       srule -> nonterminal);
		    code_line ("static void emp_%s ();",
			       srule -> nonterminal);
	          };
	     };
	};

static void generate_rule_declarations ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     generate_declarations_for_rule (all_rules[ix]);
	  code_line ("");
	};

/*
   Code the goals
*/
static void code_goal_for_rule (rule srule)
	{ if (srule -> ext) return;
	  if (!srule -> reachable) return;
	  if (srule -> kind & (rule_predicate | rule_semipredicate)) return;
	  code_line ("#define goal_%s %d", srule -> nonterminal, srule -> goal);
	};

static void code_goal_for_rules ()
	{ int ix;
	  code_line ("/* code goals for non predicate syntax rules */");
	  for (ix = 0; ix < nr_of_rules; ix++)
	     code_goal_for_rule (all_rules[ix]);
	};

static void code_goal_relation ()
	{ int i,j;
	  int eltnr = 0;
	  int mincol = (goalnr < 20)?goalnr:20;
	  code_line ("#define on_spine_to_goal(x) lc_rel[x * %d + topi()]",
		     goalnr);
	  code_line ("static char lc_rel[] =");
	  code_string ("\t{ ");
	  for (i = 0; i < goalnr; i++)
	     for (j = 0; j < goalnr; j++)
		{ code_string ("%d, ", goal_relation[i * goalnr + j]);
		  eltnr++;
		  if (eltnr == mincol) 
		     { code_string ("\n\t  ");
		       eltnr = 0;
		     };
	        };
	  code_line ("%s};\n", (eltnr)?"\n\t":"");
	};

/*
   Generate parser routines for a leftcorner parser
*/
static void generate_rule_routine (rule srule)
	{ if (!srule -> reachable) return;
	  code_line ("static void rule_%s ()", srule -> nonterminal);
	  code_line ("\t{");
	  if (traceflag)
	     code_line ("\t  trace_enter (\"rule_%s\");", srule -> nonterminal);
	  may_generate_lookahead_check (srule, srule -> first);
	  code_line ("\t     { pushi (goal_%s);", srule -> nonterminal);
	  code_line ("\t       pushq (get_%s);", srule -> nonterminal);
	  code_line ("\t       callq ();");
	  code_line ("\t       pop(2);");
	  code_line ("\t     };");
	  if (srule -> empty == may_produce_empty)
	     { may_generate_lookahead_check (srule, srule -> follow);
	       code_line ("\t     { pushq (empv_%s);", srule -> nonterminal);
	       code_line ("\t       callq ();");
	       code_line ("\t       pop(1);");
	       code_line ("\t     };");
	     };
	  code_line ("\t  pushq (rule_%s);", srule -> nonterminal);
	  if (traceflag)
	     code_line ("\t  trace_leave (\"rule_%s\");", srule -> nonterminal);
	  code_line ("\t}\n");
	};

static void generate_emp_call (member m, int *sonnr,
				int *nrpushes, int vstrict)
	{ rule def = m -> u.call.def;
	  if (layoutflag && (def == layout_rule))
	     { code_line ("\t  pushq (emp_%s);", def -> nonterminal);
	       *nrpushes += 1;
	       return;
	     };
	  code_line ("\t  pushi (%d);", *sonnr);
	  code_line ("\t  pushq (link_son);");
	  generate_display (m -> u.call.display, nrpushes);
	  code_line ("\t  pushq (make_affix_link);");
	  if (def -> kind & rule_predicate)
	     code_line ("\t  pushq (pred_%s);", def -> nonterminal);
	  else if (def -> kind & rule_semipredicate)
	     code_line ("\t  pushq (semipred_%s);", def -> nonterminal);
	  else
	     code_line ("\t  pushq (emp%s_%s);", (vstrict)?"v":"",
			def -> nonterminal);
	  *nrpushes += 4;
	  *sonnr -= 1;
	};

static void generate_emp_semi (member m, int *sonnr,
			        int *nrpushes, int vstrict)
	{ cset set = m -> u.semi.set;
	  code_line ("\t  pushi (%d);", *sonnr);
	  code_line ("\t  pushq (link_son);");
	  generate_display (m -> u.semi.display, nrpushes);
	  code_line ("\t  pushq (make_affix_link);");
	  code_line ("\t  pushq (make_leaf_node);");
	  code_line ("\t  pushq (make_empty_strstore);");
	  if ((set -> kind & star) && (set -> kind & strict) && vstrict)
	     { code_string ("\t  pushs (");
	       output_string (out, set -> str);
	       code_line (");");
	       if (set -> kind & non)
		    code_line ("\t  pushq (fail_if_iptr_not_at_set);");
	       else code_line ("\t  pushq (fail_if_iptr_at_set);");
	       *nrpushes += 2;
	     };
	  *nrpushes += 5;
	  *sonnr -= 1;
	};

static void generate_emp_member (member m, int *sonnr,
				  int *nrpushes, int vstrict)
	{ switch (m -> tag)
	     { case tag_call:
		  generate_emp_call (m, sonnr, nrpushes, vstrict);
	       case tag_terminal: break;
	       case tag_semiterminal:
		  generate_emp_semi (m, sonnr, nrpushes, vstrict);
	       case tag_cut:
	       default: break;
	     };
	};

static void generate_emp_members_before (member_list mems, int i, int *sonnr,
					  int *nrpushes, int vstrict)
	{ int j;
	  if (mems == member_list_nil) return;
	  for (j = i - 1; 0 <= j; j--)
	     generate_emp_member (mems -> array[j], sonnr, nrpushes, vstrict);
	};

static void generate_emp_rhs (member_list mems, int nrsons,
			       int *nrpushes, int vstrict)
	{ int ix;
	  int sonnr = nrsons;
	  if (mems == member_list_nil) return;
	  for (ix = 0; ix < mems -> size; ix++)
	     generate_emp_member (mems -> array[ix], &sonnr, nrpushes, vstrict);
	};

static void generate_leftcorner_lhs (alt a, int *nrpushes)
	{ generate_display (a -> display, nrpushes);
	  code_line ("\t  pushi (%d);", a -> nrsons);
	  code_line ("\t  pushi (%d);", a -> nodenr);
	  code_line ("\t  pushq (make_normal_node);");
	  *nrpushes += 3;
	};

/*
   Do not use look ahead inside the emp_routines.
   The input pointer may already point further in the text
*/
static void generate_emp_alt (rule srule, alt a, int i, int vstrict)
	{ int nrpushes = 0;
	  int code_build = !layoutflag || (srule != layout_rule);
	  char *emps = (vstrict)?"empv":"emp";
	  if (a -> empty != may_produce_empty) return;
	  code_line ("\t  {");
	  if (code_build) generate_affix_decls (a);
	  if (traceflag)
	     code_line ("\t  trace_alternative (\"%s_%s\", %d);", emps,
			srule -> nonterminal, i + 1);
	  if (code_build)
	     generate_emp_rhs (a -> members, a -> nrsons, &nrpushes, vstrict);
	  if (code_build) generate_leftcorner_lhs (a, &nrpushes);
	  code_line ("\t  callq ();");
	  code_line ("\t  pop (%d);", nrpushes);
	  if (code_build) generate_affix_undecls (a);
	  code_line ("\t  };");
	};

static void generate_emp_routine (rule srule, int vstrict)
	{ int ix;
	  char *emps = (vstrict)?"empv":"emp";
	  if (!srule -> reachable) return;
	  if (srule -> empty == may_produce_empty)
	     { code_line ("static void %s_%s ()", emps, srule -> nonterminal);
	       code_line ("\t{");
	       if (traceflag)
		  code_line ("\t  trace_enter (\"%s_%s\");", emps,
			     srule -> nonterminal);
	       for (ix = 0; ix < srule -> alts -> size; ix++)
		  generate_emp_alt (srule, srule -> alts -> array[ix],
				    ix, vstrict);
	       if (traceflag)
		  code_line ("\t  trace_leave (\"%s_%s\");", emps,
			     srule -> nonterminal);
	       code_line ("\t  pushq (%s_%s);", emps, srule -> nonterminal);
	       code_line ("\t}\n");
	     };
	};

static void generate_rule_call (member m, int *sonnr, int *nrpushes)
	{ rule def = m -> u.call.def;
	  if (layoutflag && (def == layout_rule))
	     { code_line ("\t  pushq (rule_%s);", def -> nonterminal);
	       *nrpushes += 1;
	       return;
	     };
	  code_line ("\t  pushi (%d);", *sonnr);
	  code_line ("\t  pushq (link_son);");
	  generate_display (m -> u.call.display, nrpushes);
	  code_line ("\t  pushq (make_affix_link);");
	  code_line ("\t  pushq (%s_%s);", rule_qualifier (def),
		     def -> nonterminal);
	  *nrpushes += 4;
	  *sonnr -= 1;
	};

static void generate_rule_member (member m, int *sonnr,
				   int *nrpushes, int code_build)
	{ switch (m -> tag)
	     { case tag_call:
		  generate_rule_call (m, sonnr, nrpushes); break;
	       case tag_terminal:
		  generate_terminal (m -> u.terminal, nrpushes); break;
	       case tag_semiterminal:
		  generate_semiterminal (m, sonnr, nrpushes, code_build);
	       default: break;
	     };
	};

static void generate_rule_members_from (member_list mems, int i, int *sonnr,
					 int *nrpushes, int code_build)
	{ int j;
	  for (j = mems -> size - 1; i < j; j--)
	     generate_rule_member (mems -> array[j], sonnr,
				   nrpushes, code_build);
	};

/*
   The following routines generate code to parse placeholders,
*/
static void generate_untyped_placeholder_code (rule srule)
	{ int nrpushes = 5;
	  generate_placeholder_alt_header (srule, 1, untyped_symbol[0]);
	  code_line ("\t  pushq (red_%s);", srule -> nonterminal);
	  generate_buildplaceholdernode (srule, 1, &nrpushes);
	  code_line ("\t  pushq (rule_layout);");
	  code_line ("\t  pushs (\"%s\");", untyped_symbol);
	  code_line ("\t  pushq (parse_terminal);");
	  code_line ("\t  pushq (rule_layout);");
	  generate_placeholder_alt_trailer (srule, 1, nrpushes);
	};

static void generate_typed_placeholder_code (rule srule)
	{ char buf[MAXSTRLEN * 3];
	  int nrpushes = 5;
	  generate_placeholder_alt_header (srule, 0, typed_open_symbol[0]);
	  code_line ("\t  pushq (red_%s);", srule -> nonterminal);
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

static void generate_lcit_alternative_for_terminal (rule srule, int i, alt a, int j, member m)
	{ int nrpushes = 1;
	  int sonnr = a -> nrsons;
	  int code_build = !layoutflag || (srule != layout_rule);
	  if (!srule -> reachable) return;
	  may_generate_lookahead_check (srule, m -> first);
	  generate_alt_header (srule, i, a, code_build, 0);
	  code_line ("\t  pushq (red_%s);", srule -> nonterminal);
	  generate_rule_members_from (a -> members, j, &sonnr, &nrpushes, code_build);
	  if (code_build) generate_emp_members_before (a -> members, j, &sonnr, &nrpushes, 0);
	  if (code_build) generate_leftcorner_lhs (a, &nrpushes);
	  generate_terminal (m -> u.terminal, &nrpushes);
	  generate_alt_trailer (srule, i, a, nrpushes, code_build, 0);
	};

static void generate_leftcorner_semi (member m, int sonnr, int *nrpushes, int code_build)
	{ cset set = m -> u.semi.set;
	  if (code_build)
	     { code_line ("\t  pushi (%d);", sonnr);
	       code_line ("\t  pushq (link_son);");
	       generate_display (m -> u.semi.display, nrpushes);
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
	  if (set -> kind & star) code_string ("_plus"); /* leftc <> empty */
	  if (set -> kind & plus) code_string ("_plus");
	  if (set -> kind & strict) code_string ("_strict");
	  code_line (");");
	  *nrpushes += 2;
	};

static void generate_lcit_alternative_for_semiterminal (rule srule, int i, alt a, int j, member m)
	{ int nrpushes = 1;
	  int sonnr = a -> nrsons;
	  int code_build = !layoutflag || (srule != layout_rule);
	  int sonnr2;
	  if (!srule -> reachable) return;
	  may_generate_lookahead_check (srule, m -> first);
	  generate_alt_header (srule, i, a, code_build, 0);
	  code_line ("\t  pushq (red_%s);", srule -> nonterminal);
	  generate_rule_members_from (a -> members, j, &sonnr, &nrpushes, code_build);
	  sonnr2 = sonnr;
	  sonnr -= 1;
	  if (code_build) generate_emp_members_before (a -> members, j, &sonnr, &nrpushes, 0);
	  generate_leftcorner_semi (m, sonnr2, &nrpushes, code_build);
	  if (code_build) generate_leftcorner_lhs (a, &nrpushes);
	  generate_alt_trailer (srule, i, a, nrpushes, code_build, 0);
	};

static void try_generate_lcit_alternative_for_member (rule srule,
						int i, alt a, int j, member m)
	{ switch (m -> tag)
	     { case tag_call: break;
	       case tag_terminal:
		  if (strlen (m -> u.terminal) > 0)
		     generate_lcit_alternative_for_terminal (srule, i, a, j, m);
		  break;
	       case tag_semiterminal:
		  generate_lcit_alternative_for_semiterminal(srule, i, a, j, m);
	       default: break;
	     };
	};

static void try_generate_lcit_alternative (rule srule, int i, alt a)
	{ member_list mems = a -> members;
	  int j;
	  if (mems == member_list_nil) return;
	  for (j=0; j < mems -> size; j++)
	     { try_generate_lcit_alternative_for_member (srule, i, a, j, mems -> array[j]);
	       if (mems -> array[j] -> empty == never_produces_empty)
		  return;
	     };
	};

static void try_generate_lcit_lexicon_alts (rule srule)
	{ code_line ("\t  { pushq (red_%s);", srule -> nonterminal);
	  code_line ("\t    do_lexicon_get (the_lexicon, %d);", srule -> lex - 1);
	  code_line ("\t    pop (1);");
	  code_line ("\t  };");
	};

static void try_generate_lcit_alternatives (rule srule)
	{ int ix;
	  if (srule -> ext) return;
	  if (srule -> kind & (rule_predicate | rule_semipredicate)) return;
	  if (srule -> lex) try_generate_lcit_lexicon_alts (srule);
	  else
	     for (ix = 0; ix < srule -> alts -> size; ix++)
	        try_generate_lcit_alternative (srule, ix, srule -> alts -> array[ix]);
	};

static void generate_get_routine (rule srule)
	{ int ix;
	  if (!srule -> reachable) return;
	  code_line ("static void get_%s ()", srule -> nonterminal);
	  code_line ("\t{");
	  if (traceflag)
	     code_line ("\t  trace_enter (\"get_%s\");", srule -> nonterminal);
	  for (ix = 0; ix < nr_of_rules; ix++)
	     if (refl_leftc_relation [ix * nr_of_rules + srule -> number])
		{ if (placeholderflag)
		     generate_placeholder_code (all_rules[ix]);
		  try_generate_lcit_alternatives (all_rules[ix]);
		};
	  if (traceflag)
	     code_line ("\t  trace_leave (\"get_%s\");", srule -> nonterminal);
	  code_line ("\t  pushq (get_%s);", srule -> nonterminal);
	  code_line ("\t}\n");
	};

static void generate_check_goal (rule srule)
	{ code_line ("\t  if (topi () == goal_%s)", srule -> nonterminal);
	  code_line ("\t     { pop (1);");
	  code_line ("\t       callq ();");
	  code_line ("\t       pushi (goal_%s);", srule -> nonterminal);
	  code_line ("\t     };");
	};

static void try_generate_lcin_alt_for_member (rule srule, int i, alt a, int j, member m,
					      rule rule_N)
	{ int nrpushes = 1;
	  int sonnr = a -> nrsons;
	  int code_build = !layoutflag || (srule != layout_rule);
	  int sonnr2;
	  if (!srule -> reachable) return;
	  if (m -> tag != tag_call) return;
	  if (m -> u.call.def != rule_N) return;
	
	  code_line ("\t  if (on_spine_to_goal (goal_%s)){", srule -> nonterminal);
	  if (lookahead)
	     { char *fset = gather_lcin_director_set (srule, a -> members, j);
	       may_generate_lookahead_check (srule, fset);
	     };
	  generate_alt_header (srule, i, a, code_build, 0);
	  code_line ("\t  pushq (red_%s);", srule -> nonterminal);
	  generate_rule_members_from (a -> members, j, &sonnr, &nrpushes, code_build);
	  sonnr2 = sonnr;
	  sonnr -= 1;
	  generate_emp_members_before (a -> members, j, &sonnr, &nrpushes, 0);
	  if (!layoutflag || (rule_N != layout_rule))
	     { code_line ("\t  pushi (%d);", sonnr2);
	       code_line ("\t  pushq (link_son);");
	       generate_display (m -> u.call.display, &nrpushes);
	       code_line ("\t  pushq (make_affix_link);");
	       code_line ("\t  pushq (exchange_top);");
	       nrpushes += 4;
	     };
	  if (code_build) generate_leftcorner_lhs (a, &nrpushes);
	  generate_alt_trailer (srule, i, a, nrpushes, code_build, 0);
	  code_line ("\t  };");
	};

static void try_generate_lcin_alternative (rule srule, int i, alt a, rule rule_N)
	{ int j;
	  member_list mems = a -> members;
	  if (mems == member_list_nil) return;
	  for (j = 0; j < mems -> size; j++)
	     { try_generate_lcin_alt_for_member (srule, i, a, j, mems -> array[j], rule_N);
	       if (mems -> array[j] -> empty == never_produces_empty)
		  return;
	     };
	};

static void try_generate_lcin_alternatives (rule srule, rule rule_N)
	{ int ix;
	  alt_list alts = srule -> alts;
	  for (ix = 0; ix < alts -> size; ix++)
	     try_generate_lcin_alternative (srule, ix, alts -> array[ix], rule_N);
	};

static void generate_red_routine (rule srule)
	{ int ix;
	  if (!srule -> reachable) return;
	  code_line ("static void red_%s ()", srule -> nonterminal);
	  code_line ("\t{");
	  if (traceflag)
	     code_line ("\t  trace_enter (\"red_%s\");", srule -> nonterminal);
	  generate_check_goal (srule);
	  for (ix = 0; ix < nr_of_rules; ix++)
	     try_generate_lcin_alternatives (all_rules[ix], srule);
	  if (traceflag)
	     code_line ("\t  trace_leave (\"red_%s\");", srule -> nonterminal);
	  code_line ("\t  pushq (red_%s);", srule -> nonterminal);
	  code_line ("\t}\n");
	};

static void generate_parse_routines_for_rule (rule srule)
	{ if (srule -> ext) return;
	  if (!srule -> reachable) return;
	  if (srule -> kind & rule_predicate)
	     generate_predicate_topdown (srule);
	  else if (srule -> kind & rule_semipredicate)
	     generate_semipredicate_topdown (srule);
	  else 
	     { generate_rule_routine (srule);
	       generate_emp_routine (srule, 1);
	       generate_emp_routine (srule, 0);
	       generate_get_routine (srule);
	       generate_red_routine (srule);
	     };
	};

static void generate_parser_routines ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     generate_parse_routines_for_rule (all_rules[ix]);
	};

void generate_leftcorner_parser (string basename, string_list predefs)
	{ compute_leftcorner_relations ();
	  generate_goal_for_rules ();
	  generate_goal_relation ();
	  warning ("generating left corner parser...");
	  open_output_file (basename, "leftcorner", "c");
	  generate_std_includes (predefs);
	  code_lattices ();
	  code_lexicon ();
	  generate_meta_rules ();
	  generate_rule_declarations ();
	  code_nodenrs (basename);
	  if (editor) generate_enter_templates ();
	  warning ("coding syntax rules...");
	  code_goal_for_rules ();
	  code_goal_relation ();
	  generate_parser_routines ();
	  warning ("coding postamble and main...");
	  generate_start_rule (predefs);
	  generate_module_interface (basename, predefs);
	  generate_main (basename);
	  close_output_file ();
	};
