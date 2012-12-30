/*
   File: ebs_cst.c
   Defines composite standard types and operations

   CVS ID: "$Id: ebs_cst.c,v 1.5 2008/05/28 12:47:03 marcs Exp $"
*/

/* global includes */
#include <stdio.h>

/* libebs includes */
#include "ebs_bst.h"
#include "ebs_error.h"
#include "ebs_memalloc.h"
#include "ebs_textstorage.h"
#include "ebs_cst.h"

/* Announce to use 'room' ints in a int_list */
void room_int_list (int_list il, int room)
{ if (room <= il -> room) return;
  il -> array = (int *) ckrecalloc (il -> array, room, sizeof (int));
  il -> room = room;
};

/* Allocate a new int_list */
int_list new_int_list ()
{ int_list nl = (int_list) ckmalloc (sizeof (struct int_list_rec));
  nl -> size = 0;
  nl -> room = 2;
  nl -> array = (int *) ckcalloc (2, sizeof (int));
  return (nl);
};

/* Recursively copy an int_list */
int_list rdup_int_list (int_list il)
{ int_list nl = new_int_list ();
  int ix;
  room_int_list (nl, il -> size);
  nl -> size = il -> size;
  for (ix = 0; ix < il -> size; ix++)
    nl -> array[ix] = il -> array[ix];
  return (nl);
};

/* Recursively free an int_list */
void rfre_int_list (int_list old)
{ free (old -> array);
  free (old);
};

/* Append int to int_list */
void app_int_list (int_list il, int i)
{ if (il -> size == il -> room) room_int_list (il, il -> size << 1);
  il -> array [il -> size] = i;
  il -> size++;
};

/* Relational test for int lists */
int less_int_list (int_list il1, int_list il2)
{ int ix;
  if (il1 == int_list_nil) return (0);
  if (il2 == int_list_nil) return (0);
  if (il1 -> size != il2 -> size) return (il1 -> size < il2 -> size);
  for (ix = 0; ix < il1 -> size; ix++)
    if (il1 -> array[ix] < il2 -> array[ix]) return (1);
    else if (il1 -> array[ix] > il2 -> array[ix]) return (0);
  return (0);
};

/* Equality test for int lists */
int equal_int_list (int_list il1, int_list il2)
{ int ix;
  if (il1 == int_list_nil) return (0);
  if (il2 == int_list_nil) return (0);
  if (il1 -> size != il2 -> size) return (0);
  for (ix = 0; ix < il1 -> size; ix++)
    if (il1 -> array[ix] != il2 -> array[ix])
      return (0);
  return (1);
};

/* Save an int list */
void save_int_list (FILE *f, int_list il)
{ int ix;
  save_int (f, il -> size);
  for (ix = 0; ix < il -> size; ix++)
    save_int (f, il -> array[ix]);
};

/* Load an int list */
int load_int_list (FILE *f, int_list *il)
{ int ix, size;
  int_list nl = new_int_list ();
  if (!load_int (f, &size)) return (0);
  room_int_list (nl, size);
  nl -> size = size;
  for (ix = 0; ix < size; ix++)
    if (!load_int (f, &nl -> array[ix])) return (0);
  *il = nl;
  return (1);
};

/* Announce to use 'room' strings in a string_list */
void room_string_list (string_list sl, int room)
{ if (room <= sl -> room) return;
  sl -> array = (string *) ckrecalloc (sl -> array, room, sizeof (string));
  sl -> room = room;
};

/* Allocate a new string_list */
string_list new_string_list ()
{ string_list nsl = (string_list) ckmalloc (sizeof (struct string_list_rec));
  nsl -> size = 0;
  nsl -> room = 2;
  nsl -> array = (string *) ckcalloc (2, sizeof (string));
  return (nsl);
};

/* Recursively copy a string_list */
string_list rdup_string_list (string_list sl)
{ string_list nsl = new_string_list ();
  int ix;
  room_string_list (nsl, sl -> size);
  nsl -> size = sl -> size;
  for (ix = 0; ix < sl -> size; ix++)
     nsl -> array[ix] = sl -> array[ix];
  return (nsl);
};

/* Recursively free an string_list */
void rfre_string_list (string_list old)
{ free (old -> array);
  free (old);
};

/* Append string to string_list */
void app_string_list (string_list sl, string s)
{ if (sl -> size == sl -> room) room_string_list (sl, sl -> size << 1);
  sl -> array [sl -> size] = s;
  sl -> size++;
};

/* Relational test for string lists */
int less_string_list (string_list sl1, string_list sl2)
{ int ix;
  if (sl1 == string_list_nil) return (0);
  if (sl2 == string_list_nil) return (0);
  if (sl1 -> size != sl2 -> size) return (sl1 -> size < sl2 -> size);
  for (ix = 0; ix < sl1 -> size; ix++)
    { int stat = strcmp (sl1 -> array[ix], sl2 -> array[ix]);
      if (stat < 0) return (1);
      else if (stat > 0) return (0);
    };
  return (0);
};

/* Equality test for string lists */
int equal_string_list (string_list sl1, string_list sl2)
{ int ix;
  if (sl1 == string_list_nil) return (0);
  if (sl2 == string_list_nil) return (0);
  if (sl1 -> size != sl2 -> size) return (0);
  for (ix = 0; ix < sl1 -> size; ix++)
    if (strcmp (sl1 -> array[ix], sl2 -> array[ix]))
      return (0);
  return (1);
};

/* Save an string list */
void save_string_list (FILE *f, string_list sl)
{ int ix;
  save_int (f, sl -> size);
  for (ix = 0; ix < sl -> size; ix++)
    save_string (f, sl -> array[ix]);
};

/* Load an string list */
int load_string_list (FILE *f, string_list *sl)
{ int ix, size;
  string_list nsl = new_string_list ();
  if (!load_int (f, &size)) return (0);
  room_string_list (nsl, size);
  nsl -> size = size;
  for (ix = 0; ix < size; ix++)
    if (!load_string (f, &nsl -> array[ix])) return (0);
  *sl = nsl;
  return (1);
};

/* Specific extra operations */
string concatenate_strings (string_list sl)
{ register char *dptr = strstore;
  int ix;
  for (ix = 0; ix < sl -> size; ix++)
    { register char *sptr = sl -> array[ix];
      while (*sptr) *dptr++ = *sptr++;
    };
  *dptr = '\0';
  return (addto_names (strstore));
};
