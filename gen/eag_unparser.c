/*
   File: unparser.c
   Unparses the internal datastructures.
  
   CVS ID: "$Id: eag_unparser.c,v 1.4 2008/06/18 12:24:59 marcs Exp $"
*/

/* general includes */
#include <stdio.h>

/* libeag includes */
#include <ebs_bst.h>
#include <ebs_error.h>

/* local includes */
#include <tree.h>
#include <numbering.h>
#include <main.h>
#include <typing.h>
#include <eag_unparser.h>

static void unparse_string_or_set (string str)
	{ char *ptr;
	  for (ptr = str; *ptr; ptr++)
	     { char c = *ptr;
	       if (c == '\n') eprint_log ("\\n");
	       else if (c == '\t') eprint_log ("\\t");
	       else if (c == '"')  eprint_log ("\\\"");
	       /* else if (c == '}')  eprint_log ("\\}"); */
	       else if (c == ']')  eprint_log ("\\]");
	       else if (c == '\\') eprint_log ("\\\\");
	       else eprint_log ("%c", c);
	     };
	};

static void unparse_set (cset set)
	{ if (set -> kind & non) eprint_log ("^");
	  eprint_log ("{");
	  unparse_string_or_set (set -> str);
	  eprint_log ("}");
	  if (set -> kind & star) eprint_log ("*");
	  if (set -> kind & plus) eprint_log ("+");
	  if (set -> kind & strict) eprint_log ("!");
	};

static void unparse_string (string str)
	{ eprint_log ("%c", '"');
	  unparse_string_or_set (str);
	  eprint_log ("%c", '"');
	};

static void unparse_integer (int n)
	{ eprint_log ("%d", n);
	};

static void unparse_real (real n)
	{ eprint_log ("%g", n);
	};

static void unparse_affix_variable (affix a)
	{ eprint_log ("%s", a -> u.var.nonterminal);
	  eprint_log ("%s", a -> u.var.suffix);
	};

static void unparse_affix_element (affix a)
	{ eprint_log ("%s", a -> u.elt.eltnm);
	};

static void unparse_expr (expr e);
static void unparse_affix (affix a)
	{ switch (a -> tag)
	     { case tag_affix_variable:	unparse_affix_variable (a); break;
	       case tag_affix_terminal: unparse_string (a -> u.str); break;
	       case tag_affix_integer:	unparse_integer (a -> u.inum); break;
	       case tag_affix_real:	unparse_real (a -> u.rnum); break;
	       case tag_affix_semi:	unparse_set (a -> u.semi); break;
	       case tag_affix_element:	unparse_affix_element (a); break;
	       case tag_affix_constant: unparse_expr (a -> u.cexpr); break;
	       default: bad_tag (a -> tag, "unparse_affix");
	     };
	};

static void unparse_affix_list (affix_list affl, char sep)
	{ int ix;
	  for (ix = 0; ix < affl -> size; ix++)
	     { if (ix) eprint_log (" %c ", sep);
	       unparse_affix (affl -> array[ix]);
	     };
	};

static void unparse_expr (expr e)
	{ if (e == expr_nil) return;
	  switch (e -> tag)
	     { case tag_single: unparse_affix (e -> u.single); break;
	       case tag_compos: unparse_affix_list (e -> u.compos, '*'); break;
	       case tag_concat: unparse_affix_list (e -> u.concat, '+'); break;
	       case tag_union:  unparse_affix_list (e -> u.uni, '|'); break;
	       default: bad_tag (e -> tag, "unparse_expr");
	     };
	};

static void unparse_meta_alts (meta_alt_list al)
	{ int ix;
	  if (al == meta_alt_list_nil) return;
	  for (ix = 0; ix < al -> size; ix++)
	     { eprint_log ("%s\n   ", (ix)?";":"");
	       unparse_expr (al -> array[ix] -> e);
	     };
	};

static string string_from_meta_kind (int k)
	{ switch (k)
	     { case single_meta_value: return ("single");
	       case recognizer_meta_value: return ("recognizer");
	       default: bad_tag (k, "string_from_meta_kind");
	     };
	  return ("unknown");
	};

static string string_from_rule_kind (int k)
	{ if (k & rule_nonpredicate) return ("rule");
	  if (k & rule_predicate) return ("predicate");
	  if (k & rule_semipredicate) return ("semipredicate");
	  return ("error kind");
	};

