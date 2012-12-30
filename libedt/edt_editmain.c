/*
   File: edt_editmain.c
   Main program of editor

   CVS ID: "$Id: edt_editmain.c,v 1.4 2008/06/11 14:58:03 marcs Exp $"
*/

/* general includes */
#include <stdio.h>
#include <stdlib.h>

/* libX.. includes */
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Dialog.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Toggle.h>
#include <X11/Xaw/Viewport.h>

/* libXedt includes */
#include <XSupp.h>
#include <XtSupp.h>
#include <XtArgs.h>
#include <Editor.h>
#include <ImmedLabel.h>

/* libeag includes */
#include <eag_textparsing.h>

/* libedt includes */
#include "edt_lrules.h"
#include "edt_editor.h"
#include "edt_unparser.h"
#include "edt_focus.h"
#include "edt_templates.h"
#include "edt_editorparsing.h"
#include "edt_editmain.h"

/* The status variable keeps track of the current status */
#define browsing 1
#define parsing 2
#define editing 3
#define focussed 4
static int status;

static Widget form, buttonbox, viewport, editor;
static Widget tempviewport, templateviewer;
static Widget parseslabel, complainlabel, statuslabel;

static void SetStatus (int newstatus)
	{ status = newstatus;
	  StartArgs;
	  switch (status)
	     { case browsing: SetArg (XtNlabel, "status: browsing"); break;
	       case parsing:  SetArg (XtNlabel, "status: parsing"); break;
	       case editing:  SetArg (XtNlabel, "status: editing"); break;
	       case focussed: SetArg (XtNlabel, "status: focussed"); break;
	       default:
		  { fprintf (stderr, "attempt to set impossible status\n");
		    exit (4);
		  };
	     };
	  XtSetValues (statuslabel, UseArgs);
	};

static void ActualExit ()
	{ XtDestroyApplicationContext (MyContext);
	  exit (0);
	};

static Widget check_popup, check_dialog;
static void DoSaveLayoutRules ()
	{ XtPopdown (check_popup);
	  write_layout_rules ();
	  ActualExit ();
	};

static void DontSaveLayoutRules ()
	{ XtPopdown (check_popup);
	  ActualExit ();
	};

static void CancelFinalExit ()
	{ XtPopdown (check_popup);
	};

static void CheckToSaveLayoutRules ()
	{ unsigned int mask_return;
	  Window root_return, child_return;
	  int root_x_return, root_y_return, win_x_return, win_y_return;
	  
	  if (!changed_rules) ActualExit ();
	  if (XQueryPointer (MyDisplay, MyRootWindow,
		&root_return, &child_return, &root_x_return, &root_y_return,
		&win_x_return, &win_y_return, &mask_return))
	     { Dimension width, height;
	       StartArgs;
	       SetArg (XtNwidth, &width);
	       SetArg (XtNheight, &height);
	       XtGetValues (check_popup, UseArgs);
	       StartArgs;
	       SetArg (XtNx, win_x_return - width/2);
	       SetArg (XtNy, win_y_return - height/10);
	       XtSetValues (check_popup, UseArgs);
	     };
	  XtPopup (check_popup, XtGrabExclusive);
	};

static void CreateCheckPopup ()
	{ check_popup = XtCreatePopupShell ("check-popup",
			transientShellWidgetClass, MyRootWidget, NoArgs);
	  StartArgs;
	  SetArg (XtNlabel, "Save changes to layout rules");
	  check_dialog = XtCreateManagedWidget ("check-dialog",
			 dialogWidgetClass, check_popup, UseArgs);
	  XawDialogAddButton (check_dialog, "Yes",
			      (XtCallbackProc) DoSaveLayoutRules, NULL);
	  XawDialogAddButton (check_dialog, "No",
			      (XtCallbackProc) DontSaveLayoutRules, NULL);
	  XawDialogAddButton (check_dialog, "Cancel",
			      (XtCallbackProc) CancelFinalExit, NULL);
	  XtRealizeWidget (check_popup);
	};

static void EditChange ()
	{ if (status != focussed) return;
	  SetStatus (editing);
	  changed = 1;
	  StartArgs;
	  SetArg (XtNeditable, TRUE);
	  XtSetValues (editor, UseArgs);
	};

