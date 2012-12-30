/*
   File: edt_lrules.c
   Handles the storage, reading, writing and updating of the unparsing rules

   CVS ID: "$Id: edt_lrules.c,v 1.4 2007/09/05 08:51:42 marcs Exp $"
*/

/* Global includes */
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_error.h>
#include <ebs_memalloc.h>
#include <ebs_textstorage.h>

/* local includes */
#include "edt_editor.h"
#include "edt_lrules.h"

/*
   Allocation of tuples. The tuples may change but
   the number of rules never.
*/
static tuple free_tuples;
static tuple new_tuple (int type, string text, int relates,
			 int hor_off, int vert_off, int sonnr)
	{ tuple new;
	  if (free_tuples != tuple_nil)
	     { new = free_tuples;
	       free_tuples = free_tuples -> next;
	     }
	  else new = (tuple) ckmalloc (sizeof (struct tuple_rec));
	  new -> type = type;
	  new -> text = text;
	  new -> relates = relates;
	  new -> hor_offset = hor_off;
	  new -> vert_offset = vert_off;
	  new -> sonnr = sonnr;
	  new -> next = tuple_nil;
	  return (new);
	};

static void deallocate_tuples (tuple old)
	{ tuple ptr;
	  if (old == tuple_nil) return;
	  for (ptr = old; ptr -> next != tuple_nil; ptr = ptr -> next);
	  ptr -> next = free_tuples;
	  free_tuples = old;
	};

static tuple append_tuple (tuple src, int type, char *text, int relates,
			    int hor_off, int vert_off, int sonnr)
	{ tuple ptr;
	  if (src == tuple_nil)
	     return (new_tuple (type, text, relates, hor_off, vert_off, sonnr));
	  for (ptr = src; ptr -> next != tuple_nil; ptr = ptr -> next);
	  ptr -> next = new_tuple (type, text, relates, hor_off,
				vert_off, sonnr);
	  return (src);
	};

static tuple copy_tuples (tuple old)
	{ tuple new = tuple_nil;
	  tuple ptr;
	  if (old == tuple_nil) return (tuple_nil);
	  for (ptr = old; ptr != tuple_nil; ptr = ptr -> next)
	     new = append_tuple (new, ptr -> type, ptr -> text, ptr -> relates,
			ptr -> hor_offset, ptr -> vert_offset, ptr -> sonnr);
	  return (new);
	};

static rule new_rule (tuple hrule, tuple vrule)
	{ rule new = (rule) ckmalloc (sizeof (struct rule_rec));
	  new -> hrule = hrule;
	  new -> vrule = vrule;
	  return (new);
	};

int changed_rules;
static void replace_rule (rule nrule, int dir, tuple ntuple)
	{ tuple old;
	  if (dir == ver_rule)
	       { old = nrule -> vrule; nrule -> vrule = ntuple; }
	  else { old = nrule -> hrule; nrule -> hrule = ntuple; };
	  deallocate_tuples (old);
	  changed_rules = 1;
	};

/*
   Register the correct filename for the unparsing rules
*/
static char *fname;
static void establish_tuplefile (char *gram)
	{ strcpy (strstore, gram);		/* will become gram */
	  strcat (strstore, ".rules");
	  fname = addto_names (strstore);
	};

static FILE *open_tuple_file (char *mode)
	{ FILE *f = fopen (fname, mode);
	  if (f) return (f);
	  panic ("could not open file '%s' for %s", fname,
		 (mode[0] == 'r')?"reading":"writing");
	  return ((FILE *) NULL);
	};

/*
   Tuples may be read or written either from/to file
   or from/to the tuple_edit_buffer
*/
char *tuple_edit_buffer;
static char *editptr;

static int tuple_error;
char *tuple_error_buffer;

static int use_file;
static FILE *input;
static FILE *output;

#define MAXBUFLEN 2048
static char inputbuffer[MAXBUFLEN];
static char *inputptr;

static int refill_input_buffer ()
	{ inputptr = inputbuffer;
	  if (use_file) return (!fgets (inputbuffer, MAXBUFLEN, input));
	  else
	     { char *dptr;
	       if (!(*editptr)) return (1);
	       for (dptr = inputbuffer;
		    (*editptr != '\0') && (*editptr != '\n');
		    editptr++, dptr++) *dptr = *editptr;
	       *dptr++ = '\n';
	       *dptr = '\0';
	       if ((*editptr)) editptr++;
	       return (0);
	     };
	};

/*
   The tuples are read by an ordinary LL1 parser
   'inputptr' will always point to the first unrecognized character
*/
static int linenr;
#define colnr ((int) (inputptr - inputbuffer))
static int eof;
static void may_read_next_line ()
	{ while (!eof && !(*inputptr))
	     if (refill_input_buffer ())
		{ eof = 1;
		  *inputptr = '\0';
		}
	     else linenr++;
	};

