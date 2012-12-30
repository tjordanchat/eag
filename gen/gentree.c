/*
   File: gentree.c
   Defines the actions necessary for building the syntax tree during parse
  
   CVS ID: "$Id: gentree.c,v 1.5 2008/06/18 12:22:42 marcs Exp $"
*/

/* general includes */
#include <stdio.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_error.h>
#include <ebs_memalloc.h>
#include <ebs_lexds.h>

/* local includes */
#include "tree.h"
#include "typing.h"
#include "gentree.h"

/* Allocate a character set */
cset new_cset (int kind, string str)
	{ cset new = (cset) ckmalloc (sizeof (struct cset_rec));
	  new -> kind = kind;
	  new -> str = str;
	  return (new);
	};

/* Allocate an affix variable */
affix new_affix_variable (string nonterminal)
	{ affix new = (affix) ckmalloc (sizeof (struct affix_rec));
	  new -> name = string_nil;
	  new -> type = any_type;
	  new -> next = affix_nil;
	  new -> val = value_nil;
	  new -> ref_count = 0;
	  new -> reachable = 0;
	  new -> tag = tag_affix_variable;
	  new -> u.var.nonterminal = nonterminal;
	  new -> u.var.suffix = "";
	  new -> u.var.def = meta_rule_nil;
	  return (new);
	};

/* Allocate an affix terminal */
affix new_affix_terminal (string str)
	{ affix new = (affix) ckmalloc (sizeof (struct affix_rec));
	  new -> name = string_nil;
	  new -> type = string_type;
	  new -> next = affix_nil;
	  new -> val = value_nil;
	  new -> ref_count = 0;
	  new -> reachable = 0;
	  new -> tag = tag_affix_terminal;
	  new -> u.str = str;
	  return (new);
	};

affix new_affix_integer (int ival)
	{ affix new = (affix) ckmalloc (sizeof (struct affix_rec));
	  new -> name = string_nil;
	  new -> type = integer_type;
	  new -> next = affix_nil;
	  new -> val = value_nil;
	  new -> ref_count = 0;
	  new -> reachable = 0;
	  new -> tag = tag_affix_integer;
	  new -> u.inum = ival;
	  return (new);
	};

affix new_affix_real (real rval)
	{ affix new = (affix) ckmalloc (sizeof (struct affix_rec));
	  new -> name = string_nil;
	  new -> type = real_type;
	  new -> next = affix_nil;
	  new -> val = value_nil;
	  new -> ref_count = 0;
	  new -> reachable = 0;
	  new -> tag = tag_affix_real;
	  new -> u.rnum = rval;
	  return (new);
	};

affix new_affix_semi (cset set)
	{ affix new = (affix) ckmalloc (sizeof (struct affix_rec));
	  new -> name = string_nil;
	  new -> type = string_type;
	  new -> next = affix_nil;
	  new -> val = value_nil;
	  new -> ref_count = 0;
	  new -> reachable = 0;
	  new -> tag = tag_affix_semi;
	  new -> u.semi = set;
	  return (new);
	};

affix new_affix_constant (expr cexpr)
	{ affix new = (affix) ckmalloc (sizeof (struct affix_rec));
	  new -> name = string_nil;
	  new -> type = string_type;
	  new -> next = affix_nil;
	  new -> val = value_nil;
	  new -> ref_count = 0;
	  new -> reachable = 0;
	  new -> tag = tag_affix_constant;
	  new -> u.cexpr = cexpr;
	  return (new);
	};

/* Recursively duplicate an affix */
affix rdup_affix (affix a)
	{ affix new;
	  if (a == affix_nil) return (a);
	  new = (affix) ckmalloc (sizeof (struct affix_rec));
	  new -> type = a -> type;
	  new -> name = string_nil;
	  new -> next = affix_nil;
	  new -> val = value_nil;
	  new -> tag = a -> tag;
	  switch (a -> tag)
	     { case tag_affix_variable:
		  { new -> u.var.nonterminal = a -> u.var.nonterminal;
		    new -> u.var.suffix = a -> u.var.suffix;
		    new -> u.var.def = a -> u.var.def;
		  }; break;
	       case tag_affix_terminal: new -> u.str = a -> u.str; break;
	       case tag_affix_integer:	new -> u.inum = a -> u.inum; break;
	       case tag_affix_real:	new -> u.rnum = a -> u.rnum; break;
	       case tag_affix_semi:	new -> u.semi = rdup_cset (a -> u.semi); break;
	       default: bad_tag (a -> tag, "rdup_affix");
	     };
	  return (new);
	};

