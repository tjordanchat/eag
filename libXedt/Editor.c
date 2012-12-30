/*
   File: Editor.c
   Implements the Editor widget

   CVS ID: "$Id: Editor.c,v 1.3 2004/12/25 22:28:10 marcs Exp $"
*/

/* standard includes */
#include <stdio.h>
#include <string.h>

/* X11 includes */
#include <X11/keysym.h>
#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/XawInit.h>

/* local includes */
#include "EditorP.h"

#define offset(field) XtOffset(EditorWidget,field)
static XtResource resources [] =
	{{ XtNbufferSize, XtCBufferSize, XtRCardinal, sizeof (Cardinal),
		offset(editor.buffersize), XtRImmediate, (XtPointer) 0},
	 { XtNfocusBeginX, XtCFocus, XtRCardinal, sizeof (Cardinal),
		offset(editor.focusbeginx), XtRImmediate, (XtPointer) 0},
	 { XtNfocusBeginY, XtCFocus, XtRCardinal, sizeof (Cardinal),
		offset(editor.focusbeginy), XtRImmediate, (XtPointer) 0},
	 { XtNfocusEndX, XtCFocus, XtRCardinal, sizeof (Cardinal),
		offset(editor.focusendx), XtRImmediate, (XtPointer) 0},
	 { XtNfocusEndY, XtCFocus, XtRCardinal, sizeof (Cardinal),
		offset(editor.focusendy), XtRImmediate, (XtPointer) 0},
	 { XtNinternalHeight, XtCHeight, XtRDimension, sizeof (Dimension),
		offset(editor.internalheight), XtRImmediate, (XtPointer) 2},
	 { XtNinternalWidth, XtCWidth, XtRDimension, sizeof (Dimension),
		offset(editor.internalwidth), XtRImmediate, (XtPointer) 2},
	 { XtNscreenHeight, XtCScreenHeight, XtRCardinal, sizeof (Cardinal),
		offset(editor.screenheight), XtRImmediate, (XtPointer) 0},
	 { XtNscreenWidth, XtCScreenWidth, XtRCardinal, sizeof (Cardinal),
		offset(editor.screenwidth), XtRImmediate, (XtPointer) 0},
	 { XtNsource, XtCBuffer, XtRString, sizeof (String),
		offset(editor.source), XtRImmediate, (XtPointer)NULL},
	 { XtNsink, XtCBuffer, XtRString, sizeof (String),
		offset(editor.sink), XtRImmediate, (XtPointer)NULL},
	 { XtNeditable, XtCEditable, XtRBoolean, sizeof (Boolean),
		offset(editor.editable), XtRImmediate, (XtPointer)TRUE},
	 { XtNescapeProc, XtCCallback, XtRCallback, sizeof (XtPointer),
		offset(editor.escapeProc), XtRCallback, (XtPointer)NULL},
	 { XtNpressedProc, XtCCallback, XtRCallback, sizeof (XtPointer),
		offset(editor.pressedProc), XtRCallback, (XtPointer)NULL},
	 { XtNeditChangeProc, XtCCallback, XtRCallback, sizeof (XtPointer),
		offset(editor.editChangeProc), XtRCallback, (XtPointer) NULL},
	 { XtNwrapAround, XtCWrapAround, XtRBoolean, sizeof (Boolean),
		offset(editor.wraparound), XtRImmediate, (XtPointer)FALSE},
	};
#undef offset

/*
   Editor actions
*/
static void InsertChar (Widget w, XEvent *event,
			 String *params, Cardinal *num_params);
static void InsertNewline (Widget w, XEvent *event,
			    String *params, Cardinal *num_params);
static void ForwardChar (Widget w, XEvent *event,
			  String *params, Cardinal *num_params);
static void BackwardChar (Widget w, XEvent *event,
			   String *params, Cardinal *num_params);
static void UpwardChar (Widget w, XEvent *event,
			 String *params, Cardinal *num_params);
