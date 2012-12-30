/*
   File: lex.c
   Does lexical analysis of EAGs
  
   CVS ID: "$Id: lex.c,v 1.5 2008/06/18 12:23:47 marcs Exp $"
*/

/* general includes */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_error.h>
#include <ebs_textstorage.h>

/* local includes */
#include "limits.h"
#include "lex.h"
#include "main.h"

/* public variables */
string current_fname;
int lex_errors;
int line, column;
symbol thistoken, nexttoken;
string this_sval;
real this_rval;
int this_ival;

/* private variables to implement LL(2) parse */
static int nextline, nextcolumn;
static string nextsval;
static real nextrval;
static int nextival;

/* Lexer variables */
static FILE *in;
static int thischar, nextchar;
static int lexline, lexcolumn;

static void lexical_error (int lin, int col, char *format, ...)
	{ char buf[MAXSTRLEN];
	  va_list arg_ptr;
	  va_start (arg_ptr, format);
	  vsprintf (buf, format, arg_ptr);
	  va_end (arg_ptr);
	  lex_errors++;
	  error ("lexical error in file %s, line %d, col %d: %s", current_fname, lin, col, buf);
	};

static void lexical_warning (int lin, int col, char *format, ...)
	{ char buf[MAXSTRLEN];
	  va_list arg_ptr;
	  va_start (arg_ptr, format);
	  vsprintf (buf, format, arg_ptr);
	  va_end (arg_ptr);
	  warning ("lexical warning in file %s, line %d, col %d: %s", current_fname, lin, col, buf);
	};

static void nextch ()
	{ if (thischar == '\n') { lexline++; lexcolumn = 1; }
	  else lexcolumn++;
	  thischar = nextchar;
	  if (nextchar != EOF) nextchar = fgetc (in);
	};

static int is_letter (char *value)
	{ if (('a' <= thischar) && (thischar <= 'z'))
	     { *value = thischar;
	       nextch ();
	       return (1);
	     };
	  if (('A' <= thischar) && (thischar <= 'Z'))
	     { *value = thischar;
	       nextch ();
	       return (1);
	     };
	  return (0);
	};

static int is_letgit (char *value)
	{ if (is_letter (value)) return (1);
	  if (('0' <= thischar) && (thischar <= '9'))
	     { *value = thischar;
	       nextch ();
	       return (1);
	     };
	  return (0);
	};

static void may_skip_layout()
	{ while ((thischar == ' ') || (thischar == '\t') ||
		 (thischar == '\n') || (thischar == '#'))
	     if (thischar == '#')
	        do { nextch(); } while (thischar != '\n');
	     else nextch ();
	};

static struct reserved_word_rec
	{ string word;
	  symbol coding;
	} reserved_words_table[] =
	{{ "GRAMMAR",	GRAMMAR_SYMBOL },
	 { "LEXICON",	LEXICON_SYMBOL },
	 { "DEFINES",	DEFINES_SYMBOL },
	 { "USES",	USES_SYMBOL },
	};
#define NR_RESERVED_WORDS 4

static char string_store[MAXSTRLEN];
static void scanidentifier (char firstchar)
	{ char *ptr = string_store + 1;
	  int had_space = 0;
	  int ix;
	  string_store[0] = firstchar;
	  while (1)
	     { if (is_letgit (ptr)) ptr++;
	       else if ((thischar == ' ') || (thischar == '\t'))
		  { nextch();
		    had_space = 1;
		  }
	       else break;
	       *ptr = '\0';
	       if (had_space || v1_compatibility) continue;
	       for (ix = 0; ix < NR_RESERVED_WORDS; ix++)
	          if (streq (string_store, reserved_words_table[ix].word))
		     { nexttoken = reserved_words_table[ix].coding;
		       return;
		     };
	     };
	  *ptr = '\0';
	  nexttoken = IDENTIFIER;
	  nextsval = addto_names (string_store);
	};

#define ESCAPE 0x1B
static int is_a_special (char *ptr)
	{ int lin = lexline;
	  int col = lexcolumn;
	  if (thischar != '\\') return (0);
	  nextch ();
	  switch (thischar)
	     { case 'E': *ptr = ESCAPE; break;
	       case 'n': *ptr = '\n'; break;
	       case 't': *ptr = '\t'; break;
	       case '\\': *ptr = '\\'; break;
	       case '"': *ptr = '"'; break;
	       case '-': *ptr = '-'; break;
	       case '[': *ptr = '['; break;
	       case ']': *ptr = ']'; break;
	       case '{': *ptr = '{'; break;
	       case '}': *ptr = '}'; break;
	       default:
		  lexical_warning (lin, col, "No need for special char \\%c",
				   thischar);
		  *ptr = thischar;
	     };
	  nextch ();
	  return (1);
	};

static void scanstring ()
	{ char *ptr = string_store;
	  nextch ();			/* eat '"' */
	  while (1)
	     { if ((thischar == '\n') || (thischar == EOF))
		  { lexical_error (nextline, nextcolumn, "string contains newline or eof");
		    nextsval = addto_names ("<ERROR>");
		    return;
		  }
	       else if (is_a_special (ptr)) ptr++;
	       else if (thischar == '"') break;
	       else
		  { *ptr = thischar;
		    ptr++;
		    nextch ();
		  };
	     };
	  nextch ();			/* eat closing '"' */
	  *ptr = '\0';
	  nextsval = addto_names (string_store);
	  nexttoken = STRING;
	};

