/*
   File: edt_unparser.c
   Unparses the folded syntax tree delivered by 'cpmerge.c'
   according to the unparsing rules read by 'tuples.c'

   CVS ID: "$Id: edt_unparser.c,v 1.3 2004/12/25 22:40:52 marcs Exp $"
*/

/* general includes */
#include <stdio.h>
#include <string.h>

/* libeag includes */
#include <ebs_bst.h>
#include <ebs_cst.h>
#include <ebs_error.h>
#include <ebs_memalloc.h>
#include <ebs_value.h>

/* libeag includes */
#include <eag_ds.h>
#include <eag_nodeinfo.h>

/* local includes */
#include "edt_editor.h"
#include "edt_edit_ds.h"
#include "edt_cpmerge.h"
#include "edt_lrules.h"
#include "edt_unparser.h"

char *unparse_buffer;
static char *optr;

static void find_string_size (char *s, int *dx, int *dy)
	{ char *ptr;
	  int wd = 0;
	  int ht = 0;
	  int lwd = 0;
	  for (ptr = s; *ptr; ptr++)
	     if (*ptr == '\n')
		{ if (wd < lwd) wd = lwd;
		  ht++;
		  lwd = 0;
		}
	     else lwd++;
	  if (wd < lwd) wd = lwd;
	  *dx = wd;
	  *dy = ht;
	};

/* unparsing styles */
#define anydir 0
#define horizontal 1
#define vertical 2
#define force_horizontal 3

/*
   The following function is heuristic. If the horizontal rule for
   this treenode does not specify any layout, the user does not expect
   vertical layout anyway. If a nonterminal has only sons that
   are layout horizontally by this same heuristic it will also be
   layout horizontally.
*/
static void test_if_rule_should_be_horizontal (etree_node tree)
	{ tuple ptr;
	  int sonnr = 0;
	  int total = 0;
	  for (ptr = ruletable [tree -> nodenr] -> hrule;
	       ptr;
	       ptr = ptr -> next)
	     { if ((ptr -> hor_offset) || (ptr -> vert_offset)) return;
	       switch (ptr -> type)
		  { case type_terminal:
		       total += strlen (ptr -> text);
		       break;
		    case type_set:
		       { total += tree -> sons [sonnr] -> width;
		         sonnr++;
		       };
		       break;
		    case type_nonterminal:	/* ?????  */
		       { if (tree -> sons [sonnr] -> style != force_horizontal)
			    return;
		         total += tree -> sons [sonnr] -> width;
			 sonnr++;
		       };
		       break;
		    case type_forcednonterminal: return;
		    default: bad_tag (ptr -> type,
				      "test_if_rule_should_be_horizontal");
		  };
	     };
	  tree -> style = force_horizontal;
	  tree -> width = total;
	  tree -> height = 0;
	};

static void initial_estimate_tree_size (etree_node tree, int *dx, int *dy,
					 int ambi)
	{ tree -> width = 0;
	  tree -> height = 0;
	  tree -> style = horizontal;
	  switch (tree -> type)
	     { case leaf_node:
		  { find_string_size (tree -> name,
				      &tree -> width, &tree -> height);
		    tree -> style = force_horizontal;
		  };
		  break;
	       case normal_node:
		  { int ix;
		    int max_x = 0;
		    for (ix = 0; ix < tree -> nrsons; ix++)
		       { int ldx, ldy;
			 initial_estimate_tree_size (tree -> sons[ix],
						     &ldx, &ldy, ambi);
			 if (ldx > max_x) max_x = ldx;
		       };
		    tree -> width = max_x;
		    test_if_rule_should_be_horizontal (tree);
		  };
		  break;
	       case ambiguous_node:
		  /* actually doing too much work */
		  { int ix, ldx, ldy;
		    for (ix = 0; ix < tree -> nrsons; ix++)
		       initial_estimate_tree_size (tree -> sons[ix],
						   &ldx, &ldy, 1);
		    /* then copy result of first parse tree */
		    /* more research is needed */
		    tree -> width = tree -> sons[0] -> width;
		    tree -> height = tree -> sons[0] -> height;
		    tree -> style = tree -> sons[0] -> style;
		  };
		  break;
	       case error_node:
		  { find_string_size (tree -> name,
				      &tree -> width, &tree -> height);
		    if (tree -> height) tree -> style = vertical;
		    else tree -> style = force_horizontal;
		  };
		  break;
	       case typed_placeholder_node:
		  { /* Here we see a hack */
		    tree -> width = strlen (tree -> name + 5) +
				    typed_open_symbol_len +
				    typed_close_symbol_len;
		    tree -> height = 0;
		    tree -> style = force_horizontal;
		  };
		  break;
	       case untyped_placeholder_node:
		  { if (ambi) tree -> width = untyped_symbol_len;
		    else /* Mark the side effect */
		       { tree -> width = strlen (tree -> name + 5) +
					 typed_open_symbol_len +
					 typed_close_symbol_len;
			 tree -> type = typed_placeholder_node;
		       };
		    tree -> height = 0;
		    tree -> style = force_horizontal;
		  };
		  break;
	       case predicate_node:
	       case semipredicate_node:
		  { tree -> width = 0;
		    tree -> height = 0;
		    tree -> style = force_horizontal;
		  };
		  break;
	       default: bad_tag (tree -> type, "initial_estimate_tree_size");
	     };
	  *dx = tree -> width;
	  *dy = tree -> height;
	};

