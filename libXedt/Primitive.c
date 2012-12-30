/*
   File: Primitive.c
   Implements the Primitive widget, which is a child of the Simple widget (Xaw)

   CVS ID: "$Id: Primitive.c,v 1.3 2004/12/25 22:28:11 marcs Exp $"
*/

/* X11 includes */
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>

/* local includes */
#include "PrimitiveP.h"

#define offset(field) XtOffset(PrimitiveWidget,field)
static XtResource resources [] =
	{{ XtNforeground, XtCForeground, XtRPixel, sizeof(Pixel),
		offset(primitive.foreground), XtRString, "XtDefaultForeground"},
	 { XtNfont, XtCFont, XtRFontStruct, sizeof(XFontStruct *),
		offset(primitive.font), XtRString, XtDefaultFont},
	};
#undef offset

static void Initialize (Widget request, Widget new);
static void Destroy (Widget w);
static Boolean SetValues (Widget current, Widget request, Widget new);

PrimitiveClassRec primitiveClassRec = {
  { /* core fields */
    /* superclass		*/	(WidgetClass) &simpleClassRec,
    /* class_name		*/	"Primitive",
    /* widget_size		*/	sizeof(PrimitiveRec),
    /* class_initialize		*/	NULL,
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
    /* resize			*/	NULL,
    /* expose			*/	NULL,
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
};

WidgetClass primitiveWidgetClass = (WidgetClass)&primitiveClassRec;

static void NewGCs (PrimitiveWidget pw)
	{ XGCValues Gcv;

	  Gcv.function = GXcopy;
	  Gcv.foreground = pw -> primitive.foreground;
	  Gcv.background = pw -> core.background_pixel;
	  Gcv.line_width = 0;
	  Gcv.line_style = LineSolid;
	  Gcv.join_style = JoinRound;
	  Gcv.fill_style = FillSolid;
	  Gcv.arc_mode = ArcPieSlice;
	  Gcv.font = pw -> primitive.font -> fid;
	  pw -> primitive.normalgc = XtGetGC ((Widget) pw,
		GCFunction | GCForeground | GCBackground |
		GCLineWidth | GCLineStyle | GCJoinStyle |
		GCFillStyle | GCFont | GCArcMode, &Gcv);

	  Gcv.function = GXcopy;
	  Gcv.foreground = pw -> core.background_pixel;
	  Gcv.background = pw -> primitive.foreground;
	  Gcv.line_width = 0;
	  Gcv.line_style = LineSolid;
	  Gcv.join_style = JoinRound;
	  Gcv.fill_style = FillSolid;
	  Gcv.arc_mode = ArcPieSlice;
	  Gcv.font = pw -> primitive.font -> fid;
	  pw -> primitive.invertgc = XtGetGC ((Widget) pw,
		GCFunction | GCForeground | GCBackground |
		GCLineWidth | GCLineStyle | GCJoinStyle |
		GCFillStyle | GCFont | GCArcMode, &Gcv);

	  pw -> primitive.fontheight = pw -> primitive.font -> ascent +
					pw -> primitive.font -> descent;
	  pw -> primitive.maxfontwidth =
				pw -> primitive.font -> max_bounds.width;
	};

static void Initialize (Widget request, Widget new)
	{ PrimitiveWidget pw = (PrimitiveWidget) new;
	  NewGCs (pw);
	};

static Boolean SetValues (Widget current, Widget request, Widget new)
	{ PrimitiveWidget npw = (PrimitiveWidget) new;
	  PrimitiveWidget opw = (PrimitiveWidget) current;
	  if ((npw -> primitive.foreground != opw -> primitive.foreground) ||
	      (npw -> core.background_pixel != opw -> core.background_pixel) ||
	      (npw -> primitive.font -> fid != opw -> primitive.font -> fid))
	     { XtReleaseGC (new, npw -> primitive.normalgc);
	       XtReleaseGC (new, npw -> primitive.invertgc);
	       NewGCs (npw);
	       return (True);
	     };
	  return (False);
	}; 

static void Destroy (Widget w)
	{ PrimitiveWidget pw = (PrimitiveWidget) w;

	  XtReleaseGC (w, pw -> primitive.normalgc);
	  XtReleaseGC (w, pw -> primitive.invertgc);
	};
