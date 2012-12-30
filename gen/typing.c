/*
   File: typing.c
   Type checks all rules and meta rules
  
   CVS ID: "$Id: typing.c,v 1.4 2008/06/18 12:26:02 marcs Exp $"
*/

/* general includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_error.h>
#include <ebs_memalloc.h>
#include <ebs_textstorage.h>

/* local includes */
#include "limits.h"
#include "tree.h"
#include "numbering.h"
#include "typing.h"
#include "common.h"
#include "main.h"

string string_from_type (int t)
	{ if ((t & any_type) == any_type) return ("any");
	  else if (t & lattice_type) return ("lattice");
	  else if ((t & non_lattice_type) == non_lattice_type)
	     return ("nonlat");
	  else if (t & tuple_type) return ("tuple");
	  else if ((t & concat_type) == concat_type)
	     return ("lattice, num or string");
	  else if (t & string_type) return ("string");
	  else if ((t & numeric_type) == numeric_type) return ("num");
	  else if (t & real_type) return ("real");
	  else if (t & integer_type) return ("int");
	  else return ("error");
	};

static int is_wellformed (int t)
	{ if (t == any_type) return (0);
	  if ((t & lattice_type) && (t & non_lattice_type)) return (0);
	  return (1);
	};

static int typing_errors;
static void typing_error (char *format, ...)
	{ char buf[MAXSTRLEN];
	  va_list arg_ptr;
	  va_start (arg_ptr, format);
	  vsprintf (buf, format, arg_ptr);
	  va_end (arg_ptr);

	  typing_errors++;
	  error ("typing error %s", buf);
	};

static void init_typing ()
	{ typing_errors = 0;
	};

/*
   Analyze the typing of the meta rules
*/
static int change;
static void update_meta_rule_type (meta_rule mrule, int type)
	{ int ntype;
	  if (mrule -> type == error_type) return;
	  if (type == error_type)
	     { mrule -> type = error_type;
	       change = 1;
	       return;
	     };
	  ntype = mrule -> type & type;
	  if (mrule -> type == ntype) return;
	  mrule -> type = ntype;
	  change = 1;
	  if (ntype) return;
	  typing_error ("in metarule %s\nmeta rule %s is inconsistent",
			mrule -> nonterminal, mrule -> nonterminal);
	};

static int analyze_affix (affix a)
	{ switch (a -> tag)
	     { case tag_affix_variable: return (a -> u.var.def -> type);
	       case tag_affix_terminal: return (string_type);
	       case tag_affix_integer:	return (integer_type);
	       case tag_affix_real:	return (real_type);
	       case tag_affix_semi:	return (string_type);
	       case tag_affix_element:	return (lattice_type);
	       default: bad_tag (a -> tag, "analyze_affix");
	     };
	  return (0);
	}; 

static int analyze_composition (meta_rule mrule, int altnr, affix_list affs)
	{ int ix;
	  for (ix = 0; ix < affs -> size; ix++)
	     (void) analyze_affix (affs -> array[ix]);
	  return (tuple_type);
	};

#define meta_concat_error \
"in meta rule %s, alt %d:\noperands of '+' have illegal type"
static int balance_concat_types (meta_rule mrule, int altnr,
				  int type, int ptype)
	{ if (ptype == error_type) return (error_type);
	  if (type == error_type) return (error_type);
	  if (type & ptype) return (type & ptype);
	  typing_error (meta_concat_error, mrule -> nonterminal, altnr);
	  return (error_type);
	};

static int analyze_concatenation (meta_rule mrule, int altnr, affix_list affs)
	{ int ix;
	  int ptype = concat_type;
	  for (ix = 0; ix < affs -> size; ix++)
	     { int type = analyze_affix (affs -> array[ix]);
	       ptype = balance_concat_types (mrule, altnr, type, ptype);
	     };
	  return (ptype);
	};