#define lookahead_char(c) (*inputptr == c)
static int is_char (char c)
	{ if (lookahead_char (c))
	     { inputptr++;
	       return (1);
	     };
	  return (0);
	};

static int lookahead_string (string str)
	{ char *ptr, *ptr2;
	  for (ptr = str, ptr2 = inputptr; *ptr; ptr++, ptr2++)
	     if (*ptr != *ptr2) return (0);
	  return (1);
	};
	
static int is_string (string str)
	{ char *ptr, *ptr2;
	  for (ptr = str, ptr2 = inputptr; *ptr; ptr++, ptr2++)
	     if (*ptr != *ptr2) return (0);
	  inputptr = ptr2;
	  return (1);
	};

static void skip_layout ()
	{ may_read_next_line ();
	  while (is_char (' ') || is_char ('\t') || is_char ('\n'))
	     may_read_next_line ();
	};

static int is_token (string token)
	{ if (is_string (token))
	     { skip_layout ();
	       return (1);
	     };
	  return (0);
	};

static void expected_string (string str)
	{ sprintf (tuple_error_buffer, "%s expected at line %d, col %d",
			str, linenr, colnr + 1);
	  tuple_error = 1;
	  if (use_file)
	     panic ("error while reading tuples: %s", tuple_error_buffer);
	};

static void should_be_string (string str)
	{ if (is_string (str)) return;
	  expected_string (str);
	};

static void should_be_token (string token)
	{ if (is_token (token)) return;
	  expected_string (token);
	};

static int should_be_number ()
	{ int nr;
	  if (!isdigit (*inputptr)) expected_string ("number");
	  if (tuple_error) return (0);
	  nr = *inputptr - '0';
	  inputptr++;
	  while (isdigit (*inputptr))
	     { nr = 10 * nr + *inputptr - '0';
	       inputptr++;
	     };
	  skip_layout ();
	  return (nr);
	};

#define eoln (eof || !(*inputptr))
static string should_be_string_ending_with (string close)
	{ char *ptr = strstore;
	  while (!tuple_error && !lookahead_string (close) && !eoln)
	     if (iscntrl (*inputptr)) expected_string (close);
	     else if (*inputptr == '\\')
		{ inputptr ++;
		  if (iscntrl (*inputptr)) expected_string (close);
		  else if (*inputptr == '"') *ptr++ = '"';
	          else if (*inputptr == '}') *ptr++ = '}';
		  else if (*inputptr == 'n') *ptr++ = '\n';
		  else if (*inputptr == 't') *ptr++ = '\t';
		  else if (*inputptr == '\\') *ptr++ = '\\';
		  inputptr ++;
		}
	     else *ptr++ = *inputptr++;
	  *ptr = '\0';
	  if (tuple_error) return (addto_names (strstore));
	  should_be_string (close);
	  return (addto_names (strstore));
	};

/*
   Initialize reading and writing of tuples
*/
static void init_input_buffer ()
	{ tuple_error = 0;
	  eof = 0;
	  linenr = 0;
	  inputptr = inputbuffer;
	  editptr = tuple_edit_buffer;
	  *inputptr = '\0';
	  skip_layout ();
	};

static void init_reading ()
	{ use_file = 1;
	  input = open_tuple_file ("r");
	  free_tuples = tuple_nil;
	  init_input_buffer ();
	};

static void init_writing ()
	{ use_file = 1;
	  output = open_tuple_file ("w");
	};

/*
   Finish reading and writing of tuples
*/
static void finish_reading ()
	{ fclose (input);
	  use_file = 0;
	  changed_rules = 0;
	};

static void finish_writing ()
	{ fclose (output);
	  use_file = 0;
	  changed_rules = 0;
	};

/*
   These variables are needed for intermediate storage during parsing
   The editor generator will indicate the amount of storage necessary
*/
int max_nr_of_tuples;
int max_nr_of_rules;
rule *ruletable;

static int *x_pos;
static int *y_pos;
static int *used_nrs;

