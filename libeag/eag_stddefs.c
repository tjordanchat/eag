/*
   File: eag_stddefs.c
   Implements the standard eag library

   CVS ID: "$Id: eag_stddefs.c,v 1.6 2008/06/21 14:13:17 marcs Exp $"
*/

/* global includes */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_error.h>
#include <ebs_textstorage.h>
#include <ebs_value.h>
#include <ebs_lexds.h>

/* local includes */
#include "eag_ds.h"
#include "eag_nodeinfo.h"
#include "eag_textparsing.h"
#include "eag_buildtree.h"
#include "eag_propagate.h"
#include "eag_stddefs.h"

/* our module number */
static int eag_stddefs_modnr = 1;

/*
   Meta rules
   nil, string, int, real, tuple, any
*/

/*
   meta defined nil
*/
affix_node make_nil_affix ()
{ value nilval = new_tuple_value (new_value_list (0));
  return (value_to_affix ("predef_nil", nilval));
}

void rec_nil_value ()
{ value v = popv ();
  if (v -> tag == undefined_value) callq();
  else if ((v -> tag == tuple_value) &&
	   (v -> u.tuple -> size == 0)) callq ();
  pushv (v);
  pushq (rec_nil_value);
}

/*
   meta defined string
*/
affix_node make_string_affix ()
{ affix_node new = new_affix_node ("predef_string");
  new -> mfunc = rec_string_value;
  return (new);
}

void rec_string_value ()
{ value v = popv();
  if (v -> tag == undefined_value) callq();
  else if (v -> tag == string_value) callq();
  pushv (v);
  pushq (rec_string_value);
}

/* Recognize any portion of the meta input string */
void recbup_string_value ()
{ int i;
  for (i = 0; i <= strlen (miptr); i++)
    { char *sptr = miptr;
      miptr = miptr+i;
      callq ();
      miptr = sptr;
    };
  pushq (recbup_string_value);
}

/*
   meta defined tuple
*/
affix_node make_tuple_affix ()
{ affix_node new = new_affix_node ("predef_tuple");
  new -> mfunc = rec_tuple_value;
  return (new);
}

void rec_tuple_value ()
{ value v = popv();
  if (v -> tag == undefined_value) callq ();
  else if (v -> tag == tuple_value) callq ();
  pushv (v);
  pushq (rec_tuple_value);
}

/*
   meta defined int
*/
affix_node make_int_affix ()
{ affix_node new = new_affix_node ("predef_int");
  new -> mfunc = rec_int_value;
  return (new);
}

void rec_int_value ()
{ value v = popv ();
  if (v -> tag == undefined_value) callq ();
  else if (v -> tag == integer_value) callq ();
  pushv (v);
  pushq (rec_int_value);
}

/* Recognize any portion of the integer meta input, provided it is positive */
void recbup_int_value ()
{ int i;
  if (mtotal >= 0)
    for (i=0; i <= mtotal; i++)
      { int s = mtotal;
	mtotal = mtotal - i;
	callq ();
	mtotal = s;
      };
  pushq (recbup_int_value);
}

/*
   meta defined real
*/
affix_node make_real_affix ()
{ affix_node nn = new_affix_node ("predef_real");
  nn -> mfunc = rec_real_value;
  return (nn);
}

void rec_real_value ()
{ value v = popv ();
  if (v -> tag == undefined_value) callq ();
  else if (v -> tag == real_value) callq ();
  pushv (v);
  pushq (rec_real_value);
}

/* MS: Quite impossible, to recognize all parts of a real */
void recbup_real_value ()
{ /* int i;
  if (mtotal >= 0)
    for (i=0; i <= mtotal; i++)
      { int s = mtotal;
	mtotal = mtotal - i;
	callq ();
	mtotal = s;
      };
  */
  pushq (recbup_real_value);
}

/*
   meta defined nonlat
*/
affix_node make_nonlat_affix ()
{ affix_node new = new_affix_node ("predef_nonlat");
  new -> mfunc = rec_nonlat_value;
  return (new);
}

