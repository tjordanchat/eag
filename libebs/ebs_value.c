/*
   File: value.c
   Defines operations on values

   CVS ID: "$Id: ebs_value.c,v 1.6 2008/06/11 14:58:44 marcs Exp $"
*/

/* global includes */
#include <stdio.h>

/* libebs includes */
#include "ebs_bst.h"
#include "ebs_error.h"
#include "ebs_memalloc.h"
#include "ebs_textstorage.h"
#include "ebs_cst.h"
#include "ebs_primio.h"
#include "ebs_value.h"

/*
   Some storage administration of values
   copy_value en free_value maken??
*/
static value free_values;
static value new_value (int tag)
{ value nv;
  if (free_values != value_nil)
    { nv = free_values;
      free_values = free_values -> u.free;
    }
  else nv = (value) ckmalloc (sizeof (struct value_rec));
  nv -> admin_nr = 0;
  nv -> ref_count = 1;
  nv -> dptr = NULL;
  nv -> tag = tag;
  return (nv);
};

value new_undefined_value ()
{ return (new_value (undefined_value));
};

value new_string_value (string s)
{ string rs = addto_names (s);
  value nv = new_value (string_value);
  nv -> u.str = rs;
  return (nv);
};

value new_integer_value (int inum)
{ value nv = new_value (integer_value);
  nv -> u.inum = inum;
  return (nv);
};

value new_real_value (real rnum)
{ value nv = new_value (real_value);
  nv -> u.rnum = rnum;
  return (nv);
};

value new_tuple_value (value_list vl)
{ value nv = new_value (tuple_value);
  nv -> u.tuple = vl;
  return (nv);
};

value new_tuple_value_from_array (int size, value *array)
{ value_list vl = new_value_list (size);
  value nv = new_value (tuple_value);
  int ix;
  vl -> size = size;
  for (ix = 0; ix < size; ix++) vl -> array[ix] = array[ix];
  nv -> u.tuple = vl;
  return (nv);
};

value new_small_lattice_value (int slat, string *lnames)
{ value nv = new_value (small_lattice_value);
  nv -> dptr = (void *) lnames;
  nv -> u.slat = slat;
  return (nv);
};

value new_large_lattice_value (int size, int *elat, string *lnames)
{ int_list il = new_int_list ();
  value nv = new_value (large_lattice_value);
  int ix;
  room_int_list (il, size);
  il -> size = size;
  for (ix = 0; ix < size; ix++) il -> array[ix] = (elat)?(elat[ix]):0;
  nv -> dptr = (void *) lnames;
  nv -> u.elat = il;
  return (nv);
};

value new_singleton_value (int eltnr, int size, string *lnames)
{ value nv;
  if (size > 32)
     { int asize = size/32 + 1;
       nv = new_large_lattice_value (asize, NULL, lnames);
       nv -> u.elat -> array[asize - 1 - eltnr/32] = (1 << (eltnr % 32));
     }
  else nv = new_small_lattice_value (1 << eltnr, lnames);
  return (nv);
};

/* recursively free a value */
void rfre_value (value val)
{ if (val == value_nil) return;
  val -> ref_count--;
  if (val -> ref_count) return;
  switch (val -> tag)
    { case tuple_value: rfre_value_list (val -> u.tuple); break;
      case large_lattice_value: rfre_int_list (val -> u.elat);
      default: break;
    };
  val -> u.free = free_values;
  free_values = val;
};

value rdup_value (value val)
{ if (val != value_nil) val -> ref_count++;
  return (val);
};

/*
   Define a total order for values
*/
int less_value (value v1, value v2)
{ if (v1 == value_nil) return (0);
  if (v2 == value_nil) return (0);
  if (v1 == v2) return (0);
  if (v1 -> tag != v2 -> tag) return (v1 -> tag < v2 -> tag);
  switch (v1 -> tag)
    { case undefined_value:	return (0);
      case string_value:	return (strcmp (v1 -> u.str, v2 -> u.str) < 0);
      case integer_value:	return (v1 -> u.inum < v2 -> u.inum);
      case real_value:		return (v1 -> u.rnum < v2 -> u.rnum);
      case tuple_value:		return (less_value_list (v1 -> u.tuple, v2 -> u.tuple));
      case small_lattice_value:
	if (v1 -> u.slat < v2 -> u.slat) return (1);
	if (v2 -> u.slat < v1 -> u.slat) return (0);
	break;
      case large_lattice_value:
	if (less_int_list (v1 -> u.elat, v2 -> u.elat)) return (1);
	if (less_int_list (v2 -> u.elat, v1 -> u.elat)) return (0);
	break;
      default: bad_tag (v1 -> tag, "less_value");
    };
  return (v1 -> admin_nr < v2 -> admin_nr);
};