static tuple read_alternative ()
	{ tuple current = tuple_nil;
	  int count = 0;		/* Keeps track of sons */
	  int i;
	  int lasty = linenr;
	  for (i = 0;;i++)
	     { string text;
	       int type;
	       int sonnr;
	       int relates;
	       int hor_off = 0;
	       int vert_off = 0;
	       x_pos [i] = colnr;
	       y_pos [i] = linenr;

	       /* find out to whom we are related */
	       for (relates = 0; x_pos[i] != x_pos[relates]; relates++);

	       /* if we find a '.' we really want to insert layout,
		  related either to the previous position or to
		  a related position
	       */
	       if (is_token ("."))
		  { hor_off = colnr - x_pos[i];
		    vert_off = linenr - y_pos[i];
		  }

	       /* if we are not related to a previous tuple and
		  we went to a new line register the horizontal offset
		  from the start of the line
	       */
	       if ((relates != i) && y_pos[i] != lasty) hor_off = colnr;

	       /* add the vertical difference between the starts of
		  the current and the previous tuple to the vertical
		  offset
	       */
	       vert_off += (i==0)?0:y_pos[i]-y_pos[i-1];

	       if (is_char ('"'))
		  { text = should_be_string_ending_with ("\"");
		    type = type_terminal;
		    sonnr = -1;
		  }
	       else if (is_char ('#'))
		  { should_be_string (typed_open_symbol);
		    if (tuple_error) break;
		    text = should_be_string_ending_with (typed_close_symbol);
		    if (tuple_error) break;
		    type = type_forcednonterminal;
		    sonnr = used_nrs [count];
		    count++;
		  }
	       else if (is_string (typed_open_symbol))
		  { text = should_be_string_ending_with (typed_close_symbol);
		    if (tuple_error) break;
		    type = type_nonterminal;
		    sonnr = used_nrs [count];
		    count++;
		  }
	       else if (is_char ('{'))
		  { text = should_be_string_ending_with ("}");
		    if (tuple_error) break;
		    type = type_set;
		    sonnr = used_nrs [count];
		    count++;
		  }
	       else break;

	       current = append_tuple (current, type, text, relates,
				       hor_off, vert_off, sonnr);
	       lasty = linenr;
	       skip_layout ();
	     };
	  if (tuple_error)
	     { deallocate_tuples (current);
	       return ((tuple) NULL);
	     }
	  else return (current);
	};

static int rules_consistent (tuple otuple, tuple ntuple)
	{ tuple optr, nptr;
	  for (optr = otuple, nptr = ntuple;
	       (optr != tuple_nil) && (nptr != tuple_nil);
	       optr = optr -> next, nptr = nptr -> next)
	    { if ((optr -> type == type_terminal) &&
		  (nptr -> type == type_terminal))
		 { if (strcmp (optr -> text, nptr -> text) != 0) return (0);
		 }
	      else if (((optr -> type == type_forcednonterminal) ||
			(optr -> type == type_nonterminal)) &&
		       ((nptr -> type == type_forcednonterminal) ||
			(nptr -> type == type_nonterminal)))
		 { if (strcmp (optr -> text, nptr -> text) != 0) return (0);
	         }
	      else if ((optr -> type == type_set) &&
		       (nptr -> type == type_set))
		 { if (strcmp (optr -> text, nptr -> text) != 0) return (0);
		 }
	      else return (0);
	    };
	  return (optr == nptr);	/* should equal to nil */
	};

int try_and_replace_rule (rule lrule, int dir)
	{ tuple otuple = (dir == hor_rule)?lrule -> hrule:lrule -> vrule;
	  tuple ntuple;

	  init_input_buffer ();
	  ntuple = read_alternative ();
	  if (tuple_error) return (0);
	  if (!rules_consistent (otuple, ntuple))
	     { strcpy (tuple_error_buffer,
				"inconsistency between old and new rule");
	       return (0);
	     };
	  replace_rule (lrule, dir, ntuple);
	  return (1);
	};

/*
   Writing of a rule
*/
static void write_string_to_buffer (string str, int *ccol)
	{ char *ptr;
	  int len = 1;
	  strcat (tuple_edit_buffer, "\"");
	  for (ptr = str; *ptr; ptr++)
	     if (*ptr == '"')
		{ strcat (tuple_edit_buffer, "\\\""); len += 2; }
	     else if (*ptr == '}')
		{ strcat (tuple_edit_buffer, "\\}"); len += 2; }
	     else if (*ptr == '\n')
		{ strcat (tuple_edit_buffer, "\\n"); len += 2; }
	     else if (*ptr == '\t')
		{ strcat (tuple_edit_buffer, "\\t"); len += 2; }
	     else if (*ptr == '\\')
		{ strcat (tuple_edit_buffer, "\\\\"); len += 2; }
	     else
		{ char buf[2];
		  buf[0] = *ptr;
		  buf[1] = '\0';
		  strcat (tuple_edit_buffer, buf);
		  len++;
		};
	  strcat (tuple_edit_buffer, "\"");
	  len++;
	  *ccol += len;
	};

static void write_spaces_to_buffer (int amount, int *ccol)
	{ int i;
	  for (i = 0;i < amount; i++) strcat (tuple_edit_buffer, " ");
	  *ccol += amount;
	};