static void unparse_meta_rule (meta_rule mrule)
	{ eprint_log ("\n# mrule %d: %s :: %s, %s\n", mrule -> number,
		      mrule -> nonterminal,
		      string_from_type (mrule -> type),
		      string_from_meta_kind (mrule -> kind));
	  if (mrule -> ext) eprint_log ("$ ");
	  eprint_log ("%s :: ", mrule -> nonterminal);
	  if (mrule -> ext)
	       eprint_log ("%s, %s.\n", string_from_type (mrule -> type),
			string_from_meta_kind (mrule -> kind));
	  else
	     { unparse_meta_alts (mrule -> alts);
	       eprint_log (".\n");
	     };
	};
static void unparse_meta_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_meta_rules; ix++)
	     unparse_meta_rule (all_meta_rules[ix]);
	};

static void unparse_proto_position (pos p)
	{ if (p -> kind == inherited) eprint_log (">");
	  eprint_log ("%s", string_from_type (p -> type));
	  if (p -> kind == derived) eprint_log (">");
	};

static void unparse_proto_display (pos_list dpy)
	{ int ix;
	  if (dpy == pos_list_nil) return;
	  if (!dpy -> size) return;
	  eprint_log (" (");
	  for (ix = 0; ix < dpy -> size; ix++)
	     { if (ix) eprint_log (", ");
	       unparse_proto_position (dpy -> array[ix]);
	     };
	  eprint_log (")");
	};

static void unparse_position (pos p)
	{ if (p -> kind == inherited) eprint_log (">");
	  unparse_expr (p -> ex);
	  if (p -> kind == derived) eprint_log (">");
	};

static void unparse_display (pos_list dpy)
	{ int ix;
	  if (dpy == pos_list_nil) return;
	  if (!dpy -> size) return;
	  eprint_log (" (");
	  for (ix = 0; ix < dpy -> size; ix++)
	     { if (ix) eprint_log (", ");
	       unparse_position (dpy -> array[ix]);
	     };
	  eprint_log (")");
	};

static void unparse_call (member m)
	{ eprint_log (m -> u.call.nonterminal);
	  unparse_display (m -> u.call.display);
	};

static void unparse_semiterminal (member m)
	{ unparse_set (m -> u.semi.set);
	  unparse_display (m -> u.semi.display);
	};

void unparse_member (member m)
	{ switch (m -> tag)
	     { case tag_call: unparse_call (m); break;
	       case tag_terminal: unparse_string (m -> u.terminal); break;
	       case tag_semiterminal: unparse_semiterminal (m); break;
	       case tag_cut: eprint_log ("->");
	       default: break;
	     };
	};

static void unparse_member_list (member_list ml)
	{ int ix;
	  eprint_log ("   ");
	  if (ml == member_list_nil) return;
	  for (ix = 0; ix < ml -> size; ix++)
	     { unparse_member (ml -> array[ix]);
	       if (ix != ml -> size - 1)
		  { eprint_log (", ");
		    if (ix%3 == 2) eprint_log ("\n      ");
		  };
	     };
	};

static void unparse_alt (rule srule, alt a)
	{ eprint_log ("%s", srule -> nonterminal);
	  unparse_display (a -> display);
	  eprint_log (":\n");
	  unparse_member_list (a -> members);
	};

static void unparse_alts (rule srule, alt_list alts)
	{ int ix;
	  for (ix = 0; ix < alts -> size; ix++)
	     { if (ix) eprint_log (";\n");
	       unparse_alt (srule, alts -> array[ix]);
	     };
	  eprint_log (".\n");
	};

static void unparse_external_rule (rule srule)
	{ eprint_log ("$ %s", srule -> nonterminal);
	  unparse_display (srule -> alts -> array[0] -> display);
	  eprint_log (": ");
	  eprint_log ("%s.\n", string_from_rule_kind (srule -> kind));
	};

static void unparse_rule (rule srule)
	{ eprint_log ("\n# syntax rule %d: %s %s", srule -> number,
		      string_from_rule_kind (srule -> kind),
		      srule -> nonterminal);
	  unparse_proto_display (srule -> proto_display);
	  eprint_log ("\n");
	  if (srule -> ext) unparse_external_rule (srule);
	  else unparse_alts (srule, srule -> alts);
	};

static void unparse_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     unparse_rule (all_rules[ix]);
	};

static void unparse_start_rule ()
	{ eprint_log ("\n# start rule\n");
	  unparse_call (start_rule);
	  eprint_log (".\n");
	};

void unparse_eag ()
	{ if (!full_verbose) return;
	  warning ("unparsing eag...");
	  unparse_start_rule ();
	  unparse_rules ();
	  unparse_meta_rules ();
	};
