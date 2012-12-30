/*
   File: eag_propagate.c
   Defines the affix propagation mechanism

   CVS ID: "$Id: eag_propagate.c,v 1.5 2008/05/28 15:28:33 marcs Exp $"
*/

/* global includes */
#include <stdio.h>
#include <string.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_error.h>
#include <ebs_textstorage.h>
#include <ebs_value.h>

/* local includes */
#include "eag_ds.h"
#include "eag_trace.h"
#include "eag_textparsing.h"
#include "eag_propagate.h"

value calc_affix_value (pos_node pos, int side)
	{ switch (pos -> sides[side].tag)
	     { case tag_single:
		  return (rdup_value (pos -> sides[side].a.affx -> val));
	       case tag_compos:
		  { int nr = pos -> sides[side].a.co.nr;
		    affix_node *affs = pos -> sides[side].a.co.affs;
		    value_list vl = new_value_list (nr);
		    int ix;
		    for (ix = 0; ix < nr; ix++)
		       app_value_list (vl, rdup_value (affs[ix] -> val));
		    return (new_tuple_value (vl));
		  };
	       case tag_concat:
		  { int nr = pos -> sides[side].a.co.nr;
		    affix_node *affs = pos -> sides[side].a.co.affs;
		    int tag = affs[0] -> val -> tag;
		    int ix;

		    for (ix = 1; ix < nr; ix++)
		       if (affs[ix] -> val -> tag != tag)
			  return (new_undefined_value ()); /* undefined value */
		    switch (tag)
		       { case string_value:
			    { register char *dptr = strstore;
			      for (ix = 0; ix < nr; ix++)
				 { register char *sptr =
					affs[ix] -> val -> u.str;
				   while (*sptr) *dptr++ = *sptr++;
				 };
			      *dptr = '\0';
			      return (new_string_value (strstore));
			    };
			 case integer_value:
			    { int val = 0;
			      for (ix = 0; ix < nr; ix++)
				 val += affs[ix] -> val -> u.inum;
			      return (new_integer_value (val));
			    };
			 case real_value:
			    { real val = 0.0;
			      for (ix = 0; ix < nr; ix++)
				 val += affs[ix] -> val -> u.rnum;
			      return (new_real_value (val));
			    };
			 default: break;
		       };
		  }; break;
	       case tag_union: break;		/* needs work */
	       default: bad_tag (pos -> sides[side].tag, "calc_affix_value");
	     };
	  return (new_undefined_value ());		/* undefined value */
	};

/*
   Move_affix_value is called when an affix receives a defined value
   by a call to assign_value. It decreases the sill of every position
   it is connected to and propagates the value to all connected
   positions from which the value did not originate. Calling convention:
   pushp (pos);
   pusha (affx);
   pushq (move_affix_value);
*/
static void move_affix_value ()
	{ affix_node affx = popa ();
	  pos_node orig_pos = popp ();
	  link_node ptr;

	  /* save qptr since we do not know how many items will be pushed */
	  cont *lqptr = qptr;
	  for (ptr = affx -> links; ptr != link_node_nil; ptr = ptr -> next)
	     { pos_node pos = ptr -> pos;
	       int side = ptr -> side;
	       pos -> sides[side].sill -= 1;      
	       if (pos != orig_pos)
		  { pushp (pos);
		    pushq (propagate_affix_value);
		  };
	     };
	  /*
	      check if the original position was a delayed one 
	      the others will be caught by affix propagation
	  */
	  if (orig_pos -> delayed) orig_pos -> dfunc (orig_pos -> args);
	  else callq ();
	  for (ptr = affx -> links; ptr != link_node_nil; ptr = ptr -> next)
	     { pos_node pos = ptr -> pos;
	       int side = ptr -> side;
	       pos -> sides[side].sill += 1;      
	     };
	  qptr = lqptr;
	  pushp (orig_pos);
	  pusha (affx);
	  pushq (move_affix_value);
	};