int equal_value (value v1, value v2)
{ if (v1 == value_nil) return (0);
  if (v2 == value_nil) return (0);
  if (v1 == v2) return (1);
  if (v1 -> tag != v2 -> tag) return (0);
  switch (v1 -> tag)
    { case undefined_value:	return (0);
      case string_value:	return (!strcmp (v1 -> u.str, v2 -> u.str));
      case integer_value:	return (v1 -> u.inum == v2 -> u.inum);
      case real_value:		return (v1 -> u.rnum == v2 -> u.rnum);
      case tuple_value:		return (equal_value_list (v1 -> u.tuple, v2 -> u.tuple));
      case small_lattice_value: return (v1 -> u.slat == v2 -> u.slat);
      case large_lattice_value: return (equal_int_list (v1 -> u.elat, v2 -> u.elat));
      default: bad_tag (v1 -> tag, "equal_value");
    };
  return (0);
};

void save_value (FILE *f, value v)
{ save_int (f, v -> tag);
  switch (v -> tag)
    { case string_value:	save_string (f, v -> u.str); break;
      case integer_value:	save_int (f, v -> u.inum); break;
      case real_value:		save_real (f, v -> u.rnum); break;
      case tuple_value:		save_value_list (f, v -> u.tuple); break;
      case small_lattice_value: save_int (f, v -> u.slat); break;
      case large_lattice_value: save_int_list (f, v -> u.elat); break;
      default: bad_tag (v -> tag, "save_value");
    };
};

int load_value (FILE *f, value *v, string *lnames)
{ value nv;
  int tag;
  if (!load_int (f, &tag)) return (0);
  nv = new_value (tag);
  nv -> dptr = (void *) lnames;
  switch (tag)
    { case string_value:
	if (!load_string (f, &nv -> u.str)) return (0);
	break;
      case integer_value:
	if (!load_int (f, &nv -> u.inum)) return (0);
	break;
      case real_value:
	if (!load_real(f, &nv -> u.rnum)) return (0);
	break;
      case tuple_value:
	if (!load_value_list (f, &nv -> u.tuple, lnames)) return (0);
	break;
      case small_lattice_value:
	if (!load_int (f, &nv -> u.slat)) return (0);
	break;
      case large_lattice_value:
	if (!load_int_list (f, &nv -> u.elat)) return (0);
	break;
      default:
	bad_tag (tag, "load_value");
    };
  *v = nv;
  return (1);
};

/* Announce to use 'room' values in a value_list */
void room_value_list (value_list vl, int room)
{ if (room <= vl -> room) return;
  vl -> array = (value *) ckrecalloc (vl -> array, room, sizeof (value));
  vl -> room = room;
};

/* Allocate a new value list */
value_list new_value_list (int room)
{ value_list nvl = (value_list) ckmalloc (sizeof (struct value_list_rec));
  int mroom = (room <= 0)?1:room;
  nvl -> size = 0;
  nvl -> room = mroom;
  nvl -> array = (value *) ckcalloc (mroom, sizeof (value));
  return (nvl);
};

/* Recursively copy a value list */
value_list rdup_value_list (value_list vl)
{ value_list nvl = new_value_list (vl -> size);
  int ix;
  nvl -> size = vl -> size;
  for (ix = 0; ix < vl -> size; ix++)
    nvl -> array[ix] = rdup_value (vl -> array[ix]);
  return (nvl);
};

/* Recursively free a value list */
void rfre_value_list (value_list old)
{ int ix;
  if (old == value_list_nil) return;
  for (ix = 0; ix < old -> size; ix++)
    rfre_value (old -> array[ix]);
  free (old -> array);
  free (old);
};

/* Append value to value_list */
void app_value_list (value_list vl, value v)
{ if (vl -> size == vl -> room) room_value_list (vl, vl -> size << 1);
  vl -> array [vl -> size] = v;
  vl -> size++;
};

/*
   Define a total order on value lists
*/
int less_value_list (value_list vl1, value_list vl2)
{ int ix;
  if (vl1 == value_list_nil) return (0);
  if (vl2 == value_list_nil) return (0);
  if (vl1 -> size != vl2 -> size) return (vl1 -> size < vl2 -> size);
  for (ix = 0; ix < vl1 -> size; ix++)
    if (less_value (vl1 -> array[ix], vl2 -> array[ix])) return (1);
    else if (less_value (vl2 -> array[ix], vl1 -> array[ix])) return (0);
  return (0);
};