static void Pressed (EditorWidget w,
		      XtPointer client_data, XtPointer call_data)
	{ PressedData data = (PressedData) call_data;
	  if (status == editing) return;
	  SetStatus (focussed);
	  switch (data -> button)
	     { case 1: set_focus_from_pos (data -> x, data -> y); break;
	       case 2: set_focus_to_father (); break;
	       default: ;
	     };
	  update_template_buffer (current_focus -> node -> nodenr);
	  StartArgs;
	  SetArg (XtNsource, template_buffer);
	  XtSetValues (templateviewer, UseArgs);
	  StartArgs;
	  SetArg (XtNfocusBeginX, current_focus -> begin_x);
	  SetArg (XtNfocusBeginY, current_focus -> begin_y);
	  SetArg (XtNfocusEndX, current_focus -> end_x);
	  SetArg (XtNfocusEndY, current_focus -> end_y);
	  XtSetValues (editor, UseArgs);
	};

static void FinishEditorActions ()
	{ SetStatus (browsing);
	  StartArgs;
	  SetArg (XtNeditable, FALSE);
	  SetArg (XtNfocusBeginX, 0);
	  SetArg (XtNfocusBeginY, 0);
	  SetArg (XtNfocusEndX, 0);
	  SetArg (XtNfocusEndY, 0);
	  XtSetValues (editor, UseArgs);
	};

static void UpdateEditorWidgets ()
	{ char Buffer[80];
	  StartArgs;
	  SetArg (XtNsource, unparse_buffer);
	  XtSetValues (editor, UseArgs);
	  sprintf (Buffer, "%d parse%s w%s found", nrofparses,
			(nrofparses == 1)?"":"s",
			(nrofparses == 1)?"as":"ere");
	  StartArgs;
	  SetArg (XtNlabel, Buffer);
	  XtSetValues (parseslabel, UseArgs);
	  if (nrofparses) Buffer[0] = '\0';
	  else sprintf (Buffer, "Probable error: %s", pemsg);
	  StartArgs;
	  SetArg (XtNlabel, Buffer);
	  XtSetValues (complainlabel, UseArgs);
	};

static void Escape ()
	{ FinishEditorActions ();
	  reparse ();
	  UpdateEditorWidgets ();
	};

static void PressedTemplate (EditorWidget w,
			      XtPointer client_data, XtPointer call_data)
	{ PressedData data = (PressedData) call_data;
	  FinishEditorActions ();
	  replace_focus_by_template
		(current_focus -> node -> nodenr, data -> y);
	  reparse ();
	  UpdateEditorWidgets ();
	};

/*
   Saving
*/
static Widget save_popup, save_dialog;
static void (*save_cont)();
static void Dummy ()
	{
	};

static void CancelSave ()
	{ XtPopdown (save_popup);
	};

static void DoSave (Widget w)
	{ char *sname;
	  XtPopdown (save_popup);
	  sname = XawDialogGetValueString (save_dialog);
	  save_unparse_buffer (sname);
	  save_cont ();
	};

static void DontSave (Widget w)
	{ XtPopdown (save_popup);
	  changed = 0;
	  save_cont ();
	};

static void CreateSavePopup ()
	{ save_popup = XtCreatePopupShell ("save-popup",
		       transientShellWidgetClass, MyRootWidget, NoArgs);
	  StartArgs;
	  SetArg (XtNlabel, "Save changes in");
	  SetArg (XtNvalue, "");	
	  save_dialog = XtCreateManagedWidget ("save-dialog",
			dialogWidgetClass, save_popup, UseArgs);
	  XawDialogAddButton (save_dialog, "Yes",
			      (XtCallbackProc) DoSave, NULL);
	  XawDialogAddButton (save_dialog, "No",
			      (XtCallbackProc) DontSave, NULL);
	  XawDialogAddButton (save_dialog, "Cancel",
			      (XtCallbackProc) CancelSave, NULL);
	  XtRealizeWidget (save_popup);
	}

