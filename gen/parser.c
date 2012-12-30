/*
   File: parser.c
   Parses an eag from a file and builds the rule and meta rule tree
  
   CVS ID: "$Id: parser.c,v 1.6 2008/06/21 14:13:04 marcs Exp $"
*/

/* general includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_error.h>
#include <ebs_memalloc.h>
#include <ebs_textstorage.h>

/* local includes */
#include "limits.h"
#include "tree.h"
#include "gentree.h"
#include "typing.h"
#include "lex.h"
#include "parser.h"
#include "main.h"

static int parse_errors;
static string first_nonterminal;

static string_list parsed_modules;
static int module_has_not_been_parsed (string gname)
	{ int ix;
	  for (ix = 0; ix < parsed_modules -> size; ix++)
	     if (gname == parsed_modules -> array[ix])
		return (0);
	  return (1);
	};

void init_parser ()
	{ parsed_modules = new_string_list ();
	  parse_errors = 0;
	  first_nonterminal = string_nil;
	};

static void try_announce_parse (string fname, int pred)
	{ if (pred) hint ("parsing predefines file %s...", fname);
	  else warning ("parsing %s...", fname);
	};

static void parser_error (int lin, int col, char *format, ...)
	{ char buf[MAXSTRLEN];
	  va_list arg_ptr;
	  va_start (arg_ptr, format);
	  vsprintf (buf, format, arg_ptr);
	  va_end (arg_ptr);

	  parse_errors++;
	  error ("parse error in file %s, line %d, col %d: %s",
		 current_fname, lin, col, buf);
	};

static void skiptokens (symbol *toks)
	{ int ix;
	  while (1)
	     { if (thistoken == IDENTIFIER) return;
	       for (ix = 0; toks[ix] != UNDEFINED; ix++)
		  if (thistoken == toks[ix]) return;
	       insymbol ();
	     };
	};

static int is_symbol (int sy)
	{ if (thistoken != sy) return (0);
	  insymbol ();
	  return (1);
	};

static void should_be_symbol (int sy, char *kind, symbol *s)
	{ if (is_symbol (sy)) return;
	  parser_error (line, column, "%s expected", kind);
	  skiptokens (s);
	};
 
static int is_identifier (string *val)
	{ string id = this_sval;
	  if (!is_symbol (IDENTIFIER)) return (0);
	  *val = id;
	  return (1);
	};

#define ERROR_ID "<ERROR>"
static symbol ident_rec_set[] =
	{ COMMA, COLON, SEMICOLON, POINT, PLUS, PIPE,
	  STAR, FLOW, RIGHTPARENTHESIS, EOFSYMBOL, UNDEFINED
	};
static void should_be_identifier (string *val)
	{ string id = this_sval;
	  if (is_symbol (IDENTIFIER)) *val = id;
	  else
	     { parser_error (line, column, "identifier expected");
	       *val = addto_names (ERROR_ID);
	       skiptokens (ident_rec_set);
	     };
	};

static int is_identifier_list (string_list *sl)
	{ string id;
	  string_list new;
	  if (!is_identifier (&id)) return (0);
	  new = new_string_list ();
	  app_string_list (new, id);
	  while (is_symbol (COMMA))
	     { should_be_identifier (&id);
	       app_string_list (new, id);
	     };
	  *sl = new;
	  return (1);
	};

static void should_be_identifier_list (string_list *sl)
	{ if (is_identifier_list (sl)) return;
	  parser_error (line, column, "identifier list expected");
	  *sl = new_string_list ();
	  skiptokens (ident_rec_set);
	};

/*
   Grammar headers
*/
static int is_grammar_head (int *lex)
	{ string gname;
	  *lex = is_symbol (LEXICON_SYMBOL);
	  if ((*lex) || is_symbol (GRAMMAR_SYMBOL))
	     { should_be_identifier (&gname);
	       should_be_symbol (POINT, ".", ident_rec_set);
	       app_string_list (parsed_modules, gname);
	       return (1);
	     };
	  return (0);
	};
	       
