/*
   File: edt_focus.c
   Keeps track of the focus

   CVS ID: "$Id: edt_focus.c,v 1.3 2004/12/25 22:40:51 marcs Exp $"
*/

/* general includes */
#include <stdio.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_memalloc.h>
#include <ebs_value.h>

/* libeag includes */
#include <eag_ds.h>
#include <eag_nodeinfo.h>

/* local includes */
#include "edt_unparser.h"
#include "edt_cpmerge.h"
#include "edt_focus.h"

focus root_focus;
focus current_focus;
static focus free_focus_list;

/*
   allocate a new focus
*/
static focus new_focus (int begin_x, int begin_y, int end_x, int end_y,
			 etree_node node, focus parent)
	{ focus new;
	  if (free_focus_list == focus_nil)
	     new = (focus) ckmalloc (sizeof (struct focus_rec));
	  else
	     { new = free_focus_list;
	       free_focus_list = new -> parent;
	     };
	  new -> begin_x = begin_x;
	  new -> begin_y = begin_y;
	  new -> end_x = end_x;
	  new -> end_y = end_y;
	  new -> node = node;
	  new -> parent = parent;
	  return (new);
	};

static void free_complete_focus ()
	{ if (!root_focus) return;
	  root_focus -> parent = free_focus_list;
	  free_focus_list = current_focus;
	  current_focus = focus_nil;
	  root_focus = focus_nil;
	};

/*
   free all foci from current focus to the top
   set current focus to the root focus
*/
static void free_current_focus ()
	{ focus ptr;
	  if (!root_focus) return;
	  if (current_focus == root_focus) return;
	  for (ptr = current_focus;
	       ptr -> parent != root_focus;
	       ptr = ptr -> parent);
	  ptr -> parent = free_focus_list;
	  free_focus_list = current_focus;
	  current_focus = root_focus;
	};

static int fits_in_range (int x, int y, int bx, int by, int ex, int ey)
	{ if (y < by) return (0);
	  if ((y == by) && (x < bx)) return (0);
	  if (ey < y) return (0);
	  if ((ey == y) && (ex <= x)) return (0); /* ?? < */
	  return (1);
	};

static int fits_in_focus (focus f, int x, int y)
	{ return (fits_in_range (x, y, f -> begin_x, f -> begin_y, f -> end_x, f -> end_y));
	};

void set_focus_from_pos (int x, int y)
	{ if (!fits_in_focus (root_focus, x, y)) return;
	  free_current_focus ();
	  while (current_focus -> node -> type == normal_node)
	     { etree_node node = current_focus -> node;
	       int ix;
	       int found = 0;
	       for (ix = 0; ix < node -> nrsons; ix++)
		  { int bx = node -> sons[ix] -> x;
		    int by = node -> sons[ix] -> y;
		    int ex = bx + node -> sons[ix] -> width;
		    int ey = by + node -> sons[ix] -> height;
		    if (fits_in_range (x, y, bx, by, ex, ey))
		       { current_focus = new_focus (bx, by, ex, ey,
						    node -> sons[ix], current_focus);
			 found = 1;
		       };
	          };
	       if (!found) return;
	     };
	};

void set_focus_to_father ()
	{ focus ptr;
	  if (current_focus == root_focus) return;
	  ptr = current_focus;
	  current_focus = current_focus -> parent;
	  ptr -> parent = free_focus_list;
	  free_focus_list = ptr;
	};

void reset_root_focus ()
	{ free_complete_focus ();
	  root_focus = new_focus (0, 0, unparsed_width, unparsed_height, the_root, focus_nil);
	  current_focus = root_focus;
	};

void init_focus ()
	{ root_focus = focus_nil;
	};