void rec_nonlat_value ()
{ value v = popv();
  if (v -> tag == undefined_value) callq ();
  else if ((v -> tag == string_value) ||
           (v -> tag == integer_value) ||
	   (v -> tag == real_value) ||
	   (v -> tag == tuple_value)) callq ();
  pushv (v);
  pushq (rec_nonlat_value);
}

/*
   Semi Predicates: end of sentence (0), column (1), row (2),
*/

/*
   semipredicate end of sentence
*/
void semipred_endofsentence ()
{ pushi (0);
  pushi (0);
  pushi (mk_nodenr (eag_stddefs_modnr, 0));
  pushq (make_semipredicate_node);
  pushq (endofsentence);
  callq ();
  pop (5);
  pushq (semipred_endofsentence);
}

/*
   semipredicate column
*/
void semipred_column ()
{ value cc_val = new_integer_value (currentcolumn ());
  affix_node a_col = value_to_affix ("predef_column", cc_val);
  pusha (a_col);
  pushi (tag_single);
  pushi (1);
  pushi (0);
  pushi (mk_nodenr (eag_stddefs_modnr, 1));
  pushq (make_semipredicate_node);
  callq ();
  pop (6);
  rfre_value (a_col -> val);
  free_affix_node (a_col);
  pushq (semipred_column);
}

/*
   semi predicate row
*/
void semipred_row ()
{ value cr_val = new_integer_value (currentrow ());
  affix_node a_row = value_to_affix ("predef_row", cr_val);
  pusha (a_row);
  pushi (tag_single);
  pushi (1);
  pushi (0);
  pushi (mk_nodenr (eag_stddefs_modnr, 2));
  pushq (make_semipredicate_node);
  callq ();
  pop (6);
  rfre_value (a_row -> val);
  free_affix_node (a_row);
  pushq (semipred_row);
}

/*
   Generic predicates: equal (3), not equal (4)

   Predicate equal
   When either side is known, propagate the affix value
*/
static void act_equal (pos_node ps1, pos_node ps2)
{ if (ps1 -> sides[lower_side].sill == 0)
    { value v1 = calc_affix_value (ps1, lower_side);
      pushp (ps2);
      pushv (v1);
      pushq (propagate_predicate_value);
      callq ();
      pop (3);
      rfre_value (v1);
    }
  else if (ps2 -> sides[lower_side].sill == 0)
    { value v2 = calc_affix_value (ps2, lower_side);
      pushp (ps1);
      pushv (v2);
      pushq (propagate_predicate_value);
      callq ();
      pop (3);
      rfre_value (v2);
    }
  else callq ();
}

static void delayed_equal (pos_node *ps)
{ pos_node ps1 = ps[0];
  pos_node ps2 = ps[1];
  if (!crit_pos_has_value (ps1) && !crit_pos_has_value (ps2)) callq ();
  else
    { ps1 -> delayed = 0;
      ps2 -> delayed = 0;
      act_equal (ps1, ps2);
      ps2 -> delayed = 1;
      ps1 -> delayed = 1;
    }
}

void pred_equal ()
{ affix_node af1 = new_affix_node ("equal_af1");
  affix_node af2 = new_affix_node ("equal_af2");
  pushq (delayed_equal);
  pushq (make_node_delayed);
  pusha (af2);
  pushi (tag_single);
  pusha (af1);
  pushi (tag_single);
  pushi (2);
  pushi (0);
  pushi (mk_nodenr (eag_stddefs_modnr, 3));	/* equal nodenr */
  pushq (make_predicate_node);
  callq ();
  pop (10);
  rfre_value (af1 -> val);			/* ??? */
  free_affix_node (af1);
  rfre_value (af2 -> val);			/* ??? */
  free_affix_node (af2);
  pushq (pred_equal);
}