static void may_be_grammar_head (int *lex)
	{ (void) is_grammar_head (lex);
	};

/*
   Grammar interfaces
*/
static void may_be_defines_part ()
	{ string_list sl;
	  if (is_symbol (DEFINES_SYMBOL))
	     { should_be_identifier_list (&sl);
	       should_be_symbol (POINT, ".", ident_rec_set);
	     };
	};

static void may_be_uses_part ()
	{ string_list sl;
	  int ix;
	  if (is_symbol (USES_SYMBOL))
	     { should_be_identifier_list (&sl);
	       should_be_symbol (POINT, ".", ident_rec_set);
	       for (ix = 0; ix < sl -> size; ix++)
		  if (module_has_not_been_parsed (sl -> array[ix]))
		     add_grammar_to_inputs (sl -> array[ix]);
	     };
	};

static void may_be_plus_or_star (int *kind)
	{ switch (nexttoken)
	     { case IDENTIFIER:
	       case INTEGER:
	       case STRING:
	       case REAL:
	       case SET:
	       case UP: return;
	       default:
		  { if (is_symbol (STAR)) *kind |= star;
	  	    else if (is_symbol (PLUS)) *kind |= plus;
		  };
	     };
	};

static void may_be_strict (int *kind)
	{ if (is_symbol (EXCLAMATIONMARK)) *kind |= strict;
	};

static int is_set (cset *s)
	{ string str = this_sval;
	  int kind = 0;

	  if (is_symbol (SET))
	     { may_be_plus_or_star (&kind);
	       if ((kind & star) || (kind & plus)) may_be_strict (&kind);
	     }
	  else return (0);
	  *s = new_cset (kind, str);
	  return (1);
	};

static int is_proper_set (cset *s)
	{ if (is_symbol (UP))
	     { if (is_set (s)) (*s) -> kind |= non;
	       else
		  { parser_error (line, column, "set expected");
		    skiptokens (ident_rec_set);
		    *s = cset_nil;
		  };
	       return (1);
	     }
	  return (is_set (s));
	};

static int is_integer (int *ival)
	{ int inum = this_ival;
	  if (is_symbol (INTEGER)) *ival = inum;
	  else return (0);
	  return (1);
	};

static int is_real (real *rval)
	{ real rnum = this_rval;
	  if (is_symbol (REAL)) *rval = rnum;
	  else return (0);
	  return (1);
	};

static int is_string (string *val)
	{ string s = this_sval;
	  if (is_symbol (STRING)) *val = s;
	  else return (0);
	  return (1);
	};

static int is_term (affix *a)
	{ string sval;
	  real rval;
	  int ival;
	  cset set;

	  if (is_identifier (&sval))	 *a = new_affix_variable (sval);
	  else if (is_string (&sval))    *a = new_affix_terminal (sval);
	  else if (is_integer (&ival))	 *a = new_affix_integer (ival);
	  else if (is_real (&rval))	 *a = new_affix_real (rval);
	  else if (is_proper_set (&set)) *a = new_affix_semi (set);
	  else return (0);
	  return (1);
	};

static symbol term_rec_set[] =
	{ SEMICOLON, COLON, POINT, RIGHTPARENTHESIS,
	  FLOW, STAR, PLUS, PIPE, EOFSYMBOL, UNDEFINED };
static void should_be_term (affix *a)
	{ if (is_term (a)) return;
	  parser_error (line, column, "term expected");
	  skiptokens (term_rec_set);
	  *a = affix_nil;
	};

static void should_be_concatenation (expr *e, affix a)
	{ affix_list new = new_affix_list ();
	  affix nexta;
	
	  app_affix_list (new, a);
	  should_be_term (&nexta);
	  app_affix_list (new, nexta);
	  while (is_symbol (PLUS))
	     { should_be_term (&nexta);
	       app_affix_list (new, nexta);
	     };
	  *e = new_expr_concat (new);
	};