static void DownwardChar (Widget w, XEvent *event,
			   String *params, Cardinal *num_params);
static void DeleteChar (Widget w, XEvent *event,
			 String *params, Cardinal *num_params);
static void TabChar (Widget w, XEvent *ev,
		      String *params, Cardinal *num_params);
static void NoOp (Widget w, XEvent *event,
		   String *params, Cardinal *num_params);
static void Escape (Widget w, XEvent *event,
		     String *params, Cardinal *num_params);
static void PressedProc (Widget w, XEvent *event,
			 String *params, Cardinal *num_params);

static XtActionsRec EditorActions [] =
	{{ "insert-char", InsertChar },
	 { "newline", InsertNewline },
	 { "forward-char", ForwardChar },
	 { "backward-char", BackwardChar },
	 { "up-char", UpwardChar },
	 { "down-char", DownwardChar },
	 { "delete-char", DeleteChar },
	 { "escape-char", Escape },
	 { "tab-char", TabChar },
	 { "no-op", NoOp },
	 { "pressed-proc", PressedProc },
	};
#define NrEditorActions XtNumber(EditorActions)

/*
   Editor translations
*/
static char *EditorTranslations = "\
Ctrl<Key>J:		newline() \n\
Ctrl<Key>M:		newline() \n\
Ctrl<Key>:		no-op() \n\
<Key>Left:		backward-char() \n\
<Key>Right:		forward-char() \n\
<Key>Up:		up-char() \n\
<Key>Down:		down-char() \n\
<Key>KP_4:		backward-char() \n\
<Key>KP_6:		forward-char() \n\
<Key>KP_8:		up-char() \n\
<Key>KP_2:		down-char() \n\
<Key>Linefeed:		newline() \n\
<Key>Return:		newline() \n\
<Key>Delete:		delete-char() \n\
<Key>BackSpace:		delete-char() \n\
<Key>Escape:		escape-char() \n\
<Key>Tab:		tab-char() \n\
<Key>:			insert-char() \n\
<BtnDown>:		pressed-proc() \n\
";

static void ClassInitialize ();
static void Initialize (Widget request, Widget new);
static void Redisplay (Widget w, XEvent *event, Region region);
static void Destroy (Widget w);
static Boolean SetValues (Widget current, Widget request, Widget new);

EditorClassRec editorClassRec = {
  { /* core fields */
    /* superclass		*/	(WidgetClass) &primitiveClassRec,
    /* class_name		*/	"Editor",
    /* widget_size		*/	sizeof(EditorRec),
    /* class_initialize		*/	ClassInitialize,
    /* class_part_initialize	*/	NULL,
    /* class_inited		*/	FALSE,
    /* initialize		*/	(XtInitProc) Initialize,
    /* initialize_hook		*/	NULL,
    /* realize			*/	XtInheritRealize,
    /* actions			*/	EditorActions,
    /* num_actions		*/	NrEditorActions,
    /* resources		*/	resources,
    /* num_resources		*/	XtNumber(resources),
    /* xrm_class		*/	NULLQUARK,
    /* compress_motion		*/	TRUE,
    /* compress_exposure	*/	FALSE,
    /* compress_enterleave	*/	TRUE,
    /* visible_interest		*/	FALSE,
    /* destroy			*/	Destroy,
    /* resize			*/	NULL,
    /* expose			*/	Redisplay,
    /* set_values		*/	(XtSetValuesFunc) SetValues,
    /* set_values_hook		*/	NULL,
    /* set_values_almost	*/	XtInheritSetValuesAlmost,
    /* get_values_hook		*/	NULL,
    /* accept_focus		*/	NULL,
    /* version			*/	XtVersion,
    /* callback_private		*/	NULL,
    /* tm_table			*/	NULL,
    /* query_geometry		*/	XtInheritQueryGeometry,
    /* display_accelerator	*/	XtInheritDisplayAccelerator,
    /* extension		*/	NULL
  },
  /* Simple class part */
  { /* change_sensitive		*/	XtInheritChangeSensitive
  },
  /* Primitive class part */
  { /* dummy			*/	0
  },
  /* Editor class part	*/
  { /* dummy			*/ 	0
  },
};