static void ActualSave (void (*ct)())
	{ unsigned int mask_return;
	  Window root_return, child_return;
	  int root_x_return, root_y_return, win_x_return, win_y_return;
	  
	  save_cont = ct;
	  if (!changed)
	     { ct ();
	       return;
	     };
	  StartArgs;
	  SetArg (XtNvalue, Filename);
	  XtSetValues (save_dialog, UseArgs);
	  if (XQueryPointer (MyDisplay, MyRootWindow,
		&root_return, &child_return, &root_x_return, &root_y_return,
		&win_x_return, &win_y_return, &mask_return))
	     { Dimension width, height;
	       StartArgs;
	       SetArg (XtNwidth, &width);
	       SetArg (XtNheight, &height);
	       XtGetValues (save_popup, UseArgs);
	       StartArgs;
	       SetArg (XtNx, win_x_return - width/2);
	       SetArg (XtNy, win_y_return - height/10);
	       XtSetValues (save_popup, UseArgs);
	     };
	  XtPopup (save_popup, XtGrabExclusive);
	};

static void Save ()
	{ ActualSave (Dummy);
	};

/*
   Loading
*/
static Widget load_popup, load_dialog;
static void CancelLoad ()
	{ XtPopdown (load_popup);
	};

static void DoLoad ()
	{ char *sname;
	  XtPopdown (load_popup);
	  FinishEditorActions ();
	  sname = XawDialogGetValueString (load_dialog);
	  load_parsebuffer (sname);
	  UpdateEditorWidgets ();
	};

static void CreateLoadPopup ()
	{ load_popup = XtCreatePopupShell ("load-popup",
		       transientShellWidgetClass, MyRootWidget, NoArgs);
	  StartArgs;
	  SetArg (XtNlabel, "Load from");
	  SetArg (XtNvalue, "");	
	  load_dialog = XtCreateManagedWidget ("load-dialog",
			dialogWidgetClass, load_popup, UseArgs);
	  XawDialogAddButton (load_dialog, "Yes", DoLoad, NULL);
	  XawDialogAddButton (load_dialog, "Cancel", CancelLoad, NULL);
	  XtRealizeWidget (load_popup);
	};

static void ActualLoad ()
	{ unsigned int mask_return;
	  Window root_return, child_return;
	  int root_x_return, root_y_return, win_x_return, win_y_return;
	  
	  StartArgs;
	  SetArg (XtNvalue, Filename);
	  XtSetValues (load_dialog, UseArgs);
	  if (XQueryPointer (MyDisplay, MyRootWindow,
		&root_return, &child_return, &root_x_return, &root_y_return,
		&win_x_return, &win_y_return, &mask_return))
	     { Dimension width, height;
	       StartArgs;
	       SetArg (XtNwidth, &width);
	       SetArg (XtNheight, &height);
	       XtGetValues (load_popup, UseArgs);
	       StartArgs;
	       SetArg (XtNx, win_x_return - width/2);
	       SetArg (XtNy, win_y_return - height/10);
	       XtSetValues (load_popup, UseArgs);
	     };
	  XtPopup (load_popup, XtGrabExclusive);
	};

static void Load ()
	{ ActualSave (ActualLoad);
	};

/*
   Exiting
*/
static void Exit ()
	{ ActualSave (CheckToSaveLayoutRules);
	};

/*
   Layout rule editing
*/
static Widget layout_popup, layout_edit, layout_hor, layout_error;
static rule layout_rule;
static int layout_dir;

static void SetLayoutEditWidgetFocus ()
	{ int endx = 0;
	  int endy = 0;
	  char *ptr;

	  for (ptr = tuple_edit_buffer; *ptr; ptr++)
	     if (*ptr == '\n') { endy++; endx = 0; }
	     else endx++;
	  SetArg (XtNfocusBeginX, 0);
	  SetArg (XtNfocusBeginY, 0);
	  SetArg (XtNfocusEndX, endx);
	  SetArg (XtNfocusEndY, endy);
	  SetArg (XtNeditable, True);
	  SetArg (XtNsource, tuple_edit_buffer);
	};