static void should_be_composite (expr *e, affix a)
	{ affix_list new = new_affix_list ();
	  affix nexta;
	
	  app_affix_list (new, a);
	  should_be_term (&nexta);
	  app_affix_list (new, nexta);
	  while (is_symbol (STAR))
	     { should_be_term (&nexta);
	       app_affix_list (new, nexta);
	     };
	  *e = new_expr_compos (new);
	};

static void should_be_union (expr *e, affix a)
	{ affix_list new = new_affix_list ();
	  affix nexta;
	
	  app_affix_list (new, a);
	  should_be_term (&nexta);
	  app_affix_list (new, nexta);
	  while (is_symbol (PIPE))
	     { should_be_term (&nexta);
	       app_affix_list (new, nexta);
	     };
	  *e = new_expr_union (new);
	};

static int is_expression (expr *e)
	{ affix a;
	  if (!is_term (&a)) return (0);
	  if (is_symbol (STAR)) should_be_composite (e,a);
	  else if (is_symbol (PLUS)) should_be_concatenation (e,a);
	  else if (!v1_compatibility && is_symbol (PIPE)) should_be_union (e,a);
	  else *e = new_expr_single (a);
	  return (1);
	};

static symbol expr_rec_set[] =
	{ COMMA, SEMICOLON, COLON, POINT, FLOW,
	  RIGHTPARENTHESIS, EOFSYMBOL, UNDEFINED };
static void should_be_expression (expr *e)
	{ if (is_expression (e)) return;
	  parser_error (line, column, "expression expected");
	  skiptokens (expr_rec_set);
	  *e = expr_nil;
	};

static int is_position (pos *p)
	{ expr e;
	  int kind;

	  if (is_symbol (FLOW))
	     { should_be_expression (&e);
	       kind = inherited;
	     }
	  else if (is_expression (&e))
	     { if (is_symbol (FLOW)) kind = derived;
	       else kind = noflow;
	     }
	  else return (0);
	  *p = new_pos (kind, e);
	  return (1);
	};

static symbol pos_rec_set[] = 
	{ COMMA, SEMICOLON, COLON, POINT, RIGHTPARENTHESIS,
	  EOFSYMBOL, UNDEFINED };
static void should_be_position (pos *p)
	{ if (is_position (p)) return;
	  parser_error (line, column, "position expected");
	  skiptokens (pos_rec_set);
	  *p = pos_nil;
	};

/*
   Mark that the absence of a display is represented
   by a pos_list of size 0
*/
static symbol display_rec_set[] =
	{ COMMA, SEMICOLON, COLON, POINT, EOFSYMBOL, UNDEFINED };
static void rest_display (pos_list pl)
	{ pos newp;
	  do
	     { should_be_position (&newp);
	       app_pos_list (pl, newp);
	     }
	  while (is_symbol (COMMA));
	  should_be_symbol (RIGHTPARENTHESIS, ")", display_rec_set);
	};

static int is_display (pos_list *pl)
	{ if (!is_symbol (LEFTPARENTHESIS)) return (0);
	  *pl = new_pos_list ();
	  rest_display (*pl);
	  return (1);
	};

static void may_be_display (pos_list *pl)
	{ if (is_display (pl)) return;
	  *pl = new_pos_list ();
	};

static int is_a_nonterminal_with_display (string *nont, pos_list *dpy)
	{ char buf[MAXSTRLEN];
	  int lin, col;

	  if ((thistoken != IDENTIFIER) && (thistoken != LEFTPARENTHESIS))
	     return (0);
	  buf[0] = '\0';
	  lin = line;
	  col = column;
	  *dpy = new_pos_list ();		/* see remark */
	  while (1)
	     { string id;
	       if (is_identifier (&id)) strcat (buf, id);
	       else if (is_symbol (LEFTPARENTHESIS)) rest_display (*dpy);
	       else break;
	     };
	  if (strlen (buf) == 0)
	     { parser_error (lin, col, "missing nonterminal");
	       *nont = addto_names (ERROR_ID);
	     }
	  else *nont = addto_names (buf);
	  return (1);
	};