/*
   Predicate notequal
   Only when both sides have a known value, we can check non equality
*/
static void act_notequal (pos_node ps1, pos_node ps2)
{ value v1 = calc_affix_value (ps1, lower_side);
  value v2 = calc_affix_value (ps2, lower_side);
  if (!equal_value (v1, v2)) callq ();
  rfre_value (v1);
  rfre_value (v2);
}

static void delayed_notequal (pos_node *ps)
{ pos_node ps1 = ps[0];
  pos_node ps2 = ps[1];
  if (!crit_pos_has_value (ps1) || !crit_pos_has_value (ps2)) callq ();
  else
    { ps1 -> delayed = 0;
      ps2 -> delayed = 0;
      act_notequal (ps1, ps2);
      ps2 -> delayed = 1;
      ps1 -> delayed = 1;
    }
}

void pred_notequal ()
{ affix_node af1 = new_affix_node ("notequal_af1");
  affix_node af2 = new_affix_node ("notequal_af2");
  pushq (delayed_notequal);
  pushq (make_node_delayed);
  pusha (af2);
  pushi (tag_single);
  pusha (af1);
  pushi (tag_single);
  pushi (2);
  pushi (0);
  pushi (mk_nodenr (eag_stddefs_modnr, 4));	/* notequal nodenr */
  pushq (make_predicate_node);
  callq ();
  pop (10);
  rfre_value (af1 -> val);			/* ??? */
  free_affix_node (af1);
  rfre_value (af2 -> val);			/* ??? */
  free_affix_node (af2);
  pushq (pred_notequal);
}

/*
   Numeric predicates: minus (5), mul (6), div (7), power (8)
*/

/*
   Predicate minus (>int1, >int2, int3>): pred_minus
   Note that we do not yet use possible bidirectionality
*/
static void act_minus (pos_node ps1, pos_node ps2, pos_node ps3)
{ value v1 = calc_affix_value (ps1, lower_side);
  value v2 = calc_affix_value (ps2, lower_side);
  if ((v1 -> tag == integer_value) && (v2 -> tag == integer_value))
    { value v3 = new_integer_value (v1 -> u.inum - v2 -> u.inum);
      pushp (ps3);
      pushv (v3);
      pushq (propagate_predicate_value);
      callq ();
      pop (3);
      rfre_value (v3);
    };
  rfre_value (v1);
  rfre_value (v2);
}

static void delayed_minus (pos_node *ps)
{ pos_node ps1 = ps[0];
  pos_node ps2 = ps[1];
  pos_node ps3 = ps[2];
  if (!crit_pos_has_value (ps1) || !crit_pos_has_value (ps2)) callq ();
  else
    { ps1 -> delayed = 0;
      ps2 -> delayed = 0;
      ps3 -> delayed = 0;
      act_minus (ps1, ps2, ps3);
      ps3 -> delayed = 1;
      ps2 -> delayed = 1;
      ps1 -> delayed = 1;
    };
}

void pred_minus ()
{ affix_node af1 = new_affix_node ("minus_af1");
  affix_node af2 = new_affix_node ("minus_af2");
  affix_node af3 = new_affix_node ("minus_af3");
  pushq (delayed_minus);
  pushq (make_node_delayed);
  pusha (af3);
  pushi (tag_single);
  pusha (af2);
  pushi (tag_single);
  pusha (af1);
  pushi (tag_single);
  pushi (3);
  pushi (0);
  pushi (mk_nodenr (eag_stddefs_modnr, 5));	/* minus nodenr */
  pushq (make_predicate_node);
  callq ();
  pop (10);
  rfre_value (af1 -> val);			/* ??? */
  free_affix_node (af1);
  rfre_value (af2 -> val);			/* ??? */
  free_affix_node (af2);
  rfre_value (af3 -> val);			/* ??? */
  free_affix_node (af3);
  pushq (pred_minus);
}

