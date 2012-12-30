/*
   File: eag_textparsing.c
   Defines the routines to parse terminals and terminal sets
   for recursive backup parsing.

   CVS ID: "$Id: eag_textparsing.c,v 1.6 2008/06/18 13:59:18 marcs Exp $"
*/

/* global includes */
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_error.h>
#include <ebs_memalloc.h>
#include <ebs_textstorage.h>
#include <ebs_primio.h>

/* local includes */
#include "eag_ds.h"
#include "eag_trace.h"
#include "eag_buildtree.h"
#include "eag_textparsing.h"

static int parsebuffer_size;
static int max_nr_parses;
char *parsebuffer;
char *iptr;
static int startcol;
int nrofparses;

static void check_parsebuffer_length (int len)
	{ if (len < parsebuffer_size) return;
	  free (parsebuffer);
	  parsebuffer_size = len + 1;
	  parsebuffer = (char *) ckmalloc (parsebuffer_size);
	  wlog ("Dynamically adapting size of parse buffer");
	};

void copy_into_parsebuffer (char *src, int len, int col)
	{ check_parsebuffer_length (len);
	  strncpy (parsebuffer, src, len);
	  parsebuffer[len] = EOFCHAR;
	  startcol = col;
	  iptr = parsebuffer;
	  max_nr_parses = -1;
	  nrofparses = 0;
	};

void copy_file_into_parsebuffer (FILE *fd)
	{ struct stat buf;
	  int fdnr = fileno (fd);
	  int len = 0;
	  char *bufptr = parsebuffer;
	  int c;

	  if (fstat (fdnr, &buf) == -1) panic ("Could not stat input file\n");
	  if ((buf.st_mode & S_IFMT) == S_IFREG)
	     check_parsebuffer_length ((int) buf.st_size);
	  while ((c = fgetc (fd)) != EOF)
	     { *bufptr++ = c;
	       len++;
	       if (len == parsebuffer_size)
		  panic ("too much input from input file\n");
	     };
	  *bufptr = EOFCHAR;
	  iptr = parsebuffer;
	  nrofparses = 0;
	};

#define ERRMSGSIZE 100
char pemsg[ERRMSGSIZE];			/* probable error message */
static char emsg[ERRMSGSIZE];
char *eptr;				/* farthest point reached */
static void synerror ()
	{ if (iptr-eptr > 0)
	     { eptr = iptr;
	       strcpy (pemsg, emsg);	/* save error message */
	     };
	};

void lookahead_failure (char *name)
	{ if (iptr-eptr > 0)
	     { eptr = iptr;
	       sprintf (pemsg, "lookahead failed in rule %s", name);
	     };
	};

/*
   semipredicate end of sentence.
*/
void endofsentence ()
	{ if (*iptr != EOFCHAR)
	     { sprintf (emsg, "end of parse expected");
	       synerror ();
	       pushq (endofsentence);
	       return;
	     }
	  callq ();
	  pushq (endofsentence);
	};

extern void finish_parsing ();
void increment_nrparses ()
	{ nrofparses++;
	  if (tracing) wlog ("A parse was found");
	  if ((max_nr_parses > 0) && (nrofparses >= max_nr_parses))
	    finish_parsing ();
	  callq ();
	  pushq (increment_nrparses);
	};

/*
   returns the current column
*/
int currentcolumn ()
	{ char *tiptr = iptr;
	  int col = -1;
	  while (1)
	     { col++;
	       if (tiptr == parsebuffer)
		  { col += startcol;
		    break;
	          };
	       tiptr--;
	       if (*tiptr == '\n') break;
	     };
	  return (col);
	};

/*
   return the current row
*/
int currentrow ()
	{ char *tiptr = parsebuffer;
	  int row = 0;
	  while (tiptr != iptr)
	     { if (*tiptr == '\n') row++;
	       tiptr++;
	     };
	  return (row);
	};

/*
   parse a terminal
*/
static int match_count;
void parse_terminal ()
	{ string terminal = tops();
	  char *riptr = iptr;
	  register char *tptr, *sptr;
	  match_count++;
	  for (tptr = terminal, sptr = riptr; *tptr; tptr++, sptr++)
	     if (*tptr != *sptr)
	        { sprintf (emsg, "'%s' expected", terminal);
		  synerror ();
		  pushq (parse_terminal);
	          return;
		};
	  pop (1);
	  iptr = sptr;
	  callq ();
	  iptr = riptr;
	  pushs (terminal);
	  pushq (parse_terminal);
	};