static void should_be_a_nonterminal_with_display (string *nont, pos_list *dpy)
	{ if (is_a_nonterminal_with_display (nont, dpy)) return;
	  parser_error (line, column, "nonterminal expected");
	  skiptokens (ident_rec_set);
	  *nont = addto_names (ERROR_ID);
	  *dpy = new_pos_list ();		/* absent display */
	};

static int is_call (member *m)
	{ string nont;
	  pos_list ps;
	  
	  if (!is_a_nonterminal_with_display (&nont, &ps)) return (0);
	  *m = new_member_call (nont, ps);
	  return (1);
	};

static int is_terminal (member *m)
	{ string s;

	  if (!is_string (&s)) return (0);
	  *m = new_member_terminal (s);
	  return (1);
	};

static int is_semiterminal (member *m)
	{ cset s;
	  pos_list display;

	  if (!is_proper_set (&s)) return (0);
	  may_be_display (&display);
	  *m = new_member_semiterminal (s, display);
	  return (1);
	};

static int is_cut (member *m)
	{ if (!is_symbol (CUT)) return (0);
	  *m = new_member_cut ();
	  return (1);
	};

static int is_member (member *m)
	{ if (is_call (m)) return (1);
	  if (is_terminal (m)) return (1);
	  if (is_semiterminal (m)) return (1);
	  if (is_cut (m)) return (1);
	  return (0);
	};

static void should_be_member (member *m)
	{ if (is_member (m)) return;
	  parser_error (line, column, "member expected");
	  skiptokens (display_rec_set);
	  *m = member_nil;
	};

/*
   The empty alternative is represented by a member_list of size 0
*/
static void rest_members (member m, member_list *ml)
	{ member newm;
	  *ml = new_member_list ();
	  app_member_list (*ml, m);

	  while (is_symbol (COMMA))
	     { should_be_member (&newm);
	       app_member_list (*ml, newm);
	     };
	};

static void may_be_alternative (member_list *ml)
	{ member m;
	  if (!is_member (&m))
	     { *ml = new_member_list ();	/* empty alternative */
	       return;
	     };
	  rest_members (m, ml);
	};

static void create_start_rule (string nont, pos_list dpy, int lex,
				int lin, int col)
	{ if (lex)
	     parser_error (lin, col, "a lexicon may not contain a start rule");
	  else if (start_rule != member_nil)
	     parser_error (lin, col, "multiple start rule");
	  else start_rule = new_member_call (nont, dpy);
	};

static void may_be_expression (expr *e)
	{ if (is_expression (e)) return;
	  *e = expr_nil;			/* empty meta alternative */
	};

static void create_rule (string nont, alt_list alts, int lex, int lin, int col)
	{ rule r = new_rule (nont, alts, lex);
	  if (!enter_syntax_rule (r))
	     parser_error (lin, col, "multiply defined syntax rule %s", nont);
	};

static symbol empty_rec_set[] = { EOFSYMBOL, UNDEFINED };
static void check_no_display (pos_list dpy, int lin, int col)
	{ if (dpy -> size > 0)
	     parser_error (lin, col, "meta rule has a display");
	};

static void check_no_suffix (string nont, int lin, int col)
	{ char *last;
	  if (v1_compatibility) return;
	  last = nont + strlen (nont) - 1;
	  if (('0' <= (*last)) && ((*last) <= '9'))
	     parser_error (lin, col, "meta rule has an integer suffix");
	};

/*
   The following piece of code is definitely difficult to understand
   since it copes with a number of ambiguities in the syntax of eag.
*/