/*
   pred_mul
*/
static void act_mul (pos_node ps1, pos_node ps2, pos_node ps3)
{ value v1 = calc_affix_value (ps1, lower_side);
  value v2 = calc_affix_value (ps2, lower_side);
  if ((v1 -> tag == integer_value) && (v2 -> tag == integer_value))
    { value v3 = new_integer_value (v1 -> u.inum * v2 -> u.inum);
      pushp (ps3);
      pushv (v3);
      pushq (propagate_predicate_value);
      callq ();
      pop (3);
      rfre_value (v3);
    };
  rfre_value (v1);
  rfre_value (v2);
}

static void delayed_mul (pos_node *ps)
{ pos_node ps1 = ps[0];
  pos_node ps2 = ps[1];
  pos_node ps3 = ps[2];
  if (!crit_pos_has_value (ps1) || !crit_pos_has_value (ps2)) callq ();
  else
    { ps1 -> delayed = 0;
      ps2 -> delayed = 0;
      ps3 -> delayed = 0;
      act_mul (ps1, ps2, ps3);
      ps3 -> delayed = 1;
      ps2 -> delayed = 1;
      ps1 -> delayed = 1;
     }
}

void pred_mul ()
{ affix_node af1 = new_affix_node ("mul_af1");
  affix_node af2 = new_affix_node ("mul_af2");
  affix_node af3 = new_affix_node ("mul_af3");
  pushq (delayed_mul);
  pushq (make_node_delayed);
  pusha (af3);
  pushi (tag_single);
  pusha (af2);
  pushi (tag_single);
  pusha (af1);
  pushi (tag_single);
  pushi (3);
  pushi (0);
  pushi (mk_nodenr (eag_stddefs_modnr, 6));		/* mul nodenr */
  pushq (make_predicate_node);
  callq ();
  pop (10);
  rfre_value (af1 -> val);
  free_affix_node (af1);
  rfre_value (af2 -> val);
  free_affix_node (af2);
  rfre_value (af3 -> val);
  free_affix_node (af3);
  pushq (pred_mul);
}

/*
   pred_div
*/
static void act_div (pos_node ps1, pos_node ps2, pos_node ps3)
{ value v1 = calc_affix_value (ps1, lower_side);
  value v2 = calc_affix_value (ps2, lower_side);
  if ((v1 -> tag == integer_value) && (v2 -> tag == integer_value))
    { value v3 = new_integer_value (v1 -> u.inum / v2 -> u.inum);
      pushp (ps3);
      pushv (v3);
      pushq (propagate_predicate_value);
      callq ();
      pop (3);
      rfre_value (v3);
    };
 rfre_value (v1);
 rfre_value (v2);
}

static void delayed_div (pos_node *ps)
{ pos_node ps1 = ps[0];
  pos_node ps2 = ps[1];
  pos_node ps3 = ps[2];
  if (!crit_pos_has_value (ps1) || !crit_pos_has_value (ps2)) callq ();
  else
    { ps1 -> delayed = 0;
      ps2 -> delayed = 0;
      ps3 -> delayed = 0;
      act_div (ps1, ps2, ps3);
      ps3 -> delayed = 1;
      ps2 -> delayed = 1;
      ps1 -> delayed = 1;
    };
}

void pred_div ()
{ affix_node af1 = new_affix_node ("div_af1");
  affix_node af2 = new_affix_node ("div_af2");
  affix_node af3 = new_affix_node ("div_af3");
  pushq (delayed_div);
  pushq (make_node_delayed);
  pusha (af3);
  pushi (tag_single);
  pusha (af2);
  pushi (tag_single);
  pusha (af1);
  pushi (tag_single);
  pushi (3);
  pushi (0);
  pushi (mk_nodenr (eag_stddefs_modnr, 7));	/* div nodenr */
  pushq (make_predicate_node);
  callq ();
  pop (10);
  rfre_value (af1 -> val);			/* ??? */
  free_affix_node (af1);
  rfre_value (af2 -> val);			/* ??? */
  free_affix_node (af2);
  rfre_value (af3 -> val);			/* ??? */
  free_affix_node (af3);
  pushq (pred_div);
}