static int connected_to_node_with_undefined_value (link_node lk)
	{ pos_node pos = lk -> pos;
	  int side = lk -> side;
	  int otherside = (side == lower_side)?upper_side:lower_side;
	  switch (pos -> sides [otherside].tag)
	     { case tag_single:
		  { affix_node afx = pos -> sides [otherside].a.affx;
		    return ((afx -> val != value_nil) &&
			    (afx -> val -> tag == undefined_value));
		  };
	       case tag_concat:
	       case tag_compos:
		  { affix_node *affxs = pos -> sides [otherside].a.co.affs;
		    int nr = pos -> sides [otherside].a.co.nr;
		    int ix;
		    for (ix = 0; ix < nr; ix++)
		       { affix_node afx = affxs[ix];
			 if ((afx -> val != value_nil) &&
			     (afx -> val -> tag == undefined_value))
			    return (1);
		       };
		  };
	     };
	  return (0);
	};

/*
   Move_over_undefined_affix_value is called when an affix that already
   has an undefined value receives a defined value by a call to assign_value.
   It then propagates this value to every connected position that still
   has an undefined value. Consequently this will never be the position
   from which the defined value originated. We must still check if this
   position is a delayed one. Calling convention:
   pushp (pos);
   pusha (affx);
   pushq (move_over_undefined_affix_value);
*/
static void move_over_undefined_affix_value ()
	{ affix_node affx = popa ();
	  pos_node orig_pos = popp ();
	  link_node ptr;
	  /* save qptr since we do not know how many items will be pushed */
	  cont *lqptr = qptr;
	  for (ptr = affx -> links; ptr != link_node_nil; ptr = ptr -> next)
	     if (connected_to_node_with_undefined_value (ptr))
	        { pushp (ptr -> pos);
		  pushq (propagate_affix_value);
		};
	  /* check for a delayed position */
	  if (orig_pos -> delayed) orig_pos -> dfunc (orig_pos -> args);
	  else callq ();
	  qptr = lqptr;
	  pushp (orig_pos);
	  pusha (affx);
	  pushq (move_over_undefined_affix_value);
	};

/*
   Assigns a value to an affix, calling convention:
   pusha (affx)
   pushp (pos)	for delayed evaluation
   pushv (val)
   pushq (assign_value);
*/
static void assign_value ()
	{ value val = popv ();
	  pos_node pos = popp ();
	  affix_node affx = popa ();
	  if (affx -> val == value_nil)		/* no value assigned yet */
	     { /* assign value and propagate across links */
	       affx -> val = rdup_value (val);
	       pushp (pos);
	       pusha (affx);
	       pushq (move_affix_value);
	       if (affx -> mfunc)
		  { /* check metadefinition */
		    pushv (val);
		    pushq (affx -> mfunc);
		    callq ();
		    pop (2);
		  }
	       else callq ();
	       pop (3);
	       rfre_value (affx -> val);
	       affx -> val = value_nil;
	     }
	  else if (affx -> val -> tag == undefined_value)
	     { /*
		  replace the undefined value by the given value and
		  propagate across links towards those positions that
		  evaluate into an undefined value
	       */
	       value lval = affx -> val;
	       affx -> val = rdup_value (val);
	       pushp (pos);
	       pusha (affx);
	       pushq (move_over_undefined_affix_value);
	       if (affx -> mfunc)
		  { /* check metadefinition */
		    pushv (val);
		    pushq (affx -> mfunc);
		    callq ();
		    pop (2);
		  }
	       else callq ();
	       pop (3);
	       rfre_value (affx -> val);
	       affx -> val = lval;
	     }
	  else
	     { /* try consistent substitution */
	       if (equal_value (val, affx -> val)) callq ();
	     };
	  pusha (affx);
	  pushp (pos);
	  pushv (val);
	  pushq (assign_value);
	}