int in_set (char *ptr, string set)
	{ register char *sptr;
	  for (sptr = set; *sptr; sptr++)
	     if (*ptr == *sptr) return (1);
	  return (0);
	};

void fail_if_iptr_at_set ()
	{ string set = tops ();
	  if (!in_set (iptr, set))
	     { pop (1);
	       callq ();
	       pushs (set);
	     };
	  pushq (fail_if_iptr_at_set);
	};

void fail_if_iptr_not_at_set ()
	{ string set = tops ();
	  if (in_set (iptr, set))
	     { pop (1);
	       callq ();
	       pushs (set);
	     };
	  pushq (fail_if_iptr_not_at_set);
	};

void parse_set ()
	{ string set = tops ();
	  char *riptr = iptr;
	  if (!in_set (iptr, set))
	     { sprintf (emsg, "character in '%s' expected", set);
	       synerror ();
	       pushq (parse_set);
	       return;
	     };
	  strstore[0] = *iptr;
	  strstore[1] = '\0';
	  iptr++;
	  pop(1);
	  callq ();
	  iptr = riptr;
	  pushs (set);
	  pushq (parse_set);
	};

void parse_non_set ()
	{ string set = tops ();
	  char *riptr = iptr;
	  if (in_set (iptr, set) || (*iptr == EOFCHAR))
	     { sprintf (emsg, "character not in '%s' expected", set);
	       synerror ();
	       pushq (parse_non_set);
	       return;
	     };
	  strstore[0] = *iptr;
	  strstore[1] = '\0';
	  iptr++;
	  pop(1);
	  callq ();
	  iptr = riptr;
	  pushs (set);
	  pushq (parse_non_set);
	};

void parse_set_star ()
	{ string set = pops ();
	  char *riptr = iptr;
	  char *tiptr = iptr;
	  int len = 0;
	  int count;

	  while (in_set (tiptr, set)) { tiptr++; len++; };
	  for (count = 0; count <= len; count++)
	     { strncpy (strstore, riptr, count);
	       strstore[count] = '\0';
	       iptr = riptr+count;
	       callq ();
	     };
	  iptr = riptr;
	  pushs (set);
	  pushq (parse_set_star);
	};

void parse_non_set_star ()
	{ string set = pops ();
	  char *riptr = iptr;
	  char *tiptr = iptr;
	  int len = 0;
	  int count;

	  while (!in_set (tiptr, set) && (*tiptr != EOFCHAR))
	     { tiptr++; len++; };
	  for (count = 0; count <= len; count++)
	     { strncpy (strstore, riptr, count);
	       strstore[count] = '\0';
	       iptr = riptr+count;
	       callq ();
	     };
	  iptr = riptr;
	  pushs (set);
	  pushq (parse_non_set_star);
	};

void parse_set_plus ()
	{ string set = tops ();
	  char *riptr = iptr;
	  char *tiptr = iptr;
	  int len = 1;
	  int count;
	  if (!in_set (tiptr, set))
	     { sprintf (emsg, "character(s) in '%s' expected", set);
	       synerror ();
	       pushq (parse_set_plus);
	       return;
	     };
	  pop(1);
	  tiptr++;
	  while (in_set (tiptr, set)) { tiptr++; len++; };
	  for (count = 1; count <= len; count++)
	     { strncpy (strstore, riptr, count);
	       strstore[count] = '\0';
	       iptr = riptr+count;
	       callq ();
	     };
	  iptr = riptr;
	  pushs (set);
	  pushq (parse_set_plus);
	};

void parse_non_set_plus ()
	{ string set = tops ();
	  char *riptr = iptr;
	  char *tiptr = iptr;
	  int len = 1;
	  int count;
	  if (in_set (tiptr, set) || (*tiptr == EOFCHAR))
	     { sprintf (emsg, "character(s) not in '%s' expected", set);
	       synerror ();
	       pushq (parse_non_set_plus);
	       return;
	     };
	  pop(1);
	  tiptr++;
	  while (!in_set (tiptr, set) && (*tiptr != EOFCHAR))
	     { tiptr++; len++; };
	  for (count = 1; count <= len; count++)
	     { strncpy (strstore, riptr, count);
	       strstore[count] = '\0';
	       iptr = riptr+count;
	       callq ();
	     };
	  iptr = riptr;
	  pushs (set);
	  pushq (parse_non_set_plus);
	};

