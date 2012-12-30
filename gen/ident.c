/*
   File: ident.c
   Identifies all rule applications and metadefined affix values
   checks the number of affix positions for every application
   identifies all local affixes, generates proper metadefinitions
   for affix sets occurring in rules.
  
   CVS ID: "$Id: ident.c,v 1.5 2008/06/18 12:28:26 marcs Exp $"
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
#include "numbering.h"
#include "ident.h"
#include "main.h"

/*
   set up error administration
*/
static int ident_errors;
static int generated_nr;
static void init_ident ()
	{ ident_errors = 0;
	  generated_nr = 0;
	};

static void ident_error (char *format, ...)
	{ char buf[MAXSTRLEN];
	  va_list arg_ptr;
	  va_start (arg_ptr, format);
	  vsprintf (buf, format, arg_ptr);
	  va_end (arg_ptr);

	  ident_errors++;
	  error ("identification error: %s", buf);
	};

/*
   Rule identification and prototype display creation
*/
#define call_error1 \
"in rule %s, alt %d, member %d: %s could not be identified"
static void identify_call (rule srule, int altnr, int memnr, member m)
	{ string nonterminal = m -> u.call.nonterminal;
	  rule callee = lookup_syntax_rule (nonterminal);
	  m -> u.call.def = callee;
	  if (callee == rule_nil)
	     ident_error (call_error1, srule -> nonterminal,
			  altnr, memnr, nonterminal);
	};

static void identify_in_member (rule srule, int altnr, int memnr, member m)
	{ switch (m -> tag)
	     { case tag_call: identify_call (srule, altnr, memnr, m); break;
	       case tag_terminal: return;
	       case tag_semiterminal: return;
	       case tag_cut: return;
	       default: bad_tag (m -> tag, "identify_in_member");
	     };
	};

static void identify_in_members (rule srule, int altnr, member_list ml)
	{ int ix;
	  if (ml == member_list_nil) return;	/* external rule */
	  for (ix = 0; ix < ml -> size; ix++)
	     identify_in_member (srule, altnr, ix + 1, ml -> array[ix]);
	};

#define lhs_error \
"rule %s, alt %d: inconsistent number of lhs affix positions"
static void deduce_nr_lhs_positions (rule srule, int altnr, pos_list dpy)
	{ int nrofps = dpy -> size;
	  if (srule -> proto_display == pos_list_nil)
	     srule -> proto_display = new_empty_pos_list (nrofps);
	  else if (srule -> proto_display -> size != nrofps)
	     ident_error (lhs_error, srule -> nonterminal, altnr);
	};

static void identify_in_alt (rule srule, int altnr, alt a)
	{ deduce_nr_lhs_positions (srule, altnr, a -> display);
	  identify_in_members (srule, altnr, a -> members);
	};

static void identify_in_rule (rule srule)
	{ alt_list alts = srule -> alts;
	  int ix;
	  for (ix = 0; ix < alts -> size; ix++)
	     identify_in_alt (srule, ix + 1, alts -> array[ix]);
	};

static void identify_in_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     identify_in_rule (all_rules[ix]);
	};

static void identify_start_rule ()
	{ rule srule = lookup_syntax_rule (start_rule -> u.call.nonterminal);
	  start_rule -> u.call.def = srule;
	  if (srule == rule_nil) ident_error ("could not identify start rule");
	};

/*
   Identification in meta rules and element creation for finite lattices
   V2 Affix variables in meta rules may not have an integer suffix 
*/
#define suffix_error \
"in meta rule %s, alt %d, variable %s has an integer suffix"
static int affix_has_suffix (meta_rule mrule, int altnr, string nont)
	{ char *last;
	  if (v1_compatibility) return (0);
	  last = nont + strlen (nont) - 1;
	  if (((*last) < '0') || ((*last) > '9')) return (0);
	  ident_error (suffix_error, mrule -> nonterminal, altnr, nont);
	  return (1);
	};