/*
   pred_power
*/
static int int_power (int a, int b)
{ int z = 1;
  int tb = b;
  int ta = a;
  if (b < 0) return (0);
  while (tb)
    { if (tb & 1) z *= ta;
      tb = tb >> 1;
      ta = ta * ta;
    };
  return (z);
}

static void act_power (pos_node ps1, pos_node ps2, pos_node ps3)
{ value v1 = calc_affix_value (ps1, lower_side);
  value v2 = calc_affix_value (ps2, lower_side);
  if ((v1 -> tag == integer_value) && (v2 -> tag == integer_value))
    { int pw = int_power (v1 -> u.inum, v2 -> u.inum);
      value v3 = new_integer_value (pw);
      pushp (ps3);
      pushv (v3);
      pushq (propagate_predicate_value);
      callq ();
      pop (3);
      rfre_value (v3);
    };
  rfre_value (v1);
  rfre_value (v2);
}

static void delayed_power (pos_node *ps)
{ pos_node ps1 = ps[0];
  pos_node ps2 = ps[1];
  pos_node ps3 = ps[2];
  if (!crit_pos_has_value (ps1) || !crit_pos_has_value (ps2)) callq ();
  else
    { ps1 -> delayed = 0;
      ps2 -> delayed = 0;
      ps3 -> delayed = 0;
      act_power (ps1, ps2, ps3);
      ps3 -> delayed = 1;
      ps2 -> delayed = 1;
      ps1 -> delayed = 1;
    };
}

void pred_power ()
{ affix_node af1 = new_affix_node ("power_af1");
  affix_node af2 = new_affix_node ("power_af2");
  affix_node af3 = new_affix_node ("power_af3");
  pushq (delayed_power);
  pushq (make_node_delayed);
  pusha (af3);
  pushi (tag_single);
  pusha (af2);
  pushi (tag_single);
  pusha (af1);
  pushi (tag_single);
  pushi (3);
  pushi (0);
  pushi (mk_nodenr (eag_stddefs_modnr, 8));	/* power nodenr */
  pushq (make_predicate_node);
  callq ();
  pop (10);
  rfre_value (af1 -> val);			/* ??? */
  free_affix_node (af1);
  rfre_value (af2 -> val);			/* ??? */
  free_affix_node (af2);
  rfre_value (af3 -> val);			/* ??? */
  free_affix_node (af3);
  pushq (pred_div);
}

/*
   Conversion predicates: int to string (9), string to int (10), uppercase (11)
*/

/*
   pred_inttostring
*/
static void act_inttostring (pos_node ps1, pos_node ps2)
{ value v1 = calc_affix_value (ps1, lower_side);
  if (v1 -> tag == integer_value)
    { char buf[12];
      value v2;
      sprintf (buf, "%d", v1 -> u.inum);
      v2 = new_string_value (buf);
      pushp (ps2);
      pushv (v2);
      pushq (propagate_predicate_value);
      callq ();
      pop (3);
      rfre_value (v2);
    };
  rfre_value (v1);
}

static void delayed_inttostring (pos_node *ps)
{ pos_node ps1 = ps[0];
  pos_node ps2 = ps[1];
  if (!crit_pos_has_value (ps1)) callq ();
  else
    { ps1 -> delayed = 0;
      ps2 -> delayed = 0;
      act_inttostring (ps1, ps2);
      ps2 -> delayed = 1;
      ps1 -> delayed = 1;
    };
}

void pred_inttostring ()
{ affix_node af1 = new_affix_node ("inttostring_af1");
  affix_node af2 = new_affix_node ("inttostring_af2");
  pushq (delayed_inttostring);
  pushq (make_node_delayed);
  pusha (af2);
  pushi (tag_single);
  pusha (af1);
  pushi (tag_single);
  pushi (2);
  pushi (0);
  pushi (mk_nodenr (eag_stddefs_modnr, 9));	/* inttostring nodenr */
  pushq (make_predicate_node);
  callq ();
  pop (10);
  rfre_value (af1 -> val);			/* ??? */
  free_affix_node (af1);
  rfre_value (af2 -> val);			/* ??? */
  free_affix_node (af2);
  pushq (pred_inttostring);
}