WidgetClass editorWidgetClass = (WidgetClass)&editorClassRec;

static void ClassInitialize ()
	{ extern void XmuCvtStringToLong ();
	  XawInitializeWidgetSet();
	  editorWidgetClass -> core_class.tm_table = EditorTranslations;
	  XtAddConverter (XtRString, XtRCardinal, XmuCvtStringToLong, NULL, 0);
	};

/*
   Convenience routines
*/
static int LengthOfLine (String line)
	{ char *ptr;
	  int len;
	  for (ptr = line, len = 0; *ptr && (*ptr != '\n'); ptr++, len++);
	  return (len);
	};

static String FindLine (EditorWidget ew, int line)
	{ char *ptr;
	  int ct = 0;
	  for (ptr = ew -> editor.buffer; *ptr && (ct < line); ptr++)
	     if (*ptr == '\n') ct++;
	  if (*ptr) return (ptr);
	  return (NULL);
	};

static String NextLine (String line)
	{ char *ptr;
	  for (ptr = line; *ptr && (*ptr != '\n'); ptr++);
	  if (!(*ptr)) return (NULL);
	  ptr++;
	  if (!(*ptr)) return (NULL);
	  return (ptr);
	};

static int InFocus (EditorWidget ew, int x, int y)
	{ if (y < ew -> editor.focusbeginy) return (0);
	  if ((y == ew -> editor.focusbeginy) &&
	      (x < ew -> editor.focusbeginx)) return (0);
	  if (ew -> editor.focusendy < y) return (0);
	  /* next comparison uses < (x+1) because of type coercion
	     between int x and Cardinal editor.focusendx */
	  if ((ew -> editor.focusendy == y) &&
	      (ew -> editor.focusendx < (x + 1))) return (0);
	  return (1);
	};

/*
   Size determination
*/
static void DetermineSourceWidthHeightAndSize (String src,
			int *width, int *height, int *size)
	{ char *ptr;
	  int sz;
	  int wd = 1;
	  int ht = 1;
	  int lwd = 0;
	  for (ptr = src, sz = 1; *ptr; ptr++, sz++)
	     if (*ptr == '\n')
		{ if (wd < lwd) wd = lwd;
		  ht++;
		  lwd = 0;
		}
	     else lwd++;
	  if (wd < lwd) wd = lwd;
	  *size = sz;
	  *width = wd;
	  *height = ht;
	};

static void AllocateEditorBuffer (EditorWidget ew)
	{ ew -> editor.buffer = (String) XtMalloc (ew -> editor.buffersize);
	  if (ew -> editor.source != NULL)
	     strcpy (ew -> editor.buffer, ew -> editor.source);
	};

static void TryDeferDimensions (EditorWidget ew)
	{ if (ew -> editor.source != NULL)
	     { int SourceWidth, SourceHeight, SourceSize;
	       DetermineSourceWidthHeightAndSize (ew -> editor.source,
			&SourceWidth, &SourceHeight, &SourceSize);
	       ew -> editor.textsize = SourceSize + 1;
	       if (ew -> editor.buffersize == 0)
		   ew -> editor.buffersize = ew -> editor.textsize;
	       if (ew -> editor.buffersize < ew -> editor.textsize)
		  { fprintf (stderr,
			     "buffersize %d too small, adjusting to %d\n",
			     ew -> editor.buffersize, ew -> editor.textsize);
		    ew -> editor.buffersize = ew -> editor.textsize;
		  };
	       if (ew -> editor.screenwidth == 0)
		   ew -> editor.screenwidth = SourceWidth;
	       if (ew -> editor.screenheight == 0)
		   ew -> editor.screenheight = SourceHeight;
	     };
	  if (ew -> core.height == 0)
		ew -> core.height =
		   ew -> editor.screenheight * ew -> primitive.fontheight +
		   2 * ew -> editor.internalheight;
	  if (ew -> core.width == 0)
		ew -> core.width =
		   ew -> editor.screenwidth * ew -> primitive.maxfontwidth +
		   2 * ew -> editor.internalwidth;
	};

