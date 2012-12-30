/*
   File: ImmedLabel.c
   Defines the Immediate Label widget, which is a child of the Primitive widget

   CVS ID: "$Id: ImmedLabel.c,v 1.3 2004/12/25 22:28:10 marcs Exp $"
*/

/* standard includes */
#include <stdio.h>
#include <string.h>

/* X11 includes */
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/XawInit.h>

/* local includes */
#include "ImmedLabelP.h"

#define offset(field) XtOffset(ImmedLabelWidget,field)
static XtResource resources [] =
	{{ XtNlabel, XtCLabel, XtRString, sizeof (String),
		offset(immedlabel.label), XtRString, (XtPointer) NULL},
	 { XtNjustify, XtCJustify, XtRJustify, sizeof (XtJustify),
		offset(immedlabel.justify), XtRImmediate,
		(XtPointer) XtJustifyCenter },
	 { XtNinternalHeight, XtCHeight, XtRDimension, sizeof (Dimension),
		offset(immedlabel.internalheight), XtRImmediate, (XtPointer) 2},
	 { XtNinternalWidth, XtCWidth, XtRDimension, sizeof (Dimension),
		offset(immedlabel.internalwidth), XtRImmediate, (XtPointer) 2},
	};
#undef offset

/*
   Widget actions
*/
static void ClassInitialize ();
static void Initialize (Widget request, Widget new);
static Boolean SetValues (Widget current, Widget request, Widget new);
static void Redisplay (Widget w, XEvent *event, Region region);
static void Destroy (Widget w);
static void Resize (Widget w);

ImmedLabelClassRec immedLabelClassRec = {
  { /* core fields */
    /* superclass		*/	(WidgetClass) &primitiveClassRec,
    /* class_name		*/	"ImmedLabel",
    /* widget_size		*/	sizeof(ImmedLabelRec),
    /* class_initialize		*/	ClassInitialize,
    /* class_part_initialize	*/	NULL,
    /* class_inited		*/	FALSE,
    /* initialize		*/	(XtInitProc) Initialize,
    /* initialize_hook		*/	NULL,
    /* realize			*/	XtInheritRealize,
    /* actions			*/	NULL,
    /* num_actions		*/	0,
    /* resources		*/	resources,
    /* num_resources		*/	XtNumber(resources),
    /* xrm_class		*/	NULLQUARK,
    /* compress_motion		*/	TRUE,
    /* compress_exposure	*/	TRUE,
    /* compress_enterleave	*/	TRUE,
    /* visible_interest		*/	FALSE,
    /* destroy			*/	Destroy,
    /* resize			*/	Resize,
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
  /* Primitive class part	*/
  { /* dummy			*/ 	0
  },
  /* ImmedLabel class part	*/
  { /* dummy			*/	0
  },
};

WidgetClass immedLabelWidgetClass = (WidgetClass)&immedLabelClassRec;

static void ClassInitialize ()
	{ extern void XmuCvtStringToJustify();
	  static XtConvertArgRec screenConvertArg[] =
		{{XtWidgetBaseOffset, (caddr_t) XtOffset(Widget, core.screen),
		 sizeof(Screen *)},
		};
	  XawInitializeWidgetSet();
	  XtAddConverter (XtRString, XtRJustify, XmuCvtStringToJustify,
		screenConvertArg, XtNumber(screenConvertArg));
	};

static void DetermineInitialText (ImmedLabelWidget ilw)
	{ String init_text;
	  if (!(ilw -> immedlabel.label)) init_text = ilw -> core.name;
	  else init_text = ilw -> immedlabel.label;
	  ilw -> immedlabel.label = XtNewString (init_text);
	};

static void DetermineInitialHeightAndWidth (ImmedLabelWidget ilw)
	{ if (ilw -> core.height == 0)
	     ilw -> core.height = ilw -> primitive.fontheight +
			2 * ilw -> immedlabel.internalheight;
	  if (ilw -> core.width == 0)
	     { int width = XTextWidth (ilw -> primitive.font,
		     ilw -> immedlabel.label, strlen (ilw -> immedlabel.label));
	       ilw -> core.width = width + 2 * ilw -> immedlabel.internalwidth;
	     };
	};

static void DetermineTextPosition (ImmedLabelWidget ilw)
	{ int textwidth = XTextWidth (ilw -> primitive.font,
		ilw -> immedlabel.label, strlen (ilw -> immedlabel.label));
	  switch (ilw -> immedlabel.justify)
	     { case XtJustifyLeft:
		  ilw -> immedlabel.label_x = ilw -> immedlabel.internalwidth;
		  break;
	       case XtJustifyCenter:
		  ilw -> immedlabel.label_x = (ilw -> core.width - textwidth)/2;
		  break;
	       case XtJustifyRight:
		  ilw -> immedlabel.label_x = ilw -> core.width -
			ilw -> immedlabel.internalwidth - textwidth;
	     };
	  ilw -> immedlabel.label_y =
	     (ilw -> core.height - ilw -> primitive.fontheight)/2 +
	      ilw -> primitive.font -> ascent;
	};

static void DrawText (ImmedLabelWidget ilw)
	{ XClearWindow (XtDisplay (ilw), XtWindow (ilw));
	  XDrawString (XtDisplay (ilw), XtWindow (ilw),
		ilw -> primitive.normalgc,
		ilw -> immedlabel.label_x, ilw -> immedlabel.label_y,
		ilw -> immedlabel.label, strlen (ilw -> immedlabel.label));
	};

static void Initialize (Widget request, Widget new)
	{ ImmedLabelWidget ilw = (ImmedLabelWidget) new;
	  DetermineInitialText (ilw);
	  DetermineInitialHeightAndWidth (ilw);
	  DetermineTextPosition (ilw);
	};

static Boolean SetValues (Widget current, Widget request, Widget new)
	{ ImmedLabelWidget nilw = (ImmedLabelWidget) new;
	  ImmedLabelWidget oilw = (ImmedLabelWidget) current;
	  int redraw = False;
	  if (nilw -> immedlabel.label != oilw -> immedlabel.label)
	     { redraw = strcmp (nilw -> immedlabel.label,
				oilw -> immedlabel.label);
	       XtFree (oilw -> immedlabel.label);
	       nilw -> immedlabel.label=XtNewString (nilw -> immedlabel.label);
	     };
	  if ((oilw -> immedlabel.justify != nilw -> immedlabel.justify) ||
	      (oilw -> primitive.font != nilw -> primitive.font) ||
	      (oilw -> immedlabel.internalheight !=
			nilw -> immedlabel.internalheight) ||
	      (oilw -> immedlabel.internalwidth !=
			nilw -> immedlabel.internalwidth) || redraw)
	     { DetermineTextPosition (nilw);
	       redraw = True;
	     };
	  if (redraw) DrawText (nilw);
	  return (False);
	}; 

static void Resize (Widget w)
	{ ImmedLabelWidget ilw = (ImmedLabelWidget) w;
	  DetermineTextPosition (ilw);
	};

static void Redisplay (Widget w, XEvent *event, Region region)
	{ ImmedLabelWidget ilw = (ImmedLabelWidget) w;
	  DrawText (ilw);
	};

static void Destroy (Widget w)
	{ ImmedLabelWidget ilw = (ImmedLabelWidget) w;
	  XtFree (ilw -> immedlabel.label);
	};