#define meta_union_error \
"in meta rule %s, alt %d:\noperand of '|' has a non lattice type"
static int analyze_union (meta_rule mrule, int altnr, affix_list affs)
	{ int ix;
	  for (ix = 0; ix < affs -> size; ix++)
	     { int type = analyze_affix (affs -> array[ix]);
	       if (type == error_type) continue;
	       if (!(type & lattice_type))
		  typing_error (meta_union_error, mrule -> nonterminal, altnr);
	     };
	  return (lattice_type);
	};

static int analyze_alt (meta_rule mrule, int altnr, expr e)
	{ if (e == expr_nil) return (string_type);
	  switch (e -> tag)
	     { case tag_single: return (analyze_affix (e -> u.single));
	       case tag_compos:
		  return (analyze_composition (mrule, altnr, e -> u.compos));
	       case tag_concat:
		  return (analyze_concatenation (mrule, altnr, e -> u.concat));
	       case tag_union:
		  return (analyze_union (mrule, altnr, e -> u.uni));
	       default: bad_tag (e -> tag, "analyze_alt");
	     };
	  return (unknown);
	};

#define meta_alt_error \
"in metarule %s:\ntype mismatch between alternatives"
static int balance_alt_types (meta_rule mrule, int type, int ptype)
	{ if (ptype == error_type) return (error_type);
	  if (type == error_type) return (error_type);
	  if (type & ptype) return (type & ptype);
	  typing_error (meta_alt_error, mrule -> nonterminal);
	  return (error_type);
	};

static int analyze_alts (meta_rule mrule, meta_alt_list alts)
	{ int ptype = any_type;
	  int ix;
	  for (ix = 0; ix < alts -> size; ix++)
	     { int type = analyze_alt (mrule, ix + 1, alts -> array[ix] -> e);
	       ptype = balance_alt_types (mrule, type, ptype);
	     };
	  return (ptype);
	};

static void analyze_meta_rule (meta_rule mrule)
	{ int type;
	  if (mrule -> ext) return;
	  if (mrule -> type == error_type) return;
	  type = analyze_alts (mrule, mrule -> alts);
	  update_meta_rule_type (mrule, type);
	};

static void analyze_meta_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_meta_rules; ix++)
	     analyze_meta_rule (all_meta_rules[ix]);
	};

static void finish_meta_rule_analysis (meta_rule mrule)
	{ if (!is_wellformed (mrule -> type))
	     typing_error ("in meta rule %s: could not determine type",
			   mrule -> nonterminal);
	};

static void finish_meta_rules_analysis ()
	{ int ix;
	  for (ix = 0; ix < nr_of_meta_rules; ix++)
	     finish_meta_rule_analysis (all_meta_rules[ix]);
	};

static void meta_typecheck ()
	{ int nr_passes = 0;
	  warning ("analyzing typing of meta rules...");
	  do
	     { change = 0;
	       nr_passes++;
	       analyze_meta_rules ();
	     }
	  while (change);
	  finish_meta_rules_analysis ();
	  hint ("needed %d pass%s for typing meta rules",
		nr_passes, (nr_passes == 1)?"":"es");
	};

/*
   Initial typecheck of rules
*/

/*
   Type balancing
*/
#define balance_error \
"in rule %s, alt %i\n: can not unify type %s with type %s"
static int balance_type (rule srule, int altnr, int type1, int type2)
	{ int rtype;
	  if (type1 == error_type) return (error_type);
	  if (type2 == error_type) return (error_type);
	  rtype = type1 & type2;
	  if (rtype) return (rtype);
	  typing_error (balance_error, srule -> nonterminal, altnr,
			string_from_type (type1), string_from_type (type2));
	  return (error_type);
	};

