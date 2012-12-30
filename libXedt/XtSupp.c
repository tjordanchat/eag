/*
   File: XtSupp.c
   Defines the global variables to support building applications using the X toolkit

   CVS ID: "$Id: XtSupp.c,v 1.3 2004/12/25 22:28:11 marcs Exp $"
*/

/* standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>

/* X11 includes */
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

/* local includes */
#include <XSupp.h>
#include <XtArgs.h>
#include <XtSupp.h>

Widget MyRootWidget;
XtAppContext MyContext;
long BackgroundColor;
long ForegroundColor;
GC MyDefaultGC;

/*
   options and resources are given as follows:

static XrmOptionDescRec options [] =
        {{"-stipple", "pictStipple", XrmoptionNoArg, "on"},
         {"-stip",    "pictStipple", XrmoptionNoArg, "on"},
         {"-s",       "pictStipple", XrmoptionNoArg, "on"},
         {"-nostip",  "pictStipple", XrmoptionNoArg, "off"},
         {"-scale",   "pictScale", XrmoptionSepArg, NULL}};

typedef struct
        { Boolean stip;
          int scale;
        } app_resourceRec, *app_res;

static app_resourceRec MyResources;

static XtResource resources [] =
        {{"pictStipple", "Stipple", XtRBoolean, sizeof (Boolean),
             XtOffset (app_res, stip), XtRImmediate, (caddr_t) False},
         {"pictScale", "Scale", XtRInt, sizeof (int),
             XtOffset (app_res, scale), XtRImmediate, (caddr_t) 1}};

	  SetupToolkitApplication ("xmile", "xMile",
                                   options, XtNumber (options),
                                   &MyResources,
                                   resources, XtNumber (resources),
                                   &argc, argv);
*/
void SetupToolkitApplication (char *name, char *class,
		XrmOptionDescRec options [], int nr_options,
		caddr_t myresources, XtResource resources [], int nr_resources,
		int *argc, char **argv)
	{ XGCValues Gcv;
	  XtToolkitInitialize ();
	  MyContext = XtCreateApplicationContext ();
	  MyDisplay = XtOpenDisplay (MyContext, DefaultDisplayName,
				     name, class,
				     options, nr_options,
				     argc, argv);
	  if (MyDisplay == NULL)
	     { fprintf (stderr, "Can not open display\n");
	       exit (4);
	     };

	  SetupRSimpleXApplication ();
	  MyRootWidget = XtAppCreateShell (name, class,
				     applicationShellWidgetClass,
				     MyDisplay, NoArgs);
	  XtGetApplicationResources (MyRootWidget, myresources,
				     resources, nr_resources, NULL, 0);

	  StartArgs;
	  SetArg (XtNbackground, &BackgroundColor);
	  XtGetValues (MyRootWidget, UseArgs);
	  if (BackgroundColor == Black)
	     { ForegroundColor = White;
	     }
	  else ForegroundColor = Black;
	  Gcv.function = GXcopy;
	  Gcv.foreground = ForegroundColor;
	  Gcv.background = BackgroundColor;
	  Gcv.line_width = 0;
	  MyDefaultGC = XCreateGC (MyDisplay, MyRootWindow,
			GCFunction|GCForeground|GCBackground|GCLineWidth, &Gcv);
	};
