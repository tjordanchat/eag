/*
   File: ebs_textstorage.c
   Stores text fragments in a balanced binary tree

   CVS ID: "$Id: ebs_textstorage.c,v 1.5 2008/05/28 15:01:03 marcs Exp $"
*/

/* global includes */
#include <stdio.h>
#include <string.h>

/* local includes */
#include "ebs_bst.h"
#include "ebs_error.h"
#include "ebs_memalloc.h"
#include "ebs_textstorage.h"

typedef struct text_rec
{ struct text_rec *left;
  struct text_rec *right;
  signed char balfac;
  char word[1];
} *text_tree;
#define text_tree_nil ((text_tree) NULL)

static text_tree new_leaf (char *t)
{ int len = strlen (t);
  int needed = 2 * sizeof (struct text_rec *) + sizeof (signed char) + len + 1;
  text_tree nt;
  nt = (text_tree) ckmalloc (needed);
  nt -> left = text_tree_nil;
  nt -> right = text_tree_nil;
  nt -> balfac = 0;
  strcpy (nt -> word, t);
  return (nt);
};

static text_tree root;
char *strstore;
string addto_names (char *t)
{ text_tree *fixation;
  text_tree *insert_ptr;
  text_tree last_unbal;
  text_tree below_unbal;
  text_tree twobelow_unbal;
  text_tree ptr, nt;

  /* special case for initial tree */
  if (root == text_tree_nil)
    { root = new_leaf (t);
      return (root -> word);
    };
 
  /* search in tree */
  insert_ptr = &root;
  fixation = &root;
  last_unbal = root;
  while ((*insert_ptr) != text_tree_nil)
    { int cond;
      if ((*insert_ptr) -> balfac)
	  { fixation = insert_ptr;
	    last_unbal = *insert_ptr;
	  };
      cond = strcmp (t, (*insert_ptr) -> word);
      if (cond < 0) insert_ptr = &((*insert_ptr) -> left);
      else if (cond > 0) insert_ptr = &((*insert_ptr) -> right);
      else return ((*insert_ptr) -> word);
    };

  /* found location to insert new leaf and remember it */
  /* the pointer rotations may not leave *insert_ptr intact */
  nt = new_leaf (t);
  *insert_ptr = nt;

  /* adjust balance factors from last_unbal to the inserted node */
  if (strlt (t, last_unbal -> word))
    { below_unbal = last_unbal -> left;
      last_unbal -> balfac--;
    }
  else
    { below_unbal = last_unbal -> right;
      last_unbal -> balfac++;
    };

  ptr = below_unbal;
  while (ptr != nt)
    if (strlt (t, ptr -> word))
      { ptr -> balfac--;
	ptr = ptr -> left;
      }
    else
      { ptr -> balfac++;
	ptr = ptr -> right;
      };

  /* if tree not too much out of balance, done */
  if ((-1 <= last_unbal -> balfac) && (last_unbal -> balfac <= 1))
    return (nt -> word);

  /* if last_unbal has the same sign as below_unbal, it is easy */
  if (((last_unbal -> balfac > 0) && (below_unbal -> balfac > 0)) ||
      ((last_unbal -> balfac < 0) && (below_unbal -> balfac < 0)))
    { if (last_unbal -> balfac > 0)
	{ last_unbal -> right = below_unbal -> left;
	  below_unbal -> left = last_unbal;
	}
      else
	{ last_unbal -> left = below_unbal -> right;
	  below_unbal -> right = last_unbal;
	};
      last_unbal -> balfac = 0;
      below_unbal -> balfac = 0;
      *fixation = below_unbal;
      return (nt -> word);
    };

  /* The difficult case */
  if (last_unbal -> balfac > 0)
    { twobelow_unbal = below_unbal -> left;
      last_unbal -> right = twobelow_unbal -> left;
      below_unbal -> left = twobelow_unbal -> right;
      twobelow_unbal -> left = last_unbal;
      twobelow_unbal -> right = below_unbal;
      last_unbal -> balfac = (twobelow_unbal -> balfac == 1)?-1:0;
      below_unbal -> balfac = (twobelow_unbal -> balfac == -1)?1:0;
    }
  else
    { twobelow_unbal = below_unbal -> right;
      last_unbal -> left = twobelow_unbal -> right;
      below_unbal -> right = twobelow_unbal -> left;
      twobelow_unbal -> right = last_unbal;
      twobelow_unbal -> left = below_unbal;
      last_unbal -> balfac = (twobelow_unbal -> balfac == -1)?1:0;
      below_unbal -> balfac = (twobelow_unbal -> balfac == 1)?-1:0;
    };
  twobelow_unbal -> balfac = 0;
  *fixation = twobelow_unbal;
  return (nt -> word);
};

string lookup_name (char *t)
{ text_tree ptr = root; 
  while (1)
    { int cond;
      if (ptr == text_tree_nil)
         { panic ("text %s not found in text storage\n", t);
	   exit (4);
	 };
      cond = strcmp (t, ptr -> word);
      if (cond < 0) ptr = ptr -> left;
      else if (cond > 0) ptr = ptr -> right;
      else return (ptr -> word);
    };
};

static int unique;
string new_unique_name ()
{ char buf[16];
  sprintf (buf, "_uniq%d", unique);
  unique++;
  return (addto_names (buf));
};

void init_textstorage (int strstore_size)
{ strstore = (char *) ckmalloc (strstore_size);
  root = text_tree_nil;
  unique = 0;
};

/*
   Dump
*/
static void dump_tree (text_tree ptr, int depth)
{ if (ptr != text_tree_nil)
    { char bc = (ptr -> balfac < 0)?'-':(ptr -> balfac > 0)?'+':'0';
      int ix;
      dump_tree (ptr -> left, depth + 1);
      for (ix = 0; ix < depth; ix++) eprint_log (0, 0, " ");
      wlog ("%c %s", bc, ptr -> word);
      dump_tree (ptr -> right, depth + 1);
    };
};

void dump_textstorage ()
{ wlog ("Dump of textstorage:\n");
  dump_tree (root, 0);
};