static void UpdateFromFocus (EditorWidget ew)
	{ ew -> editor.curx = ew -> editor.focusbeginx;
	  ew -> editor.cury = ew -> editor.focusbeginy;
	  ew -> editor.curlin = FindLine (ew, ew -> editor.cury);
	};

static void Initialize (Widget request, Widget new)
	{ EditorWidget ew = (EditorWidget) new;
	  TryDeferDimensions (ew);
	  AllocateEditorBuffer (ew);
	  UpdateFromFocus (ew);
	};

/*
   Displaying of lines for Expose
*/
static void DrawSingleLine (EditorWidget ew, String Line, int pos)
	{ int len = LengthOfLine (Line);
	  int width = XTextWidth (ew -> primitive.font, Line, len);
	  GC gc,ogc;
	  if (InFocus (ew, 0, pos))
	     { gc = ew -> primitive.invertgc;
	       ogc = ew -> primitive.normalgc;
	     }
	  else
	     { gc = ew -> primitive.normalgc;
	       ogc = ew -> primitive.invertgc;
	     };
	  XDrawImageString (XtDisplay (ew), XtWindow (ew), gc,
		ew -> editor.internalwidth, pos * ew -> primitive.fontheight +
			ew -> editor.internalheight +
			ew -> primitive.font -> ascent,
		Line, len);
	  XFillRectangle (XtDisplay (ew), XtWindow (ew),
		ogc, ew -> editor.internalwidth + width,
		pos * ew -> primitive.fontheight + ew -> editor.internalheight,
		ew -> core.width - width - 2 * ew -> editor.internalwidth,
		ew -> primitive.fontheight);
	};

static void DrawBrokenLine (EditorWidget ew, String Line, int nr)
	{ int firstlen, firstXlen;
	  int secondlen, secondXlen;
	  int thirdlen, thirdXlen;
	  int width;
	  GC fillgc;
	  char *ptr;
	  for (ptr = Line, firstlen = 0;
	       *ptr && (*ptr != '\n') && !InFocus (ew, firstlen, nr);
	       ptr++, firstlen++);
	  XDrawImageString (XtDisplay (ew), XtWindow (ew), 
		ew -> primitive.normalgc, ew -> editor.internalwidth,
		nr * ew -> primitive.fontheight + ew -> editor.internalheight +
		ew -> primitive.font -> ascent,
		Line, firstlen);
	  firstXlen = XTextWidth (ew -> primitive.font, Line, firstlen);
	  for (secondlen = 0;
	       *ptr && (*ptr != '\n') && InFocus (ew, firstlen+secondlen, nr);
	       ptr++, secondlen++);
	  XDrawImageString (XtDisplay (ew), XtWindow (ew),
		ew -> primitive.invertgc,
		ew -> editor.internalwidth + firstXlen,
		nr * ew -> primitive.fontheight + ew -> editor.internalheight +
		ew -> primitive.font -> ascent,
		Line+firstlen, secondlen);
	  secondXlen = XTextWidth (ew -> primitive.font,
		Line+firstlen, secondlen);
	  for (thirdlen = 0;
		*ptr && (*ptr != '\n') && !InFocus (ew,
			firstlen+secondlen+thirdlen, nr);
		ptr++, thirdlen++);
	  XDrawImageString (XtDisplay (ew), XtWindow (ew),
		ew -> primitive.normalgc,
		ew -> editor.internalwidth + firstXlen + secondXlen,
		nr * ew -> primitive.fontheight + ew -> editor.internalheight +
			ew -> primitive.font -> ascent,
		Line+firstlen+secondlen, thirdlen);
	  thirdXlen = XTextWidth (ew -> primitive.font,
		Line+firstlen+secondlen, thirdlen);
	  width = firstXlen + secondXlen + thirdXlen;
	  if (thirdlen != 0) fillgc = ew -> primitive.invertgc;
	  else if (InFocus (ew, firstlen+secondlen, nr))
	     { fillgc = ew -> primitive.normalgc; }
	  else
             { fillgc = ew -> primitive.invertgc; };
	  XFillRectangle (XtDisplay (ew), XtWindow (ew),
		fillgc, ew -> editor.internalwidth + width,
		nr * ew -> primitive.fontheight + ew -> editor.internalheight,
		ew -> core.width - width - 2 * ew -> editor.internalwidth,
		ew -> primitive.fontheight);
	};