static int estimate_tree_size (etree_node tree, int *dx, int *dy,
				int ambi, int dir, int leftmargin);
static int estimate_normalnode_in_dir (etree_node tree, int ambi, int dir,
					int leftmargin)
	{ tuple current;
	  int i = 0;
	  int x = 0;
	  int y = 0;
	  int lasty = 0;
	  int max_x = 0;
	  int *x_pos;
	  int is_hor = dir == horizontal;

	  /* generated editors always have module nr 0 */
	  if (is_hor) current = ruletable [tree -> nodenr] -> hrule;
	  else current = ruletable [tree -> nodenr] -> vrule;
	  x_pos = (int *) ckcalloc (max_nr_of_tuples, sizeof (int));
	  x_pos[0] = 0;
	  
	  while (current)
	     { if (current -> relates < i)
		  { if (is_hor & (i > 0))
		      { ckfree (x_pos);
			return (0);
		      };
		    x_pos [i] = x_pos[current -> relates] +
				current -> hor_offset;
		    y = lasty + current -> vert_offset;
		  }
	       else if (is_hor) x_pos [i] = x + current -> hor_offset;
	       else if (current -> vert_offset)
		  { x_pos [i] = current -> hor_offset;
		    y = lasty + current -> vert_offset;
		  }
	       else x_pos [i] = x + current -> hor_offset;
	       x = x_pos [i];
	       switch (current -> type)
		  { case type_terminal:
		       /* what if we allow \n in terminals */
		       x += strlen (current -> text);
		       break;
		    case type_forcednonterminal:
		       { int lx, ly;
			 etree_node son = tree -> sons[current -> sonnr -1];
			 if (is_hor)
			    { ckfree (x_pos);
			      return (0);
			    };
			 (void) estimate_tree_size (son, &lx, &ly, ambi,
				vertical, leftmargin + x_pos[i]);
			 y += ly;
			 x += lx;
		       };
		       break;
		    case type_nonterminal:
		    case type_set:
		       { int lx, ly;
			 int ldir = (is_hor)?horizontal:anydir;
			 etree_node son = tree -> sons[current -> sonnr -1];
			 if (!estimate_tree_size (son, &lx, &ly, ambi,
				ldir, leftmargin + x_pos[i]))
			    { ckfree (x_pos);
			      return (0);
			    };
			 if (is_hor && (ly > 0))
			    { ckfree (x_pos);
			      return (0);
			    };
			 y += ly;
			 x += lx;
		       };
		       break;
		    default:
		       bad_tag (current -> type, "estimate_normalnode_in_dir");
	          };
	       i++;
	       lasty = y;
	       if (max_x < x) max_x = x;
	       if (is_hor && (screenwidth <= max_x + leftmargin))
		  { ckfree (x_pos);
		    return (0);
		  };
	       current = current -> next;
	     };
	  tree -> width = max_x;
	  tree -> height = y;
	  ckfree (x_pos);
	  return (1);
	};

static int estimate_normal_node (etree_node tree, int ambi, int dir,
				  int leftmargin)
	{ switch (dir)
	     { case horizontal:
	          if (estimate_normalnode_in_dir (tree, ambi,
						  horizontal, leftmargin))
		     { tree -> style = horizontal;
		       return (1);
		     }
		  else return (0);
	       case anydir:
		  if (estimate_normalnode_in_dir (tree, ambi,
						  horizontal, leftmargin))
		     { tree -> style = horizontal;
		       return (1);
		     };
	       case vertical:
		  { (void) estimate_normalnode_in_dir (tree, ambi,
						       vertical, leftmargin);
		    tree -> style = vertical;
		  };
	     };
	  return (1);
	};

static int estimate_tree_size (etree_node tree, int *dx, int *dy,
				int ambi, int dir, int leftmargin)
	{ if (tree -> style != force_horizontal)
	     switch (tree -> type)
	     { case normal_node:
		  if (!estimate_normal_node (tree, ambi, dir, leftmargin))
		     return (0);
		  break;
	       case ambiguous_node:
		  /* actually doing too much work */
		  { int i, ldx, ldy;
		    for (i = 0; i < tree -> nrsons; i++)
		       if (!estimate_tree_size (tree -> sons[i],
				&ldx, &ldy, 1, dir, leftmargin)) return (0);
		    tree -> width = tree -> sons[0] -> width;
		    tree -> height = tree -> sons[0] -> height;
		  };
		  break;
	       case error_node:
		  break;
	       default: bad_tag (tree -> type, "estimate_tree_size");
	     };
	  *dx = tree -> width;
	  *dy = tree -> height;
	  if (dir == horizontal)
	     return ((*dy == 0) && (leftmargin + *dx < screenwidth));
	  return (1);
	};