/*
   affix analysis
*/
#define affix_error \
"in rule %s, alt %d:\ncannot unify type %s of affix %s with type %s"
static int typecheck_affix_variable (rule srule, int altnr, affix a, int dtype)
	{ meta_rule mrule = a -> u.var.def;
	  int rtype1 = a -> type;
	  int rtype2;
	  
	  if (mrule != meta_rule_nil)
	     { rtype1 = a -> type & mrule -> type;
	       if (!rtype1 && a -> type && mrule -> type)
		  typing_error (affix_error, srule -> nonterminal, altnr,
				string_from_type (a -> type), a -> name,
				string_from_type (mrule -> type));
	     };
	  rtype2 = rtype1 & dtype;
	  if (!rtype2 && rtype1 && dtype)
	     typing_error (affix_error, srule -> nonterminal, altnr,
			   string_from_type (rtype1), a -> name,
			   string_from_type (dtype));
	  if (rtype2 != a -> type)
	     { a -> type = rtype2;
	       change = 1;
	     };
	  return (rtype2);
	};

static int typecheck_affix (rule srule, int altnr, affix a, int dtype)
	{ int affix_type = 0;
	  switch (a -> tag)
	     { case tag_affix_variable: return (typecheck_affix_variable (srule, altnr, a, dtype));
	       case tag_affix_terminal: affix_type = string_type; break;
	       case tag_affix_integer:	affix_type = integer_type; break;
	       case tag_affix_real:	affix_type = real_type; break;
	       case tag_affix_semi:	affix_type = string_type; break;
	       case tag_affix_element:	affix_type = lattice_type; break;
	       default: bad_tag (a -> tag, "typecheck_affix");
	     };
	  a -> type = affix_type;
	  return (balance_type (srule, altnr, affix_type, dtype));
	};

/*
   typechecking expressions
*/
static int typecheck_compos (rule srule, int altnr, affix_list affl, int dtype)
	{ int ix;
	  for (ix = 0; ix < affl -> size; ix++)
	     (void) typecheck_affix (srule, altnr, affl -> array[ix], any_type);
	  return (balance_type (srule, altnr, tuple_type, dtype));
	};

static int typecheck_concat (rule srule, int altnr, affix_list affl, int dtype)
	{ int ix;
	  int rtype = concat_type;
	  for (ix = 0; ix < affl -> size; ix++)
	     { rtype = typecheck_affix (srule, altnr, affl -> array[ix], rtype);
	       if (!rtype) return (error_type);
	     };
	  rtype = balance_type (srule, altnr, rtype, dtype);
	  if (!rtype) return (error_type);
	  for (ix = 0; ix < affl -> size; ix++)
	     { rtype = typecheck_affix (srule, altnr, affl -> array[ix], rtype);
	     };
	  return (rtype);
	};

static int typecheck_union (rule srule, int altnr, affix_list affl, int dtype)
	{ int ix;
	  for (ix = 0; ix < affl -> size; ix++)
	     if (!typecheck_affix (srule, altnr,
				   affl -> array[ix], lattice_type))
	        return (error_type);
	  return (balance_type (srule, altnr, lattice_type, dtype));
	};

/*
   type checking positions
*/
static int typecheck_position (rule srule, int altnr, pos p, int dtype)
	{ expr ex = p -> ex;
	  int rtype = 0;
	  int ctype = balance_type (srule, altnr, dtype, p -> type);
	  switch (ex -> tag)
	     { case tag_single:
		  rtype = typecheck_affix (srule, altnr, ex -> u.single, ctype);
		  break;
	       case tag_compos:
		  rtype = typecheck_compos(srule, altnr, ex -> u.compos, ctype);
		  break;
	       case tag_concat:
		  rtype = typecheck_concat(srule, altnr, ex -> u.concat, ctype);
		  break;
	       case tag_union:
		  rtype = typecheck_union (srule, altnr, ex -> u.uni, ctype);
		  break;
	       default: bad_tag (ex -> tag, "typecheck_position");
	     };
	  if (p -> type != rtype)
	     { p -> type = rtype;
	       change = 1;
	     };
	  return (rtype);
	};