/*
   pred_stringtoint
   Perhaps too strict: read away white space ?
*/
static void act_stringtoint (pos_node ps1, pos_node ps2)
{ value v1 = calc_affix_value (ps1, lower_side);
  if (v1 -> tag == string_value)
    { char *ptr = v1 -> u.str;
      if (isdigit (*ptr))
	{ int val = 0;
	  do
	    { int digit = *ptr - '0';
	      val = 10 * val + digit;
	      ptr++;
	    }
	  while (isdigit (*ptr));
	  if (!(*ptr))
	    { value v2 = new_integer_value (val);
	      pushp (ps2);
	      pushv (v2);
	      pushq (propagate_predicate_value);
	      callq ();
	      pop (3);
	      rfre_value (v2);
	    };
	};
    };
  rfre_value (v1);
}

static void delayed_stringtoint (pos_node *ps)
{ pos_node ps1 = ps[0];
  pos_node ps2 = ps[1];
  if (!crit_pos_has_value (ps1)) callq ();
  else
    { ps1 -> delayed = 0;
      ps2 -> delayed = 0;
      act_stringtoint (ps1, ps2);
      ps2 -> delayed = 1;
      ps1 -> delayed = 1;
    };
}

void pred_stringtoint ()
{ affix_node af1 = new_affix_node ("stringtoint_af1");
  affix_node af2 = new_affix_node ("stringtoint_af2");
  pushq (delayed_stringtoint);
  pushq (make_node_delayed);
  pusha (af2);
  pushi (tag_single);
  pusha (af1);
  pushi (tag_single);
  pushi (2);
  pushi (0);
  pushi (mk_nodenr (eag_stddefs_modnr, 10));	/* stringtoint nodenr */
  pushq (make_predicate_node);
  callq ();
  pop (10);
  rfre_value (af1 -> val);			/* ??? */
  free_affix_node (af1);
  rfre_value (af2 -> val);			/* ??? */
  free_affix_node (af2);
  pushq (pred_stringtoint);
}

/*
   predicate uppercase
   converts all lowercase letters into uppercase
*/
static void act_uppercase (pos_node ps1, pos_node ps2)
{ value v1 = calc_affix_value (ps1, lower_side);
  if (v1 -> tag == string_value)
    { char *ptr1, *ptr2;
      value v2;
      for (ptr1 = v1 -> u.str, ptr2 = strstore; *ptr1; ptr1++, ptr2++)
	if (islower (*ptr1)) *ptr2 = toupper (*ptr1);
	else *ptr2 = *ptr1;
      *ptr2 = '\0';
      v2 = new_string_value (strstore);
      pushp (ps2);
      pushv (v2);
      pushq (propagate_predicate_value);
      callq ();
      pop (3);
      rfre_value (v2);
    };
  rfre_value (v1);
}

static void delayed_uppercase (pos_node *ps)
{ pos_node ps1 = ps[0];
  pos_node ps2 = ps[1];
  if (!crit_pos_has_value (ps1)) callq ();
  else
    { ps1 -> delayed = 0;
      ps2 -> delayed = 0;
      act_uppercase (ps1, ps2);
      ps2 -> delayed = 1;
      ps1 -> delayed = 1;
    };
}

void pred_uppercase ()
{ affix_node af1 = new_affix_node ("uppercase_af1");
  affix_node af2 = new_affix_node ("uppercase_af2");
  pushq (delayed_uppercase);
  pushq (make_node_delayed);
  pusha (af2);
  pushi (tag_single);
  pusha (af1);
  pushi (tag_single);
  pushi (2);
  pushi (0);
  pushi (mk_nodenr (eag_stddefs_modnr, 11));	/* uppercase nodenr */
  pushq (make_predicate_node);
  callq ();
  pop (10);
  rfre_value (af1 -> val);
  free_affix_node (af1);
  rfre_value (af2 -> val);
  free_affix_node (af2);
  pushq (pred_uppercase);
}