/* Announce to use 'room' affixes in an affix_list */
static void room_affix_list (affix_list al, int room)
	{ if (room <= al -> room) return;
	  al -> array = (affix *) ckrecalloc (al -> array, room, sizeof (affix));
	  al -> room = room;
	};

/* Allocate a new affix_list */
affix_list new_affix_list ()
	{ affix_list new = (affix_list) ckmalloc (sizeof (struct affix_list_rec));
	  new -> size = 0;
	  new -> room = 2;
	  new -> array = (affix *) ckcalloc (2, sizeof (affix));
	  return (new);
	};

/* Append affix to affix_list */
void app_affix_list (affix_list al, affix a)
	{ if (al -> size == al -> room) room_affix_list (al, al -> size << 1);
	  al -> array [al -> size] = a;
	  al -> size++;
	};

/* Recursively duplicate an affix_list */
affix_list rdup_affix_list (affix_list old)
	{ int ix;
	  affix_list new;
	  if (old == affix_list_nil) return (affix_list_nil);
	  new = (affix_list) ckmalloc (sizeof (struct affix_list_rec));
	  new -> size = old -> size;
	  new -> room = old -> room;
	  new -> array = (affix *) ckcalloc (old -> size, sizeof (affix));
	  for (ix = 0; ix < old -> size; ix++)
	     new -> array[ix] = rdup_affix (old -> array[ix]);
	  return (new);
	};

/* Allocate an affix expression */
expr new_expr_single (affix a)
	{ expr new = (expr) ckmalloc (sizeof (struct expr_rec));
	  new -> tag = tag_single;
	  new -> u.single = a;
	  return (new);
	};

expr new_expr_concat (affix_list al)
	{ expr new = (expr) ckmalloc (sizeof (struct expr_rec));
	  new -> tag = tag_concat;
	  new -> u.concat = al;
	  return (new);
	};

expr new_expr_compos (affix_list al)
	{ expr new = (expr) ckmalloc (sizeof (struct expr_rec));
	  new -> tag = tag_compos;
	  new -> u.compos = al;
	  return (new);
	};

expr new_expr_union (affix_list al)
	{ expr new = (expr) ckmalloc (sizeof (struct expr_rec));
	  new -> tag = tag_union;
	  new -> u.uni = al;
	  return (new);
	};

/* Recursively duplicate an affix expression */
expr rdup_expr (expr e)
	{ expr new;
	  if (e == expr_nil) return (e);
	  new = (expr) ckmalloc (sizeof (struct expr_rec));
	  new -> tag = e -> tag;
	  switch (e -> tag)
	     { case tag_single:
		  new -> u.single = rdup_affix (e -> u.single); break;
	       case tag_compos:
		  new -> u.compos = rdup_affix_list (e -> u.compos); break;
	       case tag_concat:
		  new -> u.concat = rdup_affix_list (e -> u.concat); break;
	       case tag_union:
		  new -> u.uni = rdup_affix_list (e -> u.uni); break;
	       default: bad_tag (e -> tag, "rdup_expr");
	     };
	  return (new);
	};

/* Allocate a position */
pos new_pos (int kind, expr ex)
	{ pos new = (pos) ckmalloc (sizeof (struct pos_rec));
	  new -> kind = kind;
	  new -> ex = ex;
	  new -> type = any_type;
	  new -> lat_repr = element_def_nil;
	  return (new);
	};

/* Recursively duplicate a position */
pos rdup_pos (pos p)
	{ pos new;
	  if (p == pos_nil) return (p);
	  new = (pos) ckmalloc (sizeof (struct pos_rec));
	  new -> kind = p -> kind;
	  new -> ex = rdup_expr (p -> ex);
	  new -> type = p -> type;
	  new -> lat_repr = p -> lat_repr;
	  return (new);
	};