static void RedrawTextCursor (EditorWidget ew)
	{ int curtx = ew -> editor.internalwidth + XTextWidth
			(ew -> primitive.font, ew -> editor.curlin,
			 ew -> editor.curx);
	  int curty = ew -> editor.internalheight +
			ew -> editor.cury * ew -> primitive.fontheight + 2;
	  int curby = curty + ew -> primitive.font -> ascent +
			ew -> primitive.font -> descent - 4;
	  XDrawLine (XtDisplay (ew), XtWindow (ew), ew -> primitive.invertgc,
			curtx, curty, curtx, curby);
	};

static void RedrawLines (EditorWidget ew, int first, int last)
	{ int current = first;
	  char *Line = FindLine (ew, first);
	  while ((Line) && (current < last))
	     { if ((current < ew -> editor.focusbeginy) ||
		   (ew -> editor.focusendy < current) ||
		   ((ew -> editor.focusendy == current) &&
		    (ew -> editor.focusendx == 0)))
		  DrawSingleLine (ew, Line, current);
	       else if ((current < ew -> editor.focusendy) &&
		        ((ew -> editor.focusbeginy < current) ||
		         ((ew -> editor.focusbeginy == current) &&
			  (ew -> editor.focusbeginx == 0))))
		  DrawSingleLine (ew, Line, current);
	       else DrawBrokenLine (ew, Line, current);
	       Line = NextLine (Line);
	       current++;
	     };
	  if ((first <= ew -> editor.cury) && (ew -> editor.cury < last))
	     RedrawTextCursor (ew);
	};

#define max(a,b) (((a)>(b))?(a):(b))
#define min(a,b) (((a)>(b))?(b):(a))
static void Redisplay (Widget w, XEvent *event, Region region)
	{ EditorWidget ew = (EditorWidget) w;
	  XExposeEvent *expev = (XExposeEvent *) event;
	  int firstline;
	  int lastline;
	  int topy = max (expev -> y, ew -> editor.internalheight);
	  int boty = min (expev -> y + expev -> height,
			ew -> core.height - ew -> editor.internalheight);
	  firstline = (topy - ew -> editor.internalheight)/
			   ew -> primitive.fontheight;
	  lastline = (boty - ew -> editor.internalheight)/
			   ew -> primitive.fontheight;
	  RedrawLines (ew, firstline, lastline);
	};

static void RedrawAllFrom (EditorWidget ew, int first)
	{ int topy = ew -> editor.internalheight +
			first * ew -> primitive.fontheight;
	  int boty = ew -> core.height - ew -> editor.internalheight;
	  int last = (boty - ew -> editor.internalheight)/
			ew -> primitive.fontheight;
	  XClearArea (XtDisplay (ew), XtWindow (ew), 0, topy,
			ew -> core.width, boty - topy, 0);
	  RedrawLines (ew, first, last);
	};