static void LayoutOnEscape ()
	{ if (try_and_replace_rule (layout_rule, layout_dir))
	     { XtPopdown (layout_popup);
	       Escape ();
	     }
	  else
	     { StartArgs;
	       SetArg (XtNlabel, tuple_error_buffer);
	       XtSetValues (layout_error, UseArgs);
	       StartArgs;
	       SetArg (XtNeditable, True);
	       XtSetValues (layout_edit, UseArgs);
	     };
	};

static void LayoutOk ()
	{ XmsForcedEscapeEditor (layout_edit);
	};

static void LayoutCancel ()
	{ XtPopdown (layout_popup);
	};

static void LayoutHorizontal ()
	{ layout_dir = hor_rule;
	  write_rule_to_buffer (layout_rule, layout_dir);
	  StartArgs;
	  SetLayoutEditWidgetFocus ();
	  XtSetValues (layout_edit, UseArgs);
	};

static void LayoutVertical ()
	{ layout_dir = ver_rule;
	  write_rule_to_buffer (layout_rule, layout_dir);
	  StartArgs;
	  SetLayoutEditWidgetFocus ();
	  XtSetValues (layout_edit, UseArgs);
	};

static void CreateLayoutPopup ()
	{ Widget layout_form, layout_box1, layout_box2;
	  Widget layout_view;
	  layout_popup = XtCreatePopupShell ("layout-popup",
			 transientShellWidgetClass, MyRootWidget, NoArgs);
	  layout_form = XtCreateManagedWidget ("layout-form",
			formWidgetClass, layout_popup, NoArgs);

	  /* First group of buttons */
	  layout_box1 = XtCreateManagedWidget ("layout-box1",
			boxWidgetClass, layout_form, NoArgs);
	  StartArgs;
	  SetCallback (LayoutOk, 0);
	  UseCallback (XtNcallback);
	  (void) XtCreateManagedWidget ("layout-ok", commandWidgetClass,
					layout_box1, UseArgs);
	  StartArgs;
	  SetCallback (LayoutCancel, 0);
	  UseCallback (XtNcallback);
	  (void) XtCreateManagedWidget ("layout-cancel", commandWidgetClass,
					  layout_box1, UseArgs);

	  /* Second group of buttons */
	  layout_box2 = XtCreateManagedWidget ("layout-box2", boxWidgetClass,
					       layout_form, NoArgs);
	  StartArgs;
	  SetCallback (LayoutHorizontal, 0);
	  UseCallback (XtNcallback);
	  layout_hor = XtCreateManagedWidget ("layout-horizontal",
				toggleWidgetClass, layout_box2, UseArgs);
	  StartArgs;
	  SetArg (XtNradioGroup, layout_hor);
	  SetCallback (LayoutVertical, 0);
	  UseCallback (XtNcallback);
	  (void) XtCreateManagedWidget ("layout-vertical", toggleWidgetClass,
					layout_box2, UseArgs);

	  /* Layout editor viewport */
	  layout_view = XtCreateManagedWidget ("layout-viewport",
			viewportWidgetClass, layout_form, NoArgs);
	  StartArgs;
	  SetArg (XtNsink, tuple_edit_buffer);
	  SetCallback (LayoutOnEscape, 0);
	  UseCallback (XtNescapeProc);
	  layout_edit = XtCreateManagedWidget ("layout-editor",
			editorWidgetClass, layout_view, UseArgs);

	  /* The error box */
	  layout_error = XtCreateManagedWidget ("layout-error",
			 labelWidgetClass, layout_form, NoArgs);

	  /* Realize the stuff */
	  XtRealizeWidget (layout_popup);
	};