/* Allocate an empty position */
pos new_empty_pos ()
	{ pos new = (pos) ckmalloc (sizeof (struct pos_rec));
	  new -> kind = undefinedflow;
	  new -> ex = expr_nil;
	  new -> type = any_type;
	  new -> lat_repr = element_def_nil;
	  return (new);
	};

/* Announce to use 'room' positions in a pos_list */
static void room_pos_list (pos_list pl, int room)
	{ if (room <= pl -> room) return;
	  pl -> array = (pos *) ckrecalloc (pl -> array, room, sizeof (pos));
	  pl -> room = room;
	};

/* Allocate a new pos_list */
pos_list new_pos_list ()
	{ pos_list new = (pos_list) ckmalloc (sizeof (struct pos_list_rec));
	  new -> size = 0;
	  new -> room = 2;
	  new -> array = (pos *) ckcalloc (2, sizeof (pos));
	  return (new);
	};

/* Append position to pos_list */
void app_pos_list (pos_list pl, pos p)
	{ if (pl -> size == pl -> room) room_pos_list (pl, pl -> size << 1);
	  pl -> array [pl -> size] = p;
	  pl -> size++;
	};

/* Recursively duplicate an pos_list */
pos_list rdup_pos_list (pos_list old)
	{ int ix;
	  pos_list new;
	  if (old == pos_list_nil) return (pos_list_nil);
	  new = (pos_list) ckmalloc (sizeof (struct pos_list_rec));
	  new -> size = old -> size;
	  new -> room = old -> room;
	  new -> array = (pos *) ckcalloc (old -> size, sizeof (pos));
	  for (ix = 0; ix < old -> size; ix++)
	     new -> array[ix] = rdup_pos (old -> array[ix]);
	  return (new);
	};

/* Allocate a list of empty positions */
pos_list new_empty_pos_list (int size)
	{ int ix;
	  pos_list new = (pos_list) ckmalloc
				(sizeof (struct pos_list_rec));
	  new -> size = size;
	  new -> room = size;
	  new -> array = (pos *) ckcalloc (size, sizeof (pos));
	  for (ix = 0; ix < size; ix++) new -> array[ix] = new_empty_pos ();
	  return (new);
	};
 
/* Allocate a member */
member new_member_call (string nonterminal, pos_list display)
	{ member m = (member) ckmalloc (sizeof (struct member_rec));
	  m -> empty = rule_undefined;
	  m -> first = "";
	  m -> followlayout = 0;
	  m -> number = 0;
	  m -> sonnr = 0;
	  m -> tag = tag_call;
	  m -> u.call.nonterminal = nonterminal;
	  m -> u.call.display = display;
	  m -> u.call.def = rule_nil;
	  return (m);
	};

member new_member_terminal (string terminal)
	{ member m = (member) ckmalloc (sizeof (struct member_rec));
	  m -> empty = rule_undefined;
	  m -> first = "";
	  m -> followlayout = 0;
	  m -> number = 0;
	  m -> tag = tag_terminal;
	  m -> u.terminal = terminal;
	  return (m);
	};

member new_member_semiterminal (cset s, pos_list display)
	{ member m = (member) ckmalloc (sizeof (struct member_rec));
	  m -> empty = rule_undefined;
	  m -> first = "";
	  m -> followlayout = 0;
	  m -> number = 0;
	  m -> tag = tag_semiterminal;
	  m -> u.semi.set = s;
	  m -> u.semi.display = display;
	  return (m);
	};

member new_member_cut ()
	{ member m = (member) ckmalloc (sizeof (struct member_rec));
	  m -> empty = rule_undefined;
	  m -> first = "";
	  m -> followlayout = 0;
	  m -> number = 0;
	  m -> tag = tag_cut;
	  return (m);
	};

/* Announce to use 'room' members in an member_list */
static void room_member_list (member_list ml, int room)
	{ if (room <= ml -> room) return;
	  ml -> array = (member *) ckrecalloc (ml -> array, room, sizeof (member));
	  ml -> room = room;
	};

