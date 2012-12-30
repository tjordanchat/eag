/*
   File: alib.c
   Implements the predefines library 'alib'

   CVS ID: "$Id: alib.c,v 1.4 2008/06/11 14:58:30 marcs Exp $"
*/

/* general includes */
#include <stdio.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_memalloc.h>
#include <ebs_textstorage.h>
#include <ebs_value.h>

/* libeag includes */
#include <eag_ds.h>
#include <eag_buildtree.h>
#include <eag_propagate.h>
#include <eag_nodeinfo.h>

/* local includes */
#include "alib.h"

/* our module number */
static int alib_modnr = 2;

static char *field;
static int width;
static int height;

static void act_makeemptyfield (pos_node ps1, pos_node ps2)
	{ value v1 = calc_affix_value (ps1, lower_side);
	  value v2 = calc_affix_value (ps2, lower_side);
	  /* Insert code here */
	  if ((v1 -> tag == integer_value) && (v2 -> tag == integer_value))
	     { char *save_field = field;			/* Ugh */
	       int save_width = width;
	       int save_height = height;
	       int ix;
	       width = v1 -> u.inum;
	       height = v2 -> u.inum;
	       field = (char *) ckcalloc (width*height, sizeof (char));
	       for (ix = 0; ix < width * height; ix++) field[ix] = 0;
	       callq ();
	       ckfree (field);
	       height = save_height;
	       width = save_width;
	       field = save_field;
	     };
	  /* End of insertion */
	  rfre_value (v1);
	  rfre_value (v2);
	};

static void delayed_makeemptyfield (pos_node *ps)
	{ pos_node ps1 = ps[0];
	  pos_node ps2 = ps[1];
	  if (!crit_pos_has_value (ps1) || !crit_pos_has_value (ps2)) callq();
	  else
	   { ps1 -> delayed = 0;
	     ps2 -> delayed = 0;
	     act_makeemptyfield (ps1, ps2);
	     ps1 -> delayed = 1;
	     ps2 -> delayed = 1;
	   };
	};

void pred_makeemptyfield ()
	{ affix_node af1 = new_affix_node ("makeemptyfield_af1");
	  affix_node af2 = new_affix_node ("makeemptyfield_af2");
	  pushq (delayed_makeemptyfield);
	  pushq (make_node_delayed);
	  pusha (af2);
	  pushi (tag_single);
	  pusha (af1);
	  pushi (tag_single);
	  pushi (2);
	  pushi (0);
	  pushi (mk_nodenr (alib_modnr, 0));
	  pushq (make_predicate_node);
	  callq ();
	  pop (10);
	  rfre_value (af1 -> val);
	  free_affix_node (af1);
	  rfre_value (af2 -> val);
	  free_affix_node (af2);
	  pushq (pred_makeemptyfield);
	};

static void act_isempty (pos_node ps1, pos_node ps2)
	{ value v1 = calc_affix_value (ps1, lower_side);
	  value v2 = calc_affix_value (ps2, lower_side);
	  /* Insert code here */
	  if ((v1 -> tag == integer_value) && (v2 -> tag == integer_value))
	     { int x = v1 -> u.inum;
	       int y = v2 -> u.inum;
	       if ((0 <= x) && (0 <= y) && (x < width) && (y < height))
		  if (!field [y * width + x]) callq ();
	     };
	  /* End of insertion */
	  rfre_value (v1);
	  rfre_value (v2);
	};

static void delayed_isempty (pos_node *ps)
	{ pos_node ps1 = ps[0];
	  pos_node ps2 = ps[1];
	  if (!crit_pos_has_value (ps1) || !crit_pos_has_value (ps2))
	     callq();
	  else
	   { ps1 -> delayed = 0;
	     ps2 -> delayed = 0;
	     act_isempty (ps1, ps2);
	     ps1 -> delayed = 1;
	     ps2 -> delayed = 1;
	   };
	};

void pred_isempty ()
	{ affix_node af1 = new_affix_node ("isempty_af1");
	  affix_node af2 = new_affix_node ("isempty_af2");
	  pushq (delayed_isempty);
	  pushq (make_node_delayed);
	  pusha (af2);
	  pushi (tag_single);
	  pusha (af1);
	  pushi (tag_single);
	  pushi (2);
	  pushi (0);
	  pushi (mk_nodenr (alib_modnr, 1));
	  pushq (make_predicate_node);
	  callq ();
	  pop (10);
	  rfre_value (af1 -> val);
	  free_affix_node (af1);
	  rfre_value (af2 -> val);
	  free_affix_node (af2);
	  pushq (pred_isempty);
	};

static void act_take (pos_node ps1, pos_node ps2)
	{ value v1 = calc_affix_value (ps1, lower_side);
	  value v2 = calc_affix_value (ps2, lower_side);
	  /* Insert code here */
	  if ((v1 -> tag == integer_value) && (v2 -> tag == integer_value))
	     { int x = v1 -> u.inum;
	       int y = v2 -> u.inum;
	       if ((0 <= x) && (0 <= y) && (x < width) && (y < height))
		  { char savecell = field [y * width + x];
		    field [y * width + x] = 1;
		    callq ();
		    field [y * width + x] = savecell;
		  };
	     };
	  /* End of insertion */
	  rfre_value (v1);
	  rfre_value (v2);
	};

static void delayed_take (pos_node *ps)
	{ pos_node ps1 = ps[0];
	  pos_node ps2 = ps[1];
	  if (!crit_pos_has_value (ps1) || !crit_pos_has_value (ps2))
	     callq();
	  else
	   { ps1 -> delayed = 0;
	     ps2 -> delayed = 0;
	     act_take (ps1, ps2);
	     ps1 -> delayed = 1;
	     ps2 -> delayed = 1;
	   };
	};

void pred_take ()
	{ affix_node af1 = new_affix_node ("take_af1");
	  affix_node af2 = new_affix_node ("take_af2");
	  pushq (delayed_take);
	  pushq (make_node_delayed);
	  pusha (af2);
	  pushi (tag_single);
	  pusha (af1);
	  pushi (tag_single);
	  pushi (2);
	  pushi (0);
	  pushi (mk_nodenr (alib_modnr, 2));
	  pushq (make_predicate_node);
	  callq ();
	  pop (10);
	  rfre_value (af1 -> val);
	  free_affix_node (af1);
	  rfre_value (af2 -> val);
	  free_affix_node (af2);
	  pushq (pred_take);
	};

void semipred_showfield ()
	{ int i,j;
	  fprintf (stderr, "Field:\n");
	  for (i = 0; i < height; i++)
	     { for (j = 0; j < width; j++)
		  fputc ((field [i*width +j])?'X':' ', stderr);
	       fputc ('\n', stderr);
	     };
	  fputc ('\n', stderr);
	  pushi (0);
	  pushi (0);
	  pushi (mk_nodenr (alib_modnr, 3));
	  pushq (make_semipredicate_node);
	  callq ();
	  pop (4);
	  pushq (semipred_showfield);
	};

char *alib_name_from_nodenr (int nodenr)
	{ int lnr = lnodenr_from_nodenr (nodenr);
	  switch (lnr)
	     { case 0: return ("pred_makeemptyfield");
	       case 1: return ("pred_isempty");
	       case 2: return ("pred_take");
	       case 3: return ("semipred_showfield");
	       default: fprintf (stderr, "strange nodenr %d in alib\n", lnr);
	     };
	  exit (4);
	};

void init_alib (int modnr)
	{ alib_modnr = modnr;
	};