static void print_string (string str)
	{ char *ptr;
	  for (ptr = str; *ptr; ptr++, optr++) *optr = *ptr;
	};

static void print_spaces (int nr)
	{ int i;
	  for (i=0; i < nr; i++) *optr++ = ' ';
	};

static void print_newlines (int nr)
	{ int i;
	  for (i=0; i < nr; i++) *optr++ = '\n';
	};

static void print_tree (etree_node tree, int line, int col);
static void print_normal_node (etree_node tree, int line, int col)
	{ tuple current;
	  int i = 0;
	  int y = line;
	  int x = col;
	  int lasty = line;
	  int *x_pos;
	  int is_hor = (tree -> style == horizontal) ||
		       (tree -> style == force_horizontal);
	  int is_vert = (tree -> style == vertical);
	  if (is_hor) current = ruletable [tree -> nodenr] -> hrule;
	  else if (is_vert) current = ruletable [tree -> nodenr] -> vrule;
	  else
	     { error ("printing '%s' fails", tree -> name);
	       current = ruletable [tree -> nodenr] -> vrule;
	     };
	  x_pos = (int *) ckcalloc (max_nr_of_tuples, sizeof (int));
	  x_pos[0] = col;

	  while (current)
	     { if (current -> relates < i)
		  { x_pos [i] = x_pos[current -> relates] +
				current -> hor_offset;
		    y = lasty + current -> vert_offset;
		    print_newlines (current -> vert_offset);
		    print_spaces (x_pos [i]);
		  }
	       else if (is_hor)
		  { x_pos [i] = x + current -> hor_offset;
		    print_spaces (current -> hor_offset);
		  }
	       else if (current -> vert_offset)
		  { x_pos [i] = current -> hor_offset;
		    y = lasty + current -> vert_offset;
		    print_newlines (current -> vert_offset);
		    print_spaces (x_pos[i]);
		  }
	       else
		  { x_pos [i] = x + current -> hor_offset;
		    print_spaces (current -> hor_offset);
		  };
	       x = x_pos [i];
	       switch (current -> type)
		  { case type_terminal:
		       /* what if we allow \n in terminals */
		       { print_string (current -> text);
		         x += strlen (current -> text);
		       };
		       break;
		    case type_forcednonterminal:
		    case type_nonterminal:
		    case type_set:
		       { etree_node son = tree -> sons[current -> sonnr -1];
			 int oldy = y;
			 y += son -> height;
			 x += son -> width;
			 print_tree (son, oldy, x_pos[i]);
		       };
		       break;
		    default: bad_tag (current -> type, "print_normal_node");
	          };
	       i++;
	       lasty = y;
	       current = current -> next;
	     };
	  ckfree (x_pos);
	};

static void print_tree (etree_node tree, int line, int col)
	{ tree -> y = line;
	  tree -> x = col;
	  switch (tree -> type)
	     { case leaf_node:
		  print_string (tree -> name);
		  break;
	       case normal_node:
		  print_normal_node (tree, line, col);
		  break;
	       case ambiguous_node:
		  /* some research is still necessary at this point */
		  print_tree (tree -> sons[0], line, col);
		  break;
	       case error_node:
		  print_string (tree -> name);
		  break;
	       case typed_placeholder_node:
		  { /* Yep, this is the same hack */
		    print_string (typed_open_symbol);
		    print_string (tree -> name + 5);	/* rule_%s */
		    print_string (typed_close_symbol);
		  };
		  break;
	       case untyped_placeholder_node:
		  print_string (untyped_symbol);
		  break;
	       case predicate_node:
	       case semipredicate_node:
		  break;
	       default: bad_tag (tree -> type, "print_tree");
	     };
	};

int unparsed_width;
int unparsed_height;
static void unparse_tree (etree_node tree)
	{ int dx, dy;
	  optr = unparse_buffer;
	  initial_estimate_tree_size (tree, &dx, &dy, 0);
	  if (!estimate_tree_size (tree, &dx, &dy, 0, anydir, 0))
	     panic ("syntax tree could not be unparsed");
	  unparsed_width = dx;
	  unparsed_height = dy;
	  print_tree (tree, 0, 0);
	  *optr = '\0';
	  if (debug)
	     { dump_etree (1);
	       wlog ("Unparse resulted in:\n%s", unparse_buffer);
	     };
	};

void unparse ()
	{ unparse_tree (the_root);
	};

void init_unparser (int ubuffer_size)
	{ unparse_buffer = (char *) ckmalloc (ubuffer_size);
	};