static void Layout ()
	{ unsigned int mask_return;
	  Window root_return, child_return;
	  int root_x_return, root_y_return, win_x_return, win_y_return;
	  
	  if (status != focussed) return;
	  layout_rule = ruletable [current_focus -> node -> nodenr];
	  if (layout_rule == rule_nil) return;
	  StartArgs;
	  SetArg (XtNstate, True);
	  XtSetValues (layout_hor, UseArgs);
	  layout_dir = hor_rule;
	  write_rule_to_buffer (layout_rule, hor_rule);
	  StartArgs;
	  SetLayoutEditWidgetFocus ();
	  XtSetValues (layout_edit, UseArgs);
	  StartArgs;
	  SetArg (XtNlabel, "");
	  XtSetValues (layout_error, UseArgs);
	  if (XQueryPointer (MyDisplay, MyRootWindow,
		&root_return, &child_return, &root_x_return, &root_y_return,
		&win_x_return, &win_y_return, &mask_return))
	     { Dimension width, height;
	       StartArgs;
	       SetArg (XtNwidth, &width);
	       SetArg (XtNheight, &height);
	       XtGetValues (layout_popup, UseArgs);
	       StartArgs;
	       SetArg (XtNx, win_x_return - width/2);
	       SetArg (XtNy, win_y_return - height/10);
	       XtSetValues (layout_popup, UseArgs);
	     };
	  XtPopup (layout_popup, XtGrabExclusive);
	};

/*
   The buttons
*/
static struct
	{ char *name;
	  void (*command)();
	} buttons [] =
	{{ "Exit", Exit },
	 { "Save", Save },
	 { "Load", Load },
	 { "Layout", Layout },
	};

void register_main_widgets (char *name, int *argc, char **argv)
	{ int i;
	  SetupToolkitApplication (name, "Editor", NULL, 0, 
					NULL, NULL, 0, argc, argv);
	  form = XtCreateManagedWidget ("form", formWidgetClass,
					MyRootWidget, NoArgs);
	  buttonbox = XtCreateManagedWidget ("buttonbox", boxWidgetClass,
					form, NoArgs);
	  for (i=0; i < XtNumber (buttons); i++)
	     { StartArgs;
	       SetCallback (buttons[i].command, 0);
	       UseCallback (XtNcallback);
	       (void) XtCreateManagedWidget (buttons [i].name,
				commandWidgetClass, buttonbox, UseArgs);
	     };
	  viewport = XtCreateManagedWidget ("viewport", viewportWidgetClass,
					form, NoArgs);
	};

void register_editor_widget (int ubuffer_size)
	{ StartArgs;
	  SetArg (XtNbufferSize, ubuffer_size);
	  SetArg (XtNsource, unparse_buffer);
	  SetArg (XtNsink, unparse_buffer);
	  SetCallback (Escape, 0);
	  UseCallback (XtNescapeProc);
	  SetCallback (Pressed, 0);
	  UseCallback (XtNpressedProc);
	  SetCallback (EditChange, 0);
	  UseCallback (XtNeditChangeProc);
	  editor = XtCreateManagedWidget ("editor", editorWidgetClass,
			viewport, UseArgs);
	};

void register_remaining_widgets ()
	{ char Buffer[80];
	  status = browsing;
	  statuslabel = XtCreateManagedWidget ("status", immedLabelWidgetClass,
       				form, NoArgs);
	  sprintf (Buffer, "%d parse%s w%s found", nrofparses,
				(nrofparses == 1)?"":"s",
				(nrofparses == 1)?"as":"ere");
	  StartArgs;
	  SetArg (XtNlabel, Buffer);
	  parseslabel = XtCreateManagedWidget ("nrofparses", labelWidgetClass,
				form, UseArgs);
	  if (nrofparses) Buffer[0] = '\0';
	  else sprintf (Buffer, "Probable error: %s", pemsg);
	  StartArgs;
	  SetArg (XtNlabel, Buffer);
	  complainlabel = XtCreateManagedWidget ("complain", labelWidgetClass,
				form, UseArgs);
	  tempviewport = XtCreateManagedWidget ("tempviewport",
				viewportWidgetClass, form, NoArgs);
	  StartArgs;
	  SetArg (XtNbufferSize, max_nr_of_rules * screenwidth);
	  SetArg (XtNsource, template_buffer);
	  SetCallback (PressedTemplate, 0);
	  UseCallback (XtNpressedProc);
	  templateviewer = XtCreateManagedWidget ("templateviewer",
				editorWidgetClass, tempviewport, UseArgs);
	};

void start_application_loop ()
	{ XtRealizeWidget (MyRootWidget);
	  CreateCheckPopup ();
	  CreateSavePopup ();
	  CreateLoadPopup ();
	  CreateLayoutPopup ();
	  XtAppMainLoop (MyContext);
	};
