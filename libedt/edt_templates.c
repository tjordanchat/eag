/*
   File: edt_templates.c
   Administers templates and template substitution

   CVS ID: "$Id: edt_templates.c,v 1.3 2004/12/25 22:40:51 marcs Exp $"
*/

/* global includes */
#include <stdio.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_memalloc.h>
#include <ebs_textstorage.h>

/* libeag includes */
#include <eag_ds.h>

/* local includes */
#include "edt_editor.h"
#include "edt_lrules.h"
#include "edt_templates.h"
#include "edt_editorparsing.h"

typedef struct template_rec
	{ char *text;
	  int nrsons;
	  int *sonnrs;
	  struct template_rec *next;
	} *template;
#define template_nil ((template) NULL)

static template *template_table;
char *template_buffer;

/*
   A placeholder can always be substituted by a text template.
   (Structure substitution fails when predicates occur in the pattern,
    or if the pattern is not a direct descendant)
*/
static void append_at_list_head (int nodenr, char *text,
				  int nrsons, int *sonnrs)
	{ template new = (template) ckmalloc (sizeof (struct template_rec));
	  new -> text = addto_names (text);
	  new -> nrsons = nrsons;
	  new -> sonnrs = sonnrs;
	  new -> next = template_table[nodenr];
	  template_table[nodenr] = new;
	};

void enter_template_in_list_struct (int nodenr, char *text,
					   int nrsons, int *sonnrs)
	{ int *nsonnrs = (int *) ckcalloc (nrsons, sizeof (int));
	  int ix;
	  for (ix = 0; ix < nrsons; ix++) nsonnrs[ix] = sonnrs[ix];
	  append_at_list_head (nodenr, text, nrsons, nsonnrs);
	};

void enter_template_in_list_text (int nodenr, char *text)
	{ append_at_list_head (nodenr, text, 0, ((int *) NULL));
	};

void update_template_buffer (int nodenr)
	{ template ptr;
	  char *bufptr = template_buffer;
	  for (ptr = template_table [nodenr]; ptr; ptr = ptr -> next)
	     { char *src;
	       for (src = ptr -> text; *src; src++, bufptr++) *bufptr = *src;
	       *bufptr = '\n';
	       bufptr++;
	     };
	  *bufptr = '\0';
	};

void replace_focus_by_template (int nodenr, int nr)
	{ template ptr;
	  int ct;
	  for (ptr = template_table[nodenr], ct = 0;
	       ptr && (ct != nr);
	       ptr = ptr -> next, ct++);
	  if (!ptr) return;
	  replace_focus_by_text (ptr -> text);
	};

void init_templates ()
	{ int ix;
	  template_table = (template *) ckcalloc
			(max_nr_of_rules, sizeof (template));
	  for (ix = 0; ix < max_nr_of_rules; ix++)
	     template_table[ix] = template_nil;
	  template_buffer = (char *) ckmalloc (max_nr_of_rules * screenwidth);
	  template_buffer[0] = '\0';
	};