#define meta_ident_error \
"in meta rule %s, alt %d, variable %s is not meta defined"
static void identify_affix_variable (meta_rule mrule, int altnr, affix a,
				      int uni)
	{ string nont = a -> u.var.nonterminal;
	  meta_rule meta_def;
	  element_def elt_def;
	  if (affix_has_suffix (mrule, altnr, nont)) return;
	  meta_def = lookup_meta_rule (nont);
	  if (meta_def != meta_rule_nil) a -> u.var.def = meta_def;
	  else if (uni)
	     { elt_def = lookup_element (nont);
	       if (elt_def == element_def_nil)
		  { elt_def = new_element_def (nont);
		    (void) enter_element (elt_def);
		  };
	       /* now change the affix */
	       a -> tag = tag_affix_element;
	       a -> u.elt.eltnm = nont;
	       a -> u.elt.def = elt_def;
	     }
	  else ident_error (meta_ident_error,
			    mrule -> nonterminal, altnr, nont);
	};

#define construct_error "in meta rule %s, alt %d, construct may not be used in union"
static void identify_in_meta_affix (meta_rule mrule, int altnr, affix a,
				     int uni)
	{ switch (a -> tag)
	     { case tag_affix_variable:
		  identify_affix_variable (mrule, altnr, a, uni);
		  break;
	       case tag_affix_terminal:
	       case tag_affix_integer:
	       case tag_affix_real:
	       case tag_affix_semi:
		  if (uni) ident_error (construct_error, mrule -> nonterminal, altnr);
		  break;
	       default: bad_tag (a -> tag, "identify_in_meta_affix");
	     };
	};

static void identify_in_meta_affix_list (meta_rule mrule, int altnr,
					  affix_list affl, int uni)
	{ int ix;
	  for (ix = 0; ix < affl -> size; ix++)
	     identify_in_meta_affix (mrule, altnr, affl -> array[ix], uni);
	};

static void identify_in_meta_expression (meta_rule mrule, int altnr, expr e)
	{ if (e == expr_nil) return;			/* empty meta alt */
	  switch (e -> tag)
	     { case tag_single:
		  identify_in_meta_affix (mrule, altnr, e -> u.single, 0);
		  break;
	       case tag_concat:
		  identify_in_meta_affix_list (mrule, altnr, e -> u.concat, 0);
		  break;
	       case tag_compos:
		  identify_in_meta_affix_list (mrule, altnr, e -> u.compos, 0);
		  break;
	       case tag_union:
		  identify_in_meta_affix_list (mrule, altnr, e -> u.uni, 1);
		  break;
	       default: bad_tag (e -> tag, "identify_in_meta_expression");
	     };
	};

static void identify_in_meta_rule (meta_rule mrule)
	{ meta_alt_list meta_alts = mrule -> alts;
	  int ix;
	  if (meta_alts == meta_alt_list_nil) return;	/* predef meta rule */
	  for (ix = 0; ix < meta_alts -> size; ix++)
	     identify_in_meta_expression (mrule, ix + 1,
					  meta_alts -> array[ix] -> e);
	};

static void identify_in_meta_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_meta_rules; ix++)
	     identify_in_meta_rule (all_meta_rules[ix]);
	};

/*
   Identification in syntax rules
*/
static int extract_suffix (string id, string *nid, string *suffix)
	{ char *suffix_ptr = id + strlen (id);
	  char *ptr, *dptr;
	  char buf[MAXSTRLEN];
	  int done = 0;

	  while (!done)
	     { ptr = suffix_ptr - 1;
	       if (('0' <= (*ptr)) && ((*ptr) <= '9')) suffix_ptr = ptr;
	       else done = 1;
	     };
	  if (*suffix_ptr == '\0') return (0);
	  for (ptr = id, dptr = buf; ptr < suffix_ptr; ptr++,dptr++)
	     *dptr = *ptr;
	  *dptr = '\0';
	  *nid = addto_names (buf);
	  for (ptr = suffix_ptr, dptr = buf; *ptr; ptr++,dptr++)
	     *dptr = *ptr;
	  *dptr = '\0';
	  *suffix = addto_names (buf);
	  return (1);
	};