/*
   assigns an undefined value to an affix and propagates it, calling conv:
   pusha (affx)
   pushp (pos)	for delayed evaluation
   pushv (val)	? is die echt nodig ?
   pushq (assign_value);
*/
static void assign_undef_value ()
	{ value val = popv ();
	  pos_node pos = popp ();
	  affix_node affx = popa ();
	  if (affx -> val == value_nil)		/* no value assigned yet */
	     { /* assign value and propagate across links */
	       affx -> val = rdup_value (val);
	       pushp (pos);
	       pusha (affx);
	       pushq (move_affix_value);
	       /* undefined against metadefinition always succeeds */	
	       callq ();
	       pop (3);
	       rfre_value (affx -> val);
	       affx -> val = value_nil;
	     }
	  else callq ();
	     /*
		if (affx -> val -> tag == undefined_value)
		then undefined meets undefined
		else undefined meets defined
	     */
	  pusha (affx);
	  pushp (pos);
	  pushv (val);
	  pushq (assign_undef_value);
	};

static void assign_single_value (value val, pos_node pos, affix_node aff)
	{ pusha (aff);
	  pushp (pos);
	  pushv (val);
	  pushq (assign_value);
	  callq ();
	  pop (4);
	};

static void assign_undef_single_value (value val, pos_node pos, affix_node aff)
	{ pusha (aff);
	  pushp (pos);
	  pushv (val);
	  pushq (assign_undef_value);
	  callq ();
	  pop (4);
	};

static void assign_compos_value (value val, pos_node pos, int nr, affix_node *affs)
	{ int ix;
	  cont *lqptr;
	  if (val -> tag != tuple_value) return;
	  if (val -> u.tuple -> size != nr) return;	/* Mismatch in number */
	  lqptr = qptr;
	  for (ix = 0; ix < nr; ix++)
	     { pusha (affs[ix]);
	       pushp (pos);
	       pushv (val -> u.tuple -> array[ix]);
	       pushq (assign_value);
	     };
	  callq ();
	  qptr = lqptr;
	};

static void assign_concat_string_value (string str, pos_node pos,
					int nr, affix_node *affs, int from)
	{ value sval;
	  int i;
	  if (from == nr - 1)
	     { sval = new_string_value (str);
	       pusha (affs[from]);
	       pushp (pos);
	       pushv (sval);
	       pushq (assign_value);
	       callq ();
	       pop (4);
	       rfre_value (sval);
	     }
	  else
	     for (i=0; i <= strlen (str); i++)
	        { string tail;
		  strncpy (strstore, str, i);
		  strstore[i] = '\0';
		  tail = addto_names (str + i);		/* ??? */
		  sval = new_string_value (strstore);
		  pusha (affs[from]);
		  pushp (pos);
		  pushv (sval);
		  pushq (assign_value);
		  assign_concat_string_value (tail, pos, nr, affs, from + 1);
		  pop (4);
		  rfre_value (sval);
		};
	};

static void assign_concat_int_value (int number, pos_node pos, int nr, affix_node *affs, int from)
	{ value ival;
	  int i;
	  if (number < 0) return;	/* split only naturals */
	  if (from == nr - 1)
	     { ival = new_integer_value (number);
	       pusha (affs[from]);
	       pushp (pos);
	       pushv (ival);
	       pushq (assign_value);
	       callq ();
	       pop (4);
	       rfre_value (ival);
	     }
	  else
	     for (i=0; i <= number; i++)
		{ ival = new_integer_value (i);
		  pusha (affs[from]);
		  pushp (pos);
		  pushv (ival);
		  pushq (assign_value);
		  assign_concat_int_value (number - i, pos, nr, affs, from + 1);
		  pop (4);
		  rfre_value (ival);
		};
	};

static void assign_concat_value (value val, pos_node pos, int nr, affix_node *affs)
	{ switch (val -> tag)
	     { case string_value:
		  assign_concat_string_value (val -> u.str, pos, nr, affs, 0);
		  break;
	       case integer_value:
		  assign_concat_int_value (val -> u.inum, pos, nr, affs, 0);
		  break;
	       default: break;
	     };
	};

static void assign_undef_values (value val, pos_node pos, int nr, affix_node *affs)
	{ int ix;
	  cont *lqptr = qptr;
	  for (ix = 0; ix < nr; ix++)
	     { pusha (affs[ix]);
	       pushp (pos);
	       pushv (val);
	       pushq (assign_undef_value);
	     };
	  callq ();
	  qptr = lqptr;
	};