static void scanset (char c)
	{ char *ptr = string_store;
	  nextch ();			/* eat { */
	  while (1)
	     { if ((thischar == '\n') || (thischar == EOF))
		  { lexical_error (nextline, nextcolumn, "set contains newline or eof");
		    nextsval = addto_names ("<ERROR>");
		    return;
		  }
	       else if (is_a_special (ptr)) ptr++;
	       else if (thischar == c) break;
	       else if ((nextchar == '-') && !v1_compatibility)
		  { char start = thischar;
		    char elt;
		    nextch ();		/* start */
		    nextch ();		/* - */
		    if ((thischar == '\n') || (thischar == EOF))
		       { lexical_error (nextline, nextcolumn, "set contains newline or eof");
			 nextsval = addto_names ("<ERROR>");
		         return;
		       }
		    else if ((thischar == '\\') || (thischar == c) || (thischar == '-'))
		       { lexical_error (nextline, nextcolumn, "set range has illegal end value");
			 nextsval = addto_names ("<ERROR>");
			 return;
		       }
		    else if (start > thischar)
		       { lexical_error (nextline, nextcolumn, "set range start > set range end");
			 nextsval = addto_names ("<ERROR>");
			 return;
		       };
		    for (elt = start; elt <= thischar; elt++) *ptr++ = elt;
		    nextch ();		/* eat end */
		  }
	       else
		  { *ptr = thischar;
		    ptr++;
		    nextch ();
		  };
	     };
	  nextch ();			/* eat } */
	  *ptr = '\0';
	  nextsval = addto_names (string_store);
	  nexttoken = SET;
	};

static int is_digit (char *dig)
{ if (('0' <= thischar) && (thischar <= '9'))
    { *dig = thischar;
      nextch ();
      return (1);
    };
  return (0);
}

#define MAXINTDIV10 214748364
#define MAXINTMOD10 8
static void scan_number (char firstdig)
{ char *ptr = string_store + 1;
  char dig;

  /* Eat remaining digits */
  string_store[0] = firstdig;
  while (is_digit (&dig))
    *ptr++ = dig;

  /* Creative Mc Carthy OR! to distinguish between INT and REAL */
  if ((thischar != '.') || (nextchar < '0') || (nextchar > '9'))
    { /* We have an integer denoter, convert it to integer value */
      int value = 0;
      *ptr = '\0';
      for (ptr = string_store; *ptr; ptr++)
	{ int dval = (int)(*ptr - '0');
	  if ((value > MAXINTDIV10) || ((value == MAXINTDIV10) && (dval >= MAXINTMOD10)))
	    { lexical_error (nextline, nextcolumn, "Integer overflow");
	      break;
	    };
	  value = value * 10 + dval;
	};
      nexttoken = INTEGER;
      nextival = value;
    }
  else
    { /* We have the first part of a real denoter, read the rest */
      real value = 0.0;
      char *ploc = ptr;
      nextch ();		/* eat '.' */
      while (is_digit (&dig))
	*ptr++ = dig;

      /* Convert to real */
      *ptr = '\0';
      for (ptr = string_store; *ptr; ptr++)
	{ real dval = (real)(int)(*ptr - '0');
	  value = value * 10.0 + dval;
	};

      nexttoken = REAL;
      nextrval = value / pow (10.0, (real) (ptr - ploc));
    };
}

static void scancolons ()
	{ nextch ();
	  if (thischar == ':')
	     { nextch ();
	       nexttoken = DOUBLECOLON;
	     }
	  else nexttoken = COLON;
	};

static void yieldsymbol (int sy)
	{ nexttoken = sy;
	  nextch ();
	};

static void yieldundefined ()
	{ lexical_error (nextline, nextcolumn, "Illegal character: '%c'", thischar);
	  nextch ();
	  nexttoken = UNDEFINED;
	};

static void scancut ()
	{ if (nextchar == '>')
	     { nextch ();
	       nextch ();
	       nexttoken = CUT;
	     }
	  else yieldundefined ();
	};

void insymbol ()
	{ char firstchar;
	  line = nextline;
	  column = nextcolumn;
	  thistoken = nexttoken;
	  this_ival = nextival;
	  this_rval = nextrval;
	  this_sval = nextsval;
	  may_skip_layout ();
	  nextcolumn = lexcolumn;
	  nextline = lexline;
	  if (is_letter (&firstchar)) scanidentifier (firstchar);
	  else if (is_digit (&firstchar)) scan_number (firstchar);
	  else
	     switch (thischar)
		{ case '!': yieldsymbol (EXCLAMATIONMARK); break;
		  case '"': scanstring (); break;
		  case '$': yieldsymbol (DOLLAR); break;
		  case '(': yieldsymbol (LEFTPARENTHESIS); break;
		  case ')': yieldsymbol (RIGHTPARENTHESIS); break;
		  case '*': yieldsymbol (STAR); break;
		  case '+': yieldsymbol (PLUS); break;
		  case ',': yieldsymbol (COMMA); break;
		  case '-': scancut (); break;
		  case '.': yieldsymbol (POINT); break;
		  case ':': scancolons (); break;
		  case ';': yieldsymbol (SEMICOLON); break;
		  case '>': yieldsymbol (FLOW); break;
		  case '[': yieldsymbol (SUB); break;
		  case ']': yieldsymbol (BUS); break;
		  case '^': yieldsymbol (UP); break;
		  case '{': scanset ('}'); break;
		  case '|': yieldsymbol (PIPE); break;
		  case EOF: yieldsymbol (EOFSYMBOL); break;
		  default: yieldundefined ();
		};
	};

void init_lex ()
	{ lex_errors = 0;
	};

void init_lex_file (FILE *fd, string fname)
	{ current_fname = fname;
	  in = fd;
	  lexline = 1;
	  lexcolumn = -1;
	  thischar = ' ';
	  nextchar = ' ';
	  insymbol ();
	  insymbol ();
	};