static void write_alternative (tuple current)
	{ tuple ptr;
	  int ccol = 0;
	  int i, j;

	  tuple_edit_buffer[0] = '\0';
	  for (ptr = current, i = 0; ptr != tuple_nil; ptr = ptr -> next, i++)
	     { /* layout stuff */
	       if (ptr -> vert_offset)
		  { for (j = 0; j < ptr -> vert_offset; j++)
		       strcat (tuple_edit_buffer, "\n");
		    ccol = 0;
		  };
	       if (ptr -> relates < i)
		  write_spaces_to_buffer (x_pos[ptr -> relates], &ccol);
	       if (ptr -> hor_offset)
		  { strcat (tuple_edit_buffer, ".");
		    ccol += 1;
		    write_spaces_to_buffer (ptr -> hor_offset - 1, &ccol);
		  };

	       /* save position for related tuples */
	       x_pos[i] = ccol;
	       switch (ptr -> type)
		  { case type_terminal:
		       write_string_to_buffer (ptr -> text, &ccol);
		       break;
		    case type_forcednonterminal:
		       { strcat (tuple_edit_buffer, "#");
			 ccol++;
		       }	/* fall thru */
		    case type_nonterminal:
		       { strcat (tuple_edit_buffer, typed_open_symbol);
			 ccol += strlen (typed_open_symbol);
			 strcat (tuple_edit_buffer, ptr -> text);
			 ccol += strlen (ptr -> text);
			 strcat (tuple_edit_buffer, typed_close_symbol);
			 ccol += strlen (typed_close_symbol);
		       };
		       break;
		    case type_set:
		       { strcat (tuple_edit_buffer, "{}");
			 ccol += 2;
		       }
		    default: break;
		  };
	     };
	};

void write_rule_to_buffer (rule lrule, int dir)
	{ if (dir == hor_rule) write_alternative (lrule -> hrule);
	  else write_alternative (lrule -> vrule);
	};

/*
   Reading and writing of rules
*/
static void read_rule ()
	{ int rulenr;
	  int count = 0;
	  tuple hrule = tuple_nil;
	  tuple vrule = tuple_nil;
	  should_be_token ("type");
	  rulenr = should_be_number ();

	  while (is_token (","))
	     { used_nrs [count] = should_be_number ();
	       count++;
	     };
	  if (is_token ("::"))
	     { hrule = read_alternative ();
	       vrule = copy_tuples (hrule);
	     }
	  else if (is_token (":H:"))
	     { hrule = read_alternative ();
	       should_be_token (":V:");
	       vrule = read_alternative ();
	     }
	  else expected_string ("::");
	  ruletable [rulenr] = new_rule (hrule, vrule);
	};

static void write_rule (int idx)
	{ tuple ptr;
	  rule wrule = ruletable[idx];
	  if (wrule == rule_nil) return;
	  fprintf (output, "type %d", idx);
	  for (ptr = wrule -> hrule; ptr != tuple_nil; ptr = ptr -> next)
	     if (ptr -> sonnr != -1) fprintf (output, ", %d", ptr -> sonnr);
	  fputs ("\n:H:\n", output);
	  write_alternative (wrule -> hrule);
	  fputs (tuple_edit_buffer, output);
	  fputs ("\n:V:\n", output);
	  write_alternative (wrule -> vrule);
	  fputs (tuple_edit_buffer, output);
	  fputc ('\n', output);
	};

/*
   The header of the tuple file contains some info on the storage
   needed during parsing.
*/
static void read_header ()
	{ int ix;
	  should_be_token (max_rule_header);
	  max_nr_of_rules = should_be_number ();
	  should_be_token (max_tuple_header);
	  max_nr_of_tuples = should_be_number ();
	  ruletable = (rule *) ckcalloc (max_nr_of_rules, sizeof (rule));
	  for (ix = 0; ix < max_nr_of_rules; ix++) ruletable[ix] = rule_nil;
	  x_pos = (int *) ckcalloc (max_nr_of_tuples, sizeof (int));
	  y_pos = (int *) ckcalloc (max_nr_of_tuples, sizeof (int));
	  used_nrs = (int *) ckcalloc (max_nr_of_tuples, sizeof (int));
	  tuple_edit_buffer = (char *) ckmalloc (max_nr_of_rules * MAXBUFLEN);
	  tuple_error_buffer = (char *) ckmalloc (MAXBUFLEN);
	};

static void write_header ()
	{ fprintf (output, "%s %d\n", max_rule_header, max_nr_of_rules);
	  fprintf (output, "%s %d\n", max_tuple_header, max_nr_of_tuples);
	};

/*
   Reading and writing of the tuples
*/
static void read_layout_rules ()
	{ init_reading ();
	  read_header ();
	  while (!eof) read_rule ();
	  finish_reading ();
	};

void write_layout_rules ()
	{ int ix;
	  init_writing ();
	  write_header ();
	  for (ix = 0; ix < max_nr_of_rules; ix++) write_rule (ix);
	  finish_writing ();
	};

void init_layout_rules (char *gram)
	{ establish_tuplefile (gram);
	  read_layout_rules ();
	};