void unparse_affix_value (value val, pos_node pos, int side)
	{ if (val -> tag == undefined_value)
	     { switch (pos -> sides[side].tag)
		 { case tag_single:
		      assign_undef_single_value (val, pos,
						 pos -> sides[side].a.affx);
		      break;
		   case tag_compos:
		   case tag_concat:
		      assign_undef_values (val, pos,
					   pos -> sides[side].a.co.nr,
					   pos -> sides[side].a.co.affs);
		      break;
		   default:
		      bad_tag (pos -> sides[side].tag, "unparse_affix_value");
		 };
	     }
	  else 
	     { switch (pos -> sides[side].tag)
	         { case tag_single:
		      assign_single_value (val, pos, pos -> sides[side].a.affx);
		      break;
	           case tag_compos:
		      assign_compos_value (val, pos,
					   pos -> sides[side].a.co.nr,
					   pos -> sides[side].a.co.affs);
		      break;
	           case tag_concat:
		      assign_concat_value (val, pos,
					   pos -> sides[side].a.co.nr,
					   pos -> sides[side].a.co.affs);
		      break;
		   default:
		      bad_tag (pos -> sides[side].tag, "unparse_affix_value");
	         };
	     };
	};

/*
   Restrict affix value
   Calling convention
   pushp (pos);
   pusha (affx);
   pushv (meet_val);
   pushq (restrict_affix_value);
*/
static void restrict_affix_pos ();
static void restrict_affix_value ()
	{ value mval = popv ();
	  affix_node affx = popa ();
	  pos_node orig_pos = popp ();
	  value new_val;
	  if (equal_value (mval, affx -> val)) callq ();
	  else if (meet_lattice_values (mval, affx -> val, &new_val))
	     { cont *lqptr = qptr;
	       link_node ptr;
	       value save_val = affx -> val;
	       affx -> val = new_val;
	       for (ptr = affx -> links; ptr != link_node_nil;ptr = ptr -> next)
		  { pos_node pos = ptr -> pos;
		    int side = ptr -> side;
		    int other_side = (side == lower_side)?upper_side:lower_side;
		    if (pos != orig_pos)
		       { pushp (pos);
			 pushi (other_side);
			 pushv (new_val);
			 pushq (restrict_affix_pos);
		       };
		  };
	       /*
		  check if the original position was a delayed one 
		  the others will be caught by affix propagation
	       */
	       if (orig_pos -> delayed) orig_pos -> dfunc (orig_pos -> args);
	       else callq ();
	       qptr = lqptr;
	       affx -> val = save_val;
	       rfre_value (new_val);
	     };
	  pushp (orig_pos);
	  pusha (affx);
	  pushv (mval);
	  pushq (restrict_affix_value);
	};

/*
   Restrict affix value on indicated side of position
   Calling convention
   pushp (pos);
   pushi (side);
   pushv (meet_val);
   pushq (restrict_affix_pos);
*/
static void restrict_affix_pos ()
	{ value mval = popv ();
	  int side = popi ();
	  pos_node pos = popp ();
	  if (tracing) trace_restrict_pos (pos, mval);
	  switch (pos -> sides[side].tag)
	     { case tag_undefined:	/* not yet filled in */
		  callq ();
		  break;
	       case tag_single:
		  { pushp (pos);	/* to know where we came from */
		    pusha (pos -> sides[side].a.affx);
		    pushv (mval);
		    pushq (restrict_affix_value);
		    callq ();
		    pop (4);
		  }; break;
	       case tag_compos:
	       case tag_concat:
	       case tag_union: internal_error ("restrict_affix_pos");
	       default: bad_tag (pos -> sides[side].tag, "restrict_affix_pos");
	     };
	  pushp (pos);
	  pushi (side);
	  pushv (mval);
	  pushq (restrict_affix_pos);
	};