/*
   Identify affixes in rules. Affix variables that are equal to elements
   in a finite lattice get promoted into singleton sets. Affix variables
   that end in an integer suffix are identified by the prefix.
*/
#define promotion_error \
"element %s in rule %s, alt %d has an integer suffix"
static void check_affix_variable (rule srule, int altnr, affix a, int uni)
	{ element_def elt_def;
	  meta_rule mrule;
	  string nont = a -> u.var.nonterminal;
	  string suffix = NULL;
	  string nid;

	  /* if the variable has a suffix, set it apart */
	  if (!v1_compatibility && extract_suffix (nont, &nid, &suffix))
	     { a -> u.var.nonterminal = nid;
	       a -> u.var.suffix = suffix;
	       nont = nid;
	     };

	  /* try identify the corresponding metarule */
	  mrule = lookup_meta_rule (nont);
	  if (mrule != meta_rule_nil)
	     { a -> u.var.def = mrule;
	       return;
	     };
	  if (v1_compatibility) return;

	  /* not a metadefined id, may be it is a lattice element */
	  elt_def = lookup_element (nont);
	  if (elt_def != element_def_nil)
	     { if (suffix != NULL)
		  { ident_error (promotion_error,
		 	nont, srule -> nonterminal, altnr);
		    return;
		  };
	       a -> tag = tag_affix_element;
	       a -> u.elt.eltnm = nont;
	       a -> u.elt.def = elt_def;
	       return;
	     };
	  if (!uni) return;

	  /* this is an unknown id occurring in a union, so it is a new lattice element */
	  if (suffix != NULL)
	     { ident_error (promotion_error, nont, srule -> nonterminal, altnr);
	       return;
	     };
	  elt_def = new_element_def (nont);
	  (void) enter_element (elt_def);
	  a -> tag = tag_affix_element;
	  a -> u.elt.eltnm = nont;
	  a -> u.elt.def = elt_def;
	};

static void check_affix (rule srule, int altnr, affix a, int uni)
	{ switch (a -> tag)
	     { case tag_affix_variable: check_affix_variable (srule, altnr, a, uni); /* fall thru */
	       case tag_affix_terminal:
	       case tag_affix_integer:
	       case tag_affix_real: 
	       case tag_affix_semi: break;
	       default: bad_tag (a -> tag, "check_affix");
	     };
	};

static void check_affix_list (rule srule, int altnr, affix_list al, int uni)
	{ int ix;
	  for (ix = 0; ix < al -> size; ix++)
	     check_affix (srule, altnr, al -> array[ix], uni);
	};

static void check_expression (rule srule, int altnr, expr e)
	{ switch (e -> tag)
	     { case tag_single:	check_affix (srule, altnr, e -> u.single, 0); break;
	       case tag_concat: check_affix_list (srule, altnr, e -> u.concat, 0); break;
	       case tag_compos: check_affix_list (srule, altnr, e -> u.compos, 0); break;
	       case tag_union:	check_affix_list (srule, altnr, e -> u.uni, 1); break;
	       default: bad_tag (e -> tag, "check_expression");
	     };
	};

static void check_positions (rule srule, int altnr, pos_list dpy)
	{ int ix;
	  for (ix = 0; ix < dpy -> size; ix++)
	     check_expression (srule, altnr, dpy -> array[ix] -> ex);
	};

#define call_error2 \
"rule %s, alt %d: application of rule %s has wrong number of positions"
static void check_positions_in_call (rule srule, int altnr, member m)
	{ rule callee = m -> u.call.def;
	  pos_list dpy = m -> u.call.display;

	  if (callee == rule_nil) return;	/* unidentifiable rule */
	  if (callee -> proto_display -> size != dpy -> size) 
	     ident_error (call_error2, srule -> nonterminal,
			  altnr, callee -> nonterminal);
	  check_positions (srule, altnr, dpy);
	};

#define semiterminal_error \
"rule %s, alt %i: semiterminal has too many (%d) affix positions"
static void check_positions_in_semiterminal (rule srule, int altnr, member m)
	{ pos_list dpy = m -> u.semi.display;
	  if (dpy -> size > 1)
	     ident_error (semiterminal_error, srule -> nonterminal,
			  altnr, dpy -> size);
	  check_positions (srule, altnr, dpy);
	};

static void check_positions_in_member (rule srule, int altnr, member m)
	{ switch (m -> tag)
	     { case tag_call: check_positions_in_call (srule, altnr, m);
	       case tag_terminal: break;
	       case tag_semiterminal:
		  check_positions_in_semiterminal (srule, altnr, m);
	       case tag_cut: break;
	       default: bad_tag (m -> tag, "check_positions_in_member");
	     };
	};

static void check_positions_in_members (rule srule, int altnr, member_list ml)
	{ int ix;
	  if (ml == member_list_nil) return;	/* empty alt (external rule) */
	  for (ix = 0; ix < ml -> size; ix++)
	     check_positions_in_member (srule, altnr, ml -> array[ix]);
	};