int equal_value_list (value_list vl1, value_list vl2)
{ int ix;
  if (vl1 == value_list_nil) return (0);
  if (vl2 == value_list_nil) return (0);
  if (vl1 -> size != vl2 -> size) return (0);
  for (ix = 0; ix < vl1 -> size; ix++)
    if (!equal_value (vl1 -> array[ix], vl2 -> array[ix])) return (0);
  return (1);
};

void save_value_list (FILE *f, value_list vl)
{ int ix;
  save_int (f, vl -> size);
  for (ix = 0; ix < vl -> size; ix++)
    save_value (f, vl -> array[ix]);
};

int load_value_list (FILE *f, value_list *vl, string *lnames)
{ int ix, size;
  value_list nvl;
  if (!load_int (f, &size)) return (0);
  nvl = new_value_list (size);
  nvl -> size = size;
  nvl -> room = size;
  for (ix = 0; ix < size; ix++)
    if (!load_value (f, &nvl -> array[ix], lnames)) return (0);
  *vl = nvl;
  return (1);
};

/*
   Value calculation
   Note: coercion of int -> real on adding real + int?
*/
value concatenate_values (value_list vl)
{ int ix;
  switch (vl -> array[0] -> tag)
    { case string_value:
	{ register char *dptr = strstore;
	  for (ix = 0; ix < vl -> size; ix++)
	    { register char *sptr = vl -> array[ix] -> u.str;
	      while (*sptr) *dptr++ = *sptr++;
	    };
	  *dptr = '\0';
	  return (new_string_value (strstore));
	};
      case integer_value:
	{ int sum = 0;
	  for (ix = 0; ix < vl -> size; ix++)
	    sum += vl -> array[ix] -> u.inum;
	  return (new_integer_value (sum));
	};
      case real_value:
        { real sum = 0.0;
	  for (ix = 0; ix < vl -> size; ix++)
            sum += vl -> array[ix] -> u.rnum;
	  return (new_real_value (sum));
	};
      default: bad_tag (vl -> array[0] -> tag, "concatenate_values");
    };
  return (value_nil);
};

value join_lattice_values (value_list vl)
{ int ix;
  string *lnames = (string *) vl -> array[0] -> dptr;
  switch (vl -> array[0] -> tag)
    { case small_lattice_value:
	{ int uni = 0;
	  for (ix = 0; ix < vl -> size; ix++)
	    uni |= vl -> array[ix] -> u.slat;
	  return (new_small_lattice_value (uni, lnames));
        };
      case large_lattice_value:
	{ int size = vl -> array[0] -> u.elat -> size;
	  value nv = new_large_lattice_value (size, NULL, lnames);
	  for (ix = 0; ix < vl -> size; ix++)
	    { int_list il = vl -> array[ix] -> u.elat;
	      int iy;
	      for (iy = 0; iy < size; iy++)
		nv -> u.elat -> array[iy] |= il -> array[iy];
	    };
	  return (nv);
	};
      default: bad_tag (vl -> array[0] -> tag, "join_lattice_values");
    };
  return (value_nil);
};

int meet_lattice_values (value v1, value v2, value *vret)
{ if ((v1 == value_nil) || (v2 == value_nil)) return (0);
  if (v1 -> tag != v2 -> tag) return (0);
  switch (v1 -> tag)
    { case small_lattice_value:
	{ int met = v1 -> u.slat & v2 -> u.slat;
	  if (!met) return (0);
	  *vret = new_small_lattice_value (met, (string *) v1 -> dptr);
	  return (1);
	};
      case large_lattice_value:
	{ int_list il1 = v1 -> u.elat;
	  int_list il2 = v2 -> u.elat;
	  int size = il1 -> size;
	  value nv = new_large_lattice_value (size, NULL, (string *) v1 -> dptr);
	  int_list ilt = nv -> u.elat;
	  int lmet = 0;
	  int ix;
	  for (ix = 0; ix < size; ix++)
	    { int met = il1 -> array[ix] & il2 -> array[ix];
	      if (met) lmet = 1;
	      ilt -> array[ix] = met;
	    };
	  if (lmet)
	    { *vret = nv;
	      return (1);
	    };
	  rfre_value (nv);
	  return (0);
	};
      default: break;
    };
  return (0);
};