/*
   Restrict pos values on both sides
*/
static void restrict_pos_values (pos_node pos, value lower_val, value upper_val, value meet_val)
	{ cont *lqptr = qptr;
	  if (!equal_value (lower_val, meet_val))
	     { pushp (pos);
	       pushi (lower_side);
	       pushv (meet_val);
	       pushq (restrict_affix_pos);
	     };
	  if (!equal_value (upper_val, meet_val))
	     { pushp (pos);
	       pushi (upper_side);
	       pushv (meet_val);
	       pushq (restrict_affix_pos);
	     };
	  callq ();
	  qptr = lqptr;
	};

/*
   Propagate affix value
   The value corresponding to an affix position is propagated
   when its sill drops to zero.
   Calling convention:
	pushp (pos);
	pushq (propagate_affix_value);
	callq ();
	pop (2);
*/
void propagate_affix_value ()
	{ pos_node pos = popp ();
	  if (tracing) trace_pos (pos);
	  if ((pos -> sides[lower_side].tag != tag_undefined) &&
	      (pos -> sides[upper_side].tag != tag_undefined))
	     { value upper_val = value_nil;
	       value lower_val = value_nil;
	       value meet_val;
	       if (pos -> sides[upper_side].sill == 0)
		  upper_val = calc_affix_value (pos, upper_side);
	       if (pos -> sides[lower_side].sill == 0)
		  lower_val = calc_affix_value (pos, lower_side);
	       if ((lower_val == value_nil) && (upper_val == value_nil))
		  callq ();
	       else if (upper_val == value_nil)
		  unparse_affix_value (lower_val, pos, upper_side);
	       else if (lower_val == value_nil)
		  unparse_affix_value (upper_val, pos, lower_side);
	       else if ((lower_val -> tag != undefined_value) &&
			(upper_val -> tag == undefined_value))
		  unparse_affix_value (lower_val, pos, upper_side);
	       else if ((lower_val -> tag == undefined_value) &&
			(upper_val -> tag != undefined_value))
		  unparse_affix_value (upper_val, pos, lower_side);
	       else if (equal_value (lower_val, upper_val))
		  callq ();
	       else if (meet_lattice_values (lower_val, upper_val, &meet_val))
		  { restrict_pos_values (pos, lower_val, upper_val, meet_val);
		    rfre_value (meet_val);
		  };
	       rfre_value (upper_val);
	       rfre_value (lower_val);
	     }
	  else callq();
	  pushp (pos);
	  pushq (propagate_affix_value);
	};

/*
   Propagate values calculated by standard predicates.
   Calling convention:
	pushp (pos);
	pushv (value);
	pushq (propagate_predicate_value);
	callq ();
	pop (3);
*/
void propagate_predicate_value ()
	{ value val = popv ();
	  pos_node pos = popp ();
	  affix_node affx = pos -> sides[lower_side].a.affx;
	  if (affx -> val == value_nil)
	     { affx -> val = rdup_value (val);
	       pos -> sides[lower_side].sill -= 1;
	       pushp (pos);
	       pushq (propagate_affix_value);
	       callq ();
	       pop (2);
	       pos -> sides[lower_side].sill += 1;
	       rfre_value (affx -> val);
	       affx -> val = value_nil;
	     }
	  else if (equal_value (val, affx -> val)) callq ();
	  pushp (pos);
	  pushv (val);
	  pushq (propagate_predicate_value);
	};

void make_node_delayed ()
	{ int ix;
	  void (*p)() = popq ();
	  tree_node top = top_tree_node ();
	  pos_node *ps = top -> affs;
	  for (ix = 0; ix < top -> nraffs; ix++)
	     { ps[ix] -> delayed = 1;
	       ps[ix] -> args = ps;
	       ps[ix] -> dfunc = p;
	     };
	  p (ps);
	  for (ix = 0; ix < top -> nraffs; ix++)
	     { ps[ix] -> delayed = 0;
	       ps[ix] -> args = pos_array_nil;
	       ps[ix] -> dfunc = NULL;
	     };
	  pushq (p);
	  pushq (make_node_delayed);
	};