static void ReallocateEditorBuffer (EditorWidget old, EditorWidget new)
	{ if (old -> editor.buffersize != new -> editor.buffersize)
	     { XtFree (new -> editor.buffer);
	       new -> editor.buffer =
			(String) XtMalloc (new -> editor.buffersize);
	     };
	  strcpy (new -> editor.buffer, new -> editor.source);
	};

static Boolean SetValues (Widget current, Widget request, Widget new)
	{ EditorWidget newew = (EditorWidget) new;
	  EditorWidget oldew = (EditorWidget) current;
	  if ((!newew -> editor.buffer) ||
	      ((newew -> editor.source) &&
	       (strcmp (newew -> editor.buffer, newew -> editor.source))))
	     { TryDeferDimensions (newew);
	       ReallocateEditorBuffer (oldew, newew);
	       UpdateFromFocus (newew);
	     }
	  else if
	     ((oldew -> editor.focusbeginx != newew -> editor.focusbeginx) ||
	      (oldew -> editor.focusbeginy != newew -> editor.focusbeginy) ||
	      (oldew -> editor.focusendx != newew -> editor.focusendx) ||
	      (oldew -> editor.focusendy != newew -> editor.focusendy))
	     UpdateFromFocus (newew);
	  return (True);
	}; 

static void Destroy (Widget w)
	{ EditorWidget ew = (EditorWidget) w;
	  XtFree (ew -> editor.buffer);
	};

/*
   Editor actions
*/
static void NoOp (Widget w, XEvent *event,
		   String *params, Cardinal *num_params)
	{
	};

/*
   Cursor movement
*/
static void ForwardChar (Widget w, XEvent *event,
			  String *params, Cardinal *num_params)
	{ EditorWidget ew = (EditorWidget) w;
	  char *curpos;
	  if (!InFocus (ew, ew -> editor.curx, ew -> editor.cury)) return;
	  curpos = ew -> editor.curlin + ew -> editor.curx;
	  if (*curpos != '\n')
	     { ew -> editor.curx += 1;
	       RedrawLines (ew, ew -> editor.cury, ew -> editor.cury + 1);
	     }
	  else if (ew -> editor.wraparound)
	     { char *newlin = curpos + 1;
	       if (*newlin)
		  { ew -> editor.curx = 0;
		    ew -> editor.cury += 1;
		    ew -> editor.curlin = newlin;
		    RedrawLines (ew, ew -> editor.cury - 1,
					ew -> editor.cury + 1);
		  };
	     };
	};

static void BackwardChar (Widget w, XEvent *event,
			   String *params, Cardinal *num_params)
	{ EditorWidget ew = (EditorWidget) w;
	  if (!InFocus (ew, ew -> editor.curx - 1, ew -> editor.cury)) return;
	  if (ew -> editor.curx != 0)
	     { ew -> editor.curx -= 1;
	       RedrawLines (ew, ew -> editor.cury, ew -> editor.cury + 1);
	     }
	  else if (ew -> editor.wraparound)
	     { if (ew -> editor.cury == 0) return;	/* At start of buffer */
	       ew -> editor.cury -= 1;
	       ew -> editor.curlin = FindLine (ew, ew -> editor.cury);
	       ew -> editor.curx = LengthOfLine (ew -> editor.curlin);
	       RedrawLines (ew, ew -> editor.cury, ew -> editor.cury + 2);
	     };
	};

static void UpwardChar (Widget w, XEvent *event,
			 String *params, Cardinal *num_params)
	{ EditorWidget ew = (EditorWidget) w;
	  char *newlin;
	  int newlen;
	  if (ew -> editor.cury == 0) return;	/* At start of buffer */
	  if (!InFocus (ew, ew -> editor.curx, ew -> editor.cury - 1)) return;
	  newlin = FindLine (ew, ew -> editor.cury - 1);
	  newlen = LengthOfLine (newlin);
	  if (newlen < ew -> editor.curx)
	     { if (ew -> editor.wraparound)
		  ew -> editor.curx = newlen;
	       else return;
	     };
	  ew -> editor.cury -= 1;
	  ew -> editor.curlin = newlin;
	  RedrawLines (ew, ew -> editor.cury, ew -> editor.cury + 2);
	};

