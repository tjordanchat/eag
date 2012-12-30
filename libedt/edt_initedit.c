/*
   File: edt_initedit.c
   Initializes the editor modules

   CVS ID: "$Id: edt_initedit.c,v 1.3 2004/12/25 22:40:51 marcs Exp $"
*/

/* global includes */
#include <stdio.h>

/* libebs includes */
#include <ebs_bst.h>
#include <ebs_textstorage.h>

/* libeag includes */
#include <eag_init.h>

/* libedt includes */
#include "edt_editor.h"
#include "edt_cpmerge.h"
#include "edt_lrules.h"
#include "edt_unparser.h"
#include "edt_focus.h"
#include "edt_editmain.h"
#include "edt_templates.h"
#include "edt_initedit.h"

static int ubuffer_size;
int init_editor (char *name, int *argc, char **argv)
	{ int status;
	  register_main_widgets (name, argc, argv);
	  status = init_transducer (*argc, argv, 1, &ubuffer_size, &Filename);
	  init_cpmerge ();
	  init_layout_rules (name);
	  init_unparser (ubuffer_size);
	  init_focus ();
	  init_templates ();
	  if (!status)
	     { char Buffer[80];
	       sprintf (Buffer, "in.%s", name);
	       Filename = addto_names (Buffer);
	     };
	  return (status);
	};

void start_editor ()
	{ register_editor_widget (ubuffer_size);
	  register_remaining_widgets ();
	  start_application_loop ();
	};