/*
   We arrive at this point when we have recognized the following construction:

      nont (pp) ::

   We first verify that the nonterminal has no suffix and then if the
   display was absent i.e. the construction recognized thus far is actually

      nont ::

*/
static void rest_meta_rule (string nont, pos_list dpy, int lin, int col)
	{ meta_alt_list new = new_meta_alt_list ();
	  meta_rule meta;
	  expr e;

	  check_no_suffix (nont, lin, col);
	  check_no_display (dpy, lin, col);
	  may_be_expression (&e);
	  app_meta_alt_list (new, new_meta_alt (e));
	  while (is_symbol (SEMICOLON))
	      { may_be_expression (&e);
		app_meta_alt_list (new, new_meta_alt (e));
	      };
	  should_be_symbol (POINT, ".", empty_rec_set);
	  meta = new_meta_rule (nont, new);
	  if (!enter_meta_rule (meta))
	     parser_error (lin, col, "multiply defined meta nonterminal %s",
			   nont);
	};

/*
   We arrive at this point when we have recognized the following construction:

      nont (pp): alt;

   Since the semicolon (;) is ambiguous when it is followed by nont2 (pp2),
   we decide by the subsequent symbol how to recognize it.

   If it is followed by a colon (:), it is the start of another alternative,
   in which case nont and nont2 must equal. If it is not followed by a colon,
   it is the first call of another alternative sharing the same left hand
   side of the rule. In this case the left hand side construction is
   duplicated. This is also done for the case that the semicolon is
   followed by something else than a call.
*/
static void expect_new_alternative (string nont, alt_list alts)
	{ string nont2;
	  int lin = line;
	  int col = column;
	  member_list ml;
	  pos_list dpy;
	  if (is_a_nonterminal_with_display (&nont2, &dpy))
	     { member m;

	       if (is_symbol (COLON))
		  { if (nont != nont2)
		       parser_error (lin, col,
				     "lhs mismatch between alternatives");
		    may_be_alternative (&ml);
		    app_alt_list (alts, new_alt (dpy, ml));
		    return;
		  };
	       m = new_member_call (nont2, dpy);
	       rest_members (m, &ml);
	       dpy = rdup_pos_list (alts ->
				    array [alts -> size - 1] -> display);
	       app_alt_list (alts, new_alt (dpy, ml));
	       return;
	     };
	  may_be_alternative (&ml);
	  dpy = rdup_pos_list (alts -> array [alts -> size - 1] -> display);
	  app_alt_list (alts, new_alt (dpy, ml));
	};

static symbol alt_rec_set[] = { SEMICOLON, POINT, EOFSYMBOL, UNDEFINED };
static symbol rule_rec_set[] = { POINT, EOFSYMBOL, UNDEFINED };
static void rest_syntax_or_start_rule (string nont, pos_list dpy, int pred,
				       int lex, int lin, int col);

/*
   We arrive at this point when we have recognized the following construction:

      nont (pp): alt

   When followed by a semicolon, we must expect a new alternative.
   If not, it must be followed by a point (.), ending the current alternative.

   If the point is not followed by something of the form nont2 (pp2),
   we may safely enter the current nonterminal and alternative list as a
   new syntax rule.

   If it is followed by nont2 (pp2), we must decide by the subsequent
   symbol, if this is the start of a next alternative (nont = nont2)
   or the start of another rule, meta rule or start rule. When it is
   a new alternative, we add it the current alternative list and repeat
   the decision process.
*/
static void rest_syntax_rule (string nont, int pred, alt first_alt,
			       int lex, int lin, int col)
	{ alt_list alts = new_alt_list ();
	  app_alt_list (alts, first_alt);
	  if (!pred && (first_nonterminal == string_nil))
	     first_nonterminal = nont;
	  while (1)
	     { if (is_symbol (SEMICOLON)) expect_new_alternative (nont, alts);
	       else
		  { string nont2;
		    pos_list dpy;
		    member_list ml;
		    int lin2, col2;
		    should_be_symbol (POINT, ".", empty_rec_set);
		    if ((thistoken != IDENTIFIER) &&
			(thistoken != LEFTPARENTHESIS))
		       { create_rule (nont, alts, lex, lin, col);
			 return;
		       };
		    lin2 = line;
		    col2 = column;
		    should_be_a_nonterminal_with_display (&nont2, &dpy);
		    if (is_symbol (DOUBLECOLON))
		       { create_rule (nont, alts, lex, lin, col);
			 rest_meta_rule (nont2, dpy, lin2, col2);
			 return;
		       };
		    if (!pred && is_symbol (POINT))
		       { create_rule (nont, alts, lex, lin, col);
			 create_start_rule (nont2, dpy, lex, lin2, col2);
			 return;
		       };
		    if (nont != nont2)	/* new syntax rule */
		       { create_rule (nont, alts, lex, lin, col);
			 rest_syntax_or_start_rule
					(nont2, dpy, pred, lex, lin2, col2);
			 return;
		       };
		    should_be_symbol (COLON, ":", alt_rec_set);
		    may_be_alternative (&ml);
		    app_alt_list (alts, new_alt (dpy, ml));
		  };
	     };
	};