/*
   predicate not in reserved word list (12)
   checks whether its first argument is not present in its second
   -> libebs???
*/
static int string_in_value (string str, value val)
{ switch (val -> tag)
    { case string_value: return (strcmp (str, val -> u.str) == 0);
      case tuple_value:
	{ value_list vl = val -> u.tuple;
	  int ix;
	  for (ix = 0; ix < vl -> size; ix++)
	    if (string_in_value (str, vl -> array[ix]))
	      return (1);
	  return (0);
	};
      default: return (0);
    };
}

static void act_notinreservedwordlist (pos_node ps1, pos_node ps2)
{ value v1 = calc_affix_value (ps1, lower_side);
  value v2 = calc_affix_value (ps2, lower_side);
  if ((v1 -> tag == string_value) && (v2 -> tag == tuple_value) &&
      !string_in_value (v1 -> u.str, v2)) callq ();
  rfre_value (v1);
  rfre_value (v2);
};

static void delayed_notinreservedwordlist (pos_node *ps)
{ pos_node ps1 = ps[0];
  pos_node ps2 = ps[1];
  if (!crit_pos_has_value (ps1) || !crit_pos_has_value (ps2)) callq ();
  else
    { ps1 -> delayed = 0;
      ps2 -> delayed = 0;
      act_notinreservedwordlist (ps1, ps2);
      ps2 -> delayed = 1;
      ps1 -> delayed = 1;
    };
}

void pred_notinreservedwordlist ()
{ affix_node af1 = new_affix_node ("notinreservedwordlist_af1");
  affix_node af2 = new_affix_node ("notinreservedwordlist_af2");
  pushq (delayed_notinreservedwordlist);
  pushq (make_node_delayed);
  pusha (af2);
  pushi (tag_single);
  pusha (af1);
  pushi (tag_single);
  pushi (2);
  pushi (0);
  pushi (mk_nodenr (eag_stddefs_modnr, 12));	/* nodenr */
  pushq (make_predicate_node);
  callq ();
  pop (10);
  rfre_value (af1 -> val);			/* ??? */
  free_affix_node (af1);
  rfre_value (af2 -> val);			/* ??? */
  free_affix_node (af2);
  pushq (pred_notinreservedwordlist);
}

/*
   predicate dumpaffix (13)
*/
static void act_dumpaffix (pos_node ps1)
{ value v1 = calc_affix_value (ps1, lower_side);
  output_value (stderr, v1);
  rfre_value (v1);
}

static void delayed_dumpaffix (pos_node *ps)
{ pos_node ps1 = ps[0];
  if (!crit_pos_has_value(ps1)) callq ();
  else
    { ps1 -> delayed = 0;
      act_dumpaffix (ps1);
      ps1 -> delayed = 1;
    };
}

void pred_dumpaffix ()
{ affix_node af1 = new_affix_node ("dumpaffix_af1");
  pushq (delayed_dumpaffix);
  pushq (make_node_delayed);
  pusha (af1);
  pushi (tag_single);
  pushi (1);
  pushi (0);
  pushi (mk_nodenr (eag_stddefs_modnr, 13));	/* dumpaffix nodenr */
  pushq (make_predicate_node);
  callq ();
  pop (10);
  rfre_value (af1 -> val);			/* ??? */
  free_affix_node (af1);
  pushq (pred_dumpaffix);
}

/*
   pred_realtostring
*/
static void act_realtostring (pos_node ps1, pos_node ps2)
{ value v1 = calc_affix_value (ps1, lower_side);
  if (v1 -> tag == real_value)
    { char buf[12];
      value v2;
      sprintf (buf, "%g", v1 -> u.rnum);
      v2 = new_string_value (buf);
      pushp (ps2);
      pushv (v2);
      pushq (propagate_predicate_value);
      callq ();
      pop (3);
      rfre_value (v2);
    };
  rfre_value (v1);
}