/*
   Formatted output for value logging and tracing
*/
void dump_value (value val)
{ if (val == value_nil)
    { eprint_log ("nil");
      return;
    };
  switch (val -> tag)
    { case undefined_value:
	eprint_log ("\bot");
	break;
      case string_value:
	output_string (stderr, val -> u.str);
	break;
      case integer_value:
	eprint_log ("%d", val -> u.inum);
	break;
      case real_value:
	eprint_log ("%g", val -> u.rnum);
	break;
      case tuple_value:
	{ value_list vl = val -> u.tuple;
	  eprint_log ("<");
	  if (vl != value_list_nil)
	    { int ix;
	      for (ix = 0; ix < vl -> size; ix++)
		{ if (ix != 0) eprint_log (" * ");
		  dump_value (vl -> array[ix]);
	        };
	    };
	  eprint_log (">");
	}; break;
      case small_lattice_value:
	if (val -> dptr == NULL)
	  eprint_log ("{ %08x }", val -> u.slat);
	else
	  { string *lnames = (string *) val -> dptr;
	    int nfirst = 0;
	    int ix;
	    eprint_log ("{ ");
	    for (ix = 0; ix < 32; ix++)
	      if (val -> u.slat & (1 << ix))
		{ eprint_log ("%s%s", (nfirst)?", ":"", lnames[ix]);
		  nfirst = 1;
		};
	    eprint_log (" }");
	  };
	break;
      case large_lattice_value:
	{ string *lnames = (string *) val -> dptr;
	  int nfirst = 0;
	  int lidx = 0;
	  int_list il = val -> u.elat;
	  int ix;
	  eprint_log ("{ ");
	  if (lnames == NULL)
	    for (ix = 0; ix < il -> size; ix++)
	      eprint_log ("%08x", il -> array[ix]);
	  else
	    for (ix = il -> size - 1; 0 <= ix; ix--)
	      { int iy;
		for (iy = 0; iy < 32; iy++, lidx++)
		  if (il -> array[ix] & (1 << iy))
		    { eprint_log ("%s%s", (nfirst)?", ":"", lnames [lidx]);
		      nfirst = 1;
		    };
	      };
	  eprint_log (" }");
	}; break;
      default: bad_tag (val -> tag, "dump_value");
    };
};

/*
   unformatted output for affix output at end of parse etc.
*/
void output_value (FILE *out, value val)
{ if (val == value_nil)
    { fputs ("<value_nil>", out);
      return;
    };
  switch (val -> tag)
    { case undefined_value: break;
      case string_value: 
	fputs (val -> u.str, out);
	break;
      case integer_value:
	fprintf (out, "%d", val -> u.inum);
	break;
      case real_value:
	fprintf (out, "%g", val -> u.rnum);
	break;
      case tuple_value:
	{ value_list vl = val -> u.tuple;
	  fputc ('<', out);
	  if (vl != value_list_nil)
	    { int ix;
	      for (ix = 0; ix < vl -> size; ix++)
		{ if (ix != 0) fputs (" * ", out);
		  output_value (out, vl -> array[ix]);
		};
	    };
	  fputc ('>', out);
	}; break;
      case small_lattice_value:
	if (val -> dptr == NULL) fprintf (out, "{ %08x }", val -> u.slat);
	else
	  { string *lnames = (string *) val -> dptr;
	    int nfirst = 0;
	    int ix;
	    fprintf (out, "{ ");
	    for (ix = 0; ix < 32; ix++)
	      if (val -> u.slat & (1 << ix))
		{ fprintf (out, "%s%s", (nfirst)?", ":"", lnames[ix]);
		  nfirst = 1;
		};
	    fprintf (out, " }");
	  };
	break;
      case large_lattice_value:
	{ string *lnames = (string *) val -> dptr;
	  int nfirst = 0;
	  int lidx = 0;
	  int_list il = val -> u.elat;
	  int ix;
	  fprintf (out, "{ ");
	  if (lnames == NULL)
	    for (ix = 0; ix < il -> size; ix++)
	      eprint_log ("%08x", il -> array[ix]);
	  else
	    for (ix = il -> size - 1; 0 <= ix; ix--)
	      { int iy;
		for (iy = 0; iy < 32; iy++, lidx++)
		  if (il -> array[ix] & (1 << iy))
		    { fprintf (out, "%s%s", (nfirst)?", ":"", lnames [lidx]);
		      nfirst = 1;
		    };
	      };
	  fprintf (out, " }");
	}; break;
      default: bad_tag (val -> tag, "output_value");
    };
};

/* Initialization */
void init_value ()
{ free_values = value_nil;
};