/*
   check this code...
*/
#define protopos_error \
"in rule %s, alt %d, position %d:\ntype mismatch between alternatives"
static void update_protopos_type (rule srule, int altnr, int posnr,
				   int postype, pos protopos)
	{ int prototype = protopos -> type;
	  int ntype;

	  if (prototype == error_type) return;
	  if (postype == error_type)
	     { protopos -> type = error_type;
	       change = 1;
	       return;
	     };
	  ntype = prototype & postype;
	  if (ntype != prototype)
	     { protopos -> type = ntype;
	       change = 1;
	     };
	  if (ntype) return;
	  typing_error (protopos_error, srule, altnr, posnr);
	};

/*
   idem
*/
static void typecheck_display (rule srule, int altnr, pos_list dpy,
			        pos_list protodisplay)
	{ int ix;
	  if (dpy == pos_list_nil) return;
	  for (ix = 0; ix < dpy -> size; ix++)
	     { pos thispos = dpy -> array[ix];
	       pos protopos = protodisplay -> array[ix];
	       int postype;
	       if (protopos -> type == error_type) continue;
	       postype = typecheck_position (srule, altnr, thispos,
					     protopos -> type);
	       update_protopos_type (srule, altnr, ix + 1, postype, protopos);
	     };
	};

static void typecheck_call (rule srule, int altnr, member m)
	{ typecheck_display (srule, altnr, m -> u.call.display,
			     m -> u.call.def -> proto_display);
	};

static void typecheck_semiterminal (rule srule, int altnr, member m)
	{ (void) typecheck_position (srule, altnr,
		m -> u.semi.display -> array[0], string_type);
	};

static void typecheck_member (rule srule, int altnr, member m)
	{ switch (m -> tag)
	     { case tag_call: typecheck_call (srule, altnr, m); break;
	       case tag_terminal: break;
	       case tag_semiterminal: typecheck_semiterminal (srule, altnr, m);
	       case tag_cut: break;
	       default: bad_tag (m -> tag, "typecheck_member");
	     };
	};

static void typecheck_members (rule srule, int altnr, member_list mems)
	{ int ix;
	  if (mems == member_list_nil) return;	/* external rule */
	  for (ix = 0; ix < mems -> size; ix++)
	     typecheck_member (srule, altnr, mems -> array[ix]);
	};

static void initial_typecheck_alt (rule srule, int altnr, alt a)
	{ typecheck_display (srule, altnr, a -> display,
			     srule -> proto_display);
	};

static void typecheck_alt (rule srule, int altnr, alt a)
	{ typecheck_display (srule, altnr, a -> display,
			     srule -> proto_display);
	  typecheck_members (srule, altnr, a -> members);
	  typecheck_display (srule, altnr, a -> display,
			     srule -> proto_display);
	};

static void typecheck_alts (rule srule, alt_list alts)
	{ int ix;
	  for (ix = 0; ix < alts -> size; ix++)
	     typecheck_alt (srule, ix + 1, alts -> array[ix]);
	};

static void initial_typecheck_rule (rule srule)
	{ int ix;
	  for (ix = 0; ix < srule -> alts -> size; ix++)
	     initial_typecheck_alt (srule, ix + 1, srule -> alts -> array[ix]);
	};

static void initial_typecheck_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     initial_typecheck_rule (all_rules[ix]);
	};

/*
   do an incremental typecheck
   check op de coding van typen
*/
static int display_completely_typed (pos_list dpy)
	{ int ix;
	  for (ix = 0; ix < dpy -> size; ix++)
	     if (!is_wellformed (dpy -> array[ix] -> type)) return (0);
	  return (1);
	};

static int alt_completely_typed (alt a)
	{ affix ptr;
	  for (ptr = a -> locals; ptr != affix_nil; ptr = ptr -> next)
	     if (!is_wellformed (ptr -> type)) return (0);
	  return (1);
	};

static int alts_completely_typed (alt_list alts)
	{ int ix;
	  for (ix = 0; ix < alts -> size; ix++)
	     if (!alt_completely_typed (alts -> array[ix])) return (0);
	  return (1);
	};

