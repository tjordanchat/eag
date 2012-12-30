/*
   File: ebs_lexds.c
   Defines operations on values

   CVS ID: "$Id: ebs_lexds.c,v 1.5 2008/05/28 14:53:57 marcs Exp $"
*/

/* general includes */
#include <stdio.h>

/* libebs includes */
#include "ebs_bst.h"
#include "ebs_error.h"
#include "ebs_memalloc.h"
#include "ebs_cst.h"
#include "ebs_value.h"
#include "ebs_lexds.h"

/* lexend manipulation */
lexend new_lexend (int nodenr, int_list valuenrs)
{ lexend nl = (lexend) ckmalloc (sizeof (struct lexend_rec));
  nl -> nodenr = nodenr;
  nl -> valuenrs = valuenrs;
  return (nl);
};

lexend rdup_lexend (lexend old)
{ lexend nl = (lexend) ckmalloc (sizeof (struct lexend_rec));
  nl -> nodenr = old -> nodenr;
  nl -> valuenrs = rdup_int_list (old -> valuenrs);
  return (nl);
};

void rfre_lexend (lexend old)
{ rfre_int_list (old -> valuenrs);
  free (old);
};

void save_lexend (FILE *f, lexend l)
{ save_int (f, l -> nodenr);
  save_int_list (f, l -> valuenrs);
};

int load_lexend (FILE *f, lexend *l)
{ lexend nl = (lexend) ckmalloc (sizeof (struct lexend_rec));
  if (!load_int (f, &nl -> nodenr)) return (0);
  if (!load_int_list (f, &nl -> valuenrs)) return (0);
  *l = nl;
  return (1);
};

/* lexend list manipulation */
lexend_list new_lexend_list ()
{ lexend_list nl = (lexend_list) ckmalloc (sizeof (struct lexend_list_rec));
  nl -> size = 0;
  nl -> room = 2;
  nl -> array = (lexend *) ckcalloc (2, sizeof (lexend));
  return (nl);
};

void room_lexend_list (lexend_list ll, int room)
{ if (room <= ll -> room) return;
  ll -> array = (lexend *) ckrecalloc (ll -> array, room, sizeof (lexend));
  ll -> room = room;
};

void app_lexend_list (lexend_list ll, lexend l)
{ if (ll -> size == ll -> room) room_lexend_list (ll, ll -> size << 1);
  ll -> array [ll -> size] = l;
  ll -> size++;
};

lexend_list rdup_lexend_list (lexend_list old)
{ lexend_list nl;
  int ix;
  if (old == lexend_list_nil) return (lexend_list_nil);
  nl = (lexend_list) ckmalloc (sizeof (struct lexend_list_rec));
  nl -> size = old -> size;
  nl -> room = old -> room;
  nl -> array = (lexend *) ckcalloc (old -> size, sizeof (lexend));
  for (ix = 0; ix < old -> size; ix++)
    nl -> array[ix] = rdup_lexend (old -> array[ix]);
  return (nl);
};

void rfre_lexend_list (lexend_list old)
{ int ix;
  if (old == lexend_list_nil) return;
  for (ix = 0; ix < old -> size; ix++)
    rfre_lexend (old -> array[ix]);
  free (old -> array);
  free (old);
};

void save_lexend_list (FILE *f, lexend_list ll)
{ int ix;
  save_int (f, ll -> size);
  for (ix = 0; ix < ll -> size; ix++)
     save_lexend (f, ll -> array[ix]);
};

int load_lexend_list (FILE *f, lexend_list *ll)
{ int ix, size;
  lexend_list nll = new_lexend_list ();
  if (!load_int (f, &size)) return (0);
  room_lexend_list (nll, size);
  nll -> size = size;
  for (ix = 0; ix < size; ix++)
     if (!load_lexend (f, &nll -> array[ix])) return (0);
  *ll = nll;
  return (1);
};

/* lextree manipulation */
lextree new_lextree (string remainder, lexend_list prods, lextree_list tails)
{ lextree nlt = (lextree) ckmalloc (sizeof (struct lextree_rec));
  nlt -> remainder = remainder;
  nlt -> prods = prods;
  nlt -> tails = tails;
  return (nlt);
};

lextree rdup_lextree (lextree old)
{ lextree nlt = (lextree) ckmalloc (sizeof (struct lextree_rec));
  nlt -> remainder = old -> remainder;
  nlt -> prods = rdup_lexend_list (old -> prods);
  nlt -> tails = rdup_lextree_list (old -> tails);
  return (nlt);
};

void rfre_lextree (lextree old)
{ rfre_lexend_list (old -> prods);
  rfre_lextree_list (old -> tails);
  free (old);
};

void save_lextree (FILE *f, lextree l)
{ save_string (f, l -> remainder);
  save_lexend_list (f, l -> prods);
  save_lextree_list (f, l -> tails);
};

int load_lextree (FILE *f, lextree *l)
{ lextree nlt = (lextree) ckmalloc (sizeof (struct lextree_rec));
  if (!load_string (f, &nlt -> remainder)) return (0);
  if (!load_lexend_list (f, &nlt -> prods)) return (0);
  if (!load_lextree_list (f, &nlt -> tails)) return (0);
  *l = nlt;
  return (1);
};