void parse_set_star_strict ()
	{ string set = pops ();
	  char *riptr = iptr;
	  char *tiptr = iptr;
	  char *dptr = strstore;
	  while (in_set (tiptr, set)) { *dptr++ = *tiptr++; }
	  *dptr = '\0';
	  iptr = tiptr;
	  callq ();
	  iptr = riptr;
	  pushs (set);
	  pushq (parse_set_star_strict);
	};

void parse_non_set_star_strict ()
	{ string set = pops ();
	  char *riptr = iptr;
	  char *tiptr = iptr;
	  char *dptr = strstore;
 
	  while (!in_set (tiptr, set) && (*tiptr != EOFCHAR))
	     { *dptr++ = *tiptr++; }
	  *dptr = '\0';
	  iptr = tiptr;
	  callq ();
	  iptr = riptr;
	  pushs (set);
	  pushq (parse_non_set_star_strict);
	};

void parse_set_plus_strict ()
	{ string set = tops ();
	  char *riptr = iptr;
	  char *tiptr = iptr;
	  char *dptr;
 
	  if (!in_set (tiptr, set))
	     { sprintf (emsg, "character(s) in '%s' expected", set);
	       synerror ();
	       pushq (parse_set_plus_strict);
	       return;
	     };
	  pop(1);
	  dptr = strstore;
	  do { *dptr++ = *tiptr++; }
	  while (in_set (tiptr, set));
	  *dptr = '\0';
	  iptr = tiptr;
	  callq ();
	  iptr = riptr;
	  pushs (set);
	  pushq (parse_set_plus_strict);
	};

void parse_non_set_plus_strict ()
	{ string set = tops ();
	  char *riptr = iptr;
	  char *tiptr = iptr;
	  char *dptr;
 
	  if (in_set (tiptr, set) || (*tiptr == EOFCHAR))
	     { sprintf (emsg, "character(s) not in '%s' expected", set);
	       synerror ();
	       pushq (parse_non_set_plus_strict);
	       return;
	     };
	  pop(1);
	  dptr = strstore;
	  do { *dptr++ = *tiptr++; }
	  while (!in_set (tiptr, set) && (*tiptr != EOFCHAR));
	  *dptr = '\0';
	  iptr = tiptr;
	  callq ();
	  iptr = riptr;
	  pushs (set);
	  pushq (parse_non_set_plus_strict);
	};

/*
   Routines for parsing with lexica
*/
static void parse_lextree (lextree lt, value_list vals, int empty_allowed)
	{ string remainder = lt -> remainder;
	  lexend_list prods = lt -> prods;
	  lextree_list tails = lt -> tails;
	  int ix;
	  if (*remainder)
	     { char *riptr = iptr;
	       char *tptr, *sptr;
	       for (tptr = remainder, sptr = riptr; *tptr; tptr++, sptr++)
		  if (*tptr != *sptr) return;	/* nothing to be done */
	       iptr = sptr;
	       for (ix = 0; ix < prods -> size; ix++)
		  make_lexicon_node (prods -> array[ix], vals);
	       iptr = riptr;
	     }
	  else
	     { char curr = *iptr;
	       if (empty_allowed)
	          for (ix = 0; ix < prods -> size; ix++)
		     make_lexicon_node (prods -> array[ix], vals);
	       if (curr == EOFCHAR) return;
	       if (curr < tails -> size)
		  { lextree slt = tails -> array[(int) curr];
		    iptr++;
		    parse_lextree (slt, vals, 1);
		    iptr--;
		  };
	     };
	};

void do_lexicon_rule (lexicon the_lexicon, int treenr)
	{ value_list vals = the_lexicon -> lex_vals;
	  lextree this_tree = the_lexicon -> lex_trees -> array[treenr];
	  parse_lextree (this_tree, vals, 1);
	};

void do_lexicon_get (lexicon the_lexicon, int treenr)
	{ value_list vals = the_lexicon -> lex_vals;
	  lextree this_tree = the_lexicon -> lex_trees -> array[treenr];
	  parse_lextree (this_tree, vals, 0);
	};