static int rule_completely_typed (rule srule)
	{ if (!display_completely_typed (srule -> proto_display)) return (0);
	  if (!alts_completely_typed (srule -> alts)) return (0);
	  return (1);
	};

static void typecheck_rule (rule srule)
	{ if (srule -> ext) return;
	  typecheck_alts (srule, srule -> alts);
	};

static void typecheck_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     typecheck_rule (all_rules[ix]);
	};

/*
   dump those affixes whose type could not be resolved
*/
static void dump_untyped_affixes_in_alt (int altnr, alt a)
	{ affix ptr;
          for (ptr = a -> locals; ptr != affix_nil; ptr = ptr -> next)
             if (!is_wellformed (ptr -> type))
		wlog ("affix %s in alt %d is not completely typed",
			ptr -> name, altnr);
	};

static void dump_untyped_affixes_in_rule (rule srule)
	{ alt_list alts = srule -> alts;
	  int ix;
	  for (ix = 0; ix < alts -> size; ix++)
	     dump_untyped_affixes_in_alt (ix + 1, alts -> array[ix]);
	};

#define final_error \
"in rule %s:\nnot all affixes could be typed"
static void final_check_if_rule_typed (rule srule)
	{ if (!rule_completely_typed (srule))
	     { typing_error (final_error, srule -> nonterminal);
	       dump_untyped_affixes_in_rule (srule);
	     };
	};

static void final_typecheck ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     final_check_if_rule_typed (all_rules[ix]);
	};

#define MAXPASSES 20
void typecheck ()
	{ int nr_passes = 0;
	  warning ("type checking rules...");
	  initial_typecheck_rules ();
	  do
	     { change = 0;
	       nr_passes++;
	       typecheck_rules ();
	     }
	  while (change && (nr_passes < MAXPASSES));
	  hint ("needed %d pass%s for typecheck",
		nr_passes, (nr_passes == 1)?"":"es");
	  final_typecheck ();
	  if (typing_errors)
	     panic ("%d typing error%s w%s found", typing_errors,
		    (typing_errors==1)?"":"s", (typing_errors==1)?"as":"ere");
	};

/*
   Dump meta rule types (temp)
*/
static void dump_meta_rule_type (meta_rule mrule)
	{ hint ("%s :: %s", mrule -> nonterminal,
			    string_from_type (mrule -> type));
	};

static void dump_meta_rule_types ()
	{ int ix;
	  hint ("Meta rule types are:");
	  for (ix = 0; ix < nr_of_meta_rules; ix++)
	     dump_meta_rule_type (all_meta_rules[ix]);
	};

static void dump_protodisplay (pos_list dpy)
	{ int ix;
	  if (dpy -> size == 0) return;
	  eprint_log (" (");
	  for (ix = 0; ix < dpy -> size; ix++)
	     { pos p = dpy -> array[ix];
	       if (p -> kind == inherited) eprint_log (">");
	       eprint_log ("%s", string_from_type (p -> type));
	       if (p -> kind == derived) eprint_log (">");
	       if (ix < dpy -> size - 1) eprint_log (", ");
	     };
	  eprint_log (")");
	};

static void dump_rule_type (rule srule)
	{ eprint_log ("%s %s", rule_qualifier (srule), srule -> nonterminal);
	  dump_protodisplay (srule -> proto_display);
	  eprint_log (".\n");
	};

static void dump_rule_types ()
	{ int ix;
	  hint ("Rule types are:");
	  for (ix = 0; ix < nr_of_rules; ix++)
	     dump_rule_type (all_rules[ix]);
	};

/*
   Typing driver
*/
void infer_typing ()
	{ init_typing ();
	  meta_typecheck ();
	  typecheck ();
	};

void dump_grammar_typing ()
	{ if (!full_verbose) return;
	  dump_meta_rule_types ();
	  dump_rule_types ();
	};