/* lextree list manipulation */
lextree_list new_lextree_list (int room)
{ lextree_list nll = (lextree_list) ckmalloc (sizeof (struct lextree_list_rec));
  int mroom = (room <= 0)?1:room;
  nll -> size = 0;
  nll -> room = mroom;
  nll -> array = (lextree *) ckcalloc (mroom, sizeof (lextree));
  return (nll);
};

void room_lextree_list (lextree_list ll, int room)
{ if (room <= ll -> room) return;
  ll -> array = (lextree *) ckrecalloc (ll -> array, room, sizeof (lextree));
  ll -> room = room;
};

void app_lextree_list (lextree_list ll, lextree l)
{ if (ll -> size == ll -> room) room_lextree_list (ll, ll -> size << 1);
  ll -> array [ll -> size] = l;
  ll -> size++;
};

lextree_list rdup_lextree_list (lextree_list old)
{ lextree_list nll;
  int ix;
  if (old == lextree_list_nil) return (lextree_list_nil);
  nll = (lextree_list) ckmalloc (sizeof (struct lextree_list_rec));
  nll -> size = old -> size;
  nll -> room = old -> room;
  nll -> array = (lextree *) ckcalloc (old -> room, sizeof (struct lextree_rec));
  for (ix = 0; ix < old -> size; ix++)
    nll -> array[ix] = rdup_lextree (old -> array[ix]);
  return (nll);
};

void rfre_lextree_list (lextree_list old)
{ int ix;
  if (old == lextree_list_nil) return;
  for (ix = 0; ix < old -> size; ix++)
    rfre_lextree (old -> array[ix]);
  free (old -> array);
  free (old);
};

void save_lextree_list (FILE *f, lextree_list ll)
{ int ix;
  save_int (f, ll -> size);
  for (ix = 0; ix < ll -> size; ix++)
    save_lextree (f, ll -> array[ix]);
};

int load_lextree_list (FILE *f, lextree_list *ll)
{ lextree_list nll;
  int ix, size;
  if (!load_int (f, &size)) return (0);
  nll = new_lextree_list (size);
  nll -> size = size;
  for (ix = 0; ix < size; ix++)
    if (!load_lextree (f, &nll -> array[ix])) return (0);
  *ll = nll;
  return (1);
};

/* lexicon manipulation */
lexicon new_lexicon (int_list lex_lnames, value_list lex_vals, lextree_list lex_trees)
{ lexicon nl = (lexicon) ckmalloc (sizeof (struct lexicon_rec));
  nl -> lex_lnames = lex_lnames;
  nl -> lex_vals = lex_vals;
  nl -> lex_trees = lex_trees;
  return (nl);
};

lexicon rdup_lexicon (lexicon old)
{ lexicon nl = (lexicon) ckmalloc (sizeof (struct lexicon_rec));
  nl -> lex_lnames = rdup_int_list (old -> lex_lnames);
  nl -> lex_vals = rdup_value_list (old -> lex_vals);
  nl -> lex_trees = rdup_lextree_list (old -> lex_trees);
  return (nl);
};

void rfre_lexicon (lexicon old)
{ rfre_int_list (old -> lex_lnames);
  rfre_value_list (old -> lex_vals);
  rfre_lextree_list (old -> lex_trees);
  free (old);
};

void save_lexicon (FILE *f, lexicon l)
{ save_int_list (f, l -> lex_lnames);
  save_value_list (f, l -> lex_vals);
  save_lextree_list (f, l -> lex_trees);
};

/* still not good enough: lattices in tuples */
static int load_lexicon_value_list (FILE *f, value_list *vl, string **lnames, int_list idxs)
{ int ix, size;
  value_list nvl;
  if (!load_int (f, &size)) return (0);
  nvl = new_value_list (size);
  nvl -> size = size;
  nvl -> room = size;
  for (ix = 0; ix < size; ix++)
    { string *lnm = NULL;
      int anr;
      if (ix >= idxs -> size) return (0);
      anr = idxs -> array[ix];
      if (anr) lnm = lnames[anr - 1];
      if (!load_value (f, &nvl -> array[ix], lnm)) return (0);
    };
  *vl = nvl;
  return (1);
};

int load_lexicon (FILE *f, lexicon *l, string **lnames)
{ lexicon nl = (lexicon) ckmalloc (sizeof (struct lexicon_rec));
  if (!load_int_list (f, &nl -> lex_lnames)) return (0);
  if (!load_lexicon_value_list (f, &nl -> lex_vals, lnames, nl -> lex_lnames)) return (0);
  if (!load_lextree_list (f, &nl -> lex_trees)) return (0);
  *l = nl;
  return (1);
};

void csave_lexicon (string fname, lexicon l)
{ FILE *f = fopen (fname, "w");
  if (f == NULL)
    panic ("could not open file '%s' for writing", fname);
  init_save (f);
  save_lexicon (f, l);
  finish_save (f);
  fclose (f);
};

void cload_lexicon (string fname, lexicon *l, string **lnames)
{ FILE *f = fopen (fname, "r");
  if (f == NULL)
    panic ("could not open file '%s' for reading", fname);
  init_load (f);
  if (!load_lexicon (f, l, lnames))
    panic ("lexicon file '%s' contains a gnarled lexicon", fname);
  if (!finish_load (f))
    panic ("lexicon file '%s' has an invalid checksum", fname);
  fclose (f);
};