static void check_positions_in_alt (rule srule, int altnr, alt a)
	{ check_positions (srule, altnr, a -> display);
	  check_positions_in_members (srule, altnr, a -> members);
	};

static void check_positions_in_rule (rule srule)
	{ alt_list alts = srule -> alts;
	  int ix;
	  for (ix = 0; ix < alts -> size; ix++)
	     check_positions_in_alt (srule, ix + 1, alts -> array[ix]);
	};

static void check_positions_in_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     check_positions_in_rule (all_rules[ix]);
	};

static void check_start_rule_positions ()
	{ rule start_def = start_rule -> u.call.def;
	  pos_list start_dpy = start_rule -> u.call.display;

	  if (start_def == rule_nil) return;	/* unidentified */
	  if (start_def -> proto_display -> size != start_dpy -> size)
	     ident_error ("start rule has a wrong number of affix positions");
	};

/*
   Build the per alternative namespace for local affixes. Affix variables
   with the same name are unified into one object. Affix semiterminals are
   changed into affix variables identifying an anonymous meta rule. All
   other affix terms are promoted into anonymous affix variables with a
   predefined value. As a side effect semiterminals without a display
   receive an anonymous one.
*/
static int affix_occurs_on_locals_list (alt a, affix affx, affix *fd_affx)
	{ affix ptr;
	  for (ptr = a -> locals; ptr != affix_nil; ptr = ptr -> next)
	     if (strcmp (affx -> name, ptr -> name) == 0)
		{ ptr -> ref_count++;		/* update expr or display */
		  *fd_affx = ptr;
		  return (1);
	        };
	  return (0);
	};

static void gather_affix_variable (alt a, affix *affx)
	{ affix current = *affx;
	  char name[MAXSTRLEN+3];

	  strcpy (name, "a_");
	  strcat (name, current -> u.var.nonterminal);
	  strcat (name, current -> u.var.suffix);

	  current -> name = name;
	  if (affix_occurs_on_locals_list (a, current, affx)) ckfree (current);
	  else /* it is a new local */
	     { current -> name = addto_names (name);
	       current -> ref_count = 1;
	       current -> next = a -> locals;
	       a -> locals = current;
	     };
	};

string generate_anonymous_affixname ()
	{ char name[20];
	  sprintf (name, "a_gen_%d", generated_nr);
	  generated_nr++;
	  return (addto_names (name));
	};

static void gather_anonymous_affix (alt a, affix *affx)
	{ affix current = *affx;
	  string name = generate_anonymous_affixname ();

	  if (current -> tag == tag_affix_semi)
	     { /* introduce a proper meta definition */
	       meta_alt a2[1];
	       meta_alt_list ml2;
	       meta_rule newmeta;

	       affix newaffix = new_affix_variable (name);
	       expr e2 = new_expr_single (current);

	       a2[0] = new_meta_alt (e2);
	       ml2 = new_meta_alt_list (1, a2);
	       newmeta = new_meta_rule (name, ml2);
	       (void) enter_meta_rule (newmeta);
	       newaffix -> name = name;
	       newaffix -> ref_count = 1;
	       *affx = newaffix;
	       newaffix -> next = a -> locals;
	       a -> locals = newaffix;
	     }
	  else
	     { current -> name = name;
	       current -> ref_count = 1;
	       current -> next = a -> locals;
	       a -> locals = current;
	     };
	};

static pos_list gather_anonymous_display (alt a)
	{ expr newexpr;
	  pos npos;
	  pos_list new = new_pos_list ();

	  string name = generate_anonymous_affixname ();
	  affix newaffix = new_affix_variable (name);
	  newaffix -> name = name;
	  newaffix -> ref_count = 1;
	  newaffix -> next = a -> locals;
	  a -> locals = newaffix;

	  newexpr = new_expr_single (newaffix);
	  npos = new_pos (noflow, newexpr);
	  app_pos_list (new, npos);
	  return (new);
	};

static void gather_affix (alt a, affix *affx)
	{ if ((*affx) -> tag == tag_affix_variable)
	     gather_affix_variable (a, affx);
	  else gather_anonymous_affix (a, affx);
	};

static void gather_affix_list (alt a, affix_list affl)
	{ int ix;
	  for (ix = 0; ix < affl -> size; ix++)
	     gather_affix (a, &affl -> array[ix]);
	};

