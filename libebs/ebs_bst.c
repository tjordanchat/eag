/*
   File: ebs_bst.c
   Defines basic operations for standard types

   CVS ID: "$Id: ebs_bst.c,v 1.5 2008/05/28 11:54:02 marcs Exp $"
*/

/* global includes */
#include <stdio.h>
#include <string.h>

/* local includes */
#include "ebs_bst.h"
#include "ebs_error.h"
#include "ebs_memalloc.h"
#include "ebs_textstorage.h"

/* saving standard types */
static int save_check_sum;
void init_save (FILE *f)
{ save_check_sum = 0;
  /* version saving??? */
}

void finish_save (FILE *f)
{ fputc (0xff - save_check_sum, f);
}

void save_char (FILE *f, char x)
{ save_check_sum = (save_check_sum + x) & 0xff;
  fputc (x, f);
}

void save_int (FILE *f, int x)
{ int value = x;
  int more = 1;
  do
    { int byte = value & 0x7f;
      value >>= 7;
      if ((value == 0) && !(byte & 0x40)) more = 0;
      if ((value == -1) && (byte & 0x40)) more = 0;
      if (more) byte |= 0x80;
      save_char (f, (char) byte);
    }
  while (more);
}

void save_real (FILE *f, real x)
{ int ix;
  char *dptr = (char *) &x;
#ifdef WORDS_BIGENDIAN
  for (ix = sizeof (real) - 1; 0 <= ix; ix--)
    save_char (f, dptr[ix]);
#else
  for (ix = 0; ix < sizeof (real); ix++)
    save_char (f, dptr[ix]);
#endif
}

void save_string (FILE *f, string x)
{ int len = (int) strlen (x);
  int ix;
  save_int (f, len);
  for (ix = 0; ix < len; ix++) save_char (f, x[ix]);
}

/* loading standard types */
static int load_check_sum;
void init_load (FILE *f)
{ load_check_sum = 0;
  /* version loading & checking??? */
}

int load_char (FILE *f, char *x)
{ int ch = fgetc (f);
  if (ch == EOF) return (0);
  load_check_sum = (load_check_sum + ch) & 0xff;
  *x = (char) ch;
  return (1);
}

int finish_load (FILE *f)
{ char ch;
  if (!load_char (f, &ch)) return (0);
  if (((int) load_check_sum & 0xff) != 0xff) return (0);
  return (fgetc (f) == EOF);
}

typedef unsigned int u_int;
int load_int (FILE *f, int *x)
{ int value = 0;
  int shift = 0;
  char bb;
  do
    { if (!load_char (f, &bb)) return (0);
      value |= (((u_int) (bb & 0x7f)) << shift);
      shift += 7;
    }
  while (bb & 0x80);
  if (shift > 31) shift = 31;
  if (bb & 0x40) value |= -(1 << shift);
  *x = value;
  return (1);
}

int load_real (FILE *f, real *x)
{ int ix;
  char ch;
  char *dptr = (char *) x;
#ifdef WORDS_BIGENDIAN
  for (ix = sizeof (real) - 1; 0 <= ix; ix--)
    { if (!load_char (f, &ch)) return (0);
      dptr[ix] = ch;
    };
#else
  for (ix = 0; ix < sizeof (real); ix++)
    { if (!load_char (f, &ch)) return (0);
      dptr[ix] = ch;
    };
#endif
  return (1);
}


#define MAX_SIZE 16384
int load_string (FILE *f, string *x)
{ int ix, size;
  char ch;
  char buffer[MAX_SIZE];
  char *ptr;
  if (!load_int (f, &size)) return (0);
  if (size < MAX_SIZE - 1) ptr = buffer;
  else ptr = ckmalloc (size + 1);
  for (ix = 0; ix < size; ix++)
    { if (!load_char (f, &ch)) return (0);
      ptr[ix] = ch;
    };
  ptr[size] = '\0';
  *x = addto_names (ptr);
  return (1);
};