/* Allocate a new member_list */
member_list new_member_list ()
	{ member_list new = (member_list) ckmalloc (sizeof (struct member_list_rec));
	  new -> size = 0;
	  new -> room = 2;
	  new -> array = (member *) ckcalloc (2, sizeof (member));
	  return (new);
	};

/* Append member to member_list */
void app_member_list (member_list ml, member m)
	{ if (ml -> size == ml -> room) room_member_list (ml, ml -> size << 1);
	  ml -> array [ml -> size] = m;
	  ml -> size++;
	};

/* Allocate a meta alternative */
meta_alt new_meta_alt (expr e)
	{ meta_alt new = (meta_alt) ckmalloc (sizeof (struct meta_alt_rec));
	  new -> e = e;
	  new -> number = 0;
	  return (new);
	};

/* Announce to use 'room' meta_alts in an meta_alt_list */
static void room_meta_alt_list (meta_alt_list mal, int room)
	{ if (room <= mal -> room) return;
	  mal -> array = (meta_alt *) ckrecalloc (mal -> array, room, sizeof (meta_alt));
	  mal -> room = room;
	};

/* Allocate a new meta_alt_list */
meta_alt_list new_meta_alt_list ()
	{ meta_alt_list new = (meta_alt_list) ckmalloc (sizeof (struct meta_alt_list_rec));
	  new -> size = 0;
	  new -> room = 2;
	  new -> array = (meta_alt *) ckcalloc (2, sizeof (meta_alt));
	  return (new);
	};

/* Append meta_alt to meta_alt_list */
void app_meta_alt_list (meta_alt_list mal, meta_alt ma)
	{ if (mal -> size == mal -> room)
	     room_meta_alt_list (mal, mal -> size << 1);
	  mal -> array [mal -> size] = ma;
	  mal -> size++;
	};

/* Allocate a meta rule */
meta_rule new_meta_rule (string nonterminal, meta_alt_list alts)
	{ meta_rule new = (meta_rule) ckmalloc (sizeof (struct meta_rule_rec));
	  new -> nonterminal = nonterminal;
	  new -> alts = alts;
	  new -> type = any_type;
	  new -> kind = undefined_meta_value;
	  new -> ext = 0;
	  new -> empty = 0;
	  new -> number = 0;
	  new -> mvalue = value_nil;
	  new -> reachable = 0;
	  new -> lat_repr = element_def_nil;
	  new -> needs_rec = 0;
	  new -> needs_recbup = 0;
	  return (new);
	};

meta_rule new_external_meta_rule (string nonterminal, int type, int kind, int empty)
	{ meta_rule new = (meta_rule) ckmalloc (sizeof (struct meta_rule_rec));
	  new -> nonterminal = nonterminal;
	  new -> alts = meta_alt_list_nil;
	  new -> type = type;
	  new -> ext = 1;
	  new -> empty = empty;
	  new -> kind = kind;
	  new -> number = 0;
	  new -> mvalue = value_nil;
	  new -> reachable = 0;
	  new -> lat_repr = element_def_nil;
	  new -> needs_rec = 0;
	  new -> needs_recbup = 0;
	  return (new);
	};

/* Allocate an alternative */
alt new_alt (pos_list pl, member_list ml)
	{ alt new = (alt) ckmalloc (sizeof (struct alt_rec));
	  new -> display = pl;
	  new -> members = ml;
	  new -> nodenr = 0;
	  new -> nrsons = 0;
	  new -> locals = affix_nil;
	  new -> empty = rule_undefined;
	  new -> number = 0;
	  new -> first = "";
	  new -> director = "";
	  new -> rule_nr = 0;
	  return (new);
	};

/* Announce to use 'room' alts in an alt_list */
static void room_alt_list (alt_list al, int room)
	{ if (room <= al -> room) return;
	  al -> array = (alt *) ckrecalloc (al -> array, room, sizeof (alt));
	  al -> room = room;
	};

/* Allocate a new alt_list */
alt_list new_alt_list ()
	{ alt_list new = (alt_list) ckmalloc (sizeof (struct alt_list_rec));
	  new -> size = 0;
	  new -> room = 2;
	  new -> array = (alt *) ckcalloc (2, sizeof (alt));
	  return (new);
	};

