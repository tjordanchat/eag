/*
   File: ebs_primio.c
   Defines some primitive IO routines

   CVS ID: "$Id: ebs_primio.c,v 1.5 2008/05/28 14:30:30 marcs Exp $"
*/

/* general includes */
#include <stdio.h>

/* local includes */
#include "ebs_bst.h"
#include "ebs_primio.h"

/*
   If the string is large break it up in parts
*/
void output_char (FILE *file, char c)
{ if (c == '\n') fprintf (file, "\\n");
  else if (c == '\t') fprintf (file, "\\t");
  else if (c == EOFCHAR) fprintf (file, "\\%3o", c & 0377);
  else if (c < ' ') fprintf (file, "\\%o", c);
  else if (c == '"') fprintf (file, "\\\"");
  else if (c == '\'') fprintf (file, "\\'");
  else if (c == '\\') fprintf (file, "\\\\");
  else fputc (c, file);
};

#define MAXCOUNT 50
void output_unquoted_string (FILE *file, string str)
{ char *ptr;
  int count = 0;
  for (ptr = str; *ptr; ptr++)
    {
#ifndef Debug
      if (count == MAXCOUNT)
	{ fprintf (file, "\\\n");
	  count = 0;
	};
#endif
      output_char (file, *ptr);
#ifndef Debug
      count++;
#endif
    };
};

void output_string (FILE *file, string str)
{ fputc ('"', file);
  output_unquoted_string (file, str);
  fputc ('"', file);
};
