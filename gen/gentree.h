/*
   File: gentree.h
   Defines the actions necessary for building the syntax tree
   of an eag during parse
  
   CVS ID: "$Id: gentree.h,v 1.4 2008/06/18 12:22:42 marcs Exp $"
*/
#ifndef IncGenTree
#define IncGenTree

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_value.h>
#include <ebs_lexds.h>

/* local includes */
#include "tree.h"

/* character set manipulation */
cset new_cset (int kind, string str);
#define rdup_cset(s) new_cset (s -> kind, s -> str)

/* affix manipulation */
affix new_affix_variable (string nonterminal);
affix new_affix_terminal (string str);
affix new_affix_integer (int ival);
affix new_affix_real (real rval);
affix new_affix_semi (cset set);
affix new_affix_constant (expr cexpr);
affix rdup_affix (affix a);
affix_list new_affix_list ();
void app_affix_list (affix_list al, affix a);
affix_list rdup_affix_list (affix_list old);

/* affix expression manipulation */
expr new_expr_single (affix a);
expr new_expr_compos (affix_list al);
expr new_expr_concat (affix_list al);
expr new_expr_union (affix_list al);
expr rdup_expr (expr e);

/* position manipulation */
pos new_pos (int kind, expr ex);
pos rdup_pos (pos p);
pos new_empty_pos ();
pos_list new_pos_list ();
void app_pos_list (pos_list pl, pos p);
pos_list rdup_pos_list (pos_list old);
pos_list new_empty_pos_list (int size);

/* member manipulation */
member new_member_call (string nont, pos_list display);
member new_member_terminal (string s);
member new_member_semiterminal (cset s, pos_list display);
member new_member_cut ();
member_list new_member_list ();
void app_member_list (member_list ml, member m);

/* alternative manipulation */
alt new_alt (pos_list pl, member_list ml);
alt_list new_alt_list ();
void app_alt_list (alt_list al, alt a);

/* suntax rule manipulation */
rule new_rule (string nont, alt_list alts, int lex);
rule new_external_rule (string nont, alt_list alts, int kind);

/* meta alternative manipulation */
meta_alt new_meta_alt (expr e);
meta_alt_list new_meta_alt_list ();
void app_meta_alt_list (meta_alt_list mal, meta_alt ma);

/* meta rule manipulation */
meta_rule new_meta_rule (string nont, meta_alt_list alts);
meta_rule new_external_meta_rule (string nont, int type, int kind, int empty);

/* element definition manipulation */
element_def new_element_def (string eltnm);

/* element definition list manipulation */
element_def_list new_element_def_list ();
void app_element_def_list (element_def_list el, element_def e);

/* element set manipulation */
element_set new_element_set (element_def_list defs);

/* element set list manipulation */
element_set_list new_element_set_list ();
void app_element_set_list (element_set_list el, element_set e);

#endif /* IncGenTree */