/*
   We arrive at this point when we have recognized the following construction:

      nont (pp)

   We must now decide whether it is a start rule or syntax rule.
*/
static void rest_syntax_or_start_rule (string nont, pos_list dpy, int pred,
					int lex, int lin, int col)
	{ if (!pred && is_symbol (POINT))
	     create_start_rule (nont, dpy, lex, lin, col);
	  else
	     { member_list ml;
	       alt new;
	       should_be_symbol (COLON, ":", alt_rec_set);
	       may_be_alternative (&ml);
	       new = new_alt (rdup_pos_list (dpy), ml);
	       rest_syntax_rule (nont, pred, new, lex, lin, col);
	     };
	};

typedef struct spec_rec 
	{ char *str;
	  int kind;
	} spec_rec;

/*
   will change
*/
static spec_rec meta_types [] =
	{{ "string", string_type },
	 { "tuple", tuple_type },
	 { "int", integer_type },
	 { "real", real_type },
	 { "nonlat", non_lattice_type },
	 { "any", any_type },
	 { string_nil, 0 }};

static spec_rec meta_kinds [] =
	{{ "single", single_meta_value },
	 { "recognizer", recognizer_meta_value },
	 { string_nil, 0}};

static spec_rec meta_emptys [] =
	{{ "mayproduceempty", may_produce_empty},
	 { "neverproducesempty", never_produces_empty},
	 { string_nil, 0}};

static spec_rec rule_kinds [] =
	{{ "predicate", rule_predicate },
	 { "semipredicate", rule_semipredicate },
	 { string_nil, 0}};

static int should_be_specification (string nont, spec_rec *table)
	{ int lin = line;
	  int col = column;
	  char *spec;
	  spec_rec *ptr;

	  should_be_identifier (&spec);
	  for (ptr = table; ptr -> str != string_nil; ptr++)
	     if (strcmp (ptr -> str, spec) == 0) return (ptr -> kind);
	  parser_error (lin, col, "illegal specification of predefined %s", nont);
	  return (0);
	};

static void should_be_external_rule ()
	{ string nont;
	  pos_list pl;
	  meta_rule mrule;
	  rule srule;
	  int lin = line;
	  int col = column;
	  int type, kind, empty;

	  should_be_identifier (&nont);
	  if (is_symbol (DOUBLECOLON))
	     { type = should_be_specification (nont, meta_types);
	       should_be_symbol (COMMA, ",", rule_rec_set);
	       kind = should_be_specification (nont, meta_kinds);
	       should_be_symbol (COMMA, ",", rule_rec_set);
	       empty = should_be_specification (nont, meta_emptys);
	       should_be_symbol (POINT, ".", empty_rec_set);
	       mrule = new_external_meta_rule (nont, type, kind, empty);
	       if (!enter_meta_rule (mrule))
		  parser_error (lin, col, "multiply predefined meta rule %s",
				nont);
	     }
	  else
	     { alt anew;
	       alt_list al;

	       may_be_display (&pl);
	       anew = new_alt (pl, member_list_nil);
	       al = new_alt_list ();
	       app_alt_list (al, anew);

	       should_be_symbol (COLON, ":", rule_rec_set);
	       kind = should_be_specification (nont, rule_kinds);
	       should_be_symbol (POINT, ".", empty_rec_set);
	       srule = new_external_rule (nont, al, kind);
	       if (!enter_syntax_rule (srule))
		  parser_error (lin, col, "multiply predefined rule %s",
				nont);
	     };
	};

