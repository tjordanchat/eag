/*
   File: ebs_error.c
   Defines routines for diagnostic messages

   CVS ID: "$Id: ebs_error.c,v 1.5 2008/05/28 12:22:16 marcs Exp $"
*/

/*
   global includes
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/* local includes */
#include "ebs_error.h"

/* global variables indicating the diagnostics level */
int debug;
int verbose;
int full_verbose;
void init_error ()
{ debug = 0;
  verbose = 0;
  full_verbose = 0;
};

/*
   Exported error logging
*/
#define MAXERRSIZE 1024
void panic (char *format, ...)
{ char buf[MAXERRSIZE];
  va_list arg_ptr;
  va_start (arg_ptr, format);
  vsprintf (buf, format, arg_ptr);
  va_end (arg_ptr);
  fprintf (stderr, "%s\n", buf);
  exit (4);
};

void error (char *format, ...)
{ char buf[MAXERRSIZE];
  va_list arg_ptr;
  va_start (arg_ptr, format);
  vsprintf (buf, format, arg_ptr);
  va_end (arg_ptr);
  fprintf (stderr, "%s\n", buf);
};

void wlog (char *format, ...)
{ char buf[MAXERRSIZE];
  va_list arg_ptr;
  va_start (arg_ptr, format);
  vsprintf (buf, format, arg_ptr);
  va_end (arg_ptr);
  fprintf (stderr, "%s\n", buf);
};

void warning (char *format, ...)
{ char buf[MAXERRSIZE];
  va_list arg_ptr;
  va_start (arg_ptr, format);
  vsprintf (buf, format, arg_ptr);
  va_end (arg_ptr);
  if (verbose) fprintf (stderr, "%s\n", buf);
};

void hint (char *format, ...)
{ char buf[MAXERRSIZE];
  va_list arg_ptr;
  va_start (arg_ptr, format);
  vsprintf (buf, format, arg_ptr);
  va_end (arg_ptr);
  if (full_verbose) fprintf (stderr, "%s\n", buf);
};

void eprint_log (char *format, ...)
{ char buf[MAXERRSIZE];
  va_list arg_ptr;
  va_start (arg_ptr, format);
  vsprintf (buf, format, arg_ptr);
  va_end (arg_ptr);
  fprintf (stderr, "%s", buf);
};

/*
   Exported exception routines
*/
void bad_tag (int tag, char *routine)
{ panic ("bad tag %d occurred in routine %s", tag, routine);
};

void internal_error (char *routine)
{ panic ("internal consistency check failed in routine %s", routine);
};