static void delayed_realtostring (pos_node *ps)
{ pos_node ps1 = ps[0];
  pos_node ps2 = ps[1];
  if (!crit_pos_has_value (ps1)) callq ();
  else
    { ps1 -> delayed = 0;
      ps2 -> delayed = 0;
      act_realtostring (ps1, ps2);
      ps2 -> delayed = 1;
      ps1 -> delayed = 1;
    };
}

void pred_realtostring ()
{ affix_node af1 = new_affix_node ("realtostring_af1");
  affix_node af2 = new_affix_node ("realtostring_af2");
  pushq (delayed_realtostring);
  pushq (make_node_delayed);
  pusha (af2);
  pushi (tag_single);
  pusha (af1);
  pushi (tag_single);
  pushi (2);
  pushi (0);
  pushi (mk_nodenr (eag_stddefs_modnr, 14));	/* realtostring nodenr */
  pushq (make_predicate_node);
  callq ();
  pop (10);
  rfre_value (af1 -> val);			/* ??? */
  free_affix_node (af1);
  rfre_value (af2 -> val);			/* ??? */
  free_affix_node (af2);
  pushq (pred_realtostring);
}

/*
   pred_stringtoreal
*/
static void act_stringtoreal (pos_node ps1, pos_node ps2)
{ value v1 = calc_affix_value (ps1, lower_side);
  if (v1 -> tag == string_value)
    { char *ptr = v1 -> u.str;
      if (isdigit (*ptr))
        { char *endptr;
	  real val = strtod (ptr, &endptr);
	  if (!(*endptr))
	    { value v2 = new_real_value (val);
	      pushp (ps2);
	      pushv (v2);
	      pushq (propagate_predicate_value);
	      callq ();
	      pop (3);
	      rfre_value (v2);
	    };
	};
    };
  rfre_value (v1);
}

static void delayed_stringtoreal (pos_node *ps)
{ pos_node ps1 = ps[0];
  pos_node ps2 = ps[1];
  if (!crit_pos_has_value (ps1)) callq ();
  else
    { ps1 -> delayed = 0;
      ps2 -> delayed = 0;
      act_stringtoreal (ps1, ps2);
      ps2 -> delayed = 1;
      ps1 -> delayed = 1;
    };
}

void pred_stringtoreal ()
{ affix_node af1 = new_affix_node ("stringtoreal_af1");
  affix_node af2 = new_affix_node ("stringtoreal_af2");
  pushq (delayed_stringtoreal);
  pushq (make_node_delayed);
  pusha (af2);
  pushi (tag_single);
  pusha (af1);
  pushi (tag_single);
  pushi (2);
  pushi (0);
  pushi (mk_nodenr (eag_stddefs_modnr, 15));	/* stringtoint nodenr */
  pushq (make_predicate_node);
  callq ();
  pop (10);
  rfre_value (af1 -> val);			/* ??? */
  free_affix_node (af1);
  rfre_value (af2 -> val);			/* ??? */
  free_affix_node (af2);
  pushq (pred_stringtoreal);
}

/*
   Module table of nodenrs
*/
string eag_stddefs_name_from_nodenr (int nodenr)
{ int lnodenr = lnodenr_from_nodenr (nodenr);
  switch (lnodenr)
    { case 0: return ("semipred_endofsentence");
      case 1: return ("semipred_column");
      case 2: return ("semipred_row");
      case 3: return ("pred_equal");
      case 4: return ("pred_notequal");
      case 5: return ("pred_minus");
      case 6: return ("pred_mul");
      case 7: return ("pred_div");
      case 8: return ("pred_power");
      case 9: return ("pred_inttostring");
      case 10: return ("pred_stringtoint");
      case 11: return ("pred_uppercase");
      case 12: return ("pred_notinreservedwordlist");
      case 13: return ("pred_dumpaffix");
      case 14: return ("pred_realtostring");
      case 15: return ("pred_stringtoreal");
      default: panic ("strange nodenr %d in stddefs\n", lnodenr);
    };
  return (NULL);
}

void init_eag_stddefs (int modnr)
{ eag_stddefs_modnr = modnr;
};