static void DownwardChar (Widget w, XEvent *event,
			   String *params, Cardinal *num_params)
	{ EditorWidget ew = (EditorWidget) w;
	  char *newlin = NextLine (ew -> editor.curlin);
	  int newlen;
	  if (!newlin) return;		/* At last line of buffer */
	  if (!InFocus (ew, ew -> editor.curx - 1, ew -> editor.cury + 1))
		return;
	  newlen = LengthOfLine (newlin);
	  if (newlen < ew -> editor.curx)
	     { if (ew -> editor.wraparound)
		  ew -> editor.curx = newlen;
	       else return;
	     };
	  ew -> editor.cury += 1;
	  ew -> editor.curlin = newlin;
	  RedrawLines (ew, ew -> editor.cury - 1, ew -> editor.cury + 1);
	};

/*
   text insertion
*/
static Boolean InsertText (EditorWidget ew, String text)
	{ int offset = strlen (text);
	  char *curpos = ew -> editor.curlin + ew -> editor.curx;
	  char *ptr;
	  if (ew -> editor.buffersize < ew -> editor.textsize + offset)
	     { fprintf (stderr, "buffer too small\n");
	       return (False);
	     };
	  for (ptr = ew -> editor.buffer + ew -> editor.textsize;
	       curpos <= ptr; ptr--) *(ptr+offset) = *ptr;
	  for (ptr = text; *ptr; ptr++, curpos++) *curpos = *ptr;
	  ew -> editor.textsize += offset;
	  return (True);
	};

#define KEYBUFSIZE 10
static XComposeStatus compose_status = {NULL, 0};
static void InsertChar (Widget w, XEvent *event,
			 String *params, Cardinal *num_params)
	{ KeySym keysym;
	  char buf[KEYBUFSIZE];
	  EditorWidget ew = (EditorWidget) w;
	  int len = XLookupString (&event -> xkey, buf, KEYBUFSIZE,
			&keysym, &compose_status);
	  buf[len]='\0';
	  if (!len) return;
	  if (!ew -> editor.editable)
	     XtCallCallbacks (w, XtNeditChangeProc, (XtPointer) NULL);
	  if (!ew -> editor.editable) return;
	  if (!InsertText (ew, buf)) return;
	  if (ew -> editor.cury == ew -> editor.focusendy)
	      ew -> editor.focusendx += 1;
	  ew -> editor.curx += 1;
	  RedrawLines (ew, ew -> editor.cury, ew -> editor.cury + 1);
	};

static void TabChar (Widget w, XEvent *ev,
		      String *params, Cardinal *num_params)
	{ EditorWidget ew = (EditorWidget) w;
	  int nrofblanks;
	  char blanks[10];
	  int i;
	  if (!ew -> editor.editable)
	     XtCallCallbacks (w, XtNeditChangeProc, (XtPointer) NULL);
	  if (!ew -> editor.editable) return;
	  nrofblanks = 8 - (ew -> editor.curx & 7);
	  for (i=0; i < nrofblanks; i++) blanks[i] = ' ';
	  blanks[nrofblanks] = '\0';
	  if (!InsertText (ew, blanks)) return;
	  if (ew -> editor.cury == ew -> editor.focusendy)
	     ew -> editor.focusendx += nrofblanks;
	  ew -> editor.curx += nrofblanks;
	  RedrawLines (ew, ew -> editor.cury, ew -> editor.cury + 1);
	};