/* Append alt to alt_list */
void app_alt_list (alt_list al, alt a)
	{ if (al -> size == al -> room) room_alt_list (al, al -> size << 1);
	  al -> array [al -> size] = a;
	  al -> size++;
	};

/* Allocate a syntax rule */
rule new_rule (string nonterminal, alt_list alts, int lex)
	{ rule new = (rule) ckmalloc (sizeof (struct rule_rec));
	  new -> nonterminal = nonterminal;
	  new -> alts = alts;
	  new -> lex = lex;
	  new -> ext = 0;
	  new -> kind = rule_undefined;
	  new -> empty = rule_undefined;
	  new -> number = 0;
	  new -> placeholder = 0;
	  new -> startswithlayout = 0;
	  new -> endsinlayout = 0;
	  new -> first = "";
	  new -> follow = "";
	  new -> proto_display = pos_list_nil;
	  new -> reachable = 0;
	  new -> has_sons = 0;
	  new -> goal = -1;
	  new -> rev_tree = lextree_nil;
	  return (new);
	};

rule new_external_rule (string nonterminal, alt_list alts, int kind)
	{ rule new = (rule) ckmalloc (sizeof (struct rule_rec));
	  new -> nonterminal = nonterminal;
	  new -> alts = alts;
	  new -> lex = 0;
	  new -> ext = 1;
	  new -> kind = kind;
	  new -> empty = rule_undefined;
	  new -> number = 0;
	  new -> placeholder = 0;
	  new -> startswithlayout = 0;
	  new -> endsinlayout = 0;
	  new -> first = "";
	  new -> follow = "";
	  new -> proto_display = pos_list_nil;
	  new -> reachable = 0;
	  new -> has_sons = 0;
	  new -> goal = -1;
	  new -> rev_tree = lextree_nil;
	  return (new);
	};

element_def new_element_def (string eltnm)
	{ element_def new = (element_def) ckmalloc (sizeof (struct element_def_rec));
	  new -> eltnm = eltnm;
	  new -> eltnr = -1;
	  new -> set = element_set_nil;
	  return (new);
	};

/* Announce to use 'room' element_defs in a element_def_list */
static void room_element_def_list (element_def_list el, int room)
	{ if (room <= el -> room) return;
	  el -> array = (element_def *) ckrecalloc (el -> array, room, sizeof (element_def));
	  el -> room = room;
	};

/* Allocate a new element_def_list */
element_def_list new_element_def_list ()
	{ element_def_list new = (element_def_list) ckmalloc (sizeof (struct element_def_list_rec));
	  new -> size = 0;
	  new -> room = 2;
	  new -> array = (element_def *) ckcalloc (2, sizeof (element_def));
	  return (new);
	};

/* Append element_def to element_def_list */
void app_element_def_list (element_def_list el, element_def e)
	{ if (el -> size == el -> room)
	     room_element_def_list (el, el -> size << 1);
	  el -> array [el -> size] = e;
	  el -> size++;
	};

/* Allocate a new element_set */
element_set new_element_set (element_def_list defs)
	{ element_set new = (element_set)
		ckmalloc (sizeof (struct element_set_rec));
	  new -> number = 0;
	  new -> defs = defs;
	  return (new);
	};

/* Announce to use 'room' element_sets in a element_set_list */
static void room_element_set_list (element_set_list el, int room)
	{ if (room <= el -> room) return;
	  el -> array = (element_set *) ckrecalloc (el -> array, room, sizeof (element_set));
	  el -> room = room;
	};

/* Allocate a new element_set_list */
element_set_list new_element_set_list ()
	{ element_set_list new = (element_set_list) ckmalloc (sizeof (struct element_set_list_rec));
	  new -> size = 0;
	  new -> room = 2;
	  new -> array = (element_set *) ckcalloc (2, sizeof (element_set));
	  return (new);
	};

/* Append element_set to element_set_list */
void app_element_set_list (element_set_list el, element_set e)
	{ if (el -> size == el -> room)
	     room_element_set_list (el, el -> size << 1);
	  el -> array [el -> size] = e;
	  el -> size++;
	};

