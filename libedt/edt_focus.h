/*
   File: edt_focus.h
   Keeps track of the focus

   CVS ID: "$Id: edt_focus.h,v 1.3 2004/12/25 22:40:51 marcs Exp $"
*/
#ifndef IncEdtFocus
#define IncEdtFocus

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_value.h>

/* libeag includes */
#include <eag_ds.h>

/* local includes */
#include "edt_edit_ds.h"

typedef struct focus_rec
	{ int begin_x;
	  int begin_y;
	  int end_x;
	  int end_y;
	  etree_node node;
	  struct focus_rec *parent;
	} *focus;
#define focus_nil ((focus) 0)
extern focus root_focus;
extern focus current_focus;

/* Routines to change the focus */
void set_focus_from_pos (int x, int y);
void set_focus_to_father ();
void reset_root_focus ();
void init_focus ();

#endif /* IncEdtFocus */