void reinit_textparsing ()
	{ pemsg[0] = '\0';
	  eptr = parsebuffer;
	  nrofparses = 0;
	  match_count = 0;
	};

void init_textparsing (int pbuffer_size, int max)
	{ parsebuffer = (char *) ckmalloc (pbuffer_size);
	  parsebuffer_size = pbuffer_size;
	  max_nr_parses = max;
	  reinit_textparsing ();
	};

void complain_on_found_parses ()
	{ if (nrofparses == 0)
	     { wlog ("No parse was found, due to: %s", pemsg);
	       if (eptr != (char *)0)
	          { eprint_log ("Farthest point reached was");
		    if (*eptr == EOFCHAR) eprint_log (" end of input\n");
		    else
		       { char *ptr;
			 eprint_log (":\n");
			 for (ptr = eptr;
			      (*ptr) && (*ptr != '\n') && (*ptr != EOFCHAR);
			      ptr++) eprint_log ("%c", *ptr);
			 eprint_log ("\n");
		       };
		  };
	     };
	  if (nrofparses > 1) wlog ("%d parses found", nrofparses);
	};

void dump_matches ()
	{ wlog ("\n%d matches tried", match_count);
	};

/*
   Recognition of meta rules
   Zouden deze niet static kunnen
*/
char *miptr;
int mtotal;

/*
   If we reach the end of the string, a meta rule has been recognized
   Save the miptr so that we may properly backtrack
*/
void meta_endofsentence ()
	{ if (*miptr == '\0')
	     { char *smiptr = miptr;
	       callq ();
	       miptr = smiptr;
	     };
	  pushq (meta_endofsentence);
	};

void meta_endofinteger ()
	{ if (mtotal == 0)
	     { callq ();
	       mtotal = 0;
	     };
	  pushq (meta_endofinteger);
	};

void recbup_parse_terminal ()
	{ string terminal = tops();
	  char *riptr = miptr;
	  register char *tptr, *sptr;
	  for (tptr = terminal, sptr = riptr; *tptr; tptr++, sptr++)
	     if (*tptr != *sptr)
		{ /* eventueel een error melding */
		  pushq (recbup_parse_terminal);
	          return;
		};
	  pop(1);
	  miptr = sptr;
	  callq();
	  miptr = riptr;
	  pushs (terminal);
	  pushq (recbup_parse_terminal);
	};

void recbup_parse_integer ()
	{ int num = popi ();
	  mtotal -= num;
	  /* if (0 <= mtotal) callq (); */
	  callq ();
	  mtotal += num;
	  pushi (num);
	  pushq (recbup_parse_integer); 
	};

void recbup_parse_set ()
	{ string set = tops ();
	  char *riptr = miptr;
	  if (!in_set (riptr, set))
	     { pushq (recbup_parse_set);
	       return;
	     };
	  miptr++;
	  pop(1);
	  callq ();
	  miptr = riptr;
	  pushs (set);
	  pushq (recbup_parse_set);
	};

void recbup_parse_non_set ()
	{ string set = tops ();
	  char *riptr = miptr;
	  if (in_set (riptr, set) || !(*riptr))
	     { pushq (recbup_parse_non_set);
	       return;
	     };
	  miptr++;
	  pop(1);
	  callq ();
	  miptr = riptr;
	  pushs (set);
	  pushq (recbup_parse_non_set);
	};

void recbup_parse_set_star ()
	{ string set = pops ();
	  char *riptr = miptr;
	  char *tiptr = miptr;
	  int len = 0;
	  int count;

	  while (in_set (tiptr, set)) { tiptr++; len++; };
	  for (count = 0; count <= len; count++)
	     { miptr = riptr+count;
	       callq ();
	     };
	  miptr = riptr;
	  pushs (set);
	  pushq (recbup_parse_set_star);
	};

void recbup_parse_non_set_star ()
	{ string set = pops ();
	  char *riptr = miptr;
	  char *tiptr = miptr;
	  int len = 0;
	  int count;

	  while (!in_set (tiptr, set) && *tiptr) { tiptr++; len++; };
	  for (count = 0; count <= len; count++)
	     { miptr = riptr+count;
	       callq ();
	     };
	  miptr = riptr;
	  pushs (set);
	  pushq (recbup_parse_non_set_star);
	};