static int is_rule (int pred, int lex)
	{ string nont;
	  int lin = line;
	  int col = column;
	  pos_list dpy;

	  if (pred && is_symbol (DOLLAR))
	     { should_be_external_rule ();
	       return (1);
	     };
	  if (!is_a_nonterminal_with_display (&nont, &dpy)) return (0);
	  if (is_symbol (DOUBLECOLON)) rest_meta_rule (nont, dpy, lin, col);
	  else rest_syntax_or_start_rule (nont, dpy, pred, lex, lin, col);
	  return (1);
	};

static void may_be_rules (int pred, int lex)
	{ while (is_rule (pred, lex));
	};

static void may_be_grammar (int pred)
	{ int lex;
	  may_be_grammar_head (&lex);
	  may_be_defines_part ();
	  may_be_uses_part ();
	  may_be_rules (pred, lex);
	};

static int is_proper_grammar (int pred)
	{ int lex;
	  if (!is_grammar_head (&lex)) return (0);
	  may_be_defines_part ();
	  may_be_uses_part ();
	  may_be_rules (pred, lex);
	  return (1);
	};

static void should_be_eof ()
	{ if (thistoken == EOFSYMBOL) return;
	  parser_error (line, column, "end of file expected");
	};

static void check_for_single_affixes (pos_list dpy)
	{ int ix;
	  if (dpy == pos_list_nil) return;
	  for (ix = 0; ix < dpy -> size; ix++)
	     if (dpy -> array[ix] -> ex -> tag != tag_single)
		{ error ("position %d of start rule is not a single affix",
			 ix + 1);
		  parse_errors++;
		};
	};

void parse_eag (FILE *fd, string fname, int pred)
	{ init_lex_file (fd, fname);
	  try_announce_parse (fname, pred);
	  may_be_grammar (pred);
	  while (is_proper_grammar (pred));
	  should_be_eof ();
	  fclose (fd);

	  if (!pred && (start_rule != member_nil))
	     check_for_single_affixes (start_rule -> u.call.display);
	  if ((parse_errors > 0) || (lex_errors > 0))
	     panic ("%d parse error%s found while parsing %s",
		    parse_errors, (parse_errors == 1)?"":"s",
		    (pred)?"predefines":"grammar");
	};

static void create_start_rule_for_first_nonterminal ()
	{ rule def = lookup_syntax_rule (first_nonterminal);
	  pos_list pl = def -> alts -> array[0] -> display;
	  pos_list dpy;
	  int ix;

	  dpy = new_pos_list ();
	  for (ix = 0; ix < pl -> size; ix++)
	     { expr e;
	       affix a;
	       string name;
	       char buf[20];

	       sprintf (buf, "startrule_%d", ix);
	       name = addto_names (buf);
	       a = new_affix_variable (name);
	       e = new_expr_single (a);
	       app_pos_list (dpy, new_pos (pl -> array[ix] -> kind, e));
	     };
	  start_rule = new_member_call (first_nonterminal, dpy);
	};

static void create_start_alt ()
	{ member_list mems = new_member_list ();
	  app_member_list (mems, start_rule);
	  start_alt = new_alt (new_pos_list (), mems);
	};

void finish_parser ()
	{ if (start_rule == member_nil)
	     { if (first_nonterminal == string_nil)
		  panic ("no start rule could be found");
	       create_start_rule_for_first_nonterminal ();
	     };
	  create_start_alt ();
	};