static void InsertNewline (Widget w, XEvent *event,
			    String *params, Cardinal *num_params)
	{ EditorWidget ew = (EditorWidget) w;
	  if (!ew -> editor.editable)
	     XtCallCallbacks (w, XtNeditChangeProc, (XtPointer) NULL);
	  if (!ew -> editor.editable) return;
	  if (!InsertText (ew, "\n")) return;
	  ew -> editor.curlin = NextLine (ew -> editor.curlin);
	  if (ew -> editor.cury == ew -> editor.focusendy)
	     ew -> editor.focusendx -= ew -> editor.curx;
	  ew -> editor.curx = 0;
	  ew -> editor.cury += 1;
	  ew -> editor.focusendy += 1;
	  RedrawAllFrom (ew, ew -> editor.cury - 1);
	};

/*
   Character deleting
*/
static void DeleteLeft (EditorWidget ew)
	{ char *ptr;
	  for (ptr = ew -> editor.curlin + ew -> editor.curx; *ptr; ptr++)
	     *(ptr - 1) = *ptr;
	  ptr--;
	  *ptr = '\0';
	  ew -> editor.textsize -= 1;
	};

static void DeleteChar (Widget w, XEvent *event,
			 String *params, Cardinal *num_params)
	{ EditorWidget ew = (EditorWidget) w;
	  if (!ew -> editor.editable)
	     XtCallCallbacks (w, XtNeditChangeProc, (XtPointer) NULL);
	  if (!ew -> editor.editable) return;
	  if ((ew -> editor.cury == ew -> editor.focusbeginy) &&
	      (ew -> editor.curx == ew -> editor.focusbeginx)) return;
	  if (ew -> editor.curx != 0)
	     { /* Delete char */
	       DeleteLeft (ew);
	       ew -> editor.curx -= 1;
	       if (ew -> editor.cury == ew -> editor.focusendy)
		  ew -> editor.focusendx -= 1;
	       RedrawLines (ew, ew -> editor.cury, ew -> editor.cury + 1);
	     }
	  else if (ew -> editor.cury)
	     { /* Delete newline */
	       char *prevlin = FindLine (ew, ew -> editor.cury - 1);
	       int prevlen = LengthOfLine (prevlin);
	       DeleteLeft (ew);
	       ew -> editor.curx = prevlen;
	       if (ew -> editor.cury == ew -> editor.focusendy)
		  ew -> editor.focusendx += prevlen;
	       ew -> editor.cury -= 1;
	       ew -> editor.curlin = prevlin;
	       ew -> editor.focusendy -= 1;
	       RedrawAllFrom (ew, ew -> editor.cury);
	     };
	};

/*
   Escape ends the editing
   If sink isnt nil copy the buffer into the sink.
*/
static void EscapeEditor (Widget w)
	{ EditorWidget ew = (EditorWidget) w;
	  ew -> editor.editable = FALSE;
	  if (ew -> editor.sink)
	     strcpy (ew -> editor.sink, ew -> editor.buffer);
	  XtCallCallbacks (w, XtNescapeProc, (XtPointer) NULL);
	};

static void Escape (Widget w, XEvent *event,
		     String *params, Cardinal *num_params)
	{ EscapeEditor (w);
	};

static void PressedProc (Widget w, XEvent *event,
			 String *params, Cardinal *num_params)
	{ EditorWidget ew = (EditorWidget) w;
	  XButtonPressedEvent *bev = (XButtonPressedEvent *) event;
	  int edx = (bev -> x - ew -> editor.internalwidth)/
			ew -> primitive.maxfontwidth;
	  int edy = (bev -> y - ew -> editor.internalheight)/
			ew -> primitive.fontheight;
	  PressedData calldata = (PressedData) XtMalloc
			(sizeof (struct pressed_data_rec));  
	  calldata -> x = edx;
	  calldata -> y = edy;
	  calldata -> button = bev -> button;
	  XtCallCallbacks (w, XtNpressedProc, (XtPointer) calldata);
	  XtFree ((char *) calldata);
	};

/* Convenience routines */
void XmsForcedEscapeEditor (Widget w)
	{ EscapeEditor (w);
	};