void recbup_parse_set_plus ()
	{ string set = tops ();
	  char *riptr = miptr;
	  char *tiptr = miptr;
	  int len = 1;
	  int count;
	  if (!in_set (tiptr, set))
	     { pushq (recbup_parse_set_plus);
	       return;
	     };
	  pop(1);
	  tiptr++;
	  while (in_set (tiptr, set)) { tiptr++; len++; };
	  for (count = 1; count <= len; count++)
	     { miptr = riptr+count;
	       callq ();
	     };
	  miptr = riptr;
	  pushs (set);
	  pushq (recbup_parse_set_plus);
	};

void recbup_parse_non_set_plus ()
	{ string set = tops ();
	  char *riptr = miptr;
	  char *tiptr = miptr;
	  int len = 1;
	  int count;
	  if (in_set (tiptr, set) || !(*tiptr))
	     { pushq (recbup_parse_non_set_plus);
	       return;
	     };
	  pop(1);
	  tiptr++;
	  while (in_set (tiptr, set) && *tiptr) { tiptr++; len++; };
	  for (count = 1; count <= len; count++)
	     { miptr = riptr+count;
	       callq ();
	     };
	  miptr = riptr;
	  pushs (set);
	  pushq (recbup_parse_non_set_plus);
	};

void recbup_parse_set_star_strict ()
	{ string set = pops ();
	  char *riptr = miptr;
	  char *tiptr = miptr;
	  while (in_set (tiptr, set)) tiptr++;
	  miptr = tiptr;
	  callq ();
	  miptr = riptr;
	  pushs (set);
	  pushq (recbup_parse_set_star_strict);
	};

void recbup_parse_non_set_star_strict ()
	{ string set = pops ();
	  char *riptr = miptr;
	  char *tiptr = miptr;
	  while (!in_set (tiptr, set) && *tiptr) tiptr++;
	  miptr = tiptr;
	  callq ();
	  miptr = riptr;
	  pushs (set);
	  pushq (recbup_parse_non_set_star_strict);
	};

void recbup_parse_set_plus_strict ()
	{ string set = tops ();
	  char *riptr = miptr;
	  char *tiptr = miptr;
 
	  if (!in_set (tiptr, set))
	     { pushq (recbup_parse_set_plus_strict);
	       return;
	     };
	  pop(1);
	  do { tiptr++; }
	  while (in_set (tiptr, set));
	  miptr = tiptr;
	  callq ();
	  miptr = riptr;
	  pushs (set);
	  pushq (recbup_parse_set_plus_strict);
	};

void recbup_parse_non_set_plus_strict ()
	{ string set = tops ();
	  char *riptr = miptr;
	  char *tiptr = miptr;
 
	  if (in_set (tiptr, set) || !(*tiptr))
	     { pushq (recbup_parse_non_set_plus_strict);
	       return;
	     };
	  pop(1);
	  do { tiptr++; }
	  while (!in_set (tiptr, set) && *tiptr);
	  miptr = tiptr;
	  callq ();
	  miptr = riptr;
	  pushs (set);
	  pushq (recbup_parse_non_set_plus_strict);
	};

void rec_parse_terminal ()
	{ string terminal = pops ();
	  value v = popv ();
	  if (v -> tag == undefined_value) callq ();
	  else if (v -> tag == string_value)
	     { if (strcmp (v -> u.str, terminal) == 0) callq ();
	     };
	  pushv (v);
	  pushs (terminal);
	  pushq (rec_parse_terminal);
	};

void rec_parse_integer ()
	{ int inum = popi ();
	  value v = popv ();
	  if (v -> tag == undefined_value) callq ();
	  else if (v -> tag == integer_value)
	     { if (v -> u.inum == inum) callq ();
	     };
	  pushv (v);
	  pushi (inum);
	  pushq (rec_parse_integer);
	};

void rec_parse_set ()
	{ string aset = pops ();
	  value v = popv ();
	  if (v -> tag == undefined_value) callq ();
	  else if (v -> tag == string_value)
	     { miptr = v -> u.str;
	       pushq (meta_endofsentence);
	       pushs (aset);
	       pushq (recbup_parse_set);
	       callq ();
	       pop (3);
	     };
	  pushv (v);
	  pushs (aset);
	  pushq (rec_parse_set);
	};