static void gather_affixes_in_expr (alt a, expr e)
	{ switch (e -> tag)
	     { case tag_single: gather_affix (a, &e -> u.single); break;
	       case tag_compos: gather_affix_list (a, e -> u.compos); break;
	       case tag_concat: gather_affix_list (a, e -> u.concat); break;
	       case tag_union: gather_affix_list (a, e -> u.uni); break;
	       default: bad_tag (e -> tag, "gather_affixes_in_expr");
	     };
	};

static void gather_affixes_in_display (alt a, pos_list dpy)
	{ int ix;
	  if (dpy == pos_list_nil) return;
	  for (ix = 0; ix < dpy -> size; ix++)
	     gather_affixes_in_expr (a, dpy -> array[ix] -> ex);
	};

static void gather_affixes_in_semi (alt a, member m)
	{ if (m -> u.semi.display -> size)
	     gather_affixes_in_display (a, m -> u.semi.display);
	  else
	     m -> u.semi.display = gather_anonymous_display (a);
	};

static void gather_affixes_in_member (alt a, member m)
	{ switch (m -> tag)
	     { case tag_call:
		  gather_affixes_in_display (a, m -> u.call.display);
		  break;
	       case tag_terminal: break;
	       case tag_semiterminal: gather_affixes_in_semi (a, m);
	       case tag_cut: break;
	       default: bad_tag (m -> tag, "gather_affixes_in_member");
	     };
	};

static void gather_affixes_in_members (alt a, member_list mems)
 	{ int ix;
	  if (mems == member_list_nil)
	     internal_error ("gather_affixes_in_members");
	  for (ix = 0; ix < mems -> size; ix++)
	     gather_affixes_in_member (a, mems -> array[ix]);
	};

static void gather_affixes_in_alt (alt a)
	{ gather_affixes_in_display (a, a -> display);
	  gather_affixes_in_members (a, a -> members);
	};

static void gather_affixes_in_alts (alt_list alts)
	{ int ix;
	  for (ix = 0; ix < alts -> size; ix++)
	     gather_affixes_in_alt (alts -> array[ix]);
	};

static void gather_affixes_in_rule (rule srule)
	{ if (srule -> ext) return;
	  gather_affixes_in_alts (srule -> alts);
	};

static void gather_affixes_in_rules ()
	{ int ix;
	  for (ix = 0; ix < nr_of_rules; ix++)
	     gather_affixes_in_rule (all_rules[ix]);
	};

/*
   When the layout flag is in effect, check that the rule for layout
   exists, that it has no positions and that it does not contain any calls.
*/
static void ident_layout_member (member m)
	{ if (m -> tag == tag_call)
	     ident_error ("rule layout contains call");
	};

static void ident_layout_members (member_list mems)
	{ int ix;
	  if (mems == member_list_nil) return;
	  for (ix = 0; ix < mems -> size; ix++)
	     ident_layout_member (mems -> array[ix]);
	};

static void ident_layout_alts (alt_list alts)
	{ int ix;
	  for (ix = 0; ix < alts -> size; ix++)
	     ident_layout_members (alts -> array[ix] -> members);
	};

static void identify_layout_rule ()
	{ layout_rule = lookup_syntax_rule ("layout");
	  if (!layout_rule)
	     ident_error ("rule layout could not be identified");
	  else if (layout_rule -> proto_display -> size)
	     ident_error ("rule layout has affixes");
	  else ident_layout_alts (layout_rule -> alts);
	};

void identification ()
	{ warning ("identification...");
	  init_ident ();
	  identify_in_rules ();
	  identify_start_rule ();
	  identify_in_meta_rules ();
	  hint ("checking positions...");
	  check_positions_in_rules ();
	  check_start_rule_positions ();
	  hint ("collecting affixes in alts...");
	  gather_affixes_in_rules ();
	  gather_affixes_in_alt (start_alt);
	  hint ("generated %d anonymous affix%s",
		generated_nr, (generated_nr == 1)?"":"es");
	  if (layoutflag) identify_layout_rule ();
	  if (ident_errors == 0) return;
	  panic ("%d identification error%s w%s found",
		 ident_errors, (ident_errors == 1)?"":"s",
		 (ident_errors == 1)?"as":"ere");
	};