void rec_parse_non_set ()
	{ string aset = pops ();
	  value v = popv ();
	  if (v -> tag == undefined_value) callq ();
	  else if (v -> tag == string_value)
	     { miptr = v -> u.str;
	       pushq (meta_endofsentence);
	       pushs (aset);
	       pushq (recbup_parse_non_set);
	       callq ();
	       pop (3);
	     };
	  pushv (v);
	  pushs (aset);
	  pushq (rec_parse_non_set);
	};

void rec_parse_set_star ()
	{ string aset = pops ();
	  value v = popv ();
	  if (v -> tag == undefined_value) callq ();
	  else if (v -> tag == string_value)
	     { miptr = v -> u.str;
	       pushq (meta_endofsentence);
	       pushs (aset);
	       pushq (recbup_parse_set_star);
	       callq ();
	       pop (3);
	     };
	  pushv (v);
	  pushs (aset);
	  pushq (rec_parse_set_star);
	};

void rec_parse_non_set_star ()
	{ string aset = pops ();
	  value v = popv ();
	  if (v -> tag == undefined_value) callq ();
	  else if (v -> tag == string_value)
	     { miptr = v -> u.str;
	       pushq (meta_endofsentence);
	       pushs (aset);
	       pushq (recbup_parse_non_set_star);
	       callq ();
	       pop (3);
	     };
	  pushv (v);
	  pushs (aset);
	  pushq (rec_parse_non_set_star);
	};

void rec_parse_set_plus ()
	{ string aset = pops ();
	  value v = popv ();
	  if (v -> tag == undefined_value) callq ();
	  else if (v -> tag == string_value)
	     { miptr = v -> u.str;
	       pushq (meta_endofsentence);
	       pushs (aset);
	       pushq (recbup_parse_set_plus);
	       callq ();
	       pop (3);
	     };
	  pushv (v);
	  pushs (aset);
	  pushq (rec_parse_set_plus);
	};

void rec_parse_non_set_plus ()
	{ string aset = pops ();
	  value v = popv ();
	  if (v -> tag == undefined_value) callq ();
	  else if (v -> tag == string_value)
	     { miptr = v -> u.str;
	       pushq (meta_endofsentence);
	       pushs (aset);
	       pushq (recbup_parse_non_set_plus);
	       callq ();
	       pop (3);
	     };
	  pushv (v);
	  pushs (aset);
	  pushq (rec_parse_non_set_plus);
	};

void rec_parse_set_star_strict ()
	{ string aset = pops ();
	  value v = popv ();
	  if (v -> tag == undefined_value) callq ();
	  else if (v -> tag == string_value)
	     { miptr = v -> u.str;
	       pushq (meta_endofsentence);
	       pushs (aset);
	       pushq (recbup_parse_set_star_strict);
	       callq ();
	       pop (3);
	     };
	  pushv (v);
	  pushs (aset);
	  pushq (rec_parse_set_star_strict);
	};

void rec_parse_non_set_star_strict ()
	{ string aset = pops ();
	  value v = popv ();
	  if (v -> tag == undefined_value) callq ();
	  else if (v -> tag == string_value)
	     { miptr = v -> u.str;
	       pushq (meta_endofsentence);
	       pushs (aset);
	       pushq (recbup_parse_non_set_star_strict);
	       callq ();
	       pop (3);
	     };
	  pushv (v);
	  pushs (aset);
	  pushq (rec_parse_non_set_star_strict);
	};

void rec_parse_set_plus_strict ()
	{ string aset = pops ();
	  value v = popv ();
	  if (v -> tag == undefined_value) callq ();
	  else if (v -> tag == string_value)
	     { miptr = v -> u.str;
	       pushq (meta_endofsentence);
	       pushs (aset);
	       pushq (recbup_parse_set_plus_strict);
	       callq ();
	       pop (3);
	     };
	  pushv (v);
	  pushs (aset);
	  pushq (rec_parse_set_plus_strict);
	};

void rec_parse_non_set_plus_strict ()
	{ string aset = pops ();
	  value v = popv ();
	  if (v -> tag == undefined_value) callq ();
	  else if (v -> tag == string_value)
	     { miptr = v -> u.str;
	       pushq (meta_endofsentence);
	       pushs (aset);
	       pushq (recbup_parse_non_set_plus_strict);
	       callq ();
	       pop (3);
	     };
	  pushv (v);
	  pushs (aset);
	  pushq (rec_parse_non_set_plus_strict);
	};
